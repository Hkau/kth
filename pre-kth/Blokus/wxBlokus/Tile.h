#ifndef TILE_H
#define TILE_H

#include "Block.h"
#include "Types.h"

//! Tile struct
/** Contains width, height and Block array. */
typedef struct
{
	u8 Width, Height;
	Block *Blocks;
} Tile;

//! Initialize tile
/** Sets width, height and allocates Tile blocks. */
void TileInit(Tile *tile, u8 width, u8 height);

//! Destroy tile
/** Deletes block array. */
void TileDestroy(Tile *tile);

//! Rotate180
/** Rotates the tile 180 degrees. */
void TileRotate180(Tile *tile);

//! Rotate tile to the right
/** Rotates tile to the right. */
void TileRotateRight(Tile *tile);

//! Rotate tile to the left
/** Rotates tile to the left. */
void TileRotateLeft(Tile *tile);

//! Flip tile horizontally
/** Flips tile horizontally. */
void TileFlipHorizontal(Tile *tile);

//! Flip tile vertically
/** Flips tile vertically. */
void TileFlipVertical(Tile *tile);

#endif

