#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <map>
#include <string>
#include <mutex>
#include <vector>

enum ConfigEnum : int;
class SongInfo;

class ServerComm
{
public:
	ServerComm() = default;
	
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	void setLocation(float longitude, float latitude);
	void start();
	
	std::string getSongName() const;
	bool isDone() const;
	bool isError() const;
	
	std::mutex _mutex;
	
private:
	std::string getWeatherTag();
	std::vector<SongInfo> getSongInfo(const std::string &tag);
	bool downloadSong(const std::string &file_name);

	bool _comm_done{true};
	bool _error{false};

	std::map<ConfigEnum, std::string>	_config_map;
	std::pair<float, float>				_location;
	std::string							_song_name;
};

#endif // SERVERCOMM_H