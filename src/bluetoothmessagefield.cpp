#include "bluetoothmessagefield.h"

using namespace Bluetooth;

std::string OutMessageField::toString() const
{
	switch(_value)
	{
		case WifiStatus_Status:
		case WifiConnect_Status:
		case MusicControl_Status:
			return "status";
		case WifiInfo_ApListString:
			return "access_points";
		default:
			return "ERROR";
	}
}

std::string InMessageField::toString() const
{
	switch(_value)
	{
		case WifiConnect_Username:
			return "username";
		case WifiConnect_Password:
			return "password";
		case WifiConnect_ESSID:
			return "essid";
		case MusicControl_Action:
			return "action";
		case MusicControl_Value:
			return "value";
		default:
			return "ERROR";
	}
}