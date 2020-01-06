#include "wificonnect.h"

#include <iostream>
#include <fstream>

bool WifiConnect::connect(const std::string &essid, const std::string &password)
{
	if(password.empty() || essid.empty())
		return false;

	if(!writeWpaSupplicant(essid, password))
		return false;
	return true;
}

bool WifiConnect::disconnect()
{
	// TODO
	return true;
}

bool WifiConnect::writeWpaSupplicant(const std::string &essid, const std::string &password)
{
	const std::string path{"/etc/wpa_supplicant/wpa_supplicant.conf"};
	std::ofstream wpa;
	wpa.open(path);
	if(!wpa)
	{
		std::cout << "WifiConnect: Cannot open file " << path << std::endl;
		return false;
	}
	
	wpa << "ctrl_interface=/var/run/wpa_supplicant" << std::endl;
	wpa << "network={" << std::endl;
	wpa << "\t" << "ssid=\"" << essid << "\"" << std::endl;
	wpa << "\t" << "psk=\"" << password << "\"" << std::endl;
	wpa << "\t" << "key_mgmt=WPA-PSK" << std::endl;
	wpa << "}" << std::endl;
	wpa.close();
	return true;
}