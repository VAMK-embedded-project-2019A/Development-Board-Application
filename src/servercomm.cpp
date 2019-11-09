#include "servercomm.h"
#include "config.h"
#include "httpsclient.h"

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
			server_comm->startCommunication();
		}
		
		// TODO: how to exit thread? from main?
		
		server_comm_lock.unlock();
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
	_song_name = "abc.wav";
	std::cout << "Server thread: Server communication done" << std::endl;
}

std::string ServerComm::getWeatherTag()
{
	auto port = static_cast<uint16_t>(std::stoul(_config_map.at(HTTPS_PORT)));
	HttpsClient https_client{_config_map.at(IP), port};
	if(!https_client.connect())
		return std::string{};

	std::string request{std::string{"GET /weather"}
						+ "?longitude=" + std::to_string(_location.first)
						+ "&latitude=" +std::to_string(_location.second)
						+ " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
		return std::string{};

	std::string response = https_client.receiveResponse();
	if(response.empty())
		return std::string{};

	std::string data_delim{"\r\n\r\n"};
	return response.substr(response.find(data_delim) + data_delim.length());
}