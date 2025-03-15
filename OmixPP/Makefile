# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# List of source files, now including main.cpp
SRC = main.cpp domap.cpp helper.cpp node.cpp stash.cpp

# Object files will have the same names but .o extension
OBJ = $(SRC:.cpp=.o)

# The final executable name
EXEC = omixPP

# The default rule
all: $(EXEC)

# Link object files into the final executable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp into a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Remove build artifacts
clean:
	rm -f $(OBJ) $(EXEC)

# Optional convenience target to run the program
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
