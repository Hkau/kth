COMMON = nbody.c gui.c -O3 -Wall -g `sdl-config --cflags --libs` -lGL
COMMON_FILES = nbody.h nbody.c gui.h gui.c

all:
	make nbody1
	make nbody2
	make nbody3
	make nbody4

nbody1: nbody1.c $(COMMON_FILES)
	gcc -o nbody1 nbody1.c $(COMMON)

