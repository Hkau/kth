#ifndef GAME_H
#define GAME_H

#include "Rules.h"
#include "Tile.h"
#include "Player.h"

class Game
{
	public:
		Game(Rules *rules);
		~Game();

		bool PlaceTile(Tile &tile, u8 x, u8 y);
		inline bool isValidPlace(Tile &tile, u8 x, u8 y) { return GameRules->ValidPlaceTile(*GameField, tile, x, y); } 

		inline Field *getField() { return GameField; }
		inline Rules *getRules() { return GameRules; }
		
		inline u8 getPlayerCount() { return PlayerCount; }
		inline Player *getPlayerList() { return Players; }

		inline Player *getCurrentPlayer() { return &Players[PlayerIndex]; }


	private:
		Rules *GameRules;

		Field *GameField;

		u8 PlayerIndex;
		u8 PlayerCount;
		Player *Players;
};

#endif

