# Makefile

# Compiler
CXX = g++
CXXFLAGS = -I. -Wall -std=c++11

# Libraries
LIBS = -lGLEW -lGL -lglfw

# Source files
SRCS = main.cpp
OBJS = main.o

# Shader files
SHADERS = cube_vertex.glsl cube_fragment.glsl color_vertex.glsl color_fragment.glsl pyramid_vertex.glsl pyramid_fragment.glsl sphere_vertex.glsl sphere_fragment.glsl

# Executable name
TARGET = project

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: clean
