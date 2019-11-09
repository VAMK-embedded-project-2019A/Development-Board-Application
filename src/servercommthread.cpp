#include "servercommthread.h"
#include "config.h"

#include <thread>
#include <chrono>

// externs
std::mutex 				server_comm_mutex;
std::condition_variable	server_comm_cond_var;
bool					server_comm_requested{false};
bool					server_comm_done{false};
std::string				next_song_name;

void serverCommunicationThread(const std::map<ConfigEnum, std::string> config_map)
{
	while(true)
	{
		// wait until main notified
		std::unique_lock<std::mutex> server_comm_lock(server_comm_mutex);
		server_comm_cond_var.wait(server_comm_lock, []{return server_comm_requested;});
		
		// main notified, we own the lock
		server_comm_requested = false;
		server_comm_lock.unlock();
		
		std::cout << "Doing server communication" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		server_comm_lock.lock();
		next_song_name = "abc.wav";
		server_comm_lock.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "Server communication done" << std::endl;
		
		// notify main
		server_comm_lock.lock();
		server_comm_done = true;
		server_comm_lock.unlock();
		server_comm_cond_var.notify_one();
	}
}