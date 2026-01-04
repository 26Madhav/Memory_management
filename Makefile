CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = \
src/main.cpp \
src/allocator/memory_simulator.cpp \
src/cache/cache.cpp \
src/virtual_memory/virtual_memory.cpp \
src/stats/stats.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = memory_sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)
