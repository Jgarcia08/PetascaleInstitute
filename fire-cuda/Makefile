CC=cc
CUDA_CC=nvcc

PREFIX=fire
EXECUTABLES=$(PREFIX)-serial $(PREFIX)-cuda

all:
	make $(EXECUTABLES)

$(PREFIX)-serial: $(PREFIX).c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(PREFIX)-cuda: $(PREFIX).c
	cp $(PREFIX).c $(PREFIX).cu
	$(CUDA_CC) $(CUDA_CFLAGS) -o $@ $(PREFIX).cu $(LIBS)
	rm $(PREFIX).cu

clean:
	rm -f $(EXECUTABLES)
