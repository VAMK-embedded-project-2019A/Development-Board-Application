#ifndef SONGINFOPARSER_H
#define SONGINFOPARSER_H

#include <string>
#include <vector>
#include <iostream>

//! Struct to store music track info.
struct SongInfo
{
	std::string _name;
	std::string _file_name;
	std::string _tag;
};

//! Helper to print out SongInfo.
std::ostream& operator<<(std::ostream& stream, const SongInfo& song_info);

//! This class is created to parse JSON response from server into SongInfo objects.
class SongInfoParser
{
public:
	SongInfoParser() = default;

	//! Parse in JSON data from \p data.
	/*!
	  Return true if there is no error, false otherwise.
	*/
	bool parseData(const std::string &data);
	//! Get a vector of SongInfo objects that has been parsed from the last call of parseData().
	/*!
	  Return an empty vector if nothing has been parsed.
	  \sa parseData()
	*/
	std::vector<SongInfo> getSongs() const;
	
private:
	std::vector<SongInfo> _songs;
};

#endif // SONGINFOPARSER_H