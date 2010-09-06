#ifndef GAME_H
#define GAME_H

#include "Rules.h"
#include "Tile.h"
#include "Player.h"

//! Game class
/** Most game-related stuff happens here. */
class Game
{
	public:

		//! Game constructor
		/** Has all game components allocated by Rules. */
		Game(Rules *rules, void (*endGameCallback)());

		//! Game destructor
		/** Destroys field and players. */
		~Game();

		//! Place tile
		/** Places a tile if possible. */
		bool PlaceTile(BlokusTile *tile, u8 x, u8 y);

		//! Checks if tile placement is valid
		/** Calls GameRules::ValidPlaceTile, returns true if tile placement is valid. */
		inline bool isValidPlace(BlokusTile *tile, u8 x, u8 y) { return GameRules->ValidPlaceTile(GameField, getCurrentPlayer(), tile, x, y); }

		//! Get Field
		/** Returns field. */
		inline Field *getField() { return GameField; }

		//! Get Rield
		/** Returns current Rules. */
		inline Rules *getRules() { return GameRules; }

		//! Get Player count
		/** Returns player count. */
		inline u8 getPlayerCount() { return PlayerCount; }

		//! Get Player list
		/** Returns pointer to player list. */
		inline Player *getPlayerList() { return Players; }

		//! Get current Player
		/** Returns pointer to current player. */
		inline Player *getCurrentPlayer() { return &Players[PlayerIndex]; }

		//! Get Player
		/** Returns pointer to requested player. */
		inline Player *getPlayer(u8 index) { return &Players[index]; }

		//! Cycle players
		/** Sets next player as current. */
		void CyclePlayers()
		{
				if(PlayerIndex == PlayerCount - 1)
					PlayerIndex = 0;
				else
					++PlayerIndex;
		}

		//! End game
		/** Calls endgame callback. */
		void EndGame()
		{
			EndGameCallback();
		}

		//! Save game
		/** Saves current state to path. */
		bool SaveGame(const char *path);

		//! Load game
		/** Loads previous state from path. */
		bool LoadGame(const char *path);

	private:

		Rules *GameRules;

		Field *GameField;

		u8 PlayerIndex;
		u8 PlayerCount;
		Player *Players;
		void (*EndGameCallback)();
};

#endif

