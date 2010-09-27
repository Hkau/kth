#include <iostream>

#include "sokoban.h"

class AutoClose
{
	FILE *file;
public:
	AutoClose(FILE *f) {file = f;}
	~AutoClose() { Close(); }
	void Close() { if (file != 0) fclose(file); file = 0;}
	FILE * GetFile() { return file; }
};

int levelfromfile(const char *filename)
{
	// stdin is default
	FILE * file = stdin;

	// non-null overrides
	if (filename != 0)
		file = fopen(filename, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Failed to open file \"%s\"\n", filename);
		return -1;
	}

	// Make sure the file is closed on exit
	AutoClose auto_close(file);

	int width = 0;
	int x = 0;
	int y = 0;

	char c;
	do
	{
		// Read character
		c = fgetc(file);

		if (c == '\n') {
			if (width !=0 && width != x)
				fprintf(stderr, "Warning: Widht is not uniform\n");
			width = x;
			y++;
		} else if (c == PLAYER) {
		} else if (c == PLAYER_GOAL) {
		} else if (c == BOX) {
		} else if (c == BOX_GOAL) {
		} else if (c == GOAL) {
		} else if (c == FLOOR) {
		}
		x++;
	} while (c != EOF);

	return 0;
}

