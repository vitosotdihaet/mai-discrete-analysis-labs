CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -ggdb

TARGET = main

SRCS = main.cpp

all: $(TARGET)

debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
