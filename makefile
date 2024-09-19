# Compiler
CC = g++

# CPP Flags
CFLAGS = -Wextra -Wall -std=c++17

# Build directory
BUILD_DIR = build
# Source files directory
SRC_DIR = src

# Name of Target
TARGET = game

# Object files
OBJS = $(BUILD_DIR)/hangman.o

# Build the target
all: $(BUILD_DIR) $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

# Link the object files to create the final executable in the build directory
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BUILD_DIR)/$(TARGET)

# Compile the source file into an object file in the build directory
$(BUILD_DIR)/hangman.o: $(SRC_DIR)/hangman.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/hangman.cpp -o $(BUILD_DIR)/hangman.o

# Clean the build directory and remove the directory
clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\$(TARGET).exe

