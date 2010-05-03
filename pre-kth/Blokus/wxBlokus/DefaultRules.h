#ifndef DEFAULTRULES_H
#define DEFAULTRULES_H

#include "Rules.h"

//! Default rules
/** The default Blokus rules */
class DefaultRules: public Rules
{
	public:
		//! DefaultRules constructor
		/** Sets field width, height, Player count and initialize standard tiles. */
		DefaultRules();

		//! DefaultRules destructor
		/** Destroy the standard tiles. */
		virtual ~DefaultRules();

		//! Create players
		/** Allocates a list of players and gives them a TileSet. */
		Player* CreatePlayers();

		//! Checks if a tile can be placed at a certain location
		/** Checks if a tile can be placed at a certain location, according to the official Blokus rules. */
		bool ValidPlaceTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y);

		//! Called after a tile has been placed
		/** Removes a tile from the count and sets that player has used a tile. */
		void PlacedTile(Field *field, Player *player, BlokusTile *tile, u8 x, u8 y);
};

#endif

