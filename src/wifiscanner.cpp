#include "wifiscanner.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

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
	
	for(std::string line; std::getline(file_stream, line);)
	{
        std::cout << line << std::endl;
    }
}
