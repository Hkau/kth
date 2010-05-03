#ifndef TILESET_H
#define TILESET_H

#include "BlokusTile.h"

//! Tile set
/** Tile set for the player, contains amount of tiles and which tiles they are. */
class TileSet
{
	public:
		//! TileSet destructor
		/** Deletes tiles. */
		inline ~TileSet() { delete[] Tiles; }

		//! Get number of tiles
		/** Gets the amount of tiles in the set. */
		inline u8 getNumTiles() { return TileAmount; }

		//! Get a certain tile
		/** Gets the requested tile. */
		inline BlokusTile *getTile(u8 index) { return &Tiles[index]; }

		//! Set a certain tile
		/** Overwrites a tile in the set to a specified one. */
		inline void setTile(u8 index, BlokusTile *tile) { Tiles[index] = *tile; }

		//Create stuffs

		//! Set tile amount
		/** Sets amount of tiles, used during creation. */
		inline void setTileAmount(u8 tileAmount) { TileAmount = tileAmount; }

		//! Set tile list
		/** Sets the pointer to a BlokusTile array. */
		inline void setTileList(BlokusTile *tileList) { Tiles = tileList; }

	private:
		u8 TileAmount;
		BlokusTile *Tiles;
};

#endif

