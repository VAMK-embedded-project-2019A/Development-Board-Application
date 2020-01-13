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
	if(_music_player.hasError())
	{
		_error = true;
		return;
	}
	_future_music_player = std::async(std::launch::async, &MusicPlayer::start, &_music_player);
	while(_future_music_player.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	_music_player.setCurrentSong("Default-Welcome.mp3");
	_music_player.control(MusicPlayer::ControlRequest::Play);
	while(!_music_player.isPlaying()); // waiting for _music_player to start playing

	// parse configurations
	_config_map = parseConfig();
	if(_config_map.empty())
	{
		_error = true;
		return;
	}
	for(auto pair : _config_map)
		std::cout << configEnumToString(pair.first) << "\t" << pair.second << std::endl;
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

bool Main::hasError() const
{
	return _error;
}

void Main::start()
{
	if(hasError())
		return;

	while(true)
	{
		std::cout << std::endl << "Main: Start loop" << std::endl;
		handleButtonPoll();

		if(!_wifi_handler.isConnected())
			goto SLEEP;

		if(_music_player.getCurrentSong().empty())
		{
			std::cout << "Main: " << "Current song empty" << std::endl;
			std::string song_name = requestGetSongResult();
			if(song_name.empty())
				goto SLEEP;
			_music_player.setCurrentSong(song_name);
		}

		if(_music_player.getNextSong().empty())
		{
			std::cout << "Main: " << "Next song empty" << std::endl;
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
		std::cout << "Main: " << "Start get song from server" << std::endl;
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
	while(button_pressed != 0xFF)
	{
		std::cout << "Main: " << "Handle button pin " << button_pressed << std::endl;
		switch(button_pressed)
		{
		case PlayPause:
			if(_music_player.isPlaying())
				_music_player.control(MusicPlayer::ControlRequest::Pause);
			else
				_music_player.control(MusicPlayer::ControlRequest::Resume);
			break;
		case Next:
			_music_player.control(MusicPlayer::ControlRequest::Next);
			break;
		case Prev:
			_music_player.control(MusicPlayer::ControlRequest::Prev);
			break;
		case VolumeUp:
			_music_player.control(MusicPlayer::ControlRequest::VolumeUp);
			break;
		case VolumeDown:
			_music_player.control(MusicPlayer::ControlRequest::VolumeDown);
			break;
		default:
			break;
		}
		button_pressed = _button_poll.getNextPressedPin();
	}
}
