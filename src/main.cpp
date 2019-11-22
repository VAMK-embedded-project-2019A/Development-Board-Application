#include "main.h"

#include "servercomm.h"
#include "wifiscanner.h"

#include <future>
#include <thread>
#include <chrono>

int main()
{
	Main main_obj;
	if(!main_obj.isError())
		main_obj.start();

	// this program should not stop
	return -1;
}

Main::Main()
{
	_config_map = parseConfig();
	if(_config_map.empty())
	{
		setError(true);
		return;
	}
	for(auto pair : _config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
}

Main::~Main()
{
}

void Main::start()
{
	if(isError())
		return;
	
	while(true)
	{
		// TODO: check if music player playing
		if(false)
			goto SLEEP; // if yes, good
		
		// TODO: if not, check if next song available
		if(false)
		{
			// TODO: if yes, play
			goto SLEEP;
		}
		
		// TODO: if not, check wifi available
		if(isWifiConnected())
		{
			// if yes, get song from server (loop until success)
			auto future = std::async(std::launch::async, &Main::getNextSong, this);
			future.wait();
			auto song_name = future.get();
			// TODO: set next song for music player
			continue;
		}
		else
		{
			// if not, do wifi stuff (loop until success)
			auto future = std::async(std::launch::async, &Main::connectWifi, this);
			future.wait();
			continue;
		}

	SLEEP:
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

std::pair<float, float> Main::getGpsLocation() const
{
	// TODO: stub, loop until a valid gps is get
	return std::pair<float, float>(0, 0);
}

std::string Main::getNextSong() const
{
	ServerComm server_comm;
	server_comm.setConfigMap(_config_map);
	
	std::string song_name;
	while(song_name.empty())
	{
		auto gps_location = getGpsLocation();
		server_comm.setLocation(gps_location.first, gps_location.second);
		if(server_comm.start())
			song_name = server_comm.getSongName();
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	return song_name;
}

bool Main::isWifiConnected() const
{
	// TODO: stub, somehow notice the user of no wifi
	return true;
}

void Main::connectWifi()
{
	// TODO: check if bluetooth client available (loop until available)
	WifiScanner wifi_scanner;
	wifi_scanner.setInfoFile(_config_map.at(WIFIINFO_PATH));
	
	std::vector<AccessPoint> ap_vec;
	while(ap_vec.empty())
	{
		if(wifi_scanner.start())
			ap_vec = wifi_scanner.getAccessPoints();
	}
	
	// TODO: do bluetooth comm and connect here (loop until connected)
	while(true);
}