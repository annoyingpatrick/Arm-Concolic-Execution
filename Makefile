# Specify the compiler
CXX = g++

# Specify the compiler flags
CXXFLAGS = -Wall -std=c++17

# Include directories for Z3 header files
INCLUDES = -I/usr/local/include -I/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/include

# Library paths for Z3 and the library to link against
LDFLAGS = -L/usr/local/lib -L/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/lib
LDLIBS = -lz3

# Specify the target
TARGET = ACE_ENGINE

# Specify the source file(s)
SRC = ACE_Engine.cpp z3temp.cpp helpers.cpp main.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $(TARGET) $(SRC) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET)
