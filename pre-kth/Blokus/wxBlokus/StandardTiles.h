#ifndef STANDARDTILES_H
#define STANDARDTILES_H

#include "TileSet.h"

//! Initialize standard tiles
/** Creates the 21 standard Blokus tiles. */
void InitStandardTiles();
//! Fill standard TileSet.
/** Fills a TileSet with the 21 tiles. */
void FillStandardTileSet(TileSet *tileSet);
//! Destroy standard tiles
/** Deletes all 21 standard tiles. */
void DestroyStandardTiles();

/*
class StandardTile1: public BlokusTile
{
	public:
		StandardTile1(u8 team):BlokusTile(3,3)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[2] = BLOCK_CORNER;
			Blocks[4] = team;
			Blocks[6] = BLOCK_CORNER;
			Blocks[8] = BLOCK_CORNER;

			Score = 1;
		}
};

class StandardTile2: public BlokusTile
{
	public:
		StandardTile2(u8 team):BlokusTile(4,3)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = team;
			Blocks[6] = team;
			Blocks[8] = BLOCK_CORNER;
			Blocks[11] = BLOCK_CORNER;

			Score = 2;
		}
};TileInit(&StandardTiles[0], 1, 1);

class StandardTile3: public BlokusTile
{
	public:
		StandardTile3(u8 team):BlokusTile(4,4)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = team;
			Blocks[6] = team;
			Blocks[8] = BLOCK_CORNER;
			Blocks[10] = team;
			Blocks[13] = BLOCK_CORNER;
			Blocks[15] = BLOCK_CORNER;

			Score = 3;
		}
};

class StandardTile4: public BlokusTile
{
	public:
		StandardTile4(u8 team):BlokusTile(5,3)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[6] = team;
			Blocks[7] = team;
			Blocks[8] = team;
			Blocks[10] = BLOCK_CORNER;
			Blocks[14] = BLOCK_CORNER;

			Score = 3;
		}
};

class StandardTile5: public BlokusTile
{
	public:
		StandardTile5(u8 team):BlokusTile(4,4)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = team;
			Blocks[6] = team;
			Blocks[9] = team;
			Blocks[10] = team;
			Blocks[12] = BLOCK_CORNER;
			Blocks[15] = BLOCK_CORNER;

			Score = 4;
		}
};

class StandardTile6: public BlokusTile
{
	public:
		StandardTile6(u8 team):BlokusTile(5,4)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[9] = BLOCK_CORNER;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[13] = team;
			Blocks[15] = BLOCK_CORNER;
			Blocks[19] = BLOCK_CORNER;

			Score = 4;
		}
};

class StandardTile7: public BlokusTile
{
	public:
		StandardTile7(u8 team):BlokusTile(6,3)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[8] = team;
			Blocks[9] = team;
			Blocks[10] = team;
			Blocks[12] = BLOCK_CORNER;
			Blocks[17] = BLOCK_CORNER;

			Score = 4;
		}
};

class StandardTile8: public BlokusTile
{
	public:
		StandardTile8(u8 team):BlokusTile(5,4)
		{
			Blocks[2] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[8] = team;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[13] = team;
			Blocks[15] = BLOCK_CORNER;
			Blocks[19] = BLOCK_CORNER;

			Score = 4;
		}
};

class StandardTile9: public BlokusTile
{
	public:
		StandardTile9(u8 team):BlokusTile(5,4)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[8] = team;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[14] = BLOCK_CORNER;
			Blocks[15] = BLOCK_CORNER;
			Blocks[18] = BLOCK_CORNER;

			Score = 4;
		}
};

class StandardTile10: public BlokusTile
{
	public:
		StandardTile10(u8 team):BlokusTile(6,4)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[2] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[11] = BLOCK_CORNER;
			Blocks[13] = team;
			Blocks[14] = team;
			Blocks[15] = team;
			Blocks[16] = team;
			Blocks[18] = BLOCK_CORNER;
			Blocks[23] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile11: public BlokusTile
{
	public:
		StandardTile11(u8 team):BlokusTile(5,5)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[10] = BLOCK_CORNER;
			Blocks[12] = team;
			Blocks[14] = BLOCK_CORNER;
			Blocks[16] = team;
			Blocks[17] = team;
			Blocks[18] = team;
			Blocks[20] = BLOCK_CORNER;
			Blocks[24] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile12: public BlokusTile
{
	public:
		StandardTile12(u8 team):BlokusTile(5,5)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[2] = BLOCK_CORNER;
			Blocks[6] = team;
			Blocks[11] = team;
			Blocks[14] = BLOCK_CORNER;
			Blocks[16] = team;
			Blocks[17] = team;
			Blocks[18] = team;
			Blocks[20] = BLOCK_CORNER;
			Blocks[24] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile13: public BlokusTile
{
	public:
		StandardTile13(u8 team):BlokusTile(6,4)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[6] = BLOCK_CORNER;
			Blocks[8] = team;
			Blocks[9] = team;
			Blocks[10] = team;
			Blocks[13] = team;
			Blocks[14] = team;
			Blocks[17] = BLOCK_CORNER;
			Blocks[18] = BLOCK_CORNER;
			Blocks[21] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile14: public BlokusTile
{
	public:
		StandardTile14(u8 team):BlokusTile(5,5)
		{
			Blocks[2] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[8] = team;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[13] = team;
			Blocks[16] = team;
			Blocks[19] = BLOCK_CORNER;
			Blocks[20] = BLOCK_CORNER;
			Blocks[22] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile15: public BlokusTile
{
	public:
		StandardTile15(u8 team):BlokusTile(3,7)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[2] = BLOCK_CORNER;
			Blocks[4] = team;
			Blocks[7] = team;
			Blocks[10] = team;
			Blocks[13] = team;
			Blocks[16] = team;
			Blocks[18] = BLOCK_CORNER;
			Blocks[20] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile16: public BlokusTile
{
	public:
		StandardTile16(u8 team):BlokusTile(4,5)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[2] = BLOCK_CORNER;
			Blocks[5] = team;
			Blocks[7] = BLOCK_CORNER;
			Blocks[9] = team;
			Blocks[10] = team;
			Blocks[13] = team;
			Blocks[14] = team;
			Blocks[16] = BLOCK_CORNER;
			Blocks[19] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile17: public BlokusTile
{
	public:
		StandardTile17(u8 team):BlokusTile(5,5)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[8] = team;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[14] = BLOCK_CORNER;
			Blocks[16] = team;
			Blocks[18] = BLOCK_CORNER;
			Blocks[20] = BLOCK_CORNER;
			Blocks[22] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile18: public BlokusTile
{
	public:
		StandardTile18(u8 team):BlokusTile(4,5)
		{
			Blocks[0] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = team;
			Blocks[6] = team;
			Blocks[9] = team;
			Blocks[11] = BLOCK_CORNER;
			Blocks[13] = team;
			Blocks[14] = team;
			Blocks[16] = BLOCK_CORNER;
			Blocks[19] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile19: public BlokusTile
{
	public:
		StandardTile19(u8 team):BlokusTile(5,5)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[4] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[8] = team;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[14] = BLOCK_CORNER;
			Blocks[15] = BLOCK_CORNER;
			Blocks[17] = team;
			Blocks[21] = BLOCK_CORNER;
			Blocks[23] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile20: public BlokusTile
{
	public:
		StandardTile20(u8 team):BlokusTile(5,5)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[5] = BLOCK_CORNER;
			Blocks[7] = team;
			Blocks[9] = BLOCK_CORNER;
			Blocks[11] = team;
			Blocks[12] = team;
			Blocks[13] = team;
			Blocks[15] = BLOCK_CORNER;
			Blocks[17] = team;
			Blocks[19] = BLOCK_CORNER;
			Blocks[21] = BLOCK_CORNER;
			Blocks[23] = BLOCK_CORNER;

			Score = 5;
		}
};

class StandardTile21: public BlokusTile
{
	public:
		StandardTile21(u8 team):BlokusTile(6,4)
		{
			Blocks[1] = BLOCK_CORNER;
			Blocks[3] = BLOCK_CORNER;
			Blocks[6] = BLOCK_CORNER;
			Blocks[8] = team;
			Blocks[11] = BLOCK_CORNER;
			Blocks[13] = team;
			Blocks[14] = team;
			Blocks[15] = team;
			Blocks[16] = team;
			Blocks[18] = BLOCK_CORNER;
			Blocks[23] = BLOCK_CORNER;

			Score = 5;
		}
};
*/
#endif

