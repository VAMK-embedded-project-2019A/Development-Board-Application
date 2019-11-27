#ifndef MAIN_H
#define MAIN_H

#include <utility>
#include <future>

#include "config.h"
#include "process.h"
#include "musicplayer.h"
#include "buttonpoll.h"
#include "wifihandler.h"
#include "servercomm.h"

class Main : public Process
{
public:
	Main();
	~Main();
	enum ButtonPins : int
	{
		PlayPause	= 4,
		Next		= 27,
		Prev		= 22,
		VolumeUp	= 6,
		VolumeDown	= 13,
	};
	
	void start() override;

private:
	std::string getSong();
	void handleButtonPressed(int pin);

	std::map<ConfigEnum, std::string> _config_map;
	ServerComm _server_comm;
	MusicPlayer _music_player;
	ButtonPoll _button_poll;
	WifiHandler _wifi_handler;
	
	std::future<void> _future_button_poll;
};

#endif // MAIN_H