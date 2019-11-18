#include "wifiscanner.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

std::ostream& operator<<(std::ostream& stream, const AccessPoint& access_point)
{
	stream << "AP {" << access_point._ESSID;
	stream << "; " << access_point._channel;
	stream << "; " << access_point._dbm_strength;
	stream << "; " << access_point._MAC << "}";
	return stream;
}

void WifiScanner::setInfoFile(const std::string &file_path)
{
	_info_file_path = file_path;
	_full_cmd = _cmd + " > " + file_path;
}

void WifiScanner::start()
{
	// TODO: we have a race here, how to fix?
	std::unique_lock<std::mutex> wifi_scanner_lock(_mutex);
	_scan_done = false;
	auto full_cmd = _full_cmd;
	wifi_scanner_lock.unlock();
	
	if(full_cmd.empty())
	{
		std::cout << "WifiScanner: Set info file path before scanning for APs" << std::endl;
		goto FINISH;
	}
	if(system(full_cmd.c_str()) == -1) // blocking
	{
		std::cout << "WifiScanner: Execute cmd failed: " << full_cmd << std::endl;
		goto FINISH;
	}

	readWifiInfo();
	
FINISH:
	wifi_scanner_lock.lock();
	_scan_done = true;
	wifi_scanner_lock.unlock();
}

std::vector<AccessPoint> WifiScanner::getAccessPoints() const
{
	return _ap_list;
}

bool WifiScanner::isDone() const
{
	return _scan_done;
}

void WifiScanner::readWifiInfo()
{
	if(_info_file_path.empty())
	{
		std::cout << "WifiScanner: Set info file path before scanning for APs" << std::endl;
		return;
	}
	
	std::ifstream file_stream(_info_file_path);
	if(!file_stream.is_open())
	{
		std::cout << "WifiScanner: Cannot open file " << _info_file_path << std::endl;
		return;
	}
	
	AccessPoint access_point;
	const int MAX_LINE_NUM = 4;
	int current_line = 0;
	for(std::string line; std::getline(file_stream, line);)
	{
		switch(current_line % MAX_LINE_NUM)
		{
			case 0:
				if(current_line != 0)
					_ap_list.push_back(AccessPoint{access_point});
				current_line++;
				if(!getMAC(line, &access_point))
					break;
				continue;
			case 1:
				current_line++;
				if(!getChannel(line, &access_point))
					break;
				continue;
			case 2:
				current_line++;
				if(!getStrength(line, &access_point))
					break;
				continue;
			case 3:
				current_line++;
				if(!getESSID(line, &access_point))
					break;
				continue;
		}
		
		// something's wrong
		std::cout << "WifiScanner: Error reading file " << _info_file_path << " around line " << current_line << std::endl;
		return;
    }
}

bool WifiScanner::getMAC(const std::string &line, AccessPoint *access_point)
{
	std::string delim{"Address: "};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto mac_str = line.substr(pos + delim.length());
	if(mac_str.length() != 17) // sanity check
		return false;
	access_point->_MAC = mac_str;
	
	return true;
}

bool WifiScanner::getChannel(const std::string &line, AccessPoint *access_point)
{
	std::string delim{"Channel "};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto channel_str = line.substr(pos + delim.length());
	try
	{
		access_point->_channel = std::stoi(channel_str);
	}
	catch(const std::out_of_range &exception)
	{
		std::cout << "WifiScanner: getChannel out of range" << std::endl;
		return false;
	}
	catch(const std::invalid_argument &exception)
	{
		std::cout << "WifiScanner: getChannel invalid argument" << std::endl;
		return false;
	}
	
	return true;
}

bool WifiScanner::getStrength(const std::string &line, AccessPoint *access_point)
{
	std::string delim{"Signal level="};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto strength_str = line.substr(pos + delim.length());
	try
	{
		access_point->_dbm_strength = std::stoi(strength_str);
	}
	catch(const std::out_of_range &exception)
	{
		std::cout << "WifiScanner: getStrength out of range" << std::endl;
		return false;
	}
	catch(const std::invalid_argument &exception)
	{
		std::cout << "WifiScanner: getStrength invalid argument" << std::endl;
		return false;
	}
	
	return true;
}

bool WifiScanner::getESSID(const std::string &line, AccessPoint *access_point)
{
	std::string delim{"ESSID:"};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	access_point->_ESSID = line.substr(pos + delim.length());
	
	return true;
}

