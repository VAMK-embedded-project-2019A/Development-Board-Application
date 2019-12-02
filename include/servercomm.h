#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <map>
#include <string>
#include <vector>
#include <chrono>

enum ConfigEnum : uint8_t;
class SongInfo;

class ServerComm
{
public:
	ServerComm() = default;
	
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	bool start();
	
	std::string getSongName() const;
	
private:
	void getGpsLocation();
	std::string getWeatherTag();
	std::vector<SongInfo> getSongInfo(const std::string &tag);
	bool downloadSong(const std::string &file_name);
	void setSongName(const std::string &file_name);

	std::map<ConfigEnum, std::string>		_config_map;
	std::string								_song_name;

	std::pair<float, float>					_location;
	std::chrono::steady_clock::time_point 	_last_gps_timestamp;

	std::string								_last_tag;
	std::chrono::steady_clock::time_point 	_last_tag_timestamp;
};

#endif // SERVERCOMM_H