#include "config.h"

#include <fstream>
using namespace std;

std::string ConfigEnumToString(const ConfigEnum& config_enum)
{
	switch(config_enum)
	{
		case IP:
			return "IP";
		case HTTPS_PORT:
			return "HTTPS_PORT";
		case SFTP_USERNAME:
			return "SFTP_USERNAME";
		case SFTP_PASSWORD:
			return "SFTP_PASSWORD";
		case SFTP_KNOWNHOSTS:
			return "SFTP_KNOWNHOSTS";
		case SFTP_PUBLICKEY:
			return "SFTP_PUBLICKEY";
		case SFTP_PRIVATEKEY:
			return "SFTP_PRIVATEKEY";
		case SFTP_SAVEPATH:
			return "SFTP_SAVEPATH";
		case SFTP_SERVERPATH:
			return "SFTP_SERVERPATH";
		case WIFIINFO_PATH:
			return "WIFIINFO_PATH";
	}
	return {};
}

map<ConfigEnum, string> parseConfig()
{
	ifstream config_stream(CONFIG_PATH);
	if(!config_stream.is_open())
	{
		cout << "Error: Cannot open " << CONFIG_PATH << endl;
		return {};
	}
	
	map<ConfigEnum, string> config_map;
	for(int i=0; i<CONFIG_COUNT; i++)
	{
		string line;
		getline(config_stream, line);
		string config_value = handleConfigLine(line, i);
		if(config_value.empty())
			return {};
		
		config_map.insert({static_cast<ConfigEnum>(i), config_value});
	}
	return config_map;
}

string handleConfigLine(const string &line, int line_number)
{
	string separator{"\t"};
	auto separator_pos = line.find(separator);
	if(separator_pos == string::npos)
	{
		cout << "Error: Config value not found in config.txt line " << line_number << endl;
		return {};
	}
	
	string config_key = line.substr(0, separator_pos);
	if(config_key != ConfigEnumToString(static_cast<ConfigEnum>(line_number)))
	{
		cout << "Error: Config key mismatch in config.txt line " << line_number << endl;
		return {};
	}
	
	string config_value = line.substr(separator_pos + separator.length());
	if(*config_value.rbegin() == '\r')
		config_value.pop_back();
	return config_value;
}