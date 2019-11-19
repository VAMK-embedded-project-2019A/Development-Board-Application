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
	server_comm.setConfigMap(config_map);
	auto server_comm_future = startProcess(server_comm);
	
	WifiScanner wifi_scanner;
	wifi_scanner.setInfoFile(config_map.at(WIFIINFO_PATH));
	auto wifi_scanner_future = startProcess(wifi_scanner);
	
	while(true)
	{
		if(wifi_scanner.isDone())
		{
			if(wifi_scanner.isError())
			{
				std::cout << "Main: WifiScanner error" << std::endl;
				break;
			}
			
			std::cout << "Main: WifiScanner done" << std::endl;
			auto access_points = wifi_scanner.getAccessPoints();
			for(auto access_point : access_points)
				std::cout << access_point << std::endl;
		}
		else
			std::cout << "Main: WifiScanner not done" << std::endl;
		
		if(server_comm.isDone())
		{
			if(server_comm.isError())
			{
				std::cout << "Main: ServerComm error" << std::endl;
				break;
			}
			
			std::cout << "Main: ServerComm done" << std::endl;
			std::cout << "Song name: " << server_comm.getSongName() << std::endl;
		}
		else
			std::cout << "Main: ServerComm not done" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	server_comm_future.wait();
	wifi_scanner_future.wait();

	return 0;
}

std::future<void> startProcess(Process &process)
{
	auto future = std::async(std::launch::async, &Process::start, &process);
	// wait for the thread to start
	while(future.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	
	return future;
}