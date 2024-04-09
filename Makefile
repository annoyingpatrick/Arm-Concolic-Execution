# Specify the compiler
CXX = g++

# Specify the compiler flags
CXXFLAGS = -Wall -std=c++17

# Include directories for Z3 header files
INCLUDES = -I/usr/local/include

# Library paths for Z3 and the library to link against
LDFLAGS = -L/usr/local/lib
LDLIBS = -lz3

# Specify the target
TARGET = simulator

# Specify the source file(s)
SRC = simulator.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $(TARGET) $(SRC) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET)
