#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <mutex>
#include <vector>

#include "mpg123.h"
#include "out123.h"

//! This class wraps the mpg123 library and provides interfaces to control the music player. 
class MusicPlayer
{
public:
	//! Create a music player
	/*!
	  Initialize the handles for the mpg123 library. If the initialization fails, hasError() will return true.
	*/
	MusicPlayer();
	//! Destroy the music player, release the handles.
	~MusicPlayer();

	enum ControlRequest
	{
		Play,
		Stop,
		Pause,
		Resume,
		Next,
		Prev,
		VolumeUp,
		VolumeDown,
		None
	};

	//! An infinite loop to handle control requests.
	/*!
	  Whenever there is an error, this function will return immediately.
	*/
	void start();
	//! Check if the music player is playing.
	/*!
	  \return True if the music player is playing, false otherwise.
	*/
	bool isPlaying();
	//! Check if any error occurs.
	/*!
	  \return True if there is an error, false otherwise.
	*/
	bool hasError();
	//! Get the current song playing or to be played.
	/*!
	  \return The file name, or an empty string if none has been set.
	*/
	std::string getCurrentSong();
	//! Get the next song to be played.
	/*!
	  \return The file name, or an empty string if none has been set.
	*/
	std::string getNextSong();
	//! Get the current volume.
	/*!
	  \return The volume ranging from 0 to 100, or -1 if there is an error.
	*/
	int getVolume();

	//! Set current song name.
	void setCurrentSong(const std::string &name);
	//! Set next song name.
	void setNextSong(const std::string &name);
	//! Control the music player.
	/*!
	  \return True if the request is acknowledged, false if there is already an unhandled request.
	*/
	bool control(ControlRequest request);

private:
	//! Set _error to \p error.
	void setError(bool error);
	//! Set _is_playing to \p is_playing.
	void setIsPlaying(bool is_playing);
	//! Get the saved control request and reset it back to ControlRequest::None.
	ControlRequest getRequest();
	//! Open the library handles. If the anything fails, hasError() will return true.
	void loadSong();

	//! Play the current song, if there is no current song, play the waiting track once. Do nothing if the handles are already opened.
	void handlePlayRequest();
	//! Close the library handles. Do nothing if the handles are already closed.
	void handleStopRequest();
	//! Handle next request.
	void handleNextRequest();
	//! Handle prev request. Do nothing if there is no previous song.
	void handlePrevRequest();

	// handles and lib properties
	mpg123_handle *_mpg_handle = nullptr;
	out123_handle *_out_handle = nullptr;
	size_t _out_block_size;
	unsigned char *_out_block{nullptr};

	// properties that can be accessed from many threads
	std::string _current_song;
	std::string _next_song;
	ControlRequest _control_request{None};
	bool _is_playing{false};
	bool _error{false};
	std::mutex _mutex;

	// private
	bool _handles_opened{false};
	std::vector<std::string> _song_history;
	const unsigned int MAX_HISTORY{10};
	const double MAX_VOLUME{0.5};
	const double VOLUME_PRECISION{0.005};
	const std::string FILE_PATH{"./../files/songs/"};
	const std::string WAITING_TRACK{"Default-Download.mp3"};
	bool _waiting_next_song{false};
};

#endif
