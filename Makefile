CXX = clang++
CXXFLAGS_OTHER =
CXXFLAGS = ${CXXFLAGS_OTHER} -std=c++11 -g -O0 -DDEBUG -D_DEBUG -D_LIBCPP_DEBUG=0 -D_GLIBCXX_DEBUG -Wall -Wextra
LDFLAGS = ${CXXFLAGS_OTHER}

.PHONY: all
all: Simulation

.PHONY: run
run: | all
	./Simulation 10 10 25 9999

Simulation: Computer.o EventQueue.o Globals.o main.o UnionFind.o
	${CXX} ${LDFLAGS} $^ -o $@

Computer.o:   Computer.cpp
EventQueue.o: EventQueue.cpp
Globals.o:    Globals.cpp
main.o:       main.cpp
UnionFind.o:  UnionFind.cpp

Computer.h:
EventQueue.h:
Globals.h:
UnionFind.h:

.PHONY: clean
clean::
	rm *.o Simulation

.PHONY: rebuild
rebuild: clean all

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@
