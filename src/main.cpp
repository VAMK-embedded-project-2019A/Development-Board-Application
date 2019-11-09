#include "main.h"
#include "config.h"
#include "servercommthread.h"

#include <thread>
#include <chrono>

int main()
{
	auto config_map = parseConfig();
	for(auto pair : config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
	
	std::unique_lock<std::mutex> server_comm_lock(server_comm_mutex);
	server_comm_requested = true;
	server_comm_lock.unlock();
	
	std::thread server_comm_thread(serverCommunicationThread, config_map);
	server_comm_cond_var.notify_one();
	
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "Main wake. Checking if server communication finished" << std::endl;
		
		server_comm_lock.lock();
		bool done = server_comm_cond_var.wait_until(server_comm_lock, std::chrono::steady_clock::now(), []{return server_comm_done;});
		if(!done)
			std::cout << "Still not finished" << std::endl;
		else
		{
			std::cout << "Hey we're done. Song name: " << next_song_name << std::endl;
		}
		server_comm_lock.unlock();
	}

	return 0;
}