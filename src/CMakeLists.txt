#Add the given directories to those the compiler uses to search for include files. 
#Relative paths are interpreted as relative to the current source directory.
include_directories( "${PROJECT_SOURCE_DIR}/include" )

#Add a library to the project using the specified source files.
add_library(SOURCE_DIR
	button.cpp
	buttonpoll.cpp
	config.cpp
	httpsclient.cpp
	main.cpp
	musicplayer.cpp
	process.cpp
	servercomm.cpp
	sftpclient.cpp
	songinfoparser.cpp
	wifiscanner.cpp
)
