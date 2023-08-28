# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Find all source files except *_main.cpp
SRC_FILES := $(filter-out $(MAIN_FILES), $(wildcard $(SRC_DIR)/*.cpp))
BUILD_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Find all *_main.cpp files
MAIN_FILES := $(wildcard $(SRC_DIR)/*_main.cpp)
EXECUTABLES := $(patsubst $(SRC_DIR)/%_main.cpp,$(BIN_DIR)/%,$(notdir $(MAIN_FILES)))

# Include directories
INC_FLAGS := -I$(INCLUDE_DIR)

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_FILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c -o $@ $<

# Rule to build executables from *_main.cpp files
define compile_and_link_template
$$(BIN_DIR)/$(notdir $(basename $1)): $$(BUILD_DIR)/$(notdir $(basename $1)).o $(BUILD_FILES)
	$$(CXX) -o $$@ $(BUILD_FILES)
endef

# Apply the template for each *_main.cpp file
$(foreach MAIN_FILE,$(MAIN_FILES),$(eval $(call compile_and_link_template,$(MAIN_FILE))))

# Default target: build all executables
all: $(EXECUTABLES)

# Clean up generated files
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all clean
