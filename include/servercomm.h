#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <map>
#include <string>
#include <vector>
#include <chrono>

enum ConfigEnum : uint8_t;
class SongInfo;

//! Communicate with the server.
/*!
  This class implements the flow of the server communication process.
*/
class ServerComm
{
public:
	ServerComm() = default;
	
	//! Set the configurations.
	void setConfigMap(const std::map<ConfigEnum, std::string> &config_map);
	//! Start communicating with the server.
	/*!
	  If the weather tag has not been updated in the last 1 minute, call getGpsLocation() and then getWeatherTag().
	  Subsequently call getSongInfo() and choose a random song in the returned vector to call downloadSong().
	  Return true if none operation failed, false otherwise.
	*/
	bool start();
	
	//! Return the song name retrieved from the last start().
	std::string getSongName() const;
	
private:
	//! Get a new GPS coordinate with a 5 minutes cooldown
	/*!
	  If the GPS coordinate has been updated in the last 5 minutes, do nothing.
	  Otherwise, block until a valid GPS coordinate is gotten.
	*/
	void getGpsLocation();
	//! Send the GPS coordinate to the server to get a weather tag
	/*!
	  Return a weather tag string on success, an empty string otherwise.
	*/
	std::string getWeatherTag();
	//! Send the weather tag \p tag to the server to get a song name list
	/*!
	  Return a vector of song's file name on success, an empty vector otherwise.
	*/
	std::vector<SongInfo> getSongInfo(const std::string &tag);
	//! Download the file named \p file_name from the server
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