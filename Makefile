# Compiler and flags
CXX = g++
CXXFLAGS = -I imgui/include/ -I headers/ -Wall -Wextra -O2 -std=c++17
# Libraries
LIBS = -lGL -lGLEW -lglfw

# Source files
IMGUI_SRC = $(wildcard imgui/src/*.cpp)
SRC = $(wildcard source/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o) $(IMGUI_SRC:.cpp=.o)

# Output binary
TARGET = a.out

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBS) -o $(TARGET)

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)

