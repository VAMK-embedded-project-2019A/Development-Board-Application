#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <map>
#include <string>
#include <mutex>
#include <memory>

enum ConfigEnum : int;

class ServerComm
{
public:
	ServerComm() = default;
	friend void serverCommunicationThread(std::shared_ptr<ServerComm> server_comm);
	
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	void setLocation(float longitude, float latitude);
	void setStartRequestFlag(bool set = true);
	
	std::string getSongName() const;
	bool isStartRequested() const;
	bool isDone() const;
	
	std::mutex _mutex;
	
private:
	void startCommunication();
	std::string getWeatherTag();

	// flags for signaling between threads
	bool _start_requested{false};
	bool _comm_done{false};

	std::map<ConfigEnum, std::string>	_config_map;
	std::pair<float, float>				_location;
	std::string							_song_name;
};

#endif // SERVERCOMM_H