#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <map>

#define CONFIG_PATH "./../files/config.txt"
#define CONFIG_COUNT 10
enum ConfigEnum : uint8_t
{
	IP,
	HTTPS_PORT,
	SFTP_USERNAME,
	SFTP_PASSPHRASE,
	SFTP_KNOWNHOSTS,
	SFTP_PUBLICKEY,
	SFTP_PRIVATEKEY,
	SFTP_SAVEPATH,
	SFTP_SERVERPATH,
	WIFIINFO_PATH
};

std::string configEnumToString(const ConfigEnum& config_enum);
std::map<ConfigEnum, std::string> parseConfig();
std::string handleConfigLine(const std::string &line, int line_number);

#endif // CONFIG_H
