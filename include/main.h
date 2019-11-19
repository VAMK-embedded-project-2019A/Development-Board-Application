#ifndef MAIN_H
#define MAIN_H

#include "config.h"

#include <string>
#include <future>

class ServerComm;
class WifiScanner;

void initWifiScanner(WifiScanner &wifi_scanner, const std::map<ConfigEnum, std::string> &config_map);
void printWifiScannerResult(WifiScanner &wifi_scanner);

void initServerComm(ServerComm &server_comm, const std::map<ConfigEnum, std::string> &config_map);
void printServerCommResult(ServerComm &server_comm);

template <typename T>
std::future<void> startProcess(T &process)
{
	auto future = std::async(std::launch::async, &T::start, &process);
	// wait for the thread to start
	while(future.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	
	return future;
}

template <typename T>
bool isProcessDone(T &process, const std::string &context)
{
	std::unique_lock<std::mutex> lock(process._mutex, std::defer_lock);
	if(lock.try_lock())
	{
		if(process.isDone())
		{
			std::cout << "Main: " << context << " done" << std::endl;
			return true;
		}
		else
			std::cout << "Main: " << context << " still not finished" << std::endl;
		lock.unlock();
	}
	else
		std::cout << "Main: Cannot lock " << context << std::endl;
	
	return false;
}

#endif // MAIN_H