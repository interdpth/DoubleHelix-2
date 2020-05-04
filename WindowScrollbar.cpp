#include "WindowScrollbar.h"



WindowScrollbar::WindowScrollbar(HWND hwnd, bool vertical)
{
	_window = hwnd;
	_vert = vertical;
	_max = 0;
	_index = 0;
}


WindowScrollbar::~WindowScrollbar()
{
}

int WindowScrollbar::GetMax()
{
	return _max;
}

void WindowScrollbar::SetMax(int max)
{
	_max = max;
}

int WindowScrollbar::GetIndex()
{
	return _index;
}

void WindowScrollbar::SetIndex(int newindex)
{
	_index = newindex;
}


void WindowScrollbar::ChangeScrollbars()
{

	_index = 0;

								/*
								*  Calculating max scroll:
								*
								*	our full image is 256 pixels wide (the width of the Image object)
								*    only nDisplayWidth pixels are displayed
								*
								*  256 - nDisplayWidth is the number of pixels that won't be displayed
								*  doing a +7 / 8 with that value rounds up to the number of *tiles* that won't be
								*  displayed.  This is the maximum scroll value.
								*
								*  If all but 1 tile is visible, then valid scroll values would be 0,1
								*  If all but 2 tiles are visible, the scroll between 0-2, etc
								*/



	SCROLLINFO		inf;
	memset(&inf, 0, sizeof(SCROLLINFO));		// zero out our SCROLLINFO struct
	inf.cbSize = sizeof(SCROLLINFO);		// cbSize must be the size of the struct (weird WinAPI nonsense)

											// now we start filling our scroll struct with relevent info.
											//   start with H Scroll whynot

											// fMask determines the properties of this scrollbar we'll be changing.
											// Combine values with OR
	inf.fMask = SIF_DISABLENOSCROLL |	// This makes it so the scrollbar is disabled when unnecesary
										// otherwise it's hidden (this is usually more applicable to scrollbars
										// that are attached to normal windows -- not so much for scrollbar
										// controls on dialogs -- but whatever)
		SIF_PAGE |				// we will be updating the 'nPage' value
		SIF_POS |				// and the 'nPos' value
		SIF_RANGE;				// and min/max values

								// I'm unsure how this works exactly.  It affects the size
								// of the scrollable part of the bar, but I'm unclear on details.
								// higher values mean fatter bar.  Let's just use 1 because it's simple

	inf.nPos = 0;		// reset position to 0

	inf.nMin = 0;		// minimum H scroll is 0

	int scrollType = _vert ? SB_VERT : SB_HORZ;

	
		inf.nMax = _max;

			SetScrollInfo(_window,		// HWND to scrollbar
				scrollType,					// this scrollbar is a dialog control
				&inf,					// pointer to our info struct
				1);
		// redraw flag (redraw the scrollbar after this change)

		inf.nPage = 0;

	
		SetScrollPos(_window,scrollType, 0, 1);


}



void WindowScrollbar::UpdateScroll( WPARAM how)
{

	/*
	*	Called whenever the user scrolls (called by WM_HSCROLL and WM_VSCROLL)
	*
	*    dialogwnd is the HWND to the dialog
	*    how is the WPARAM paGlobalVars::gblVars->SSEd to WM_HSCROLL or WM_VSCROLL (it contains info about the scroll)
	*    isvertical is 0 for H scrolling, 1 for V scrolling
	*/

	// I usually do tricks with pointers and LUTs and stuff here to recycle a lot of code
	//   so I don't have to have if/else chains everywhere, but that might make this confusing
	//   so I'll stick to more basic practices:

	int curscroll = 0;
	int maxscroll = 0;
	// get cur/max scroll
	
		curscroll = GetIndex();
		maxscroll = GetMax();
	


	// find out how the user is scrolling
	//  admittedly, this is a royal pain in the arse
	switch (LOWORD(how))
	{
	case SB_LEFT:	curscroll = 0;			break;	// scroll all the way to the left
	case SB_RIGHT:	curscroll = maxscroll;	break;	// all the way to the right
	case SB_LINELEFT: curscroll -= 1;		break;	// one click to the left
	case SB_LINERIGHT: curscroll += 1;		break;	// one click to the right
	case SB_PAGELEFT: curscroll -= 10;		break;	// one page to the left
	case SB_PAGERIGHT: curscroll += 10;		break;	// one page to the right
	case SB_THUMBTRACK: curscroll = HIWORD(how); break;	// user is dragging bar
	}

	// keep scroll within bounds
	if (curscroll > maxscroll)	curscroll = maxscroll;
	if (curscroll < 0)			curscroll = 0;



	SetIndex(curscroll);
	// update our scroll vars, as well as the scrollbar itself
	if (_vert)
	{
	
			SetScrollPos(_window, SB_VERT, curscroll, 1);
	

	}
	else
	{
	
		
			SetScrollPos(_window, SB_HORZ, curscroll, 1);
	
	}

																									//  the rect by filling it with a solid color before redrawing
																									//  this will cause flickering if redrawn many times (like if the user
																									//  is dragging the bar), but will prevent "ghost tiles" appearing
																									//  on the right/bottom sides if you scroll to the very end
	
}

int WindowScrollbar::ResizeScrollBar(int Res, int Min, int Max) {

	SCROLLINFO		inf;
	memset(&inf, 0, sizeof(SCROLLINFO));		// zero out our SCROLLINFO struct
	inf.cbSize = sizeof(SCROLLINFO);		// cbSize must be the size of the struct (weird WinAPI nonsense)

											// now we start filling our scroll struct with relevent info.
											//   start with H Scroll whynot

											// fMask determines the properties of this scrollbar we'll be changing.
											// Combine values with OR
	inf.fMask = SIF_DISABLENOSCROLL |	// This makes it so the scrollbar is disabled when unnecesary
										// otherwise it's hidden (this is usually more applicable to scrollbars
										// that are attached to normal windows -- not so much for scrollbar
										// controls on dialogs -- but whatever)
		SIF_PAGE |				// we will be updating the 'nPage' value
		SIF_POS |				// and the 'nPos' value
		SIF_RANGE;				// and min/max values

	inf.nPage = 0;		// I'm unsure how this works exactly.  It affects the size
						// of the scrollable part of the bar, but I'm unclear on details.
						// higher values mean fatter bar.  Let's just use 1 because it's simple

	inf.nPos = 0;		// reset position to 0

	inf.nMin = Min;		// minimum H scroll is 0

	return 0;
}