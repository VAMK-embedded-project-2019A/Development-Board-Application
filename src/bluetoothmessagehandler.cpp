#include "bluetoothmessagehandler.h"
using namespace Bluetooth;

#include <iostream>

MessageType BluetoothMessageHandler::parseMessage(const std::string &message)
{
	_root.clear();

	Json::Reader reader;
	if(!reader.parse(message, _root))
	{
		std::cout << "BluetoothMessageHandler: " << reader.getFormattedErrorMessages() << std::endl;
		_message_type = MessageType::None;
	}
	else
		// asString() returns empty string if key not exist
		_message_type = MessageType(_root["type"].asString());

	return _message_type; 
}

std::string BluetoothMessageHandler::getField(const InMessageField &field) const
{
	std::string return_string;
	switch(field)
	{
		case InMessageField::WifiConnect_Username:
		case InMessageField::WifiConnect_Password:
		case InMessageField::WifiConnect_ESSID:
			if(_message_type != MessageType::WifiConnect)
				break;
			return_string = _root[field.toString()].asString();
			break;
		case InMessageField::MusicControl_Action:
		case InMessageField::MusicControl_Value:
			if(_message_type != MessageType::MusicControl)
				break;
			return_string = _root[field.toString()].asString();
			break;
	}
	return return_string;
}