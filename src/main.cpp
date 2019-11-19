#include "main.h"
#include "config.h"
#include "servercomm.h"
#include "wifiscanner.h"

#include <future>
#include <thread>
#include <chrono>

int main()
{
	auto config_map = parseConfig();
	for(auto pair : config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
	
	ServerComm server_comm;
	std::unique_lock<std::mutex> server_comm_lock(server_comm._mutex);
	server_comm.setConfigMap(config_map);
	server_comm_lock.unlock();
	auto server_comm_future = std::async(std::launch::async, &ServerComm::start, &server_comm);
	// wait for the thread to start
	while(server_comm_future.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	
	WifiScanner wifi_scanner;
	std::unique_lock<std::mutex> wifi_scanner_lock(wifi_scanner._mutex);
	wifi_scanner.setInfoFile(config_map.at(WIFIINFO_PATH));
	wifi_scanner_lock.unlock();
	auto wifi_scanner_future = std::async(std::launch::async, &WifiScanner::start, &wifi_scanner);
	// wait for the thread to start
	while(wifi_scanner_future.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	
	while(true)
	{
		if(wifi_scanner_lock.try_lock())
		{
			if(wifi_scanner.isDone())
			{
				if(wifi_scanner.isError())
				{
					std::cout << "Main: WifiScanner error" << std::endl;
					wifi_scanner_lock.unlock();
					break;
				}
				
				std::cout << "Main: WifiScanner done" << std::endl;
				auto access_points = wifi_scanner.getAccessPoints();
				for(auto access_point : access_points)
					std::cout << access_point << std::endl;
			}
			else
				std::cout << "Main: WifiScanner still not finished" << std::endl;
			wifi_scanner_lock.unlock();
		}
		else
			std::cout << "Main: Cannot lock WifiScanner" << std::endl;
		
		if(server_comm_lock.try_lock())
		{
			if(server_comm.isDone())
			{
				if(server_comm.isError())
				{
					std::cout << "Main: ServerComm error" << std::endl;
					server_comm_lock.unlock();
					break;
				}
				std::cout << "Main: ServerComm done" << std::endl;
				std::cout << "Song name: " << server_comm.getSongName() << std::endl;
			}
			else
				std::cout << "Main: ServerComm still not finished" << std::endl;
			server_comm_lock.unlock();
		}
		else
			std::cout << "Main: Cannot lock ServerComm" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	server_comm_future.wait();
	wifi_scanner_future.wait();

	return 0;
}