
#include "MainHeader.h"
#include "clsScroll.h"
//#include "globals.h"
extern  clsUIScroll scrMapV;
extern clsUIScroll scrMapH;
extern clsUIScroll scrTSV;

void ChangeScrollbars(HWND dialogwnd,int Res,int Scroll)
{
	
     /*
	 *	This function will prep our scrollbars so that they have the desired min/max
	 *    values and other crap
	 */
    //If Res == 0 horizontal if Res == 1 vertical
	/*
	 *	Before we worry about the scrollbars, let's prep our own variables
	 */
	nHScroll[Scroll] = 0;
	nVScroll[Scroll] = 0;		// reset scroll to 0
		// maximum V scroll

	
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
	memset(&inf,0,sizeof(SCROLLINFO));		// zero out our SCROLLINFO struct
	inf.cbSize = sizeof(SCROLLINFO);		// cbSize must be the size of the struct (weird WinAPI nonsense)

	// now we start filling our scroll struct with relevent info.
	//   start with H Scroll whynot

	// fMask determines the properties of this scrollbar we'll be changing.
	// Combine values with OR
	inf.fMask =		SIF_DISABLENOSCROLL |	// This makes it so the scrollbar is disabled when unnecesary
											// otherwise it's hidden (this is usually more applicable to scrollbars
											// that are attached to normal windows -- not so much for scrollbar
											// controls on dialogs -- but whatever)
					SIF_PAGE |				// we will be updating the 'nPage' value
					SIF_POS |				// and the 'nPos' value
					SIF_RANGE;				// and min/max values

	// I'm unsure how this works exactly.  It affects the size
							// of the scrollable part of the bar, but I'm unclear on details.
							// higher values mean fatter bar.  Let's just use 1 because it's simple

	inf.nPos =		0;		// reset position to 0

	inf.nMin =		0;		// minimum H scroll is 0

	

	// now that the SCROLLINFO struct is filled, set the properties of the scrollbar
	if((!Res)){
			// maximum H scroll
     
		inf.nMax =	(nMaxHScroll[Scroll]);	
		if(dialogwnd == UiState::stateManager->GetMapWindow() ) {
			
          	SetScrollInfo(	dialogwnd,		// HWND to scrollbar
					SB_HORZ,					// this scrollbar is a dialog control
					&inf,					// pointer to our info struct
					1 );
		}		// redraw flag (redraw the scrollbar after this change)
		
		inf.nPage =	 0;	
	
	}else{

	// V Scroll is the same thing.  All values in the struct remain the same except for the max

		inf.nMax =		nMaxVScroll[Scroll];
	if(dialogwnd == UiState::stateManager->GetTilesetWindow()){
       	SetScrollInfo(	UiState::stateManager->GetTilesetWindow(),
					SB_VERT,
					&inf,
					1 );
		
	}else if(dialogwnd == UiState::stateManager->GetMapWindow()){
        SetScrollInfo(	UiState::stateManager->GetMapWindow(),
					SB_VERT,
					&inf,
					1 );
		inf.nPage =	0;	

	}
	}
	if(dialogwnd == UiState::stateManager->GetTilesetWindow()){
	
     SetScrollPos( UiState::stateManager->GetTilesetWindow(), SB_VERT, 0, 1 );
	 
	}else if(dialogwnd == UiState::stateManager->GetMapWindow()){
	
        SetScrollPos( UiState::stateManager->GetMapWindow(), SB_VERT, 0, 1 );
	
	}else if(dialogwnd == UiState::stateManager->GetMapWindow()){
	
        SetScrollPos( UiState::stateManager->GetMapWindow(), SB_HORZ, 0, 1 );
	
	}
}



void UpdateScroll(HWND dialogwnd,WPARAM how,int isvertical,int Res,int Scroll)
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

	int curscroll=0;
	int maxscroll=0;
	// get cur/max scroll
	if(isvertical)
	{
		curscroll = nVScroll[Scroll];
		maxscroll = nMaxVScroll[Scroll];
	}
	else
	{
		curscroll = nHScroll[Scroll];
		maxscroll = nMaxHScroll[Scroll];
	}

	// find out how the user is scrolling
	//  admittedly, this is a royal pain in the arse
	switch(LOWORD(how))
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
	if(curscroll > maxscroll)	curscroll = maxscroll;
	if(curscroll < 0)			curscroll = 0;

	// update our scroll vars, as well as the scrollbar itself
	if(isvertical)
	{
		nVScroll[Scroll] = curscroll;
		if(dialogwnd == UiState::stateManager->GetMapWindow()){
        SetScrollPos(UiState::stateManager->GetMapWindow(),SB_VERT,curscroll,1);
		}else 	if(dialogwnd == UiState::stateManager->GetTilesetWindow()){
			SetScrollPos( UiState::stateManager->GetTilesetWindow(),SB_VERT,curscroll,1);
		}else{
		SetScrollPos( GetDlgItem(dialogwnd,Res), SB_CTL, curscroll, 1 );
		}
		
	}
	else
	{
		nHScroll[Scroll] = curscroll;
		if(dialogwnd == UiState::stateManager->GetMapWindow()){
        SetScrollPos(UiState::stateManager->GetMapWindow(),SB_HORZ,nHScroll[Scroll],1);
		}else if(dialogwnd == UiState::stateManager->GetTilesetWindow()){
        SetScrollPos(UiState::stateManager->GetTilesetWindow(),SB_HORZ,nHScroll[Scroll],1);
		
		}else {
		
		SetScrollPos( GetDlgItem(dialogwnd,Res), SB_CTL, nHScroll[Scroll], 1 );
		}
	}

	// invalidate our display (signals it to redraw)
//	if((Scroll == sVMap) || (Scroll == sHMap)) {
	//	 DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);
	
//		DisplayDoors();
	
	    InvalidateRect(UiState::stateManager->GetMapWindow(),0,1);
	  
	//}
    if(Scroll == sTileset)	InvalidateRect(UiState::stateManager->GetTilesetWindow(),0,1 );		// erase BG flag.  Setting this to 1 will make the window erase
					//  the rect by filling it with a solid color before redrawing
					//  this will cause flickering if redrawn many times (like if the user
					//  is dragging the bar), but will prevent "ghost tiles" appearing
					//  on the right/bottom sides if you scroll to the very end
    if(Scroll == SceneVScroll || Scroll == SceneHScroll) InvalidateRect(SceneGraph,0,1);
	if(Scroll == STVScroll ) InvalidateRect(TSScene,0,1);

	//  "ghost tiles" *and* flickering can be avoided if you double-buffer the image and
	//   don't erase the BG.  But doublebuffering is a topic for another day.  You'll probably want
	//  it to draw stuff like rectangles around the selecTextEditor tile, etc

	//  maybe I'll make a class for you that handles that.
}

int ResizeScrollBar(int Res, int Min, int Max){
   
	SCROLLINFO		inf;
	memset(&inf,0,sizeof(SCROLLINFO));		// zero out our SCROLLINFO struct
	inf.cbSize = sizeof(SCROLLINFO);		// cbSize must be the size of the struct (weird WinAPI nonsense)

	// now we start filling our scroll struct with relevent info.
	//   start with H Scroll whynot

	// fMask determines the properties of this scrollbar we'll be changing.
	// Combine values with OR
	inf.fMask =		SIF_DISABLENOSCROLL |	// This makes it so the scrollbar is disabled when unnecesary
											// otherwise it's hidden (this is usually more applicable to scrollbars
											// that are attached to normal windows -- not so much for scrollbar
											// controls on dialogs -- but whatever)
					SIF_PAGE |				// we will be updating the 'nPage' value
					SIF_POS |				// and the 'nPos' value
					SIF_RANGE;				// and min/max values

	inf.nPage =		0;		// I'm unsure how this works exactly.  It affects the size
							// of the scrollable part of the bar, but I'm unclear on details.
							// higher values mean fatter bar.  Let's just use 1 because it's simple

	inf.nPos =		0;		// reset position to 0

	inf.nMin =		Min;		// minimum H scroll is 0

	return 0;
}