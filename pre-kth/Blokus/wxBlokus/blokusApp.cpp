#include "blokusApp.h"

#include "wxBlokus.h"

bool BlokusApp::OnInit()
{
	blokusFrame = new wxBlokus(wxT("Blokusfest!"), wxDefaultPosition, wxSize(300, 300));
	blokusFrame->Show(true);
	SetTopWindow(blokusFrame);

	return true;
}

IMPLEMENT_APP(BlokusApp)
