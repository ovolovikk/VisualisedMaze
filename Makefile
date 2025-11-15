CXX = g++
CXXFLAGS = -std=c++17 -Wall

# --- SFML Paths for macOS (using Homebrew) ---
# For Apple Silicon
SFML_INCLUDE = -I/opt/homebrew/include
SFML_LIBS = -L/opt/homebrew/lib
# For Intel Macs, uncomment the following lines and comment out the ones above
# SFML_INCLUDE = -I/usr/local/include
# SFML_LIBS = -L/usr/local/lib

TARGET = VisualisedMaze
SRCDIR = src
OBJDIR = obj

# Add 'lib' directory to include paths
INCLUDES = -Ilib $(SFML_INCLUDE)
LDFLAGS = $(SFML_LIBS) -lsfml-graphics -lsfml-window -lsfml-system

# Find all .cpp files in the source directory
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Create a list of object files in the object directory
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# --- New: Find all .hpp files ---
HEADERS = $(wildcard lib/*.hpp)

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to compile .cpp files into .o files in the OBJDIR
# Now depends on header files, so it recompiles if a header changes.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets are not files
.PHONY: all clean