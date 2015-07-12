# Makefile for Blue Waters
# Author: Aaron Weeden, Shodor, May 2015

PREFIX=pi
LIBS=-lm

CC=CC

# CUDA
CUDA_CC=nvcc
CUDA_CFLAGS=-arch sm_13
EXECUTABLES+=$(PREFIX)-cuda-1
EXECUTABLES+=$(PREFIX)-cuda-2

all:
	make $(EXECUTABLES)

# EXPENDABLES
$(PREFIX)-io.o: $(PREFIX)-io.c $(PREFIX)-io.h
	$(CC) $(CFLAGS) -c $(PREFIX)-io.c $(LIBS)
EXPENDABLES+=$(PREFIX)-io.o

$(PREFIX)-calc.o: $(PREFIX)-calc.c $(PREFIX)-calc.h
	$(CC) $(CFLAGS) -c $(PREFIX)-calc.c $(LIBS)
EXPENDABLES+=$(PREFIX)-calc.o

$(PREFIX)-cuda-1.o: $(PREFIX)-cuda-1.cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $@ -c $^
EXPENDABLES+=$(PREFIX)-cuda-1.o

$(PREFIX)-cuda-2.o: $(PREFIX)-cuda-2.cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $@ -c $^ 
EXPENDABLES+=$(PREFIX)-cuda-2.o

# EXECUTABLES
$(PREFIX)-cuda-1: $(PREFIX)-cuda-1.o $(PREFIX)-io.o $(PREFIX)-cuda.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-cuda-2: $(PREFIX)-cuda-2.o $(PREFIX)-io.o $(PREFIX)-cuda.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# CLEAN
clean:
	rm -f $(EXPENDABLES) $(EXECUTABLES)

clean-pbs:
	rm -f *.pbs.{o,e}*
