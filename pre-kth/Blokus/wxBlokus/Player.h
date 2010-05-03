#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include "TileSet.h"

#include <cstring>

//! Player
/** Contains information on a certain player. */
class Player
{
	public:
		//! Player constructor
		/** Sets default properties. */
		Player(){ setTeam(1); TileIndex = 0; HasPlacedTile = false; }


		//! Get team
		/** Returns player's team. */
		inline u8 getTeam() { return Team; }

		//! Set team
		/** Sets player's team. */
		inline void setTeam(u8 team) { Team = team; }

		//! Set next tile
		/** Sets next tile as current. */
		inline void setNextTile() { TileIndex=NextIndex(); }

		//! Set previous tile
		/** Sets previous tile as current. */
		inline void setPreviousTile(){ TileIndex=PreviousIndex(); }

		//! Get previous tile
		/** Gets previous tile. */
		inline BlokusTile *getPreviousTile() { return Tiles.getTile(PreviousIndex());}

		//! Get next tile
		/** Gets next tile. */
		inline BlokusTile *getNextTile() { return Tiles.getTile(NextIndex()); }

		//! Get current tile
		/** Gets current tile. */
		inline BlokusTile *getCurrentTile() { return Tiles.getTile(TileIndex); }

		//! Set next valid tile
		/** Sets next valid tile as current, if there are any. */
		bool setNextValidTile()
		{
			u8 index = NextIndex();
			while(index != TileIndex)
			{
				if(Tiles.getTile(index)->getAmount() > 0)
				{
					TileIndex = index;
					return true;
				}

				if(index >= Tiles.getNumTiles()-1)
					index = 0;
				else
					++index;
			}

			return getCurrentTile()->getAmount() != 0;
		}

		//! Get TileSet
		/** Gets player's TileSet. */
		inline TileSet *getTileSet() { return &Tiles; }

		//! Gets player's placed first tile status
		/** Returns true if the player has placed a tile, else false. */
		inline bool PlacedFirstTile() { return HasPlacedTile; }

		//! Used tile
		/** Sets player's tile as used. */
		inline void UsedTile() { HasPlacedTile = true; }

		//! Get score
		/** Calculates and returns player's score. */
		u16 getScore()
		{
			u16 score = 0;
			for(u8 index = 0; index < Tiles.getNumTiles(); ++index)
			{
				score += Tiles.getTile(index)->getScore() * Tiles.getTile(index)->getAmount();
			}

			return score;
		}

	protected:

		//! Next index
		/** Returns next index. */
		u8 NextIndex()
		{
			if(TileIndex >= Tiles.getNumTiles()-1)
				return 0;
			return TileIndex + 1;
		}

		//! Previous index
		/** Returns previous index. */
		u8 PreviousIndex()
		{
			if(TileIndex == 0)
				return Tiles.getNumTiles() - 1;
			else
				return TileIndex - 1;
		}

		u8 Team;

		u8 TileIndex;
		TileSet Tiles;
		bool HasPlacedTile;
};

#endif
