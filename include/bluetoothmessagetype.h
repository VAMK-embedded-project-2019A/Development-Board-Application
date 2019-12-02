#ifndef BLUETOOTHMESSAGETYPE_H
#define BLUETOOTHMESSAGETYPE_H

#include <string>

namespace Bluetooth
{
	class MessageType
	{
	public:
		enum MessageTypeEnum : uint8_t
		{
			WifiStatus,
			WifiInfo,
			WifiConnect,
			MusicControl,
			MusicInfo,
			GpsInfo,
			None
		};
		MessageType() = default;
		constexpr MessageType(MessageTypeEnum enum_value) : _value(enum_value){}
		MessageType(const std::string &enum_string);

		// overload conversion operator
		operator MessageTypeEnum() const { return _value; } // enable switch()
		explicit operator bool() = delete; // prevent if()

		constexpr bool operator==(MessageTypeEnum value) const { return _value == value; }
		constexpr bool operator!=(MessageTypeEnum value) const { return _value != value; }

		std::string toString() const;

	private:
		MessageTypeEnum _value;
	};
};

#endif // BLUETOOTHMESSAGETYPE_H