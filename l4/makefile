CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -ggdb

TARGET = main

SRCS = main.cpp

# Правило по умолчанию: сборка программы
all: $(TARGET)

# Правило сборки исполняемого файла с отладочными символами
debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

# Правило сборки исполняемого файла
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Правило очистки
clean:
	rm -f $(TARGET) *.o

# Указание, что файлы не являются действительными целями
.PHONY: all clean
