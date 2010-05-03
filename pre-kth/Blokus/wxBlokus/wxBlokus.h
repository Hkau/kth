#ifndef WXBLOKUS_H
#define WXBLOKUS_H

#include <wx/wx.h>

#include "Blokus.h"

#include "FieldPanel.h"
#include "TilePanel.h"

class wxBlokus;
extern wxBlokus *CurrentBlokus;

//! Main frame for Blokus
/** Main window. Contains Rules and Game for gameplay mechanics.
	A FieldPanel, and a few instances of TilePanel show the
	previous, current and next Tile. This frame also handles key
	events from child windows and have a couple of commands from
	menus that allows players save, load, skip one's turn etc.*/
class wxBlokus: public wxFrame
{
	public:
		//! wxBlokus constructor
		/** Allocates Game, Rules, and panels etc. */
		wxBlokus(const wxString& title, const wxPoint& pos, const wxSize& size);

		//! wxBlokus destructor
		/** Deletes game and rules. */
		~wxBlokus();

		//! Quit event
		/** Closes the frame. */
		void OnQuit(wxCommandEvent &event);

		//! Load event
		/** Loads a previously saved game from file selector. */
		void OnLoad(wxCommandEvent &event);

		//! Save event
		/** Saves the current game. */
		void OnSave(wxCommandEvent &event);

		//! About event
		/** Displays Blokus information. */
		void OnAbout(wxCommandEvent &event);

		//! Skip event
		/** Closes the frame. */
		void OnSkip(wxCommandEvent &event);

		//! Count score event
		/** Displays the score in a message box. */
		void OnCountScore(wxCommandEvent &event);

		//! New event
		/** Resets the game. */
		void OnNew(wxCommandEvent &event);

		//! KeyDown event
		/** Moves, changes and manipulates tiles based on key presses. */
		void KeyDown(wxKeyEvent &event);

		//! Game End callback
		/** Displays the score in a message box. */
		void OnGameEnd();

		//! Refresh tile panels
		/** Updates tile panels and has them redrawn. */
		void RefreshTilePanels();

		//! Static Game End callback
		/** Calls the current wxBlokus' GameEnd function. */
		static void GameEnd() { CurrentBlokus->OnGameEnd(); }

		//! ShowScore
		/** Displays the score in a message box. */
		void ShowScore();

		DECLARE_EVENT_TABLE()

	private:
		Rules *rules;
		Game *game;
		FieldPanel *BlokusFieldPanel;
		TilePanel *PrevTilePanel, *CurrentTilePanel, *NextTilePanel;
};


// Menu IDs
enum
{
	ID_Quit = 1,
	ID_About,
	ID_Skip,
	ID_Score,
	ID_New,
	ID_Load,
	ID_Save,
};

#endif

