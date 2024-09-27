# Compiler
CC = g++

# Compiler Flags
CFLAGS = -Wall -std=c++17 $(SDL_CFLAGS) $(INCLUDE_DIRS) -MMD -MP

# SDL2 Flags
SDL_CFLAGS := $(shell sdl2-config --cflags)

# Include directories (add any directories containing header files)
INCLUDE_DIRS = -Iinclude -Iinclude/SDL2 -Isrc

# Library directories (add directories containing libraries)
LIB_DIRS = -Llib/SDL2

# Libraries to link (add the actual libraries, without 'lib' prefix)
LIBS =  -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

# Build directory
BUILD_DIR = build

# Source files directory
SRC_DIR = src

# Name of Target
TARGET = hangman

# Function to perform recursive wildcard search
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# List all .cpp source files from subdirectories recursively
SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp)

# Convert source file paths to object file paths in the build directory, preserving directory structure
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Build the target
all: $(BUILD_DIR)/$(TARGET)

# Link the object files to create the final executable in the build directory
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIB_DIRS) $(LIBS) -o $@

# Compile each source file into an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Include the generated .d files
-include $(OBJS:.o=.d)

# Clean the build directory and remove the directory
.PHONY: clean
clean:
	rm -rf $(call rwildcard,$(BUILD_DIR)/,*.o) $(BUILD_DIR)/$(TARGET).exe
