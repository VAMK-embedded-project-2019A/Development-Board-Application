#ifndef WIFISCANNER_H
#define WIFISCANNER_H

#include <string>
#include <mutex>
#include <vector>

struct AccessPoint
{
	std::string	_MAC;
	int 		_channel;
	std::string	_ESSID;
	int 		_dbm_strength;
	std::string	_auth_suite{"None"};
};

std::ostream& operator<<(std::ostream& stream, const AccessPoint& access_point);

class WifiScanner
{
public:
	WifiScanner() = default;
	
	void setInfoFile(const std::string &file_path);
	void start();
	
	std::vector<AccessPoint> getAccessPoints() const;
	bool isDone() const;
	bool isError() const;
	
	std::mutex _mutex;

private:
	bool readWifiInfo();
	bool isNewWifiLine(const std::string &line);
	bool getMAC(const std::string &line, AccessPoint &access_point);
	bool getChannel(const std::string &line, AccessPoint &access_point);
	bool getStrength(const std::string &line, AccessPoint &access_point);
	bool getESSID(const std::string &line, AccessPoint &access_point);
	bool getAuthSuite(const std::string &line, AccessPoint &access_point);

	bool _scan_done{true};
	bool _error{false};
	
	std::vector<AccessPoint> _ap_list;
	std::string _info_file_path;
	const std::string _cmd{"iwlist wlan0 scan | grep -E 'Address|Frequency|ESSID|Quality|Authentication Suites'"};
	std::string _full_cmd;
};

#endif // WIFISCANNER_H