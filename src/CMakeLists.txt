include_directories( "${CURDIR}/include" )

add_library(SOURCE_DIR
	button.cpp
	buttonpoll.cpp
	config.cpp
	httpsclient.cpp
	main.cpp
	musicplayer.cpp
	process.cpp	add
	servercomm.cpp
	sftpclient.cpp
	songinfoparser.cpp
	wifiscanner.cpp
)