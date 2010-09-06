#include "wxBlokus.h"

#include <wx/filename.h>

wxBlokus *CurrentBlokus;

wxBlokus::wxBlokus(const wxString& title, const wxPoint& pos, const wxSize& size)
:wxFrame((wxFrame*) NULL, -1, title, pos, size)
{
	rules = new DefaultRules;
	game = new Game(rules, &GameEnd);

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menu = new wxMenu;

	menu->Append(ID_New, wxT("&New game\tCtrl+N"));

	menu->AppendSeparator();

	menu->Append(ID_Load, wxT("&Load state\tCtrl+L"));
	menu->Append(ID_Save, wxT("&Save state\tCtrl+S"));

    menu->AppendSeparator();
    menu->Append(ID_Quit, wxT("E&xit\tCtrl+X"));
    menuBar->Append(menu, wxT("&File"));

	menu = new wxMenu;

	menu->Append(ID_Skip, wxT("&Skip\tAlt+s"));
	menu->AppendSeparator();
	menu->Append(ID_Score, wxT("&Count Score"));
	menuBar->Append(menu, wxT("A&ction"));

	menu = new wxMenu;

	menu->Append(ID_About, wxT("&About...\tF1"));

    menuBar->Append(menu, wxT("&Help"));


    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText(wxT("Tjo blokuskamrat!"));

	//Set minimum size
	SetMinSize(wxSize(280, 280));

	SetOwnBackgroundColour(*wxBLACK);

	//Create sizers
	wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(topSizer);

	BlokusFieldPanel = new FieldPanel(game, this, -1, wxDefaultPosition, wxDefaultSize);

	PrevTilePanel = new TilePanel(game->getCurrentPlayer()->getPreviousTile(),
	game->getCurrentPlayer()->getTeam(), this, -1, wxDefaultPosition, wxDefaultSize);

	CurrentTilePanel = new TilePanel(game->getCurrentPlayer()->getCurrentTile(),
	game->getCurrentPlayer()->getTeam(), this, -1, wxDefaultPosition, wxDefaultSize);

	NextTilePanel=new TilePanel(game->getCurrentPlayer()->getNextTile(),
	game->getCurrentPlayer()->getTeam(), this, -1, wxDefaultPosition, wxDefaultSize);

	topSizer->Add(BlokusFieldPanel, 3, wxEXPAND | wxRIGHT, 2);

	// Add sizer to top-level sizer
	topSizer->Add(sizer, 1, wxEXPAND);

	sizer->Add(PrevTilePanel, 1, wxEXPAND);

	sizer->Add(CurrentTilePanel, 1, wxEXPAND | wxTOP | wxBOTTOM, 1);

	sizer->Add(NextTilePanel, 1, wxEXPAND);

	// Allow frame to pick up key events from sub panels.
	// BlokusFieldPanel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxBlokus::KeyDown));
	/*BlokusFieldPanel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxBlokus::KeyDown), (wxObject*) NULL, this);
	PrevTilePanel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxBlokus::KeyDown), (wxObject*) NULL, this);
	CurrentTilePanel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxBlokus::KeyDown), (wxObject*) NULL, this);
	NextTilePanel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(wxBlokus::KeyDown), (wxObject*) NULL, this);*/
}

wxBlokus::~wxBlokus()
{
	delete game;
	delete rules;
}

void wxBlokus::RefreshTilePanels()
{
	PrevTilePanel->setTeam(game->getCurrentPlayer()->getTeam());
	PrevTilePanel->setTile(game->getCurrentPlayer()->getPreviousTile());
	PrevTilePanel->Refresh();

	CurrentTilePanel->setTeam(game->getCurrentPlayer()->getTeam());
	CurrentTilePanel->setTile(game->getCurrentPlayer()->getCurrentTile());
	CurrentTilePanel->Refresh();

	NextTilePanel->setTeam(game->getCurrentPlayer()->getTeam());
	NextTilePanel->setTile(game->getCurrentPlayer()->getNextTile());
	NextTilePanel->Refresh();
}

void wxBlokus::OnSkip(wxCommandEvent &event)
{
	game->CyclePlayers();

	RefreshTilePanels();

	BlokusFieldPanel->Refresh();
}

void wxBlokus::OnNew(wxCommandEvent &event)
{
	delete game;
	game = new Game(rules, &GameEnd);

	BlokusFieldPanel->setGame(game);

	BlokusFieldPanel->Refresh();

	RefreshTilePanels();

	BlokusFieldPanel->setTileX(0);
	BlokusFieldPanel->setTileY(0);

	Refresh();
}

void wxBlokus::ShowScore()
{
	wxString s;

	s << wxT("Score, lower is better.\n\n");
	for(u8 playerIndex = 0; playerIndex < game->getPlayerCount(); ++playerIndex)
	{
		s << wxT("Player ") << (playerIndex + 1) << wxT(": ");

		u16 score = game->getPlayer(playerIndex)->getScore();

		if(score == 0)
			s << wxT("WIN");
		else
			s << score;
		s << wxT("\n\n");
	}
	wxMessageBox(s, wxT("Score"), wxOK, this);
}

void wxBlokus::OnCountScore(wxCommandEvent &event)
{
	ShowScore();
}

void wxBlokus::OnGameEnd()
{
	ShowScore();
}

void wxBlokus::OnQuit(wxCommandEvent &event)
{
    Close();
}

void wxBlokus::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(wxT(
"Blokus\n\n\
Up,Down,Left,Right: Move tile\n\n\
Shift+Up,Down: Cycle tiles\n\
Shift+Left,Right: Rorate tile\n\n\
Ctrl+Up,Down,Left,Right: Flip tile"), wxT("About Blokus"), wxOK, this);
}

void wxBlokus::OnLoad(wxCommandEvent &event)
{
	//wxStandardPaths::GetUserConfigDir();
	wxFileName dir = wxFileName::DirName(wxGetCwd());
	dir.AppendDir(wxT("saves"));
	wxString filename = wxFileSelector(wxT("Load state"), dir.GetPath(),wxT(""),
	wxT(".bls"), wxT("Blokus savefiles (*.bls)|*.bls"), wxOPEN | wxFD_FILE_MUST_EXIST);
	if(filename.empty())
		return;
	game->LoadGame(filename.mb_str());
	//wxMessageBox(wxT("Load failed D:"), wxT("Errorish"), wxOK | wxICON_ERROR, this);

	RefreshTilePanels();

	Refresh();
}

void wxBlokus::OnSave(wxCommandEvent &event)
{
	wxFileName dir = wxFileName::DirName(wxGetCwd());
	dir.AppendDir(wxT("saves"));
	wxString filename = wxFileSelector(wxT("Save state"), dir.GetPath(), wxT(""),
	wxT(".bls"), wxT("Blokus savefiles (*.bls)|*.bls"), wxSAVE);
	if(filename.empty())
		return;
	game->SaveGame(filename.mb_str());
	//wxMessageBox(wxT("Not implemented."), wxT("Errorish"), wxOK | wxICON_ERROR, this);
}

void wxBlokus::KeyDown(wxKeyEvent &event)
{
	bool updateTiles = false;
	switch(event.GetKeyCode())
	{
		case WXK_LEFT:
			if(event.ShiftDown())
			{
				TileRotateLeft(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}
			if(event.ControlDown())
			{
				TileFlipHorizontal(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}
			if(BlokusFieldPanel->getTileX() == 0)
				break;
			BlokusFieldPanel->setTileX(BlokusFieldPanel->getTileX()-1);
			break;
		case WXK_RIGHT:
			if(event.ShiftDown())
			{
				TileRotateRight(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}
			if(event.ControlDown())
			{
				TileFlipHorizontal(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}
			if(BlokusFieldPanel->getTileX() == game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Width)
				break;
			BlokusFieldPanel->setTileX(BlokusFieldPanel->getTileX()+1);
			break;
		case WXK_UP:
			if(event.ShiftDown())
			{
				game->getCurrentPlayer()->setPreviousTile();

				updateTiles = true;
				break;
			}
			if(event.ControlDown())
			{
				TileFlipVertical(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}

			if(BlokusFieldPanel->getTileY() == 0)
				break;
			BlokusFieldPanel->setTileY(BlokusFieldPanel->getTileY()-1);
			break;
		case WXK_DOWN:
			if(event.ShiftDown())
			{
				game->getCurrentPlayer()->setNextTile();

				updateTiles = true;
				break;
			}
			if(event.ControlDown())
			{
				TileFlipVertical(game->getCurrentPlayer()->getCurrentTile()->getTileData());

				updateTiles = true;
				break;
			}
			if(BlokusFieldPanel->getTileY() == game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Height)
				break;
			BlokusFieldPanel->setTileY(BlokusFieldPanel->getTileY()+1);
			break;
		case WXK_RETURN:
			game->PlaceTile(game->getCurrentPlayer()->getCurrentTile(), BlokusFieldPanel->getTileX(), BlokusFieldPanel->getTileY());
			updateTiles = true;
			break;
		default:
			break;
	}

	BlokusFieldPanel->Refresh();

	if(updateTiles)
	{
		if(BlokusFieldPanel->getTileX() > game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Width)
			BlokusFieldPanel->setTileX(game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Width);

		if(BlokusFieldPanel->getTileY() > game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Height)
			BlokusFieldPanel->setTileY(game->getField()->Width-game->getCurrentPlayer()->getCurrentTile()->getTileData()->Height);

		RefreshTilePanels();
	}
}

BEGIN_EVENT_TABLE(wxBlokus, wxFrame)

	EVT_MENU(ID_New, wxBlokus::OnNew)
	EVT_MENU(ID_Load, wxBlokus::OnLoad)
	EVT_MENU(ID_Save, wxBlokus::OnSave)

    EVT_MENU(ID_Quit, wxBlokus::OnQuit)

	EVT_MENU(ID_Skip, wxBlokus::OnSkip)

	EVT_MENU(ID_Score, wxBlokus::OnCountScore)
    EVT_MENU(ID_About, wxBlokus::OnAbout)

    EVT_CHAR(wxBlokus::KeyDown)

END_EVENT_TABLE()

