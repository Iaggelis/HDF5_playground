CXXFLAGS     := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow -Wcast-align -ggdb -O0
LIBS         := $(shell pkg-config --libs hdf5)

.PHONY: all
all: extend

extend: extend.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f *.h5
