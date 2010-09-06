#ifndef TILE_H
#define TILE_H

#include "Block.h"
#include "Types.h"

class Tile
{
	public:
		Tile(u8 width, u8 height);
		~Tile();

		inline u8 getWidth() {return Width;}

		inline u8 getHeight() {return Height;}

		inline Block getBlock(u8 x, u8 y)
		{
			return Blocks[y*Width + x];
		}

		inline void setBlock(Block block, u8 x, u8 y)
		{
			Blocks[y*Width + x] = block;
		}

		void Rotate180();

		void RotateRight();

		void RotateLeft();

		void FlipH();

		void FlipV();

	protected:
		u8 Width, Height;

		Block *Blocks;
};

#endif

