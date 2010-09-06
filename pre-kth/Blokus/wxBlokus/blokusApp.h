#ifndef BLOKUSAPP_H
#define BLOKUSAPP_H

#include <wx/wx.h>

#include "wxBlokus.h"

//! Application entry point
/** Sets up a wxBlokus frame as a top-level window when the application starts.*/
class BlokusApp: public wxApp
{
	//! Application entry point
	/** Starts wxBlokus */
	virtual bool OnInit();

	wxBlokus *blokusFrame;
};

#endif
