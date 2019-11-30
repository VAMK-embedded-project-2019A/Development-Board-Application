#include "bluetoothcomm.h"

#include <vector>
#include <algorithm>

void BluetoothComm::startAdvertising()
{
	_server.start();
}

bool BluetoothComm::getClient()
{
	// blocking until get valid client
	if(!_client)
		_client = _server.accept();
	return _client->exchangeKey();
}

std::string BluetoothComm::readFromClient()
{
	if(!_client)
		return std::string{};

	std::vector<char> container;
	// blocking until valid message or timeout
	if(_client->read(container) == -1)
	{
		_client.reset();
		return std::string{};
	}
	return std::string(container.begin(), container.end());
}

bool BluetoothComm::writeToClient(const std::string &message)
{
	if(!_client)
		return false;

	std::vector<char> data;
	std::copy(message.begin(), message.end(), std::back_inserter(data));
	return (_client->write(data) != -1);
}