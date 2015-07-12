CC:=cc
SRC=heat.c
PROGRAM=heat
EXECUTABLES=$(PROGRAM)
NC_OUT=heat-data.nc

$(PROGRAM): $(SRC)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(SRC) $(LIBS)

all:
	make $(EXECUTABLES)

clean:
	rm -f $(EXECUTABLES) $(NC_OUT)
