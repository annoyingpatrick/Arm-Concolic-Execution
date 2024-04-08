# Specify the compiler
CXX = g++

# Specify the compiler flags
CXXFLAGS = -Wall -std=c++17

# Specify the target
TARGET = simulator

# Specify the source file(s)
SRC = simulator.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)