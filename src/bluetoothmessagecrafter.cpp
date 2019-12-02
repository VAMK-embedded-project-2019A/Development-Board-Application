#include "bluetoothmessagecrafter.h"

using namespace Bluetooth;

void BluetoothMessageCrafter::setMessageType(const MessageType &message_type)
{
	_message_type = message_type;
	_root["type"] = message_type.toString();
}

void BluetoothMessageCrafter::setField(const OutMessageField &field, const std::string &value_string)
{
	if(field != OutMessageField::WifiInfo_ApListString)
		_root[field.toString()] = value_string;

	// TODO: handle AP string
}