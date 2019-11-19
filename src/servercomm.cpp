#include "servercomm.h"
#include "config.h"
#include "httpsclient.h"
#include "sftpclient.h"
#include "songinfoparser.h"

#include <thread>
#include <chrono>

void ServerComm::setConfigMap(const std::map<ConfigEnum, std::string> &config_map)
{
	_config_map = config_map;
}
	
void ServerComm::setLocation(float longitude, float latitude)
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	_location.first = longitude;
	_location.second = latitude;
	server_comm_lock.unlock();
}

std::string ServerComm::getSongName()
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	return _song_name;
}

void ServerComm::start()
{
	setDone(false);
	std::vector<SongInfo> song_info_vec;
	std::string file_name;
	
	std::string tag = getWeatherTag();
	std::cout << "ServerComm: Weather tag: " << tag << std::endl;
	if(tag.empty())
	{
		std::cout << "ServerComm: getWeatherTag() failed" << std::endl;
		goto ERROR;
	}
	
	// TODO: how to choose from a list of song?
	// TODO: change tag back when server supports all tag
	// auto song_info_vec = getSongInfo(tag);
	song_info_vec = getSongInfo("rain");
	if(song_info_vec.empty())
	{
		std::cout << "ServerComm: getSongInfo() failed" << std::endl;
		goto ERROR;
	}
	file_name = song_info_vec.front()._file_name;
	setSongName(file_name);
	std::cout << "Server thread: Song name: " << file_name << std::endl;
	
	std::cout << "Server thread: Getting file: " << file_name << std::endl;
	// TODO: change file name back when server supports
	// downloadSong(file_name);
	if(!downloadSong("test.txt"))
	{
		std::cout << "ServerComm: downloadSong() failed" << std::endl;
		goto ERROR;
	}
	goto FINISH;

ERROR:
	setError(true);
	
FINISH:
	setDone(true);
}

std::string ServerComm::getWeatherTag()
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	auto port = static_cast<uint16_t>(std::stoul(_config_map.at(HTTPS_PORT)));
	HttpsClient https_client{_config_map.at(IP), port};
	if(!https_client.connect())
		return {};

	std::string request{std::string{"GET /weather"}
						+ "?longitude=" + std::to_string(_location.first)
						+ "&latitude=" + std::to_string(_location.second)
						+ " HTTP/1.1\r\n\r\n"};
	server_comm_lock.unlock();
	if(!https_client.sendRequest(request))
		return {};
	
	std::string response = https_client.receiveResponse();
	if(response.empty())
		return {};

	std::string data_delim{"\r\n\r\n"};
	return response.substr(response.find(data_delim) + data_delim.length());
}

std::vector<SongInfo> ServerComm::getSongInfo(const std::string &tag)
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	auto port = static_cast<uint16_t>(std::stoul(_config_map.at(HTTPS_PORT)));
	HttpsClient https_client{_config_map.at(IP), port};
	server_comm_lock.unlock();
	if(!https_client.connect())
		return {};

	std::string request{"GET /song?tag=" + tag + " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
		return {};

	std::string response = https_client.receiveResponse();
	if(response.empty())
		return {};

	std::string data = response.substr(response.find("{"));
	SongInfoParser parser;
	parser.parseData(data);

	return parser.getSongs();
}

bool ServerComm::downloadSong(const std::string &file_name)
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	
	SftpClient sftp_client{_config_map.at(IP), _config_map.at(SFTP_USERNAME)};
	sftp_client.setPasswordFilePath		(_config_map.at(SFTP_PASSWORD));
	sftp_client.setKnownHostsFilePath	(_config_map.at(SFTP_KNOWNHOSTS));
	sftp_client.setPublicKeyFilePath	(_config_map.at(SFTP_PUBLICKEY));
	sftp_client.setPrivateKeyFilePath	(_config_map.at(SFTP_PRIVATEKEY));
	
	auto save_path		= _config_map.at(SFTP_SAVEPATH);
	auto server_path	= _config_map.at(SFTP_SERVERPATH);
	
	server_comm_lock.unlock();
	
	return sftp_client.getFile(server_path + file_name, save_path + file_name);
}

void ServerComm::setSongName(const std::string &file_name)
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	_song_name = file_name;
	server_comm_lock.unlock();
}