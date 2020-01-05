#include "bluetoothmessagecrafter.h"
#include "wifiscanner.h"

using namespace Bluetooth;

void BluetoothMessageCrafter::setMessageType(const MessageType &message_type)
{
	_message_type = message_type;
	_root["type"] = message_type.toString();
}

void BluetoothMessageCrafter::setField(const OutMessageField &field, const std::string &value_string)
{
	switch(field)
	{
		case OutMessageField::WifiStatus_Status:
			if(_message_type != MessageType::WifiStatus)
				return;
			break;
		case OutMessageField::WifiConnect_Status:
			if(_message_type != MessageType::WifiConnect)
				return;
			break;
		case OutMessageField::MusicControl_Status:
			if(_message_type != MessageType::MusicControl)
				return;
			break;
		default:
			return;
	}
	_root[field.toString()] = value_string;
}

void BluetoothMessageCrafter::setField(const OutMessageField &field, const AccessPoint &access_point)
{
	if(field != OutMessageField::WifiInfo_ApListString)
		return;

	_root["essid"]		= access_point._ESSID;
	_root["auth_suite"]	= access_point._auth_suite;
	_root["strength"]	= access_point._dbm_strength;
}