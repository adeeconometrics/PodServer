# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Include directories
INC_FLAGS := -I$(INCLUDE_DIR)

# Targets and their dependencies
CLIENT_SRC := $(SRC_DIR)/client_main.cpp $(SRC_DIR)/tcpclient.cpp
SERVER_SRC := $(SRC_DIR)/server_main.cpp $(SRC_DIR)/tcpserver.cpp

CLIENT_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CLIENT_SRC))
SERVER_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SERVER_SRC))

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c -o $@ $<

# All target: build both client and server
all: client server

# Rule to build client executable
client: $(CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/client $(CLIENT_OBJ)

# Rule to build server executable
server: $(SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/server $(SERVER_OBJ)

# Clean up generated files
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: client server clean
