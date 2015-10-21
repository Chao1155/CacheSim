# this is a simple make file to compile my program. :)

SRCDIR=./src
CC=g++
CFLAGS=-I$(SRCDIR)

_DEPS= baseCache.h  global.h  racetrackCache.h  normalCache.h racetrack.h  trace_parser.h 
DEPS = $(patsubst %, $(SRCDIR)/%, $(_DEPS))

_OBJ = baseCache.o  global.o  main.o  racetrackCache.o normalCache.o
OBJ = $(patsubst %, $(SRCDIR)/%, $(_OBJ))

$(SRCDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o cacheSim $^ $(CFLAGS) 
	rm $(SRCDIR)/*.o

#all: main.cc baseCache.cc racetrackCache.cc global.cc
#	g++ -o cacheSim main.cc baseCache.cc racetrackCache.cc global.cc -I.
#clean:
#	rm cacheSim
