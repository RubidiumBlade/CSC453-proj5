CC = gcc
CFLAGS = -Wall -g

all: minget minls

minixFileExplorer.o: minixFileExplorer.c
	gcc $(CFLAGS) -o minixFileExplorer.o -c minixFileExplorer.c

minls.o: minls.c
	gcc $(CFLAGS) -o minls.o -c minls.c

minget.o: minget.c
	gcc $(CFLAGS) -o minget.o -c minget.c

minget: minget.o minixFileExplorer.o minixFiles.h printVerbose.o
	gcc $(CFLAGS) -o minget -lm minget.o minixFileExplorer.o minixFiles.h printVerbose.o

minls: minls.o minixFileExplorer.o minixFiles.h printVerbose.o
	gcc $(CFLAGS) -o minls -lm minls.o minixFileExplorer.o minixFiles.h printVerbose.o

printVerbose.o: printVerbose.c
	gcc $(CFLAGS) -o printVerbose.o -c printVerbose.c

clean:
	rm *.o
	make
