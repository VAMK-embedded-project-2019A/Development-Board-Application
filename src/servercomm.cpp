#include "servercomm.h"
#include "config.h"
#include "httpsclient.h"
#include "sftpclient.h"
#include "songinfoparser.h"

#include "jsoncpp/json/json.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <fstream>

void ServerComm::setConfigMap(const std::map<ConfigEnum, std::string> &config_map)
{
	_config_map = config_map;
}

std::string ServerComm::getSongName() const
{
	return _song_name;
}

void ServerComm::getGpsLocation()
{
	auto time_now = std::chrono::steady_clock::now();
	if(time_now - _last_gps_timestamp < std::chrono::minutes(5))
		return;

	// TODO: loop until get valid GPS data
	// while(true)
	// {
		// std::string cmd{"python3 ./../files/gps.py"};
		// std::string file_path{"./../files/gpsinfo.txt"};
		// std::system(std::string(cmd + " > " + file_path).c_str());

		// std::ifstream file_stream(file_path);
		// if(!file_stream.is_open())
		// {
			// std::cout << "ServerComm: Cannot open file " << file_path << std::endl;
			// continue;
		// }

		// std::string line;
		// std::getline(file_stream, line);
		// if(line.empty())
		// {
			// std::cout << "ServerComm: " << file_path << " is empty" << std::endl;
			// continue;
		// }
		// Json::Reader reader;
		// Json::Value root;
		// if(!reader.parse(line, root))
		// {
			// std::cout << "ServerComm: JSON parse failed " << reader.getFormattedErrorMessages() << std::endl;
			// continue;
		// }

		// const Json::Value items = root[1];
		// std::string longitude_str = items["longitude"].asString();
		// std::string latitude_str = items["latitude"].asString();
		// std::cout << longitude_str << " " << latitude_str << std::endl;
		// if(longitude_str.empty() || latitude_str.empty())
			// continue;
		// _location.first = std::atof(longitude_str.c_str());
		// _location.second = std::atof(latitude_str.c_str());
	// }
	_location = std::pair<float, float>(63.1060497, 21.5945991); // VAMK
	_last_gps_timestamp = time_now;
}

bool ServerComm::start()
{
	auto time_now = std::chrono::steady_clock::now();
	if(time_now - _last_tag_timestamp > std::chrono::minutes(1))
	{
		std::cout << "ServerComm: Getting GPS info" << std::endl;
		getGpsLocation();
		std::cout << "ServerComm: Getting weather info" << std::endl;
		_last_tag = getWeatherTag();
		std::cout << "ServerComm: Weather tag: " << _last_tag << std::endl;
		if(_last_tag.empty())
		{
			std::cout << "ServerComm: getWeatherTag() failed" << std::endl;
			return false;
		}
		_last_tag_timestamp = time_now;
	}

	auto song_info_vec = getSongInfo(_last_tag);
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::shuffle(song_info_vec.begin(), song_info_vec.end(), generator);
	if(song_info_vec.empty())
	{
		std::cout << "ServerComm: getSongInfo() failed" << std::endl;
		return false;
	}
	std::string file_name = song_info_vec.front()._file_name;
	setSongName(file_name);
	std::cout << "ServerComm: Song name: " << file_name << std::endl;

	std::cout << "ServerComm: Getting file: " << file_name << std::endl;
	if(!downloadSong(file_name))
	{
		std::cout << "ServerComm: downloadSong() failed" << std::endl;
		return false;
	}

	return true;
}

std::string ServerComm::getWeatherTag()
{
	auto port = static_cast<uint16_t>(std::stoul(_config_map.at(HTTPS_PORT)));
	HttpsClient https_client{_config_map.at(IP), port};
	if(!https_client.connect())
		return {};

	std::string request{std::string{"GET /weather"}
						+ "?longitude=" + std::to_string(_location.first)
						+ "&latitude=" + std::to_string(_location.second)
						+ " HTTP/1.1\r\n\r\n"};
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
	auto port = static_cast<uint16_t>(std::stoul(_config_map.at(HTTPS_PORT)));
	HttpsClient https_client{_config_map.at(IP), port};
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
	SftpClient sftp_client{_config_map.at(IP), _config_map.at(SFTP_USERNAME)};
	sftp_client.setPassphraseFilePath	(_config_map.at(SFTP_PASSPHRASE));
	sftp_client.setKnownHostsFilePath	(_config_map.at(SFTP_KNOWNHOSTS));
	sftp_client.setPublicKeyFilePath	(_config_map.at(SFTP_PUBLICKEY));
	sftp_client.setPrivateKeyFilePath	(_config_map.at(SFTP_PRIVATEKEY));

	auto save_path		= _config_map.at(SFTP_SAVEPATH);
	auto server_path	= _config_map.at(SFTP_SERVERPATH);

	return sftp_client.getFile(server_path + file_name, save_path + file_name);
}

void ServerComm::setSongName(const std::string &file_name)
{
	_song_name = file_name;
}
