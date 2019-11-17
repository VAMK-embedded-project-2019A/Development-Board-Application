#include "wifiscanner.h"
#include <cstdlib>

void WifiScanner::setInfoFile(const std::string &file_path)
{
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
		goto FINISH;
	if(system(full_cmd.c_str()) == -1) // blocking
		goto FINISH;

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
	
}
