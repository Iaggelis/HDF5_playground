CXXFLAGS     := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow -Wcast-align -ggdb -O0
LIBS         := $(shell pkg-config --libs hdf5)
HDF5_LIB_CXX := $(LIBS) -lhdf5_cpp -lhdf5_hl_cpp -lsz -lz -ldl -lm

.PHONY: all
all: createfile extend

createfile: createfile.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(HDF5_LIB_CXX)

extend: extend.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(HDF5_LIB_CXX)

.PHONY: clean
clean:
	rm -f *.h5
