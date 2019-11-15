BUILD_DIR 	:= $(CURDIR)/build
SOURCE_DIR 	:= $(CURDIR)/src
INCLUDE_DIR	:= $(CURDIR)/include

NAME    := main
SRCS    := $(SOURCE_DIR)/main.cpp
SRCS    += $(SOURCE_DIR)/config.cpp
SRCS    += $(SOURCE_DIR)/httpsclient.cpp
SRCS    += $(SOURCE_DIR)/sftpclient.cpp
SRCS    += $(SOURCE_DIR)/servercomm.cpp
SRCS    += $(SOURCE_DIR)/songinfoparser.cpp
OBJS    := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CC 		:= g++
FLAGS 	:= -Wall --std=c++11 -I $(INCLUDE_DIR)
LIBS 	:= -lpthread -lssl -lcrypto -ljsoncpp -lcurl

first: $(NAME)
	@echo "\nEverything is OK.\nExecute ./run.sh to run the program."

$(NAME): $(OBJS)
	cd $(BUILD_DIR) && \
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)
	
$(OBJS): $(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	
fclean: clean
	rm -rf $(BUILD_DIR)/$(NAME)

.PHONY: first clean fclean

# $^ all dependencies
# $< first dependency
# $@ target