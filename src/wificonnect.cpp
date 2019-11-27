#include "wificonnect.h"

#include <iostream>
#include <fstream>

bool WifiConnect::connect(const std::string &essid, const std::string &password)
{
	if(password.empty() || essid.empty())
		return false;

	if(!writeNetworkInterface(essid))
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

bool WifiConnect::writeNetworkInterface(const std::string &essid)
{
	const std::string path{"/etc/network/interfaces"};
	std::ofstream interfaces;
	interfaces.open(path);
	if(!interfaces)
	{
		std::cout << "WifiConnect: Cannot open file " << path << std::endl;
		return false;
	}

	interfaces << "auto wlan0" << std::endl;
	interfaces << "allow-hotplug wlan0" << std::endl;
	interfaces << "iface wlan0 inet dhcp" << std::endl;
	interfaces << "\t" << "wpa-ssid " << essid << std::endl;
	interfaces << "\t" << "pre-up wpa_supplicant -B -Dwext -i wlan0 -c/etc/wpa_supplicant/wpa_supplicant.conf -f /var/log/wpa_supplicant.log" << std::endl;
	interfaces << "\t" << "post-down killall -q wpa_supplicant" << std::endl;
	interfaces.close();
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