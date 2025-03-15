# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -std=c++17 -I../OmixPP

# Final executable name
TARGET = graphos_executable

# List all .cpp files you actually need, both local and in ../OmixPP
SOURCES = main.cpp \
          Graph_Processing.cpp \
          Priority_queue.cpp \
          ../OmixPP/node.cpp \
          ../OmixPP/helper.cpp \
          ../OmixPP/domap.cpp \
          ../OmixPP/stash.cpp

# Generate a list of .o files from the .cpp files
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule to build the final executable
all: $(TARGET)

# Link all .o files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compile each .cpp into an .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove .o files and the final executable
clean:
	rm -f $(OBJECTS) $(TARGET)
