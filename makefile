# Compiler
CC = g++

# CPP Flags
CFLAGS = -Wall -std=c++17

# Include directories (add any directories containing header files)
INCLUDE_DIRS = -Iinclude -Iinclude/SDL2 -Isrc

# Library directories (add directories containing libraries)
LIB_DIRS = -Llib/SDL2

# Libraries to link (add the actual libraries, without 'lib' prefix)
LIBS = -lmingw32  -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lopengl32 -lglu32

# Build directory
BUILD_DIR = build
# Source files directory
SRC_DIR = src

# Name of Target
TARGET = game

# List all .cpp source files from subdirectories
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/GameStates/*.cpp) \
       $(wildcard $(SRC_DIR)/Util/*.cpp)

# Convert .cpp files to .o files in the build directory without subdirectory structure
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.cpp=.o))

# Build the target
all: $(BUILD_DIR) $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Link the object files to create the final executable in the build directory
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_DIRS) $(LIBS) -o $(BUILD_DIR)/$(TARGET)

# Compile each source file into an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean the build directory and remove the directory
clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\$(TARGET).exe
