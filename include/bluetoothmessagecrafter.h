#ifndef BLUETOOTHMESSAGECRAFTER_H
#define BLUETOOTHMESSAGECRAFTER_H

#include <string>
#include <jsoncpp/json/json.h>

#include "bluetoothmessagetype.h"
#include "bluetoothmessagefield.h"

class AccessPoint;

namespace Bluetooth
{
	class BluetoothMessageCrafter
	{
	public:
		BluetoothMessageCrafter() = default;

		void setMessageType(const MessageType &message_type);
		void setField(const OutMessageField &field, const std::string &value_string);
		void setField(const OutMessageField &field, const AccessPoint &access_point);

	private:
		Json::Value _root;
		MessageType _message_type;
	};
};

#endif // BLUETOOTHMESSAGECRAFTER_H