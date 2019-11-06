BUILD_DIR = build
FLAGS = -Wall

first: main

main: src/main.cpp
	g++ $(FLAGS) src/main.cpp -o $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)/*
