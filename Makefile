# Probably needs to be adjusted for the comp3004 vm

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/include/x86_64-linux-gnu/qt5 -fPIC $(shell pkg-config --cflags Qt5Core)
LDFLAGS = $(shell pkg-config --libs Qt5Core)

TARGET = insulin_pump_sim
SRC_DIR = cpp
HEADER_DIR = headers

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o *.o $(TARGET)

.PHONY: clean