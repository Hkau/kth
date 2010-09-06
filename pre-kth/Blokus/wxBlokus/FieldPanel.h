#ifndef FIELD_PANEL_H
#define FIELD_PANEL_H

#include <wx/wx.h>
#include "Game.h"

//! Field panel
/** Displays a field. */
class FieldPanel: public wxPanel
{
	public:
		//! FieldPanel constructor
		/** Shows itself, initialises and sets values. */
		FieldPanel(Game *game, wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

		//! Paint event
		/** Paints out the field. */
		void OnPaint(wxPaintEvent& event);

		//! Gets X coordinate for the tile
		/** Gets the X coordinate for the displayed tile. */
		inline u8 getTileX() { return TileX; }

		//! Gets Y coordinate for the tile
		/** Gets the Y coordinate for the displayed tile. */
		inline u8 getTileY() { return TileY; }

		//! Sets X coordinate for the tile
		/** Sets the X coordinate for the displayed tile. */
		inline void setTileX(u8 value) { TileX = value; }

		//! Sets Y coordinate for the tile
		/** Sets the Y coordinate for the displayed tile. */
		inline void setTileY(u8 value) { TileY = value; }

		//! Sets pointer to game
		/** Sets the Game pointer from which the class reads. */
		inline void setGame(Game *game) { BlokusGame = game; }

		DECLARE_EVENT_TABLE()
	private:
		Game *BlokusGame;

		u8 TileX, TileY;
};

#endif

