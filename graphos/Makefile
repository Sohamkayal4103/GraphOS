# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -std=c++17

# Final executable name
TARGET = graphos_executable

# List all .cpp files here (adjust if you don't want all .cpp files)
SOURCES = main.cpp \
          DOMAP.cpp \
          Graph_Processing.cpp \
          DORAM.cpp \
          Node.cpp \
          Stash.cpp \
          helper.cpp \
          Priority_queue.cpp

# Generate a list of object files from the .cpp files
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule: build the final executable
all: $(TARGET)

# Link all object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compile each .cpp into an .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the final executable
clean:
	rm -f $(OBJECTS) $(TARGET)
