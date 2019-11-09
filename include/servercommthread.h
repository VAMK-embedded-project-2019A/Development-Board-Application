#ifndef SERVERCOMMTHREAD_H
#define SERVERCOMMTHREAD_H

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
enum ConfigEnum : int;

extern std::mutex 				server_comm_mutex;
extern std::condition_variable	server_comm_cond_var;
extern bool						server_comm_requested;
extern bool						server_comm_done;
extern std::string				next_song_name;

void serverCommunicationThread(const std::map<ConfigEnum, std::string> config_map);

#endif // SERVERCOMMTHREAD_H