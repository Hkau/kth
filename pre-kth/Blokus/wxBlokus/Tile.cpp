#include "Tile.h"
#include <cstdlib>

void TileInit(Tile *tile, u8 width, u8 height)
{
	tile->Width = width;
	tile->Height = height;
	
	tile->Blocks = new Block[width*height];
}

void TileDestroy(Tile *tile)
{
	delete[] tile->Blocks;
}

void TileRotate180(Tile *tile)
{
	Block *oldBlocks = tile->Blocks;

	tile->Blocks = new Block[tile->Width*tile->Height];

	for(u16 i = 0; i < tile->Width*tile->Height; ++i)
	{
		tile->Blocks[i] = oldBlocks[tile->Width*tile->Height-1-i];
	}

	delete[] oldBlocks;
}

void TileRotateRight(Tile *tile)
{
	Block *oldBlocks = tile->Blocks;

	tile->Blocks = new Block[tile->Width*tile->Height];

	u8 oldHeight = tile->Height, oldWidth = tile->Width;

	tile->Width = oldHeight;

	tile->Height = oldWidth;

	for(unsigned int i = 0; i < tile->Height; ++i)
	{
		for(unsigned int j = 0; j < tile->Width; ++j)
		{
			tile->Blocks[i*tile->Width + j] = oldBlocks[(oldHeight-1-j)*oldWidth + i];
		}
	}

	delete[] oldBlocks;
}

void TileRotateLeft(Tile *tile)
{
	Block *oldBlocks = tile->Blocks;

	tile->Blocks = new Block[tile->Width*tile->Height];

	u8 oldHeight = tile->Height, oldWidth = tile->Width;

	tile->Width = oldHeight;

	tile->Height = oldWidth;

	for(unsigned int i = 0; i < tile->Height; ++i)
	{
		for(unsigned int j = 0; j < tile->Width; ++j)
		{
			tile->Blocks[i*tile->Width + j] = oldBlocks[j*oldWidth + oldWidth-1-i];
		}
	}

	delete[] oldBlocks;
}

void TileFlipHorizontal(Tile *tile)
{
	Block *oldBlocks = tile->Blocks;

	tile->Blocks = new Block[tile->Width*tile->Height];

	for(unsigned int i = 0; i < tile->Height; ++i)
	{
		for(unsigned int j = 0; j < tile->Width; ++j)
		{
			tile->Blocks[i*tile->Width + j] = oldBlocks[i*tile->Width + tile->Width-1-j];
		}
	}

	delete[] oldBlocks;
}

void TileFlipVertical(Tile *tile)
{
	Block *oldBlocks = tile->Blocks;

	tile->Blocks = new Block[tile->Width*tile->Height];

	for(unsigned int i = 0; i < tile->Height; ++i)
	{
		for(unsigned int j = 0; j < tile->Width; ++j)
		{
			tile->Blocks[i*tile->Width + j] = oldBlocks[(tile->Height-1-i)*tile->Width + j];
		}
	}

	delete[] oldBlocks;
}

/*Tile::Tile(u8 width, u8 height)
{
	Width = width;

	Height = height;

	Blocks = new Block[Width*Height];

	//for (u16 i = 0; i < Width * Height; ++i)
		//Blocks[i].setValue(1);

	//printf("LOLGNESTA?");

	//for(u16 i = 0; i < Height; ++i)
		//FieldBlocks[i*Width].setValue(0xAA);
}

Tile::~Tile()
{
	delete[] Blocks;
}

void Tile::Rotate180()
{
	Block *oldBlocks = Blocks;

	Blocks = new Block[Width*Height];

	for(u16 i = 0; i < Width*Height; ++i)
	{
		Blocks[i] = oldBlocks[Width*Height-1-i];
	}

	delete[] oldBlocks;
}

void Tile::RotateLeft()
{
	Block *oldBlocks = Blocks;

	Blocks = new Block[Width*Height];

	u8 oldHeight = Height, oldWidth = Width;

	Width = oldHeight;

	Height = oldWidth;

	for(unsigned int i = 0; i < Height; ++i)
	{
		for(unsigned int j = 0; j < Width; ++j)
		{
			Blocks[i*Width + j] = oldBlocks[j*oldWidth + oldWidth-1-i];
		}
	}

	delete[] oldBlocks;
}

void Tile::RotateRight()
{
	Block *oldBlocks = Blocks;

	Blocks = new Block[Width*Height];

	u8 oldHeight = Height, oldWidth = Width;

	Width = oldHeight;

	Height = oldWidth;

	for(unsigned int i = 0; i < Height; ++i)
	{
		for(unsigned int j = 0; j < Width; ++j)
		{
			Blocks[i*Width + j] = oldBlocks[(oldHeight-1-j)*oldWidth + i];
		}
	}

	delete[] oldBlocks;
}

void Tile::FlipH()
{
	Block *oldBlocks = Blocks;

	Blocks = new Block[Width*Height];

	for(unsigned int i = 0; i < Height; ++i)
	{
		for(unsigned int j = 0; j < Width; ++j)
		{
			Blocks[i*Width + j] = oldBlocks[i*Width + Width-1-j];
		}
	}

	delete[] oldBlocks;
}

void Tile::FlipV()
{
	Block *oldBlocks = Blocks;

	Blocks = new Block[Width*Height];

	for(unsigned int i = 0; i < Height; ++i)
	{
		for(unsigned int j = 0; j < Width; ++j)
		{
			Blocks[i*Width + j] = oldBlocks[(Height-1-i)*Width + j];
		}
	}

	delete[] oldBlocks;
}*/
