#ifndef RULES_H
#define RULES_H

#include "Field.h"
#include "Types.h"
#include "Player.h"

class Rules
{
	public:
		inline Field *CreateField() {return new Field(FieldWidth, FieldHeight);}

		virtual Player *CreatePlayers() = 0;
		
		inline u8 getPlayerCount() { return PlayerCount; }

		virtual bool ValidPlaceTile(Field &field, Tile &tile, u8 x, u8 y) = 0;

	protected:

		u8 FieldWidth, FieldHeight;

		u8 PlayerCount;
};

#endif

