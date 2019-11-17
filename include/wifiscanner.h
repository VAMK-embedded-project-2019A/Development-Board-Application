#ifndef WIFISCANNER_H
#define WIFISCANNER_H

#include <string>
#include <mutex>
#include <vector>

struct AccessPoint
{
	std::string _MAC;
	int 		_channel;
	std::string _ESSID;
	int 		_dbm_strength;
};

class WifiScanner
{
public:
	WifiScanner() = default;
	
	void setInfoFile(const std::string &file_path);
	void start();
	
	std::vector<AccessPoint> getAccessPoints() const;
	bool isDone() const;
	
	std::mutex _mutex;

private:
	void readWifiInfo();

	bool _scan_done{true};
	
	std::vector<AccessPoint> _ap_list;
	std::string _info_file_path;
	const std::string _cmd{"iwlist wlan0 scan | grep -E 'Address|Frequency|ESSID|Quality'"};
	std::string _full_cmd;
};

#endif // WIFISCANNER_H