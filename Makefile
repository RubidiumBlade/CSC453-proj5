CC = gcc
CFLAGS = -Wall -g

all:
	make minget
	make minls

minixFileExplorer.o: minixFileExplorer.c minixFiles.h
	gcc $(CFLAGS) -o minixFileExplorer.o -c minixFileExplorer.c

minls.o: minls.c
	gcc $(CFLAGS) -o minls.o -c minls.c

minget.o: minget.c
	gcc $(CFLAGS) -o minget.o -c minget.c

minget: minget.o minixFileExplorer.o minixFiles.h
	gcc $(CFLAGS) -o minget minget.o minixFileExplorer.o minixFiles.h

minls: minls.o minixFileExplorer.o minixFiles.h
	gcc $(CFLAGS) -o minls minls.o minixFileExplorer.o minixFiles.h

clean:
	rm *.o
	make
