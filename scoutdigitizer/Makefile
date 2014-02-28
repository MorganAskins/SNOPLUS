MAIN = $(wildcard *.cpp)
PROGRAM = $(patsubst %.cpp, %, $(MAIN))

OPTIONS = -std=c++11 -Wall
CXX = g++
CC = g++
CPPLIBS = -lhdf5 -lhdf5_cpp
INCLUDES = include
SRC = src
ODIR = objects

_OBJS = $(PROGRAM).o $(patsubst $(SRC)/%.cc, %.o, $(wildcard $(SRC)/*.cc))
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))
ALL = $(OPTIONS) -I$(INCLUDES) $(CPPLIBS)

all: standard

clean:
	rm -rf $(ODIR)/*.o $(PROGRAM)

standard: $(OBJS)
	g++ $(ALL) $(OBJS) -o $(PROGRAM)

$(ODIR)/%.o: $(SRC)/%.cc
	g++ $(ALL) -c $(SRC)/$*.cc -o $(ODIR)/$*.o

$(ODIR)/%.o: %.cpp
	g++ $(ALL) -c $*.cpp -o $(ODIR)/$*.o
