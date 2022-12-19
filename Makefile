CXX = g++
CXXFLAGS = -std=c++11
#  -Wall -O3 -Wsign-compare

EXES = main
OBJS = ${EXES:=.o} main.o

%: %.o
	${CXX} ${LDFLAGS} -o $@ $^

all: ${EXES}

clean:
	${RM} ${EXES} core *~