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

std::string levelfromfile(const char *filename)
{
	// stdin is default
	FILE * file = stdin;

	// non-null overrides
	if (filename != 0)
		file = fopen(filename, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Failed to open file \"%s\"\n", filename);
		return "";
	}

	// Make sure the file is closed on exit
	AutoClose auto_close(file);

    std::string s;
	char c[2] = {fgetc(file), '\0'};
	while (c[0] != EOF)
    {
        if (c[0] == '\r')
            c[0] = '\n';
        //putchar(c[0]);
        s.append(c);
		// Read character
		c[0] = fgetc(file);
	}

	return s;
}

