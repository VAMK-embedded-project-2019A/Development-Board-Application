#ifndef MAIN_H
#define MAIN_H

#include <utility>
#include <future>

#include "config.h"
#include "musicplayer.h"
#include "buttonpoll.h"
#include "wifihandler.h"
#include "servercomm.h"
#include "bluetoothcomm.h"

#define RASP_PI

class Main
{
public:
	Main();
	~Main();
	enum ButtonPins : uint8_t
	{
#ifdef RASP_PI
		PlayPause	= 4,
		Next		= 27,
		Prev		= 22,
		VolumeUp	= 6,
		VolumeDown	= 13,
#else
		PlayPause	= 106,
		Next		= 107,
		Prev		= 180,
		VolumeUp	= 181,
		VolumeDown	= 172,
#endif
	};

	void start();
	bool hasError() const;

private:
	std::string requestGetSongResult();
	std::string getSong();
	void handleButtonPoll();
	void handleBluetoothComm();

	bool _error{false};

	std::map<ConfigEnum, std::string>	_config_map;
	ServerComm							_server_comm;
	BluetoothComm						_bluetooth_comm;
	MusicPlayer							_music_player;
	ButtonPoll							_button_poll;
	WifiHandler							_wifi_handler;
	
	std::future<void>					_future_button_poll;
	std::future<std::string>			_future_get_song;
	std::future<bool>					_future_get_bt_client;
	std::future<std::string>			_future_read_bt_message;
};

#endif // MAIN_H
