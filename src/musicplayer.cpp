#include "musicplayer.h"

#include <iostream>

bool MusicPlayer::isPlaying() const
{
	return _is_playing;
}

std::string MusicPlayer::getCurrentSong()
{
	return _current_song;
}

std::string MusicPlayer::getNextSong()
{
	return _next_song;
}

int MusicPlayer::getVolume()
{
	return _volume;
}

void MusicPlayer::play()
{
	if(_is_playing || _current_song.empty())
		return;

	std::cout << "MusicPlayer: " << "Play " << _current_song << std::endl;
	_is_playing = true;
}

void MusicPlayer::pause()
{
	if(_is_playing)
	{
		std::cout << "MusicPlayer: " << "Pause " << _current_song << std::endl;
		_is_playing = false;
	}
}

void MusicPlayer::next()
{
	if(_next_song.empty())
		return;

	if(!_current_song.empty())
	{
		while(_song_history.size() >= MAX_HISTORY)
			_song_history.erase(_song_history.begin());
		_song_history.push_back(_current_song);
	}

	pause();
	_current_song = _next_song;
	_next_song.clear();
	play();
}

void MusicPlayer::prev()
{
	if(_song_history.empty())
		return;

	pause();
	_current_song = _song_history.back();
	_song_history.pop_back();
	play();
}

void MusicPlayer::increaseVolume(const int amount)
{
	// non-reentrant
	if(_volume + amount <= 0)
	{
		_volume = 0;
		return;
	}
	
	if(_volume + amount >= MAX_VOLUME)
	{
		_volume = MAX_VOLUME;
		return;
	}
	
	_volume += amount;
	std::cout << "MusicPlayer: " << "IncreaseVolume " << _volume << " + " << amount << std::endl;
}

void MusicPlayer::decreaseVolume(const int amount)
{
	increaseVolume(-amount);
}

void MusicPlayer::setCurrentSong(const std::string &name)
{
	_current_song = name;
	std::cout << "MusicPlayer: " << "SetCurrentSong " << name << std::endl;
}

void MusicPlayer::setNextSong(const std::string &name)
{
	_next_song = name;
	std::cout << "MusicPlayer: " << "SetNextSong " << name << std::endl;
}
