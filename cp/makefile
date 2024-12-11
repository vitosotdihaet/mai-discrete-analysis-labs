CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -ggdb
TESTFLAG = 

TARGET = main

SRCS = main.cpp

all: $(TARGET)

debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(TESTFLAG) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
