#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <string>

//! Class to handle connection/disconnection to wifi access points.
class WifiConnect
{
public:
	WifiConnect() = default;
	
	//! Connect to the wifi access point named \p essid, using the password \p password.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool connect(const std::string &essid, const std::string &password);
	//! Disconnect from the current connecting wifi access point.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool disconnect();

private:
	//! Write the access point info to wpa supplicant.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool writeWpaSupplicant(const std::string &essid, const std::string &password);
};

#endif // WIFICONNECT_H