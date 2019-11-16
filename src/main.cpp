#include "main.h"
#include "config.h"
#include "servercomm.h"

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
	
	while(true)
	{
		if(server_comm_lock.try_lock())
		{
			if(server_comm.isDone())
				std::cout << "Main: Hey we're done. Song name: " << server_comm.getSongName() << std::endl;
			else
				std::cout << "Main: Still not finished" << std::endl;
			server_comm_lock.unlock();
		}
		else
			std::cout << "Main: Cannot lock" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	return 0;
}