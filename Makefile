# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wextra -Wall -pedantic -O0 -g

TARGET = main
OBJS = main.o LRUCache.o

all: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${TARGET} ${OBJS}

main.o: main.cpp LRUCache.h
	${CXX} ${CXXFLAGS} -c main.cpp

LRUCache.o: LRUCache.cpp LRUCache.h
	${CXX} ${CXXFLAGS} -c LRUCache.cpp

.PHONY: clean
clean:
	rm -f *.o ${TARGET}