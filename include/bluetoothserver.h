#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include "process.h"

class BluetoothServer : public Process
{
public:
	BluetoothServer() = default;
	
	void start()
	{
		while(true)
		{
			read()
			write()
		}
	}
	
	
	bool isConnectRequested() const;
	socket getClient();

private:
	exchangeKey();
	void stop();
	
	bool isSendRequested;
	bool dataAvailable;
	std::string json_data;
};

;

#endif BLUETOOTHSERVER_H