BUILD_DIR 	:= $(CURDIR)/build
SOURCE_DIR 	:= $(CURDIR)/src
INCLUDE_DIR	:= $(CURDIR)/include

NAME    := $(BUILD_DIR)/main
SRCS    := $(BUILD_DIR)/main.cpp
SRCS    += $(BUILD_DIR)/config.cpp
SRCS    += $(BUILD_DIR)/httpsclient.cpp
SRCS    += $(BUILD_DIR)/sftpclient.cpp
SRCS    += $(BUILD_DIR)/servercomm.cpp
SRCS    += $(BUILD_DIR)/songinfoparser.cpp
OBJS    := $(SRCS:.cpp=.o)

CC 		:= g++
FLAGS 	:= -Wall --std=c++11 -I $(INCLUDE_DIR)
LIBS 	:= -lpthread -lssl -lcrypto -ljsoncpp -lcurl

first: $(NAME)
	@echo "\nEverything is OK.\nExecute ./run.sh to run the program."

$(NAME): $(OBJS)
	cd $(BUILD_DIR) && \
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)
	
%.o: ../src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	
fclean: clean
	rm -rf $(NAME)

.PHONY: first clean fclean re

# $^ all
# $< first dependency
# $@ target