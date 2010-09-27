/*
Level element	   Character   ASCII Code
Wall				'#'			0x23
Player				'@'			0x40
Player on goal  	'+'			0x2b
Box				 '$'			0x24
Box on goal	 	'*'			0x2a
Goal square		 '.'		 0x2e
Floor			   ' '			0x20
*/
static const char WALL		= '#';
static const char PLAYER	  = '@';
static const char PLAYER_GOAL = '+';
static const char BOX		 = '$';
static const char BOX_GOAL	= '*';
static const char GOAL		= '.';
static const char FLOOR	   = ' ';

#include <stdlib.h>

typedef struct
{
	int x, y;
} pos_t;

class SokobanState
{
private:
	// Lådornas positioner
	int num_boxes;
	pos_t *boxes;

	// Spelarens position
	int x;
	int y;

	// Kostnad
	int cost;

	// Referensräkning
	SokobanState *parent;
	int refs; // Hur många barn som pekar hit

public:
	SokobanState(size_t width, size_t height, size_t num_boxes)
	{
		this->num_boxes = num_boxes;
		boxes = (pos_t *)calloc(num_boxes, sizeof(pos_t));
	}
	SokobanState(const SokobanState &state)
	{
	}
	~SokobanState()
	{
		free(boxes);
		boxes = NULL;
	}
	bool equal(const SokobanState &state) const
	{
		return false;
	}
};

class SokobanLevel
{
private:
	size_t width;
	size_t height;
	char *level;
public:
	SokobanState createState();
};
