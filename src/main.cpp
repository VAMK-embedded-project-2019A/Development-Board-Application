#include "main.h"
#include "config.h"
#include "servercomm.h"

#include <thread>
#include <chrono>

void serverCommunicationThread(std::shared_ptr<ServerComm> server_comm);

int main()
{
	auto config_map = parseConfig();
	for(auto pair : config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
	
	auto server_comm = std::make_shared<ServerComm>();
	std::unique_lock<std::mutex> server_comm_lock(server_comm->_mutex);
	server_comm->setConfigMap(config_map);
	server_comm->setStartRequestFlag();
	server_comm_lock.unlock();
	
	std::thread server_comm_thread(serverCommunicationThread, server_comm);
	
	while(true)
	{		
		if(server_comm_lock.try_lock())
		{
			if(server_comm->isDone() && !server_comm->getSongName().empty())
				std::cout << "Main: Hey we're done. Song name: " << server_comm->getSongName() << std::endl;
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