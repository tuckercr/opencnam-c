CC      = gcc
CFLAGS  = -L/opt/local/lib -lcurl -lssl -lcrypto -lssl -lcrypto -lz -L./lib -linih 

all:	opencnam

opencam:	opencnam.c
	$(CC) -o $@ $< $(LIBS) $(CFLAGS)

clean:
	rm -f opencnam.o opencnam
