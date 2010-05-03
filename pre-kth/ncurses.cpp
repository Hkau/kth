#include "Blokus.h"
#include <curses.h>
#include <cstring>
#include "StandardTiles.h" //temp

#define	FIELD_X	2
#define	FIELD_Y	1

#define PLAYERNAME_X	50
#define PLAYERNAME_Y	1

#define	TILE_X	50
#define	TILE_Y	4

#define INFO_X 50
#define INFO_Y 15

static int cursorX = 0;
static int cursorY = 0;

void printField(Field *field)
{
	for(unsigned int i = 0; i < field->getHeight(); ++i)
	{
		mvaddch(FIELD_Y+i, FIELD_X, '|');
		for(unsigned int j = 0; j < field->getWidth(); ++j)
		{
			//move(FIELD_Y+i, FIELD_X+j*2+1);
			if(field->getBlock(j, i).getValue() <= 9 && field->getBlock(j, i).getValue() != 0) //ncurses port knows how to print up to 9 teams
				addch('0'+field->getBlock(j, i).getValue());
			else
				addch(' ');

			addch('|');
		}
	}
}

void printTileOverField(Tile *tile, u8 x, u8 y)
{
	for(unsigned int i = 1; i < tile->getHeight()-1; ++i)
	{
		for(unsigned int j = 1; j < tile->getWidth()-1; ++j)
		{
			if(tile->getBlock(j, i).getValue() <= 9 && tile->getBlock(j, i).getValue() != 0) //ncurses port knows how to print up to 9 teams
				mvaddch(FIELD_Y+y+i-1, FIELD_X+2*(x+j)-1, '0'+tile->getBlock(j, i).getValue());
		}
	}
}

void printTile(Tile *tile, u8 x, u8 y)
{
	for(unsigned int i = 1; i < tile->getHeight()-1; ++i) // Don't print edges which only contain BLOCK_CORNER and BLOCK_EMPTY
	{
		for(unsigned int j = 1; j < tile->getWidth()-1; ++j) // Don't print edges which only contain BLOCK_CORNER and BLOCK_EMPTY
		{
			u8 value = tile->getBlock(j, i).getValue();
			if(value != 0 && value <= 9) //ncurses port knows how to print up to 9 teams, print 0 < value <= 9
				mvaddch(y+i-1, x+j-1, '0' + tile->getBlock(j, i).getValue());
		}
	}
}

int main()
{

	//Initialize ncurses
	initscr();

	//Enable special keyboard stuff, like KEY_UP/DOWN etc, in ncurses.
	keypad(stdscr, true);

	DefaultRules gameRules;
	Game game(&gameRules);

	Player *playerList = game.getPlayerList();

	for(u8 i = 0; i < game.getPlayerCount(); ++i)
	{
		char nameBuffer[16] = "test lol\0";
		printw("Enter name for player %d: ", i+1); //Antar namn mindre än 16 bokstäver
		scanw("%s", nameBuffer);
		printw("%s\n", nameBuffer);
		
		playerList[i].setName(nameBuffer);
	}

	//Disable output
	noecho();
	//Hide cursor
	curs_set(0);

	StandardTile19 foo(1);
	
	//printTile(game.getField(), FIELD_X, FIELD_Y);

	refresh();

	while(1)
	{
		clear();

		printField(game.getField());

		printTileOverField(playerList[0].getCurrentTile(), cursorX, cursorY);

		move(FIELD_Y+cursorY,FIELD_X+1+2*cursorX);

		mvaddstr(INFO_Y, INFO_X, "Next block - N, Previous - P");
		mvaddstr(INFO_Y+1, INFO_X, "Rotate (Left - Z, Right - X)");
		mvaddstr(INFO_Y+2, INFO_X, "Flip (Horizontally - A,");
		mvaddstr(INFO_Y+3, INFO_X+6, "Vertically - S)");
		
		mvaddstr(INFO_Y+5, INFO_X, "Exit - Q");

		
		//mvaddstr(INFO_Y+1, INFO_X+4, "Right - X");

		int keyPress = getch();

		if(keyPress == 'q')
			break;

		switch(keyPress)
		{
			case KEY_UP:
				if(cursorY > 0)
					--cursorY;
				break;

			case KEY_DOWN:
				if(cursorY < (game.getField()->getHeight()-game.getCurrentPlayer()->getCurrentTile()->getHeight()+2))
					++cursorY;
				break;

			case KEY_LEFT:
				if(cursorX > 0)
					--cursorX;
				break;

			case KEY_RIGHT:
				if(cursorX < (game.getField()->getWidth()-game.getCurrentPlayer()->getCurrentTile()->getWidth()+2))
					++cursorX;
				break;

			case 'z':
				game.getCurrentPlayer()->getCurrentTile()->RotateLeft();

				while(cursorX >= (game.getField()->getWidth()-game.getCurrentPlayer()->getCurrentTile()->getWidth()+2))
					--cursorX;

				while(cursorY >= (game.getField()->getHeight()-game.getCurrentPlayer()->getCurrentTile()->getHeight()+2))
					--cursorY;

				break;

			case 'x':
				game.getCurrentPlayer()->getCurrentTile()->RotateRight();

				while(cursorX >= (game.getField()->getWidth()-game.getCurrentPlayer()->getCurrentTile()->getWidth()+2))
					--cursorX;

				while(cursorY >= (game.getField()->getHeight()-game.getCurrentPlayer()->getCurrentTile()->getHeight()+2))
					--cursorY;

				break;
				
			case 'a':
				game.getCurrentPlayer()->getCurrentTile()->FlipH();
				
				break;
				
			case 's':
				game.getCurrentPlayer()->getCurrentTile()->FlipV();
				
				break;
			
			case 'p':
				game.getCurrentPlayer()->PreviousTile();

				while(cursorX >= (game.getField()->getWidth()-game.getCurrentPlayer()->getCurrentTile()->getWidth()+2))
					--cursorX;

				while(cursorY >= (game.getField()->getHeight()-game.getCurrentPlayer()->getCurrentTile()->getHeight()+2))
					--cursorY;
				
				break;

			case 'n':
				game.getCurrentPlayer()->NextTile();

				while(cursorX >= (game.getField()->getWidth()-game.getCurrentPlayer()->getCurrentTile()->getWidth()+2))
					--cursorX;

				while(cursorY >= (game.getField()->getHeight()-game.getCurrentPlayer()->getCurrentTile()->getHeight()+2))
					--cursorY;
				
				break;

			default:
				break;
		}

		refresh();
	}

	endwin();

	return 0;
}

