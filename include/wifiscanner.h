#ifndef WIFISCANNER_H
#define WIFISCANNER_H

#include <string>
#include <vector>

//! Struct to store access point info.
struct AccessPoint
{
	std::string	_MAC;
	int 		_channel;
	std::string	_ESSID;
	int 		_dbm_strength;
	std::string	_auth_suite{"None"};
};

//! Helper to print out AccessPoint.
std::ostream& operator<<(std::ostream& stream, const AccessPoint& access_point);

//! Class to scan for available wifi access points.
class WifiScanner
{
public:
	WifiScanner() = default;
	
	//! Set the destination file for the scan info.
	void setInfoFile(const std::string &file_path);
	//! Start scanning.
	/*!
	  The info file should be already set using setInfoFile().
	  Return true if there is no error, false otherwise.
	  \sa setInfoFile().
	*/
	bool scan();
	
	//! Get a vector of AccessPoint objects that has been scanned from the last call of scan().
	/*!
	  Return an empty vector if any error happens.
	  \sa scan()
	*/
	std::vector<AccessPoint> getAccessPoints() const;

private:
	std::string getFullCmd() const;
	std::string getInfoFilePath() const;

	//! Read the wifi info file and create a vector of AccessPoint objects.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool readWifiInfo();
	//! Check if \p line is the first line of a new access point from the scan output.
	bool isNewWifiLine(const std::string &line);
	//! Get the MAC address of the access point from \p line and store it to \p access_point.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool getMAC(const std::string &line, AccessPoint &access_point);
	//! Get the operating channel of the access point from \p line and store it to \p access_point.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool getChannel(const std::string &line, AccessPoint &access_point);
	//! Get the strength of the access point from \p line and store it to \p access_point.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool getStrength(const std::string &line, AccessPoint &access_point);
	//! Get the ESSID of the access point from \p line and store it to \p access_point.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool getESSID(const std::string &line, AccessPoint &access_point);
	//! Get the authentication suite of the access point from \p line and store it to \p access_point.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool getAuthSuite(const std::string &line, AccessPoint &access_point);
	
	std::vector<AccessPoint> _ap_list;
	std::string _info_file_path;
	const std::string _cmd{"iwlist wlan0 scan | grep -E 'Address|Frequency|ESSID|Quality|Authentication Suites'"};
	std::string _full_cmd;
};

#endif // WIFISCANNER_H