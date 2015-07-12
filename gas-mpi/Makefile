# HPCU Bi-Weekly Challenge: Distributed Ideal Gas
# MPI version
# Makefile
#
# @author Aaron Weeden, Shodor Education Foundation
# @version 3.0

CC=mpicc
CFLAGS=-Wall --pedantic
LIBS=-lm
EXECUTABLE=ideal-gas
DEPS=*.h
OBJS=main.o initialization.o memory-management.o simulation.o \
	output.o movement.o random.o
SRC=main.c

$(EXECUTABLE): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXECUTABLE)
