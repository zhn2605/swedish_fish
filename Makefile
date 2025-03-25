# Compiler and flags
CXX = g++
CXXFLAGS = -O1 -g -Wall -Wextra -std=c++17 -I./include -I/usr/include/SDL3 -I/usr/include/GL -I/usr/local/include/glm
LDFLAGS = -lSDL3 -lGL

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files and objects
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
TARGET = $(BIN_DIR)/swedish_fish

# Detect all subdirectories
SRC_SUBDIRS = $(shell find $(SRC_DIR) -type d)
BUILD_SUBDIRS = $(SRC_SUBDIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	mkdir -p $(BUILD_SUBDIRS)
	mkdir -p $(BIN_DIR)

# Link objects to create executable
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile source files to objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Print sources
print-srcs:
	@echo "Source files found:"
	@echo $(SRCS)

# Debug build with symbols
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Release build with optimization
release: CXXFLAGS += -O3 -DNDEBUG
release: all

# Clean built files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the application
run: all
	$(TARGET)

# Run with debugger
debug-run: debug
	gdb $(TARGET)

.PHONY: all directories debug release clean run debug-run