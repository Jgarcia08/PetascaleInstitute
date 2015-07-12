# Makefile for Blue Waters
# Author: Aaron Weeden, Shodor, May 2015

PREFIX=pi
LIBS=-lm

CC=CC

# Serial
EXECUTABLES+=$(PREFIX)-serial

# MPI
MPI_CC=$(CC)
MPI_CFLAGS=$(CFLAGS)
EXECUTABLES+=$(PREFIX)-mpi-1
EXECUTABLES+=$(PREFIX)-mpi-2

# CUDA
CUDA_CC=nvcc
CUDA_CFLAGS=-arch sm_13
EXECUTABLES+=$(PREFIX)-cuda-1
EXECUTABLES+=$(PREFIX)-cuda-2

# MPI+CUDA
EXECUTABLES+=$(PREFIX)-mpi-cuda

all:
	make $(EXECUTABLES)

# EXPENDABLES
$(PREFIX)-io.o: $(PREFIX)-io.c $(PREFIX)-io.h
	$(CC) $(CFLAGS) -c $(PREFIX)-io.c $(LIBS)
EXPENDABLES+=$(PREFIX)-io.o

$(PREFIX)-calc.o: $(PREFIX)-calc.c $(PREFIX)-calc.h
	$(CC) $(CFLAGS) -c $(PREFIX)-calc.c $(LIBS)
EXPENDABLES+=$(PREFIX)-calc.o

$(PREFIX)-mpi.o: $(PREFIX)-mpi.c $(PREFIX)-mpi.h
	$(MPI_CC) $(MPI_CFLAGS) -c $(PREFIX)-mpi.c $(LIBS)
EXPENDABLES+=$(PREFIX)-mpi.o

$(PREFIX)-sync-data-1.o: $(PREFIX)-sync-data-1.c
	$(MPI_CC) $(MPI_CFLAGS) -c $(PREFIX)-sync-data-1.c $(LIBS)
EXPENDABLES+=$(PREFIX)-sync-data-1.o

$(PREFIX)-sync-data-2.o: $(PREFIX)-sync-data-2.c
	$(MPI_CC) $(MPI_CFLAGS) -c $(PREFIX)-sync-data-2.c $(LIBS)
EXPENDABLES+=$(PREFIX)-sync-data-2.o

$(PREFIX)-cuda-1.o: $(PREFIX)-cuda-1.cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $@ -c $^
EXPENDABLES+=$(PREFIX)-cuda-1.o

$(PREFIX)-cuda-2.o: $(PREFIX)-cuda-2.cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $@ -c $^ 
EXPENDABLES+=$(PREFIX)-cuda-2.o

$(PREFIX)-mpi-cuda.o: $(PREFIX)-mpi-cuda.cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $(PREFIX)-mpi-cuda.o -c $(PREFIX)-mpi-cuda.cu
EXPENDABLES+=$(PREFIX)-mpi-cuda.o

# EXECUTABLES
$(PREFIX)-serial: $(PREFIX)-serial.c $(PREFIX)-io.o $(PREFIX)-calc.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-mpi-1: $(PREFIX)-mpi-1.c $(PREFIX)-io.o $(PREFIX)-calc.o $(PREFIX)-mpi.o $(PREFIX)-sync-data-1.o
	$(MPI_CC) $(MPI_CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-mpi-2: $(PREFIX)-mpi-2.c $(PREFIX)-io.o $(PREFIX)-calc.o $(PREFIX)-mpi.o $(PREFIX)-sync-data-2.o
	$(MPI_CC) $(MPI_CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-cuda-1: $(PREFIX)-cuda-1.o $(PREFIX)-io.o $(PREFIX)-cuda.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-cuda-2: $(PREFIX)-cuda-2.o $(PREFIX)-io.o $(PREFIX)-cuda.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-mpi-cuda: $(PREFIX)-mpi.o $(PREFIX)-io.o $(PREFIX)-mpi-cuda.o $(PREFIX)-calc.o $(PREFIX)-sync-data-2.o $(PREFIX)-mpi-cuda.cpp
	$(MPI_CC) $(MPI_CFLAGS) -o $@ $^ $(LIBS)

# CLEAN
clean:
	rm -f $(EXPENDABLES) $(EXECUTABLES)

clean-pbs:
	rm -f *.pbs.{o,e}*
