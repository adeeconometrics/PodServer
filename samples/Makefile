# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Find all .cpp files in the source directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Create corresponding .o file paths
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Identify main source files and create corresponding binary file paths
MAIN_SRC_FILES := $(wildcard $(SRC_DIR)/*_main.cpp)
BIN_FILES := $(patsubst $(SRC_DIR)/%_main.cpp,$(BIN_DIR)/%,$(notdir $(MAIN_SRC_FILES)))

# Build all binaries
all: $(BIN_FILES)

# Rule for building object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Rule for building binary files
$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Declare the `all` and `clean` targets as phony
.PHONY: all clean
