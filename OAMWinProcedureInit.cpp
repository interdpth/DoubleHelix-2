#include "cOAMEdit.h"

int cOAMEdit::CreatePalHWND() {

	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &OAMPalProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMPAL";  // name of window class 


									  // Register the window class. 
	RegisterClassEx(&blahf);


	//Create the Window



	hwndPal = CreateWindowEx(NULL, "cOAMPAL", NULL, WS_VISIBLE | WS_CHILD, 5, 12, 256, 256, GetDlgItem(_oamWindow, fraPal), 0, hGlobal, 0);



	return 0;
}






int cOAMEdit::CreateTileHWND() {
	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &OAMTileProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMTile";  // name of window class 


									   // Register the window class. 
	RegisterClassEx(&blahf);
	//Create the Window
	hwndTiles = CreateWindowEx(NULL, "cOAMTile", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 256, 256, GetDlgItem(_oamWindow, fraTiles), 0, hGlobal, 0);
	return 0;
}

int cOAMEdit::CreateSpriteHWND() {

	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &OAMSpriteProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(RGB(255, 255, 255));				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMSprite";  // name of window class 


										 // Register the window class. 
	RegisterClassEx(&blahf);

	hwndSpritePreview = CreateWindowEx(NULL, "cOAMSprite", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 2048, 2048, GetDlgItem(_oamWindow, fraSprPreview), 0, hGlobal, 0);

	return 0;
}

int cOAMEdit::CreateSpriteAnimationHWND() {

	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &AnimationProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(RGB(255, 255, 255));				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMAnimation";  // name of window class 


											// Register the window class. 
	RegisterClassEx(&blahf);


	//Create the Window



	hwndAnimationWindow = CreateWindowEx(NULL, "cOAMAnimation", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 2048, 2048, GetDlgItem(_oamWindow, fraSprAnimPreview), 0, hGlobal, 0);

	return 0;
}

int cOAMEdit::CreatePartHWND() {

	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &OAMPartProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(RGB(255, 255, 255));				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMPart";  // name of window class 
									   // Register the window class. 
	RegisterClassEx(&blahf);
	//Create the Window
	//hwndPartWindow = CreateWindowEx(NULL, "cOAMPart", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 1024, 1024, GetDlgItem(me, fraPartPreview), 0, hGlobal, 0);
	return 0;
}



int cOAMEdit::CreateInfoWindow() {

	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = &OAMPartProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = hGlobal;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(RGB(255, 255, 255));				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cOAMPart";  // name of window class 
									   // Register the window class. 
	RegisterClassEx(&blahf);
	//Create the Window
	//hwndPartWindow = CreateWindowEx(NULL, "cOAMPart", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 1024, 1024, GetDlgItem(me, fraPartPreview), 0, hGlobal, 0);
	return 0;
}
