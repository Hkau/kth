#ifndef PLAYER_H
#define PLAYER_H

#include "Types.h"
#include "BlokusTile.h"

#include <cstring>

class Player
{
	public:
		Player(){ setTeam(1); strcpy(Name,"SpelarN^"); TileIndex = 0; }
		Player(u8 team) { setTeam(team); strcpy(Name,"SpelarN^"); TileIndex = 0; }
		
		~Player() { for(u8 i = 0; i < TileAmount; ++i) delete Tiles[i]; delete[] Tiles; }

		inline void setTeam(u8 team) { Team = team; }

		inline u8 getTileAmount() { return TileAmount; }
		inline BlokusTile **getTiles() { return Tiles; }
		inline void setTile(BlokusTile *tile, int index) { Tiles[index] = tile; }
		inline void AllocateTiles(int tiles) {Tiles = new BlokusTile*[tiles]; TileAmount = tiles; }

		void NextTile() { ++TileIndex; if(TileIndex >= TileAmount) TileIndex = 0;}

		void PreviousTile() { if(TileIndex == 0) TileIndex = TileAmount - 1; else --TileIndex; }
		
		inline BlokusTile *getCurrentTile() { return Tiles[TileIndex]; }

		inline void setName(const char *name) {strcpy(Name, name);}
		inline const char *getName() { return Name; }

		// TODO - score

	protected:
		u8 Team;
		char Name[16];
		
		u8 TileAmount;
		u8 TileIndex;
		BlokusTile **Tiles;
};

#endif
