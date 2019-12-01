#include "main.h"

#include <thread>
#include <chrono>

int main()
{
	Main main_obj;
	if(!main_obj.hasError())
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
		_error = true;
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
	_bluetooth_comm.startAdvertising();
}

Main::~Main()
{
}

bool Main::hasError() const
{
	return _error;
}

void Main::start()
{
	if(hasError())
		return;

	bool first_play_finished{false};
	while(true)
	{
		// handle buttons only after the first song is played
		if(first_play_finished)
			handleButtonPoll();
		
		handleBluetoothComm();
		
		if(!_wifi_handler.isConnected())
			goto SLEEP;
		
		if(_music_player.getCurrentSong().empty())
		{
			std::string song_name = requestGetSongResult();
			if(song_name.empty())
				goto SLEEP;
			_music_player.setCurrentSong(song_name);

			if(!first_play_finished)
			{
				_music_player.play();
				first_play_finished = true;
			}
		}
			
		if(_music_player.getNextSong().empty())
		{
			std::string song_name = requestGetSongResult();
			if(song_name.empty())
				goto SLEEP;
			_music_player.setNextSong(song_name);
		}

	SLEEP:
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

std::string Main::requestGetSongResult()
{
	// if getSong() isnt started yet, start one
	if(!_future_get_song.valid())
		_future_get_song = std::async(std::launch::async, &Main::getSong, this);

	if(_future_get_song.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		return std::string{};

	return _future_get_song.get();
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

void Main::handleButtonPoll()
{
	int button_pressed = _button_poll.getNextPressedPin();
	while(button_pressed != -1)
	{
		switch(button_pressed)
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
		button_pressed = _button_poll.getNextPressedPin();
	}
}

void Main::handleBluetoothComm()
{
	if(!_future_get_bt_client.valid())
		_future_get_bt_client = std::async(std::launch::async, &BluetoothComm::getClient, &_bluetooth_comm);
	if(_future_get_bt_client.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		return;

	if(!_future_read_bt_message.valid())
		_future_read_bt_message = std::async(std::launch::async, &BluetoothComm::readFromClient, &_bluetooth_comm);
	if(_future_read_bt_message.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		return;

	std::string message = _future_read_bt_message.get();
	if(message.empty())
		return;
	// TODO: handle message and write back (async?)
}