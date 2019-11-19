#include "wifiscanner.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

std::ostream& operator<<(std::ostream& stream, const AccessPoint& access_point)
{
	stream << "AP {" << access_point._ESSID;
	stream << "; AuthSuite " << access_point._auth_suite;
	stream << "; Channel " << access_point._channel;
	stream << "; " << access_point._dbm_strength << " dBm";
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
		goto ERROR;
	}
	if(system(full_cmd.c_str()) == -1) // blocking
	{
		std::cout << "WifiScanner: Execute cmd failed: " << full_cmd << std::endl;
		goto ERROR;
	}

	if(readWifiInfo())
		goto FINISH;
	
ERROR:
	wifi_scanner_lock.lock();
	_error = true;
	wifi_scanner_lock.unlock();

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

bool WifiScanner::isError() const
{
	return _error;
}

bool WifiScanner::readWifiInfo()
{
	std::unique_lock<std::mutex> wifi_scanner_lock(_mutex);
	auto file_path = _info_file_path;
	wifi_scanner_lock.unlock();
	
	if(file_path.empty())
	{
		std::cout << "WifiScanner: Set info file path before scanning for APs" << std::endl;
		return false;
	}
	
	std::ifstream file_stream(file_path);
	if(!file_stream.is_open())
	{
		std::cout << "WifiScanner: Cannot open file " << file_path << std::endl;
		return false;
	}
	
	AccessPoint access_point;
	int current_line = 0;
	int start_AP_line = 0;
	for(std::string line; std::getline(file_stream, line);)
	{
		if(isNewWifiLine(line))
			start_AP_line = current_line;
		switch(current_line - start_AP_line)
		{
			case 0:
				if(current_line != 0)
					_ap_list.push_back(AccessPoint{access_point});
				current_line++;
				if(!getMAC(line, access_point))
					break;
				continue;
			case 1:
				current_line++;
				if(!getChannel(line, access_point))
					break;
				continue;
			case 2:
				current_line++;
				if(!getStrength(line, access_point))
					break;
				continue;
			case 3:
				current_line++;
				if(!getESSID(line, access_point))
					break;
				continue;
			case 4:
			case 5:
				current_line++;
				if(!getAuthSuite(line, access_point))
					break;
				continue;
		}
		
		// something's wrong
		file_path.pop_back();
		std::cout << "WifiScanner: Error reading file " << file_path << " around line " << current_line << std::endl;
		std::cout << "WifiScanner: Please contact support with the file " << file_path << std::endl;
		return false;
    }
	
	return true;
}

bool WifiScanner::isNewWifiLine(const std::string &line)
{
	std::string delim{"Cell"};
	return (line.find(delim) != std::string::npos);
}

bool WifiScanner::getMAC(const std::string &line, AccessPoint &access_point)
{
	std::string delim{"Address: "};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto mac_str = line.substr(pos + delim.length());
	if(mac_str.length() != 17) // sanity check
		return false;
	access_point._MAC = mac_str;
	
	return true;
}

bool WifiScanner::getChannel(const std::string &line, AccessPoint &access_point)
{
	std::string delim{"Channel "};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto channel_str = line.substr(pos + delim.length());
	try
	{
		access_point._channel = std::stoi(channel_str);
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

bool WifiScanner::getStrength(const std::string &line, AccessPoint &access_point)
{
	std::string delim{"Signal level="};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	auto strength_str = line.substr(pos + delim.length());
	try
	{
		access_point._dbm_strength = std::stoi(strength_str);
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

bool WifiScanner::getESSID(const std::string &line, AccessPoint &access_point)
{
	std::string delim{"ESSID:"};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	access_point._ESSID = line.substr(pos + delim.length());
	
	return true;
}

bool WifiScanner::getAuthSuite(const std::string &line, AccessPoint &access_point)
{
	std::string delim{"Authentication Suites (1) : "};
	auto pos = line.find(delim);
	if(pos == std::string::npos)
		return false;
	
	access_point._auth_suite = line.substr(pos + delim.length());
	
	return true;
}

