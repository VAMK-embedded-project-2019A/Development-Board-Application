#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <map>
#include <string>
#include <mutex>
#include <memory>
#include <vector>

enum ConfigEnum : int;
class SongInfo;

class ServerComm
{
public:
	ServerComm() = default;
	friend void serverCommunicationThread(std::shared_ptr<ServerComm> server_comm);
	
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	void setLocation(float longitude, float latitude);
	void start();
	
	std::string getSongName() const;
	bool isDone() const;
	
	std::mutex _mutex;
	
private:
	void startCommunication();
	std::string getWeatherTag();
	std::vector<SongInfo> getSongInfo(const std::string &tag);
	void downloadSong(const std::string &file_name);

	// flags for signaling between threads
	bool _start_requested{false};
	bool _comm_done{true};

	std::map<ConfigEnum, std::string>	_config_map;
	std::pair<float, float>				_location;
	std::string							_song_name;
};

void serverCommunicationThread(std::shared_ptr<ServerComm> server_comm);

#endif // SERVERCOMM_H