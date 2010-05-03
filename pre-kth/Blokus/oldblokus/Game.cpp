#include "Game.h"

Game::Game(Rules *rules)
{
	GameRules = rules;

	GameField = GameRules->CreateField();
	
	Players = GameRules->CreatePlayers();
	
	PlayerCount = GameRules->getPlayerCount();

	PlayerIndex = 0;
}

Game::~Game()
{
	delete GameField;
	
	delete[] Players;
}


bool Game::PlaceTile(Tile &tile, u8 x, u8 y)
{
	if(GameRules->ValidPlaceTile(*GameField, tile, x, y))
	{
		u8 width = tile.getWidth(), height = tile.getHeight();

		for(u8 i = 0; i < width; ++i)
		{
			for(u8 j = 0; j < height; ++j)
				GameField->setBlock(tile.getBlock(i, j), i+x, j+y);
		}
		
		return true;
	}

	return false;
}

