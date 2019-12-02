#ifndef BLUETOOTHCOMM_H
#define BLUETOOTHCOMM_H

#include "espp_bt_client.h"
#include "espp_bt_server.h"
using espp::BtServer;
using espp::BtClient;

#include <memory>

class BluetoothComm
{
public:
	BluetoothComm() = default;
	BluetoothComm(const BluetoothComm &) = delete;
	BluetoothComm operator=(const BluetoothComm &) = delete;

	void startAdvertising();
	bool getClient(); // blocking until get valid client
	std::string readFromClient(); // blocking until valid message or timeout
	bool writeToClient(const std::string &message);
	
private:
	BtServer _server;
	std::unique_ptr<BtClient> _client;
};

#endif // BLUETOOTHCOMM_H