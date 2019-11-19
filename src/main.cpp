#include "main.h"

#include "servercomm.h"
#include "wifiscanner.h"

#include <thread>
#include <chrono>

int main()
{
	auto config_map = parseConfig();
	for(auto pair : config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
	
	ServerComm server_comm;
	std::unique_lock<std::mutex> server_comm_lock(server_comm._mutex, std::defer_lock);
	initServerComm(server_comm, config_map);
	auto server_comm_future = startProcess(server_comm);
	
	WifiScanner wifi_scanner;
	std::unique_lock<std::mutex> wifi_scanner_lock(wifi_scanner._mutex, std::defer_lock);
	initWifiScanner(wifi_scanner, config_map);
	auto wifi_scanner_future = startProcess(wifi_scanner);
	
	while(true)
	{
		if(isProcessDone(wifi_scanner, "WifiScanner"))
		{
			wifi_scanner_lock.lock();
			if(wifi_scanner.isError())
			{
				std::cout << "Main: WifiScanner error" << std::endl;
				wifi_scanner_lock.unlock();
				break;
			}
			
			printWifiScannerResult(wifi_scanner);
			wifi_scanner_lock.unlock();
		}
		
		if(isProcessDone(server_comm, "ServerComm"))
		{
			server_comm_lock.lock();
			if(server_comm.isError())
			{
				std::cout << "Main: ServerComm error" << std::endl;
				server_comm_lock.unlock();
				break;
			}
			
			printServerCommResult(server_comm);
			server_comm_lock.unlock();
		}

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	server_comm_future.wait();
	wifi_scanner_future.wait();

	return 0;
}

void initWifiScanner(WifiScanner &wifi_scanner, const std::map<ConfigEnum, std::string> &config_map)
{
	std::unique_lock<std::mutex> wifi_scanner_lock(wifi_scanner._mutex);
	wifi_scanner.setInfoFile(config_map.at(WIFIINFO_PATH));
	wifi_scanner_lock.unlock();
}

void initServerComm(ServerComm &server_comm, const std::map<ConfigEnum, std::string> &config_map)
{
	std::unique_lock<std::mutex> server_comm_lock(server_comm._mutex);
	server_comm.setConfigMap(config_map);
	server_comm_lock.unlock();
}

void printWifiScannerResult(WifiScanner &wifi_scanner)
{
	auto access_points = wifi_scanner.getAccessPoints();
	for(auto access_point : access_points)
		std::cout << access_point << std::endl;
}

void printServerCommResult(ServerComm &server_comm)
{
	std::cout << "Song name: " << server_comm.getSongName() << std::endl;
}