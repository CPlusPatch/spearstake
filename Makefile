# Compiler and Compiler Flags
CXX = g++
CXXFLAGS = $(shell sdl2-config --libs --cflags) -ggdb3 -O0 -std=c++11 -Wall -lSDL2_image -lm

# Header Files and Source Files
HDRS =
SRCS = main.cpp

# Object Files and Executable
OBJS = $(SRCS:.cpp=.o)
EXEC = spearstake.out

# Recipes
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean