#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include "process.h"

#include <map>
#include <string>
#include <vector>

enum ConfigEnum : int;
class SongInfo;

class ServerComm : public Process
{
public:
	ServerComm() = default;
	
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	void setLocation(float longitude, float latitude);
	void start() override;
	
	std::string getSongName();
	
private:
	std::string getWeatherTag();
	std::vector<SongInfo> getSongInfo(const std::string &tag);
	bool downloadSong(const std::string &file_name);
	void setSongName(const std::string &file_name);

	std::map<ConfigEnum, std::string>	_config_map;
	std::pair<float, float>				_location;
	std::string							_song_name;
};

#endif // SERVERCOMM_H