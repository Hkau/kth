#include "DefaultRules.h"

#include "StandardTiles.h"

DefaultRules::DefaultRules()
{
	FieldWidth = 20;
	FieldHeight = 20;

	PlayerCount = 4;
}

bool DefaultRules::ValidPlaceTile(Field&, Tile&, u8 x, u8 y)
{
	return true;
}

Player *DefaultRules::CreatePlayers()
{
	Player *players = new Player[PlayerCount];
	
	for(u8 i = 0; i < PlayerCount; ++i)
	{
		players[i].setTeam(i+1);

		players[i].AllocateTiles(21);

		//Riktigt ful lösning, borde använt structs från början och kopierat, viktigaste nu är att den fungerar dock.

		players[i].setTile(new StandardTile1(i+1), 0);
		players[i].setTile(new StandardTile2(i+1), 1);
		players[i].setTile(new StandardTile3(i+1), 2);
		players[i].setTile(new StandardTile4(i+1), 3);
		players[i].setTile(new StandardTile5(i+1), 4);
		players[i].setTile(new StandardTile6(i+1), 5);
		players[i].setTile(new StandardTile7(i+1), 6);
		players[i].setTile(new StandardTile8(i+1), 7);
		players[i].setTile(new StandardTile9(i+1), 8);
		players[i].setTile(new StandardTile10(i+1), 9);
		players[i].setTile(new StandardTile11(i+1), 10);
		players[i].setTile(new StandardTile12(i+1), 11);
		players[i].setTile(new StandardTile13(i+1), 12);
		players[i].setTile(new StandardTile14(i+1), 13);
		players[i].setTile(new StandardTile15(i+1), 14);
		players[i].setTile(new StandardTile16(i+1), 15);
		players[i].setTile(new StandardTile17(i+1), 16);
		players[i].setTile(new StandardTile18(i+1), 17);
		players[i].setTile(new StandardTile19(i+1), 18);
		players[i].setTile(new StandardTile20(i+1), 19);
		players[i].setTile(new StandardTile21(i+1), 20);
	}
	
	return players;
}
