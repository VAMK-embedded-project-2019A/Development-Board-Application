#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <string>

class WifiConnect
{
public:
	WifiConnect() = default;
	
	bool connect(const std::string &essid, const std::string &password);
	bool disconnect();

private:
	bool writeNetworkInterface(const std::string &essid);
	bool writeWpaSupplicant(const std::string &essid, const std::string &password);
};

#endif // WIFICONNECT_H