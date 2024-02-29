# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++11 -Wall -I./include -g
LDFLAGS := -L./lib -lm -lSDL2 -lSDL2_image -llua -msse4.1 -lbox2d

# Directories
SRC_DIR := ./src
INCLUDE_DIR := ./include
BUILD_DIR := ./build
LEAGUE_DIR := $(BUILD_DIR)/LEAGUE

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Directory structure for object files
OBJ_DIRS := $(sort $(dir $(OBJ_FILES)))

# Executable name
TARGET := game

# Default target
all: directories $(TARGET)

# Linking the object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean the generated files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all directories clean

directories: $(BUILD_DIR) $(LEAGUE_DIR)

$(BUILD_DIR) $(LEAGUE_DIR):
	mkdir -p $@

