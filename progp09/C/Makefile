CFLAGS += -W -g

all:	program

program: nod.o program.o
	gcc $(CFLAGS) -o program nod.o program.o

program.o: program.c
	gcc $(CFLAGS) -c program.c

nod.o:	nod.c nod.h
	gcc $(CFLAGS) -c nod.c

clean:
	rm -f *.o utfil.exe

