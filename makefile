#
# Makefile for the socket-server program.
#
# Little program to test a little C++ socket server in Linux.
#

#
# The processor that this computer is using
#
PROCESSOR = $(shell uname -m)

#
# Libraries
# 	prefix LIBS with -l
# 	prefix LIBDIR with -L
#
LIBS :=
LIBDIR :=

OPTIONS := -Wall -std=c++17

#
# Header files.
#
HEADERS := socket-server-test.h socket-server.h

#
# Source objects (prefix with obj/)
#
OBJS := \
obj/socket-server-test.o \
obj/socket-server.o

bin/socker-server : $(OBJS)
	@mkdir -p $(@D)
	g++ $(OPTIONS) -o bin/socket-server-test $(OBJS) $(LIBDIR) $(LIBS)

obj/socket-server-test.o : socket-server-test.cpp $(HEADERS)
	@mkdir -p $(@D)
	g++ -c $(OPTIONS) -o obj/socket-server-test.o socket-server-test.cpp

obj/socket-server.o : socket-server.cpp $(HEADERS)
	@mkdir -p $(@D)
	g++ -c $(OPTIONS) -o obj/socket-server.o socket-server.cpp


clean:
	-rm -f bin/socket-server-test
	-rm -f $(OBJS)

