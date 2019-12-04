#Add the given directories to those the compiler uses to search for include files.
#Relative paths are interpreted as relative to the current source directory.
include_directories( "${PROJECT_SOURCE_DIR}/include" )

#Add a library to the project using the specified source files.
add_library(SOURCE_DIR
        bluetoothcomm.cpp
        bluetoothmessagecrafter.cpp
        bluetoothmessagefield.cpp
        bluetoothmessagehandler.cpp
        bluetoothmessagetype.cpp
        button.cpp
        buttonpoll.cpp
        config.cpp
        espp_bt_client.cpp
        espp_bt_server.cpp
        httpsclient.cpp
        main.cpp
        musicplayer.cpp
        servercomm.cpp
        sftpclient.cpp
        songinfoparser.cpp
        vamk_aes.cpp
        vamk_random.cpp
        vamk_rfcomm.cpp
        vamk_rsa.cpp
        vamk_sdp.cpp
        vamk_socket.cpp
        wificonnect.cpp
        wifihandler.cpp
        wifiscanner.cpp

)

add_executable(main main.cpp)
