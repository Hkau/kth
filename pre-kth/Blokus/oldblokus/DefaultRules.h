#ifndef DEFAULTRULES_H
#define DEFAULTRULES_H

#include "Rules.h"

class DefaultRules: public Rules
{
	public:
		DefaultRules();
		
		Player* CreatePlayers();

		bool ValidPlaceTile(Field&, Tile&, u8 x, u8 y);
};

#endif

