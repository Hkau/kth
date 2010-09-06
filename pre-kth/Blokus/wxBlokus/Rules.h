#ifndef RULES_H
#define RULES_H

#include "Field.h"
#include "Types.h"
#include "Player.h"

//! Rules
/** Virtual base class. Handles creation of field, players etc.. */
class Rules
{
	public:
		//! Create field
		/** Allocates and returns a field. */
		virtual Field *CreateField()
		{
			Field *field = new Field;
			TileInit(field , FieldWidth, FieldHeight);
			for(unsigned int i = 0; i < field->Width * field->Height; ++i)
				field->Blocks[i] = BLOCK_EMPTY;
			return field;
		}

		//! Create players
		/** Returns array of players. */
		virtual Player *CreatePlayers() = 0;

		//! Get player count
		/** Returns player count. */
		inline u8 getPlayerCount() { return PlayerCount; }

		//! Tells if tile can be placed at location
		/** Returns true if the tile can be placed by player at the location. */
		virtual bool ValidPlaceTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y) = 0;

		//! Placed Tile
		/** Gets called by game after tile has been placed. */
		virtual void PlacedTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y) = 0;

	protected:

		u8 FieldWidth, FieldHeight;

		u8 PlayerCount;
};

#endif

