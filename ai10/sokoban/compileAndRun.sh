#/bin/sh
gcc -g -c -o lookup3.o lookup3.c
g++ -g -I ./ -o sokoban *cpp lookup3.o
./sokoban
