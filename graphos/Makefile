# Compiler and flags
CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -DBOOST_SERIALIZATION_DYN_LINK -DBOOST_ALL_DYN_LINK -DCRYPTOPP_DISABLE_ASM -stdlib=libc++ -I/opt/homebrew/opt/boost/include -I/opt/homebrew/opt/cryptopp/include -I../OmixPP

LDFLAGS = -L/opt/homebrew/opt/boost/lib -L/opt/homebrew/opt/cryptopp/lib -lboost_serialization -lcryptopp -stdlib=libc++

# Final executable name
TARGET = graphos_executable

# List all .cpp files you need (local, OmixPP, and the new boost serialization source file)
SOURCES = main.cpp \
          Graph_Processing.cpp \
          Priority_queue.cpp \
          ../OmixPP/node.cpp \
          ../OmixPP/helper.cpp \
          ../OmixPP/domap.cpp \
          ../OmixPP/stash.cpp \
          boost_serialization_source.cpp

# Generate a list of .o files from the .cpp files
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule to build the final executable
all: $(TARGET)

# Link all .o files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compile each .cpp into an .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove .o files and the final executable
clean:
	rm -f $(OBJECTS) $(TARGET)
