#the compiler
CPP = g++
CC = gcc

#compiler options

CPPOPTS = -g -Wall -c $(shell sdl-config --cflags) -Wno-deprecated
#CPPOPTS = -O3 -Wall -c $(shell sdl-config --cflags) -Wno-deprecated
#CPPOPTS = -O3 -g -Wall -c $(shell sdl-config --cflags) -Wno-deprecated

#header files
INCLUDES = \
	-I. \
	$(NULL)

#library directories
LIBDIRS = \
	-L/usr/lib \
	$(NULL)

#libraries to link with
LIBS = \
	-lSDL_net \
	$(NULL)

#all compiler flags
CFLAGS = $(CPPOPTS) $(INCLUDES)

#all linker options and libs
LFLAGS = $(LIBDIRS) $(shell sdl-config --libs)

#default compilation rules
%.o:	%.cpp
	$(CPP) -c $< $(CFLAGS) -o $@

%.o:	%.c
	$(CC) -c $< $(CFLAGS) -o $@

#local headers to depend on
HEADERS = \
	queue.h		\
	set.h		\
	net2.h		\
	net2sets.h	\
	fastevents.h	\
	SDLUtils.h	\
	trace.h		\
	$(NULL)

#library files to compile
CCLIBS = \
	net2.c		\
	net2sets.c	\
	fastevents.c	\
	SDLUtils.c	\
	trace.c		\
	$(NULL)

CPPLIBS = \
	$(NULL)

#main programs to compile
CCMAINS = \
	$(NULL)

CPPMAINS = \
	client.cpp	\
	server.cpp	\
	ts.cpp		\
	tc.cpp		\
	us.cpp		\
	uc.cpp		\
	tus.cpp		\
	set.cpp		\
	$(NULL)

#all local .o files
OBJS =  ${CPPLIBS:.cpp=.o} ${CCLIBS:.c=.o}
MAINS =  ${CPPMAINS:.cpp=.o} ${CCMAINS:.c=.o}

default: depend client server ts tc us uc tus set

client: $(OBJS) $(HEADERS) client.o
	$(CPP) $(LFLAGS) client.o $(OBJS) $(LIBS) -o $@

server: $(OBJS) $(HEADERS) server.o
	$(CPP) $(LFLAGS) server.o $(OBJS) $(LIBS) -o $@

ts: $(OBJS) $(HEADERS) ts.o
	$(CPP) $(LFLAGS) ts.o $(OBJS) $(LIBS) -o $@

tc: $(OBJS) $(HEADERS) tc.o
	$(CPP) $(LFLAGS) tc.o $(OBJS) $(LIBS) -o $@

us: $(OBJS) $(HEADERS) us.o
	$(CPP) $(LFLAGS) us.o $(OBJS) $(LIBS) -o $@

uc: $(OBJS) $(HEADERS) uc.o
	$(CPP) $(LFLAGS) uc.o $(OBJS) $(LIBS) -o $@

tus: $(OBJS) $(HEADERS) tus.o
	$(CPP) $(LFLAGS) tus.o $(OBJS) $(LIBS) -o $@

set: $(OBJS) $(HEADERS) set.o
	$(CPP) $(LFLAGS) set.o $(OBJS) $(LIBS) -o $@

depend: ${CPPMAINS} ${CCMAINS} ${CPPLIBS} ${CCLIBS} ${HEADERS} makefile
	rm -f depend ; ${CPP} -MM -c ${CFLAGS} ${CPPMAINS} \
	${CCMAINS} ${CPPLIBS} ${CCLIBS} > depend

clean:
	rm -f core $(OBJS) $(MAINS) client server ts tc us uc tus set t k j *~ 

#
# get the depend info
#
include depend
