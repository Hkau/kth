#include "Tile.h"

Tile::Tile(u8 width, u8 height)
{
	Width = width;

	Height = height;

	Blocks = new Block[Width*Height];

	//for (u16 i = 0; i < Width * Height; ++i)
		//Blocks[i].setValue(1);

	//printf("LOLGNESTA?");

	/*for(u16 i = 0; i < Height; ++i)
		FieldBlocks[i*Width].setValue(0xAA);*/
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
}
