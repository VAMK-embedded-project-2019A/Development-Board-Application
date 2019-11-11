#include "servercomm.h"
#include "config.h"
#include "httpsclient.h"
#include "sftpclient.h"
#include "songinfoparser.h"

#include <thread>
#include <chrono>

void serverCommunicationThread(std::shared_ptr<ServerComm> server_comm)
{
	std::unique_lock<std::mutex> server_comm_lock(server_comm->_mutex, std::defer_lock);
	while(true)
	{
		server_comm_lock.lock();
		if(server_comm->isStartRequested())
		{
			std::cout << "Server thread: Starting server communication" << std::endl;
			server_comm->setStartRequestFlag(false);
			server_comm_lock.unlock();
			server_comm->startCommunication();
		}
		else
			server_comm_lock.unlock();
		
		// TODO: how to exit thread? from main?
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	pthread_exit(NULL);
}

void ServerComm::setConfigMap(const std::map<ConfigEnum, std::string> &config_map)
{
	_config_map = config_map;
}
	
void ServerComm::setLocation(float longitude, float latitude)
{
	_location.first = longitude;
	_location.second = latitude;
}

void ServerComm::setStartRequestFlag(bool set)
{
	_start_requested = set;
	_comm_done = false;
}

std::string ServerComm::getSongName() const
{
	return _song_name;
}

bool ServerComm::isStartRequested() const
{
	return _start_requested;
}

bool ServerComm::isDone() const
{
	return _comm_done;
}

void ServerComm::startCommunication()
{
	std::string tag = getWeatherTag();
	std::cout << "Server thread: Weather tag: " << tag << std::endl;
	if(tag.empty())
	{
		std::cout << "Server thread: getWeatherTag() failed" << std::endl;
		return;
	}
	
	// TODO: how to choose from a list of song?
	// TODO: change tag back when server supports all tag
	// auto song_info_vec = getSongInfo(tag);
	auto song_info_vec = getSongInfo("rain");
	if(song_info_vec.empty())
	{
		std::cout << "Server thread: getSongInfo() failed" << std::endl;
		return;
	}
	
	auto file_name = song_info_vec.front()._file_name;
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	_song_name = file_name;
	server_comm_lock.unlock();
	std::cout << "Server thread: Song name: " << file_name << std::endl;
	
	std::cout << "Server thread: Getting file: " << file_name << std::endl;
	// TODO: change file name back when server supports
	// downloadSong(file_name);
	downloadSong("test.txt");
	
	server_comm_lock.lock();
	_comm_done = true;
	server_comm_lock.unlock();
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

void ServerComm::downloadSong(const std::string &file_name)
{
	std::unique_lock<std::mutex> server_comm_lock(_mutex);
	SftpClient sftp_client{_config_map.at(IP), _config_map.at(SFTP_USERNAME)};
	sftp_client.setPasswordFilePath(_config_map.at(SFTP_PASSWORD));
	sftp_client.setKnownHostsFilePath(_config_map.at(SFTP_KNOWNHOSTS));
	sftp_client.setPublicKeyFilePath(_config_map.at(SFTP_PUBLICKEY));
	sftp_client.setPrivateKeyFilePath(_config_map.at(SFTP_PRIVATEKEY));
	auto save_path = _config_map.at(SFTP_SAVEPATH);
	auto server_path = _config_map.at(SFTP_SERVERPATH);
	server_comm_lock.unlock();
	
	sftp_client.getFile(server_path + file_name, save_path + file_name);
}