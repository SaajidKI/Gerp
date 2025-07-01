###
### Makefile for Gerp Project
### Gerp is a program that indexes and searches files for strings
###
### Authors: Alexander Fomin and Saajid Islam

MAKEFLAGS += -L 
CXX      = clang++
CXXFLAGS = -g3 -Ofast -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 -Ofast

gerp: main.o gerp.o DirNode.o FSTree.o
	$(CXX) $(LDFLAGS) -o gerp $^

main.o: main.cpp gerp.h
	$(CXX) $(CXXFLAGS) -c main.cpp

gerp.o: gerp.cpp gerp.h DirNode.h FSTree.h
	$(CXX) $(CXXFLAGS) -c gerp.cpp

provide:
	provide comp15 proj4_gerp gerp.h gerp.cpp main.cpp README Makefile \
							  ourOutput.txt refOutput.txt

clean:
	rm main.o gerp.o
