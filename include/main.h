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

//! Class for main() to call upon. 
/*!
  This class is used as an intermediate for information exchange between submodules.
*/

class Main
{
public:
	//! Constructs a main object and initialize all submodules.
	/*!
	  If the configurations parsing failed, hasError() will return true.
	*/
	Main();
	~Main();
	
	//! GPIO pins for buttons.
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

	//! Start an infinite loop to handle all submodules.
	void start();
	//! Return true if configurations parsing failed, false otherwise.
	bool hasError() const;

private:
	/*!
	  If a getSong() async operation has not been started, start one. 
	  Return a song's file name if the getSong() async operation is done. Return an empty string otherwise.
	*/
	std::string requestGetSongResult();
	/*!
	  Repeatedly calling ServerComm::start() until a song's file name is retrieved.
	*/
	std::string getSong();
	/*!
	  Handle all buttons that has been recorded pressed by the ButtonPoll object.
	*/
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
