#include "TilePanel.h"

#define TILE_MAX 5

static wxPen pens[5] = {*wxTRANSPARENT_PEN, wxPen(wxColour(0, 0, 255)), wxPen(wxColour(255, 255, 0)), *wxRED_PEN, *wxGREEN_PEN};
static wxBrush brushes[5] = {*wxTRANSPARENT_BRUSH, *wxBLUE_BRUSH, wxBrush(wxColour(255, 255, 0)), *wxRED_BRUSH, *wxGREEN_BRUSH};

static wxPen invalidPen = wxPen(wxColour(192, 192, 192));
static wxBrush invalidBrush = wxBrush(wxColour(192, 192, 192));

TilePanel::TilePanel(BlokusTile *tile, u8 team, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
:wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE)
{
	SetOwnBackgroundColour(*wxWHITE);
	Show(true);

	DisplayTile = tile;
	Team = team;
}

void TilePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	int width, height;

	GetClientSize(&width, &height);

	if(DisplayTile->getAmount() > 0)
	{
		dc.SetPen(pens[Team]);
		dc.SetBrush(brushes[Team]);
	}
	
	else
	{
		dc.SetPen(invalidPen);
		dc.SetBrush(invalidBrush);
	}

	for(unsigned int i = 0; i < DisplayTile->getTileData()->Width; ++i)
	{
		for(unsigned int j = 0; j < DisplayTile->getTileData()->Height; ++j)
		{
			Block b = DisplayTile->getTileData()->Blocks[j*DisplayTile->getTileData()->Width + i];
			if(b == BLOCK_EMPTY)
				continue;

			/*dc.DrawRectangle(i*width/DisplayTile->Width, j*height/DisplayTile->Height,
			(i+1)*width/DisplayTile->Width-i*width/DisplayTile->Width,
			(j+1)*height/DisplayTile->Height-j*height/DisplayTile->Height);*/

			dc.DrawRectangle(i*width/TILE_MAX, j*height/TILE_MAX,
			(i+1)*width/TILE_MAX-i*width/TILE_MAX,
			(j+1)*height/TILE_MAX-j*height/TILE_MAX);
		}
	}

	// Draw lines
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);

	for(unsigned int i = 1; i < DisplayTile->getTileData()->Width; ++i)
	{
		//dc.DrawLine(i*width/DisplayTile->Width, 0, i*width/DisplayTile->Width, height);
		dc.DrawLine(i*width/TILE_MAX, 0, i*width/TILE_MAX, height);
	}

	for(unsigned int i = 1; i < DisplayTile->getTileData()->Height; ++i)
	{
		dc.DrawLine(0, i*height/TILE_MAX, width, i*height/TILE_MAX);
	}
}

BEGIN_EVENT_TABLE(TilePanel, wxPanel)
    EVT_PAINT(TilePanel::OnPaint)
END_EVENT_TABLE()

