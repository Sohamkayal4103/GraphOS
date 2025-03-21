# Makefile for priority_queue project

# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Source and header files
SOURCES  = helper.cpp pq.cpp main.cpp
HEADERS  = helper.h pq.h node.h
OBJECTS  = $(SOURCES:.cpp=.o)

# Default target
all: main

# Link the final executable
main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS)

# Compile each .cpp into .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build artifacts
clean:
	rm -f *.o main
