COMMON = -O3 -Wall -g `sdl-config --cflags --libs` -lGL

all:
	make nbody1
	make nbody2
	make nbody3
	make nbody4

nbody1: nbody1.c nbody.h gui.c gui.h
	gcc -o nbody1 nbody1.c gui.c $(COMMON)

