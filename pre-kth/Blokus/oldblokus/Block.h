#ifndef BLOCK_H
#define BLOCK_H

#include "Types.h"

#define	BLOCK_EMPTY		0		// Used in Field and empty parts of Tiles
#define	BLOCK_CORNER	0xFF	// Connecting corner, not actual corner piece

// Block values should be the same as team's value, block value of 1 = team 1, etc.

class Block
{
	public:
		inline Block() {Value=BLOCK_EMPTY;}				// Defaults to empty block
		inline Block(u8 value) {Value=value;}			// Sets block value to value

		// Get and set block values
		inline u8 getValue() {return Value;}
		inline void setValue(u8 value) {Value=value;}

	private:
		u8 Value;
};

#endif

