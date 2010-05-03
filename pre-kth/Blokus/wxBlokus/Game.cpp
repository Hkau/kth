#include "Game.h"

#include <cstdio>

Game::Game(Rules *rules, void (*endGameCallback)())
{
	GameRules = rules;

	GameField = GameRules->CreateField();

	Players = GameRules->CreatePlayers();

	PlayerCount = GameRules->getPlayerCount();

	PlayerIndex = 0;

	EndGameCallback = endGameCallback;
}

Game::~Game()
{
	TileDestroy(GameField);
	delete GameField;

	delete[] Players;
}


bool Game::PlaceTile(BlokusTile *blokusTile, u8 x, u8 y)
{
	if(GameRules->ValidPlaceTile(GameField, getCurrentPlayer(), blokusTile, x, y))
	{
		Block b = getCurrentPlayer()->getTeam();
		for(unsigned int i = 0; i < blokusTile->getTileData()->Width; ++i)
		{
			for(unsigned int j = 0; j < blokusTile->getTileData()->Height; ++j)
			{
				if(blokusTile->getTileData()->Blocks[j*blokusTile->getTileData()->Width + i] != BLOCK_EMPTY)
					GameField->Blocks[(y+j)*GameField->Width + x + i] = b;
			}
		}

		GameRules->PlacedTile(GameField, getCurrentPlayer(), blokusTile, x, y);

		if(!getCurrentPlayer()->setNextValidTile())
			EndGame();
		else
			CyclePlayers();

		return true;

/*		Tile *tile = blokusTile->getTileData();

		for(u8 i = 0; i < tile->Width; ++i)
		{
			for(u8 j = 0; j < tile->Height; ++j)
			{
				Block b = tile->Blocks[y*tile->Width + x];
				if(b != BLOCK_EMPTY)
					GameField->Blocks[(j+y)*tile->Width + i+x] = b;
				//GameField->Blocks(tile.getBlock(i, j), i+x, j+y);
			}
		}

		GameRules->PlacedTile(GameField, blokusTile, x, y);

		return true;*/
	}


	return false;
}

bool Game::SaveGame(const char *path)
{
	FILE *file = fopen(path, "w");

	if(file == NULL)
		return false;

	// Save field
	fwrite(GameField->Blocks, sizeof(Block), GameField->Width * GameField->Height, file);

	// Save player index
	fwrite(&PlayerIndex, sizeof(u8), 1, file);

	// Save players
	for(u8 playerIndex = 0; playerIndex < PlayerCount; ++playerIndex)
	{
		Player *player = getPlayer(playerIndex);
		bool placedFirstTile = player->PlacedFirstTile();
		fwrite(&placedFirstTile, sizeof(bool), 1, file);

		TileSet *tileSet = player->getTileSet();

		for(u8 tileIndex = 0; tileIndex < tileSet->getNumTiles(); ++tileIndex)
		{
			u8 amount = tileSet->getTile(tileIndex)->getAmount();
			fwrite(&amount, sizeof(u8), 1, file);
		}
	}

	fclose(file);
	
	return true;
}

bool Game::LoadGame(const char *path)
{
	FILE *file = fopen(path, "r");

	if(file == NULL)
		return false;

	// Read field
	fread(GameField->Blocks, sizeof(Block), GameField->Width * GameField->Height, file);

	// Read player index
	fread(&PlayerIndex, sizeof(u8), 1, file);

	// Read players
	for(u8 playerIndex = 0; playerIndex < PlayerCount; ++playerIndex)
	{
		Player *player = getPlayer(playerIndex);

		bool playerPlacedFirstTile;

		fread(&playerPlacedFirstTile, sizeof(bool), 1, file);

		if(playerPlacedFirstTile)
			player->UsedTile();

		TileSet *tileSet = player->getTileSet();

		for(u8 tileIndex = 0; tileIndex < tileSet->getNumTiles(); ++tileIndex)
		{
			u8 tileAmount;
			fread(&tileAmount, sizeof(u8), 1, file);
			tileSet->getTile(tileIndex)->setAmount(tileAmount);
		}
	}

	fclose(file);

	return true;
}


