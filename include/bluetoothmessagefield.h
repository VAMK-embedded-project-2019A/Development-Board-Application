#ifndef BLUETOOTHMESSAGEFIELD_H
#define BLUETOOTHMESSAGEFIELD_H

#include <string>

namespace Bluetooth
{
	class OutMessageField
	{
	public:
		enum OutMessageFieldEnum : uint8_t
		{
			WifiStatus_Status,
			WifiInfo_ApListString,
			WifiConnect_Status,
			MusicControl_Status
			// TODO: what fields for MusicInfo and GpsInfo responses
		};
		OutMessageField() = default;
		constexpr OutMessageField(OutMessageFieldEnum enum_value) : _value(enum_value){}

		// overload conversion operator
		operator OutMessageFieldEnum() const { return _value; } // enable switch()
		explicit operator bool() = delete; // prevent if()

		constexpr bool operator==(OutMessageFieldEnum value) const { return _value == value; }
		constexpr bool operator!=(OutMessageFieldEnum value) const { return _value != value; }

		std::string toString() const;

	private:
		OutMessageFieldEnum _value;
	};

	class InMessageField
	{
	public:
		enum InMessageFieldEnum : uint8_t
		{
			WifiConnect_Username,
			WifiConnect_Password,
			WifiConnect_ESSID,
			MusicControl_Action,
			MusicControl_Value
		};
		InMessageField() = default;
		constexpr InMessageField(InMessageFieldEnum enum_value) : _value(enum_value){}

		// overload conversion operator
		operator InMessageFieldEnum() const { return _value; } // enable switch()
		explicit operator bool() = delete; // prevent if()

		constexpr bool operator==(InMessageFieldEnum value) const { return _value == value; }
		constexpr bool operator!=(InMessageFieldEnum value) const { return _value != value; }

		std::string toString() const;

	private:
		InMessageFieldEnum _value;
	};
};

#endif // BLUETOOTHMESSAGEFIELD_H