#include "FieldPanel.h"

static wxPen pens[5] = {*wxTRANSPARENT_PEN, wxPen(wxColour(0, 0, 255)), wxPen(wxColour(255, 255, 0)), *wxRED_PEN, *wxGREEN_PEN};
static wxBrush brushes[5] = {*wxTRANSPARENT_BRUSH, *wxBLUE_BRUSH, wxBrush(wxColour(255, 255, 0)), *wxRED_BRUSH, *wxGREEN_BRUSH};

static wxPen invalidPen = wxPen(wxColour(192, 192, 192));
static wxBrush invalidBrush = wxBrush(wxColour(192, 192, 192));


FieldPanel::FieldPanel(Game *game, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE)
{
	SetOwnBackgroundColour(*wxWHITE);
	Show(true);
	BlokusGame = game;


	TileX = 0;
	TileY = 0;
}

void FieldPanel::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	int width, height;
	Tile *displayTile = BlokusGame->getField();

	GetClientSize(&width, &height);

	for(unsigned int i = 0; i < displayTile->Width; ++i)
	{
		for(unsigned int j = 0; j < displayTile->Height; ++j)
		{
			Block b = displayTile->Blocks[j*displayTile->Width + i];
			if(b == BLOCK_EMPTY)
				continue;

			dc.SetPen(pens[b]);
			dc.SetBrush(brushes[b]);

			dc.DrawRectangle(i*width/displayTile->Width, j*height/displayTile->Height,
			(i+1)*width/displayTile->Width-i*width/displayTile->Width,
			(j+1)*height/displayTile->Height-j*height/displayTile->Height);
		}
	}

	BlokusTile *blokusTile = BlokusGame->getCurrentPlayer()->getCurrentTile();

	if(BlokusGame->isValidPlace(blokusTile, TileX, TileY))
	{
		dc.SetPen(pens[BlokusGame->getCurrentPlayer()->getTeam()]);
		dc.SetBrush(brushes[BlokusGame->getCurrentPlayer()->getTeam()]);
	}

	else
	{
		dc.SetPen(invalidPen);
		dc.SetBrush(invalidBrush);
	}

	for(unsigned int i = 0; i < blokusTile->getTileData()->Width; ++i)
	{
		for(unsigned int j = 0; j < blokusTile->getTileData()->Height; ++j)
		{
			Block b = blokusTile->getTileData()->Blocks[j*blokusTile->getTileData()->Width + i];

			if(b == BLOCK_EMPTY)
				continue;

			dc.DrawRectangle((TileX+i)*width/displayTile->Width, (TileY+j)*height/displayTile->Height,
			(TileX+i+1)*width/displayTile->Width-(TileX+i)*width/displayTile->Width,
			(TileY+j+1)*height/displayTile->Height-(TileY+j)*height/displayTile->Height);

		}
	}

	// Draw lines
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	for(unsigned int i = 1; i < displayTile->Width; ++i)
	{
		dc.DrawLine(i*width/displayTile->Width, 0, i*width/displayTile->Width, height);
	}

	for(unsigned int i = 1; i < displayTile->Height; ++i)
	{
		dc.DrawLine(0, i*height/displayTile->Height, width, i*height/displayTile->Height);
	}
}

BEGIN_EVENT_TABLE(FieldPanel, wxPanel)
    EVT_PAINT(FieldPanel::OnPaint)
   //temp EVT_KEY_DOWN(FieldPanel::onch)
END_EVENT_TABLE()

