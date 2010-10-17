#/bin/sh
gcc -g -c -O3 -o lookup3.o lookup3.c
g++ -g -O3 -I ./ -o sokoban *cpp lookup3.o
./sokoban
