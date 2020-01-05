#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include "wifiscanner.h"
#include "wificonnect.h"
#include <string>

class WifiHandler : public WifiScanner, public WifiConnect
{
public:
	WifiHandler() = default;
	
	bool isConnected();
	
private:
};

#endif // WIFIHANDLER_H