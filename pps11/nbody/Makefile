COMMON = nbody.c gui.c -O3 -Wall -g `sdl-config --cflags --libs` -lGL -lpthread
COMMON_FILES = nbody.h nbody.c gui.h gui.c

all:
	make nbody1
	make nbody2
	make nbody3
	make nbody4

nbody1: nbody1.c $(COMMON_FILES)
	gcc -o nbody1 nbody1.c $(COMMON)

nbody2: nbody2.c $(COMMON_FILES)
	gcc -o nbody2 nbody2.c $(COMMON)

nbody3: nbody3.c octree.c octree.h $(COMMON_FILES)
	gcc -o nbody3 nbody3.c octree.c -DNO_THREADS -DUSE_OCTREE $(COMMON)

nbody4: nbody4.c octree.c octree.h $(COMMON_FILES)
	gcc -o nbody4 nbody4.c octree.c -DUSE_OCTREE $(COMMON)

