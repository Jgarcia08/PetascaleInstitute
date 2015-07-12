CC=cc
CFLAGS=-h pragma=acc
PISRC=pi_acc.c
VADDSRC=vecAdd.c
ADDVEC=addvectors.c
PIPROGRAM=pi_acc

$(PIPROGRAM): $(PISRC)
	$(CC) $(CFLAGS) -o $(PIPROGRAM) $(PISRC)

vecAdd: $(VADDSRC)
	$(CC) $(CFLAGS) -o vecAdd $(VADDSRC)

addvec: $(ADDVEC)
	$(CC) $(CFLAGS) -o addvectors $(VADDSRC)
    
all:
	make $(EXECUTABLES)

clean:
	rm -f pi_acc vecAdd $(EXECUTABLES) *.ptx *.cub
