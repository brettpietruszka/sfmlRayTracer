

CXX = g++
CXXFLAGS := -std=c++11 -g

ifeq ($(OS),Windows_NT)
    lib_dir := ./SFML-2.5.1/build-win/lib
    TARGET := ./build/sfml-raytracer.exe
else
    lib_dir = ./SFML-2.5.1/build/lib
    TARGET := ./build/sfml-raytracer.out
endif

incl_dir := ./include
sfml_inc := ./SFML-2.5.1/include

INCLUDES := -I$(sfml_inc) -I$(incl_dir)
LIBRARIES := -L$(lib_dir)
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Root Directory For CPP_FILES
SRC_DIR := ./src
# Specify the output directory for object files
OBJ_DIR := ./build

# Find all cpp Files
CPP_FILES := $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)

# Define the object file names based on the source file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))

all: $(TARGET)

# Rule to create the build directory
$(OBJ_DIR):
	@echo $(CPP_FILES)
	@echo $(OBJ_FILES)
	@echo Creating build directory...
	@mkdir -p $(OBJ_DIR)

# Rule to build object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo Compiling $<...
	@mkdir -p $(@D)
	@$(CXX) -c $< -o $@ $(INCLUDES) $(CXXFLAGS)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	@echo Linking $(TARGET)...
	@$(CXX) $^ -o $@ $(LDFLAGS) $(LIBRARIES)

# Phony target to clean the build artifacts
.PHONY: clean
clean:
	@echo Cleaning...
	@rm -rf $(OBJ_DIR) $(TARGET)