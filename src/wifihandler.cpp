#include "wifihandler.h"

#include <cstdio>	// popen(), pclose()

bool WifiHandler::isConnected()
{
	std::string cmd;
	cmd += "ping -c 1 1.1.1.1";		// ping 1 time to 1.1.1.1
	cmd += " | grep 'packet loss'";
	cmd += " | cut -d ',' -f3";
	cmd += " | cut -d ' ' -f2";
	cmd += " | cut -d '%' -f1";

	FILE *output = popen(cmd.c_str(), "r" /* read */);
	if(output == nullptr)
		return false;

	unsigned int percent_packet_loss;
	if(std::fscanf(output, "%u", &percent_packet_loss) == EOF)
		return false;

	return (percent_packet_loss == 0);
}