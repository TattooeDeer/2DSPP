CXX=g++
CXXFLAGS=-std=c++14 -o

all: solver_HC_MM.cpp
	$(CXX) $(CXXFLAGS) solver solver_HC_MM.cpp