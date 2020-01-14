#ifndef MAIN_H
#define MAIN_H

#include <utility>
#include <future>

#include "config.h"
#include "musicplayer.h"
#include "buttonpoll.h"
#include "wifihandler.h"
#include "servercomm.h"

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
		PlayPause	= 4,
		Next		= 27,
		Prev		= 22,
		VolumeUp	= 6,
		VolumeDown	= 13,
	};

	//! Start an infinite loop to handle all submodules.
	void start();
	//! Check if there is any error happens.
	/*!
	  \return True if configurations parsing failed, false otherwise.
	*/
	bool hasError() const;

private:
	/*!
	  If a getSong() async operation has not been started, start one. 
	  \return A song's file name if the getSong() async operation is done, an empty string otherwise.
	*/
	std::string requestGetSongResult();
	//! Repeatedly calling ServerComm::start() until a song's file name is retrieved.
	/*!
	  \return A non-empty song's file name.
	*/
	std::string getSong();
	//! Handle all buttons that has been recorded pressed by the ButtonPoll object.
	void handleButtonPoll();

	bool _error{false};

	std::map<ConfigEnum, std::string>	_config_map;
	ServerComm							_server_comm;
	MusicPlayer							_music_player;
	ButtonPoll							_button_poll;
	WifiHandler							_wifi_handler;
	
	std::future<void>					_future_button_poll;
	std::future<void>					_future_music_player;
	std::future<std::string>			_future_get_song;
};

#endif // MAIN_H
