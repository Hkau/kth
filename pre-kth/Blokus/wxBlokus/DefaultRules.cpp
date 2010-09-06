#include "DefaultRules.h"

#include "StandardTiles.h"

DefaultRules::DefaultRules()
{
	FieldWidth = 20;
	FieldHeight = 20;

	PlayerCount = 4;

	InitStandardTiles();
}

DefaultRules::~DefaultRules()
{
	DestroyStandardTiles();
}

bool DefaultRules::ValidPlaceTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y)
{
	if(tile->getAmount() == 0)
		return false;

	// Check corners at start
	if(!player->PlacedFirstTile())
	{
		if(x == 0)
		{
			if(y == 0)
			{
				if(field->Blocks[0] != BLOCK_EMPTY)
					return false;
				if(tile->getTileData()->Blocks[0] == BLOCK_EMPTY)
					return false;
				return true;
			}
			else if (y == field->Height - tile->getTileData()->Height)
			{
				if(field->Blocks[(field->Height-1)*field->Width] != BLOCK_EMPTY)
					return false;
				if(tile->getTileData()->Blocks[(tile->getTileData()->Height-1)*tile->getTileData()->Width] == BLOCK_EMPTY)
					return false;
				return true;
			}
			return false;
		}
		else if (x == field->Width - tile->getTileData()->Width)
		{
			if(y == 0)
			{
				if(field->Blocks[field->Width-1] != BLOCK_EMPTY)
					return false;
				if(tile->getTileData()->Blocks[tile->getTileData()->Width-1] == BLOCK_EMPTY)
					return false;
				return true;
			}
			else if (y == field->Height - tile->getTileData()->Height)
			{
				if(field->Blocks[(field->Height-1)*field->Width+field->Width-1] != BLOCK_EMPTY)
					return false;
				return true;
			}
			return false;
		}
		return false;
	}

	// Check for overlapping
	for(u8 i = 0; i < tile->getTileData()->Width; ++i)
	{
		for(u8 j = 0; j < tile->getTileData()->Height; ++j)
		{
			if(tile->getTileData()->Blocks[j*tile->getTileData()->Width + i] != BLOCK_EMPTY
			&& field->Blocks[(j+y)*field->Width + x + i] != BLOCK_EMPTY)
				return false;
		}
	}

	// Check team color

	Block b = player->getTeam();

/*	// Check left edge.

	if(x != 0)
	{
		for(u8 i = 0; i < tile->getTileData()->Height; ++i)
		{
			if(tile->getTileData()->Blocks[i*tile->getTileData()->Width] != BLOCK_EMPTY
			&& field->Blocks[(y+i)*field->Width + x - 1] == b)
				return false;
		}
	}

	// Check right edge.

	if(x != field->Width - tile->getTileData()->Width)
	{
		for(u8 i = 0; i < tile->getTileData()->Height; ++i)
		{
			if(tile->getTileData()->Blocks[(i+1)*tile->getTileData()->Width - 1] != BLOCK_EMPTY
			&& field->Blocks[(y+i)*field->Width + tile->getTileData()->Width + x] == b)
				return false;
		}
	}

	// Check top edge.

	if(y != 0)
	{
		for(u8 i = 0; i < tile->getTileData()->Width; ++i)
		{
			if(tile->getTileData()->Blocks[i] != BLOCK_EMPTY
			&& field->Blocks[(y-1)*field->Width + (x+i)] == b)
				return false;
		}
	}

	// Check bottom edge.

	if(y != field->Height - tile->getTileData()->Height)
	{
		for(u8 i = 0; i < tile->getTileData()->Width; ++i)
		{
			if(tile->getTileData()->Blocks[(tile->getTileData()->Height-1)*tile->getTileData()->Width + i] != BLOCK_EMPTY
			&& field->Blocks[(y+tile->getTileData()->Height)*field->Width + (x+i)] == b)
				return false;
		}
	}*/

	// Check for adjacent tiles of same color

	for(u8 i = 0; i < tile->getTileData()->Width; ++i)
	{
		for(u8 j = 0; j < tile->getTileData()->Height; ++j)
		{
			if(tile->getTileData()->Blocks[j*tile->getTileData()->Width + i] != BLOCK_EMPTY)
			{
				// Check left edge.

				if((x+i) != 0 && field->Blocks[(y+j)*field->Width + (x+i-1)] == b)
					return false;

				// Check right side.

				if((x+i) != field->Width -1 && field->Blocks[(y+j)*field->Width + (x+i+1)] == b)
					return false;

				// Check above block.

				if((y+j) != 0 && field->Blocks[(y+j-1)*field->Width + (x+i)] == b)
					return false;

				// Check below block.

				if((y+j) != field->Height -1 && field->Blocks[(y+j+1)*field->Width + (x+i)] == b)
					return false;
			}
		}
	}

	// Check for diagonal tiles of same color

/*	if(x != 0 && y != 0)
	{
		if(tile->getTileData()->Blocks[0] != BLOCK_EMPTY && field->Blocks[(y-1)*field->Width + (x-1)] == b)
			return true;
	}*/

	for(u8 i = 0; i < tile->getTileData()->Width; ++i)
	{
		for(u8 j = 0; j < tile->getTileData()->Height; ++j)
		{
			if(tile->getTileData()->Blocks[j*tile->getTileData()->Width + i] != BLOCK_EMPTY)
			{

				// Check tile corners

				if((x+i) != 0 && (y+j) != 0 && field->Blocks[(y-1+j)*field->Width + (x+i-1)] == b)
					return true;

				if((x+i) != 0 && (y+j) != field->Height - 1 && field->Blocks[(y+j+1)*field->Width + (x+i-1)] == b)
					return true;

				if((x+i) != field->Width -1 && (y+j) != 0 && field->Blocks[(y+j-1)*field->Width + (x+i+1)] == b)
					return true;

				if((x+i) != field->Width -1 && (y+j) != field->Height -1 && field->Blocks[(y+j+1)*field->Width + (x+i+1)] == b)
					return true;
			}
		}
	}

	return false;
}

void DefaultRules::PlacedTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y)
{
	tile->setAmount(tile->getAmount() - 1);
	player->UsedTile();
}

Player *DefaultRules::CreatePlayers()
{
	Player *players = new Player[PlayerCount];

	for(u8 i = 0; i < PlayerCount; ++i)
	{
		players[i].setTeam(i+1);

		FillStandardTileSet(players[i].getTileSet());
	}

	return players;
}

