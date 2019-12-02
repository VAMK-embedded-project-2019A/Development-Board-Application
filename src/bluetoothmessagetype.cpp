#include "bluetoothmessagetype.h"

using namespace Bluetooth;

MessageType::MessageType(const std::string &enum_string)
{
	// cannot switch on std::string
	if(enum_string == "WifiStatus")
		_value = MessageTypeEnum::WifiStatus;
	else if(enum_string == "WifiInfo")
		_value = MessageTypeEnum::WifiInfo;
	else if(enum_string == "WifiConnect")
		_value = MessageTypeEnum::WifiConnect;
	else if(enum_string == "MusicControl")
		_value = MessageTypeEnum::MusicControl;
	else if(enum_string == "MusicInfo")
		_value = MessageTypeEnum::MusicInfo;
	else if(enum_string == "GpsInfo")
		_value = MessageTypeEnum::GpsInfo;
	else
		_value = MessageTypeEnum::None;
}

std::string MessageType::toString() const
{
	switch(_value)
	{
		case WifiStatus:
			return "WifiStatus";
		case WifiInfo:
			return "WifiInfo";
		case WifiConnect:
			return "WifiConnect";
		case MusicControl:
			return "MusicControl";
		case MusicInfo:
			return "MusicInfo";
		case GpsInfo:
			return "GpsInfo";
		default:
			return "None";
	}
}