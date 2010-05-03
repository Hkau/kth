all:
	gcc `sdl-config --cflags --libs` -I linux/ -I ./ -o musikspel *.c linux/*.c -lGL -lvorbisfile
clean:
	rm musikspel
