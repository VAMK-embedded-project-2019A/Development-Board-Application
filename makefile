BUILD_DIR	:= $(CURDIR)/build
SOURCE_DIR	:= $(CURDIR)/src
INCLUDE_DIR	:= $(CURDIR)/include

NAME    := main
SRCS    := $(SOURCE_DIR)/main.cpp
SRCS    += $(SOURCE_DIR)/config.cpp
SRCS    += $(SOURCE_DIR)/httpsclient.cpp
SRCS    += $(SOURCE_DIR)/sftpclient.cpp
SRCS    += $(SOURCE_DIR)/servercomm.cpp
SRCS    += $(SOURCE_DIR)/songinfoparser.cpp
SRCS    += $(SOURCE_DIR)/wifihandler.cpp
SRCS    += $(SOURCE_DIR)/wificonnect.cpp
SRCS    += $(SOURCE_DIR)/wifiscanner.cpp
SRCS    += $(SOURCE_DIR)/process.cpp
SRCS    += $(SOURCE_DIR)/musicplayer.cpp
SRCS    += $(SOURCE_DIR)/buttonpoll.cpp
SRCS    += $(SOURCE_DIR)/button.cpp
SRCS    += $(SOURCE_DIR)/espp_bt_client.cpp
SRCS    += $(SOURCE_DIR)/espp_bt_server.cpp
SRCS    += $(SOURCE_DIR)/vamk_aes.cpp
SRCS    += $(SOURCE_DIR)/vamk_random.cpp
SRCS    += $(SOURCE_DIR)/vamk_rfcomm.cpp
SRCS    += $(SOURCE_DIR)/vamk_rsa.cpp
SRCS    += $(SOURCE_DIR)/vamk_sdp.cpp
SRCS    += $(SOURCE_DIR)/vamk_socket.cpp
OBJS    := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CC	:= g++
FLAGS	:= -Wall --std=c++11 -I $(INCLUDE_DIR)
LIBS	:= -lpthread -lssl -lcrypto -ljsoncpp -lcurl -lbluetooth

first: $(NAME)
	@echo "\nEverything is OK.\nExecute ./run.sh to run the program."

$(NAME): $(OBJS)
	cd $(BUILD_DIR) && \
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

# TODO: this makes the makefile relinks everytime
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
