all: tsh.c
	gcc -std=c99 -D_POSIX_SOURCE -pedantic -Wall -g -o tsh tsh.c

poll: tsh.c
	gcc -std=c99 -DUSE_POLL -D_POSIX_SOURCE -pedantic -Wall -g -o tsh tsh.c

