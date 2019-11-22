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
	// parse configurations
	_config_map = parseConfig();
	if(_config_map.empty())
	{
		setError(true);
		return;
	}
	for(auto pair : _config_map)
		std::cout << ConfigEnumToString(pair.first) << "\t" << pair.second << std::endl;
	std::cout << std::endl;
	
	// init buttons
	_button_poll.addButton(PlayPause,	ButtonPoll::TriggerEdge::Rising);
	_button_poll.addButton(Next,		ButtonPoll::TriggerEdge::Rising);
	_button_poll.addButton(Prev,		ButtonPoll::TriggerEdge::Rising);
	_button_poll.addButton(VolumeUp,	ButtonPoll::TriggerEdge::Rising);
	_button_poll.addButton(VolumeDown,	ButtonPoll::TriggerEdge::Rising);
	_future_button_poll = std::async(std::launch::async, &ButtonPoll::start, &_button_poll);
	
	// get first song from server and play
	auto future = std::async(std::launch::async, &Main::getNextSong, this);
	future.wait();
	auto song_name = future.get();
	_music_player.setCurrentSong(song_name);
	_music_player.play();
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
		// handle buttons
		int button_pressed = _button_poll.getNextPressedPin();
		while(button_pressed != -1)
		{
			handleButtonPressed(button_pressed);
			button_pressed = _button_poll.getNextPressedPin();
		}
		
		// TODO: if not, check if next song available
		if(!_music_player.getNextSong().empty())
		{
			// TODO: if yes, good
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
			_music_player.setNextSong(song_name);
		}
		else
		{
			// if not, do wifi stuff (loop until success)
			auto future = std::async(std::launch::async, &Main::connectWifi, this);
			future.wait();
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

void Main::handleButtonPressed(int pin)
{
	switch(pin)
	{
	case PlayPause:
		if(_music_player.isPlaying())
			_music_player.pause();
		else
			_music_player.play();
		break;
	case Next:
		_music_player.next();
		break;
	case Prev:
		_music_player.prev();
		break;
	case VolumeUp:
		_music_player.increaseVolume();
		break;
	case VolumeDown:
		_music_player.decreaseVolume();
		break;
	default:
		break;
	}
}