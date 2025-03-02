# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Source and Object Files
SRCS = doram.cpp helper.cpp main.cpp node.cpp stash.cpp
OBJS = $(SRCS:.cpp=.o)

# Executable Name
TARGET = doram

# Build Rule (Linking)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile C++ Files to Object Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule (removes compiled files)
clean:
	rm -f $(OBJS) $(TARGET)
