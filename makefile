# Compiler
CC = g++

# Build directory
BUILD_DIR = build

# Source files directory
SRC_DIR = src

# Function to perform recursive wildcard search
rwildcard = $(foreach d,$(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

# List all .cpp source files from subdirectories recursively
SRCS := $(call rwildcard, $(SRC_DIR)/, *.cpp)

# Convert source file paths to object file paths in the build directory, preserving directory structure
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Name of Target
TARGET = hangman

# Include directories (add any directories containing header files)
INCLUDE_DIRS = -Iinclude -Iinclude/SDL2 -Isrc

# Source folder to copy
ASSETS_DIR = Assets

# Target folder in the build directory
BUILD_ASSETS_DIR = $(BUILD_DIR)/Assets

# Detect OS and set copy command based on the environment
ifeq ($(OS), Windows_NT)
    # Windows: copy the Assets directory and then DLL files
    COPY_CMD = \
        xcopy $(ASSETS_DIR) $(BUILD_DIR)\Assets /s /e /h /i /y
else
    # Linux: copy the Assets directory (ignoring DLLs as they’re not used)
    COPY_CMD = \
        cp -r $(ASSETS_DIR)/* $(BUILD_DIR)/Assets || true
endif

# Detect OS and set variables based on the environment
ifeq ($(OS), Windows_NT)
    # Windows settings
    SHELL = cmd.exe
    .SHELLFLAGS = /C
    
    EXE_EXT = .exe
    RM = cmd /C del /Q /F
    RMDIR = cmd /C rmdir /S /Q
    
    # Function to remove trailing slashes and convert to backslashes
    fixpath = $(subst /,\,$(patsubst %/,%,$1))
    MKDIR = mkdir $(call fixpath,$(dir $@)) 2> NUL || exit 0
    
	# Include directories (add any directories containing header files)
    INCLUDE_DIRS = -Iinclude -Iinclude/SDL2 -Isrc

    # Library directories (add directories containing libraries)
    LIB_DIRS = -Llib/windows/SDL2

    # Compiler Flags
    CFLAGS = -Wall -std=c++17 $(INCLUDE_DIRS) -MMD -MP -DSDL_MAIN_HANDLED

    # Libraries to link (static linking) -mwindows
    LIBS = $(LIB_DIRS) -lmingw32 -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_mixer -lSDL2_image 
else
    # Unix-like system settings (Linux, macOS)
    EXE_EXT =
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(dir $@)
    
	# SDL2 Flags
    SDL_CFLAGS := $(shell sdl2-config --cflags)
    SDL_LIBS := $(shell sdl2-config --libs)
    # Include directories (add any directories containing header files)
    INCLUDE_DIRS = -Iinclude -Iinclude/SDL2 -Isrc
    # Library directories (add directories containing libraries)
    LIB_DIRS = -Llib/linux/SDL2
    # Compiler Flags
    CFLAGS = -Wall -std=c++17 $(SDL_CFLAGS) $(INCLUDE_DIRS) -MMD -MP
    # Libraries to link
    LIBS = $(SDL_LIBS) -lSDL2_ttf -lSDL2_mixer -lSDL2_image
endif

# Build the target
all: $(BUILD_DIR)/$(TARGET)$(EXE_EXT) copy_assets

# Link the object files to create the final executable in the build directory
$(BUILD_DIR)/$(TARGET)$(EXE_EXT): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

# Compile each source file into an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Include the generated .d files
-include $(OBJS:.o=.d)

# Copy assets folder to build directory
copy_assets: $(BUILD_DIR) copy_dll
	@echo "Copying Necessary Assets.."
	@$(MKDIR) $(BUILD_ASSETS_DIR)
	@$(COPY_CMD)

ifeq ($(OS), Windows_NT) 
# DLLs to copy individually for Windows
copy_dll: 
	@cmd /C "xcopy DLLs\SDL2.dll $(BUILD_DIR) /y /q > NUL 2>&1"
	@cmd /C "xcopy DLLs\SDL2_image.dll $(BUILD_DIR) /y /q > NUL 2>&1"
	@cmd /C "xcopy DLLs\SDL2_ttf.dll $(BUILD_DIR) /y /q > NUL 2>&1"
	@cmd /C "xcopy DLLs\SDL2_mixer.dll $(BUILD_DIR) /y /q > NUL 2>&1"

else

copy_dll:

endif

# Clean the build directory and remove the directory
.PHONY: clean
clean:
	$(RMDIR) $(BUILD_DIR)

.PHONY: help
help:
	@echo "Usage:"
	@echo "  make            - Build the project"
	@echo "  make clean      - Clean the build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "This Makefile supports building the project on both Windows and Linux."
	@echo ""
	@echo "On **Windows**:"
	@echo "  - Ensure you have MinGW-w64 or MSYS2 installed and added to your PATH."
	@echo "  - Ensure SDL2 and its extensions (SDL2_image, SDL2_mixer, SDL2_ttf) libraries are included in 'lib/windows/SDL2' and headers in 'include/SDL2'."
	@echo "  - Run 'make' in the Windows PowerShell or the appropriate shell."
	@echo "  - The executable will be created in the 'build' directory as 'hangman.exe'."
	@echo ""
	@echo "On **Linux**:"
	@echo "  - Ensure you have g++ and SDL2 development packages installed."
	@echo "  - Ensure to run sudo apt update && sudo apt upgrade before continuing.
	@echo "    Install them using your package manager, e.g.:"
	@echo "      sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
	@echo "  - Run 'make' in the terminal."
	@echo "  - The executable will be created in the 'build' directory as 'hangman'."
	@echo ""
