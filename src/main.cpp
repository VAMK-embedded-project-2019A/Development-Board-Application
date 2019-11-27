#include "main.h"

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
	
	_wifi_handler.setInfoFile(_config_map.at(WIFIINFO_PATH));
	_server_comm.setConfigMap(_config_map);
}

Main::~Main()
{
}

void Main::start()
{
	if(isError())
		return;

	bool first_play_finished{false};
	std::future<std::string> get_song_future;
	while(true)
	{
		// handle buttons only after the first song is played
		if(first_play_finished)
		{
			int button_pressed = _button_poll.getNextPressedPin();
			while(button_pressed != -1)
			{
				handleButtonPressed(button_pressed);
				button_pressed = _button_poll.getNextPressedPin();
			}
		}
		
		// TODO: BT server handle here
		
		if(!_wifi_handler.isConnected())
			goto SLEEP;
		
		if(_music_player.getCurrentSong().empty())
		{
			if(!get_song_future.valid())
				get_song_future = std::async(std::launch::async, &Main::getSong, this);
			else if(get_song_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				_music_player.setCurrentSong(get_song_future.get());
				if(!first_play_finished)
				{
					_music_player.play();
					first_play_finished = true;
				}
			}
			goto SLEEP;
		}
			
		if(_music_player.getNextSong().empty())
		{
			if(!get_song_future.valid())
				get_song_future = std::async(std::launch::async, &Main::getSong, this);
			else if(get_song_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				_music_player.setNextSong(get_song_future.get());
		}

	SLEEP:
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

std::string Main::getSong()
{
	std::string song_name;
	while(true)
	{
		if(_server_comm.start())
			song_name = _server_comm.getSongName();
		if(!song_name.empty())
			break;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	return song_name;
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