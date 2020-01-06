#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include "wifiscanner.h"
#include "wificonnect.h"
#include <string>

//! Class to handle all wifi related operations.
/*!
  This class inherits from WifiScanner and WifiConnect.
*/
class WifiHandler : public WifiScanner, public WifiConnect
{
public:
	WifiHandler() = default;
	
	//! Check if there is internet connection.
	/*!
	  \return True if there is internet connection, false otherwise.
	*/
	bool isConnected();
	
private:
};

#endif // WIFIHANDLER_H