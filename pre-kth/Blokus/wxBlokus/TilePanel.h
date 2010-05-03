#ifndef TILE_PANEL_H
#define TILE_PANEL_H

#include <wx/wx.h>
#include "Game.h"

//! Tile panel
/** Displays a tile with color related to set team. */
class TilePanel: public wxPanel
{
	public:
		//! TilePanel constructor
		/** Sets tile and team. */
		TilePanel(BlokusTile *tile, u8 team, wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

		//! Paint event
		/** Draws the tile. */
		void OnPaint(wxPaintEvent& event);

		//! Set team
		/** Sets display team for the tile. */
		inline void setTeam(u8 team) { Team = team; }

		//! Set Tile
		/** Sets tile to be displayed. */
		inline void setTile(BlokusTile *tile) { DisplayTile = tile; }

		DECLARE_EVENT_TABLE()
	private:
		BlokusTile *DisplayTile;

		u8 Team;
};

#endif

