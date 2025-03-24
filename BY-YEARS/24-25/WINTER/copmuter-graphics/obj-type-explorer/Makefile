CXX      = g++
CXXFLAGS = -std=c++17 -Wall
LIBS     = -lGLEW -lglfw -lGL -lassimp -lpthread -ldl

TARGET   = project

SRCS = main.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
