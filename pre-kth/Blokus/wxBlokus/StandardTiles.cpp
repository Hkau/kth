#include "StandardTiles.h"

#define	NUM_TILES	21 

Tile StandardTiles[NUM_TILES];
u8 StandardScore[NUM_TILES] = { 1, 2, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

void InitStandardTiles()
{
	// Tile #1
	TileInit(&StandardTiles[0], 1, 1);

	StandardTiles[0].Blocks[0] = 1;

	// Tile #2
	TileInit(&StandardTiles[1], 2, 1);
	
	StandardTiles[1].Blocks[0] = 1;
	StandardTiles[1].Blocks[1] = 1;
	
	// Tile #3
	TileInit(&StandardTiles[2], 2, 2);
	
	StandardTiles[2].Blocks[0] = 1;
	StandardTiles[2].Blocks[1] = 1;
	StandardTiles[2].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[2].Blocks[3] = 1;
	
	// Tile #4
	TileInit(&StandardTiles[3], 3, 1);
	
	StandardTiles[3].Blocks[0] = 1;
	StandardTiles[3].Blocks[1] = 1;
	StandardTiles[3].Blocks[2] = 1;
	
	// Tile #5
	TileInit(&StandardTiles[4], 2, 2);
	
	StandardTiles[4].Blocks[0] = 1;
	StandardTiles[4].Blocks[1] = 1;
	StandardTiles[4].Blocks[2] = 1;
	StandardTiles[4].Blocks[3] = 1;

	// Tile #6
	TileInit(&StandardTiles[5], 3, 2);
	
	StandardTiles[5].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[5].Blocks[1] = 1;
	StandardTiles[5].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[5].Blocks[3] = 1;
	StandardTiles[5].Blocks[4] = 1;
	StandardTiles[5].Blocks[5] = 1;

	// Tile #7
	TileInit(&StandardTiles[6], 4, 1);
	
	StandardTiles[6].Blocks[0] = 1;
	StandardTiles[6].Blocks[1] = 1;
	StandardTiles[6].Blocks[2] = 1;
	StandardTiles[6].Blocks[3] = 1;
	
	// Tile #8
	TileInit(&StandardTiles[7], 3, 2);
	
	StandardTiles[7].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[7].Blocks[1] = BLOCK_EMPTY;
	StandardTiles[7].Blocks[2] = 1;
	StandardTiles[7].Blocks[3] = 1;
	StandardTiles[7].Blocks[4] = 1;
	StandardTiles[7].Blocks[5] = 1;

	// Tile #9
	TileInit(&StandardTiles[8], 3, 2);
	
	StandardTiles[8].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[8].Blocks[1] = 1;
	StandardTiles[8].Blocks[2] = 1;
	StandardTiles[8].Blocks[3] = 1;
	StandardTiles[8].Blocks[4] = 1;
	StandardTiles[8].Blocks[5] = BLOCK_EMPTY;

	// Tile #10
	TileInit(&StandardTiles[9], 4, 2);
	
	StandardTiles[9].Blocks[0] = 1;
	StandardTiles[9].Blocks[1] = BLOCK_EMPTY;
	StandardTiles[9].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[9].Blocks[3] = BLOCK_EMPTY;
	StandardTiles[9].Blocks[4] = 1;
	StandardTiles[9].Blocks[5] = 1;
	StandardTiles[9].Blocks[6] = 1;
	StandardTiles[9].Blocks[7] = 1;

	// Tile #11
	TileInit(&StandardTiles[10], 3, 3);
	
	StandardTiles[10].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[10].Blocks[1] = 1;
	StandardTiles[10].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[10].Blocks[3] = BLOCK_EMPTY;
	StandardTiles[10].Blocks[4] = 1;
	StandardTiles[10].Blocks[5] = BLOCK_EMPTY;
	StandardTiles[10].Blocks[6] = 1;
	StandardTiles[10].Blocks[7] = 1;
	StandardTiles[10].Blocks[8] = 1;

	// Tile #12
	TileInit(&StandardTiles[11], 3, 3);
	
	StandardTiles[11].Blocks[0] = 1;
	StandardTiles[11].Blocks[1] = BLOCK_EMPTY;
	StandardTiles[11].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[11].Blocks[3] = 1;
	StandardTiles[11].Blocks[4] = BLOCK_EMPTY;
	StandardTiles[11].Blocks[5] = BLOCK_EMPTY;
	StandardTiles[11].Blocks[6] = 1;
	StandardTiles[11].Blocks[7] = 1;
	StandardTiles[11].Blocks[8] = 1;

	// Tile #13
	TileInit(&StandardTiles[12], 4, 2);
	
	StandardTiles[12].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[12].Blocks[1] = 1;
	StandardTiles[12].Blocks[2] = 1;
	StandardTiles[12].Blocks[3] = 1;
	StandardTiles[12].Blocks[4] = 1;
	StandardTiles[12].Blocks[5] = 1;
	StandardTiles[12].Blocks[6] = BLOCK_EMPTY;
	StandardTiles[12].Blocks[7] = BLOCK_EMPTY;

	// Tile #14
	TileInit(&StandardTiles[13], 3, 3);
	
	StandardTiles[13].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[13].Blocks[1] = BLOCK_EMPTY;
	StandardTiles[13].Blocks[2] = 1;
	StandardTiles[13].Blocks[3] = 1;
	StandardTiles[13].Blocks[4] = 1;
	StandardTiles[13].Blocks[5] = 1;
	StandardTiles[13].Blocks[6] = 1;
	StandardTiles[13].Blocks[7] = BLOCK_EMPTY;
	StandardTiles[13].Blocks[8] = BLOCK_EMPTY;

	// Tile #15
	TileInit(&StandardTiles[14], 1, 5);
	
	StandardTiles[14].Blocks[0] = 1;
	StandardTiles[14].Blocks[1] = 1;
	StandardTiles[14].Blocks[2] = 1;
	StandardTiles[14].Blocks[3] = 1;
	StandardTiles[14].Blocks[4] = 1;

	// Tile #16
	TileInit(&StandardTiles[15], 2, 3);
	
	StandardTiles[15].Blocks[0] = 1;
	StandardTiles[15].Blocks[1] = BLOCK_EMPTY;
	StandardTiles[15].Blocks[2] = 1;
	StandardTiles[15].Blocks[3] = 1;
	StandardTiles[15].Blocks[4] = 1;
	StandardTiles[15].Blocks[5] = 1;

	// Tile #17
	TileInit(&StandardTiles[16], 3, 3);
	
	StandardTiles[16].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[16].Blocks[1] = 1;
	StandardTiles[16].Blocks[2] = 1;
	StandardTiles[16].Blocks[3] = 1;
	StandardTiles[16].Blocks[4] = 1;
	StandardTiles[16].Blocks[5] = BLOCK_EMPTY;
	StandardTiles[16].Blocks[6] = 1;
	StandardTiles[16].Blocks[7] = BLOCK_EMPTY;
	StandardTiles[16].Blocks[8] = BLOCK_EMPTY;

	// Tile #18
	TileInit(&StandardTiles[17], 2, 3);
	
	StandardTiles[17].Blocks[0] = 1;
	StandardTiles[17].Blocks[1] = 1;
	StandardTiles[17].Blocks[2] = 1;
	StandardTiles[17].Blocks[3] = BLOCK_EMPTY;
	StandardTiles[17].Blocks[4] = 1;
	StandardTiles[17].Blocks[5] = 1;

	// Tile #19
	TileInit(&StandardTiles[18], 3, 3);
	
	StandardTiles[18].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[18].Blocks[1] = 1;
	StandardTiles[18].Blocks[2] = 1;
	StandardTiles[18].Blocks[3] = 1;
	StandardTiles[18].Blocks[4] = 1;
	StandardTiles[18].Blocks[5] = BLOCK_EMPTY;
	StandardTiles[18].Blocks[6] = BLOCK_EMPTY;
	StandardTiles[18].Blocks[7] = 1;
	StandardTiles[18].Blocks[8] = BLOCK_EMPTY;

	// Tile #20
	TileInit(&StandardTiles[19], 3, 3);
	
	StandardTiles[19].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[19].Blocks[1] = 1;
	StandardTiles[19].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[19].Blocks[3] = 1;
	StandardTiles[19].Blocks[4] = 1;
	StandardTiles[19].Blocks[5] = 1;
	StandardTiles[19].Blocks[6] = BLOCK_EMPTY;
	StandardTiles[19].Blocks[7] = 1;
	StandardTiles[19].Blocks[8] = BLOCK_EMPTY;

	// Tile #21
	TileInit(&StandardTiles[20], 4, 2);
	
	StandardTiles[20].Blocks[0] = BLOCK_EMPTY;
	StandardTiles[20].Blocks[1] = 1;
	StandardTiles[20].Blocks[2] = BLOCK_EMPTY;
	StandardTiles[20].Blocks[3] = BLOCK_EMPTY;
	StandardTiles[20].Blocks[4] = 1;
	StandardTiles[20].Blocks[5] = 1;
	StandardTiles[20].Blocks[6] = 1;
	StandardTiles[20].Blocks[7] = 1;
}

void FillStandardTileSet(TileSet *tileSet)
{
	BlokusTile *tiles = new BlokusTile[NUM_TILES];

	for(unsigned int i = 0; i < NUM_TILES; ++i)
	{
		tiles[i].setTile(&StandardTiles[i]);
		tiles[i].setScore(StandardScore[i]);
		tiles[i].setAmount(1);
	}

	tileSet->setTileAmount(NUM_TILES);
	tileSet->setTileList(tiles);
}

void DestroyStandardTiles()
{
	for(unsigned int i = 0; i < NUM_TILES; ++i)
		TileDestroy(&StandardTiles[i]);
}

