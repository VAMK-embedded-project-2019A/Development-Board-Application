BUILD_DIR			:= $(CURDIR)/build
SOURCE_DIR			:= $(CURDIR)/src
INCLUDE_DIR			:= $(CURDIR)/include

TEST_DIR			:= $(CURDIR)/test
GOOGLETEST_DIR		:= $(TEST_DIR)/googletest

NAME    := main
SRCS    := $(SOURCE_DIR)/config.cpp
SRCS    += $(SOURCE_DIR)/bluetoothcomm.cpp
SRCS    += $(SOURCE_DIR)/httpsclient.cpp
SRCS    += $(SOURCE_DIR)/sftpclient.cpp
SRCS    += $(SOURCE_DIR)/servercomm.cpp
SRCS    += $(SOURCE_DIR)/songinfoparser.cpp
SRCS    += $(SOURCE_DIR)/wifihandler.cpp
SRCS    += $(SOURCE_DIR)/wificonnect.cpp
SRCS    += $(SOURCE_DIR)/wifiscanner.cpp
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
SRCS    += $(SOURCE_DIR)/bluetoothmessagehandler.cpp
SRCS    += $(SOURCE_DIR)/bluetoothmessagecrafter.cpp
SRCS    += $(SOURCE_DIR)/bluetoothmessagetype.cpp
SRCS    += $(SOURCE_DIR)/bluetoothmessagefield.cpp
OBJS    := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CXX		:= g++
FLAGS	:= -Wall --std=c++11
LIBS	:= -lpthread -lssl -lcrypto -ljsoncpp -lcurl -lbluetooth

#------------------Main--------------------------------------------------------

first: $(NAME)
	@echo ""
	@echo "Everything is OK."
	@echo "Execute ./run.sh to run the program."

$(NAME): $(OBJS) $(BUILD_DIR)/main.o
	cd $(BUILD_DIR) && \
	$(CXX) $(FLAGS) $^ -o $@ $(LIBS)

$(OBJS): $(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(FLAGS) -I $(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/main.o: $(SOURCE_DIR)/main.cpp
	$(CXX) $(FLAGS) -I $(INCLUDE_DIR) -c $< -o  $@

#------------------Test--------------------------------------------------------

TEST_CASE_SRCS	:= $(TEST_DIR)/sample_test_file_1.cpp
TEST_CASE_SRCS	+= $(TEST_DIR)/sample_test_file_2.cpp
TEST_CASE_OBJS	:= $(patsubst %.cpp, %.o, $(TEST_CASE_SRCS))

TEST_SRCS		:= $(SRCS)
TEST_OBJS		:= $(patsubst $(SOURCE_DIR)/%.cpp, $(TEST_DIR)/%.o, $(TEST_SRCS))

test: $(TEST_DIR)/libgtest.so $(TEST_CASE_OBJS) $(TEST_OBJS) $(TEST_DIR)/gtest_main.o
	cd $(TEST_DIR) && \
	$(CXX) $(FLAGS) --coverage -L $(TEST_DIR) $(TEST_CASE_OBJS) $(TEST_DIR)/gtest_main.o $(TEST_OBJS) -o $@ -lgtest $(LIBS)
	@echo ""
	@echo "Everything is OK."
	@echo "Execute 'LD_LIBRARY_PATH=./test ./test/test' to run the test."

$(TEST_CASE_OBJS): $(TEST_DIR)/%.o : $(TEST_DIR)/%.cpp
	$(CXX) $(FLAGS) -I $(TEST_DIR) -I $(INCLUDE_DIR) -c $< -o $@

$(TEST_OBJS): $(TEST_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(FLAGS) --coverage -I $(INCLUDE_DIR) -c $< -o $@

$(TEST_DIR)/gtest_main.o: $(TEST_DIR)/gtest_main.cpp
	$(CXX) $(FLAGS) -I $(TEST_DIR) -c $< -o  $@

$(TEST_DIR)/libgtest.so:
	# download googletest to ./test/googletest
	wget -P $(TEST_DIR) https://github.com/google/googletest/archive/release-1.10.0.tar.gz && \
	mkdir $(GOOGLETEST_DIR)
	tar xf $(TEST_DIR)/release-1.10.0.tar.gz -C $(GOOGLETEST_DIR) --strip-components 1

	# build shared library
	cd $(GOOGLETEST_DIR) && \
	cmake -D BUILD_SHARED_LIBS=ON . && \
	make

	# gather needed components
	cp $(GOOGLETEST_DIR)/lib/libgtest.so $(TEST_DIR)
	cp -r $(GOOGLETEST_DIR)/googletest/include/gtest $(TEST_DIR)
	
	# clean up
	rm -rf $(GOOGLETEST_DIR)
	rm -rf $(TEST_DIR)/*.tar.gz

#------------------Others------------------------------------------------------

clean:
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(TEST_DIR)/*.o
	
fclean: clean
	rm -rf $(BUILD_DIR)/$(NAME)
	rm -rf $(TEST_DIR)/test

.PHONY: first test clean fclean

# $^ all dependencies
# $< first dependency
# $@ target
