#ifndef BLUETOOTHMESSAGEHANDLER_H
#define BLUETOOTHMESSAGEHANDLER_H

#include <string>
#include <jsoncpp/json/json.h>

#include "bluetoothmessagetype.h"
#include "bluetoothmessagefield.h"

namespace Bluetooth
{
	class BluetoothMessageHandler
	{
	public:
		BluetoothMessageHandler() = default;

		MessageType parseMessage(const std::string &message);
		std::string getField(const InMessageField &field) const;

	private:

		Json::Value _root;
		MessageType _message_type;
	};
};

#endif // BLUETOOTHMESSAGEHANDLER_H