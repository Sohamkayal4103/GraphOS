# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -DBOOST_SERIALIZATION_DYN_LINK -DBOOST_ALL_DYN_LINK -DCRYPTOPP_DISABLE_ASM -stdlib=libc++ -I/opt/homebrew/opt/boost/include -I/opt/homebrew/opt/cryptopp/include -I../OmixPP

# Linker flags for Boost.Serialization and Crypto++
LDFLAGS = -L/opt/homebrew/opt/boost/lib -L/opt/homebrew/opt/cryptopp/lib -lboost_serialization -lcryptopp -stdlib=libc++

# List of source files
SRC = main.cpp domap.cpp helper.cpp node.cpp stash.cpp

# Object files will have the same names but .o extension
OBJ = $(SRC:.cpp=.o)

# The final executable name
EXEC = omixPP

# The default rule
all: $(EXEC)

# Link object files into the final executable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

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
