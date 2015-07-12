CC=cc
SRC=accel.c
PROGRAM=accel
EXECUTABLES=$(PROGRAM)

$(PROGRAM): $(SRC)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(SRC) $(LIBS)

all:
	make $(EXECUTABLES)

clean:
	rm -f $(EXECUTABLES)
