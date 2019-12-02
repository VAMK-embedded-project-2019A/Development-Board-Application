#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <vector>
#include <string>

class MusicPlayer
{
public:
	MusicPlayer() = default;
	
	bool isPlaying() const;
	std::string getCurrentSong();
	std::string getNextSong();
	int getVolume();
	
	void play();
	void pause();
	void next();
	void prev();
	void increaseVolume(const int amount = 1);
	void decreaseVolume(const int amount = 1);
	void setCurrentSong(const std::string &name);
	void setNextSong(const std::string &name);
	
private:
	std::string _current_song;
	std::string _next_song;
	std::vector<std::string> _song_history;
	int _volume{5};
	bool _is_playing{false};
	
	const unsigned int MAX_HISTORY{10};
	const int MAX_VOLUME{10};
};

#endif // MUSICPLAYER_H