BUILD_DIR = build
INCLUDE_DIR = include
FLAGS = -Wall -I $(INCLUDE_DIR)

first: main

main: src/main.cpp src/config.cpp
	g++ $(FLAGS) src/main.cpp src/config.cpp -o $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)/*
