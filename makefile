BUILD_DIR = build
INCLUDE_DIR = include
FLAGS = -Wall --std=c++11 -I $(INCLUDE_DIR)
CPPFILES = src/main.cpp src/config.cpp src/httpsclient.cpp src/sftpclient.cpp src/servercomm.cpp src/songinfoparser.cpp
LIBS = -lpthread -lssl -lcrypto -ljsoncpp -lcurl

first: main

main: $(CPPFILES)
	g++ $(FLAGS) $(CPPFILES) -o $(BUILD_DIR)/main $(LIBS)

clean:
	rm -rf $(BUILD_DIR)/*
