#ifndef MAIN_H
#define MAIN_H

#include <utility>

#include "config.h"
#include "process.h"

class Main : public Process
{
public:
	Main();
	~Main();
	void start() override;

private:
	std::pair<float, float> getGpsLocation() const;
	std::string getNextSong() const;
	bool isWifiConnected() const;
	void connectWifi();

	std::map<ConfigEnum, std::string> _config_map;
};

#endif // MAIN_H