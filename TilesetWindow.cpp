#include "MainHeader.h"
#include "MapManager.h"
#include "GlobalVars.h"
#include "TilesetManager.h"
#include "BaseGame.h"

#include "OpenGL2.h"
COpenGL *gl;


LRESULT CALLBACK TilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	RECT coord;
	int i = 0;
	GetWindowRect(UiState::stateManager->GetTilesetWindow(), &coord);
	TileCurs.X = GetX(lParam);
	TileCurs.Y = GetY(lParam);
	switch (message)
	{

	case WM_LBUTTONDOWN:
		//if((HWND)wParam == GetDlgItem(hwnd,picButton)) sMessage("Mouse was clicked down");

		if (wParam == MK_LBUTTON) {
			//TP[0] = 0;
			//TP[1] = 0;
			// mpTileset.Width = 1;
			// mpTileset.Height = 1;
			//MessageBox(Main,"Hey, you clicked the child.","BOO",MB_OK);
			//mpTileset.Width = 1;
			//mpTileset.Height =1;

			Tile = (GetX(lParam) / 16) + ((GetY(lParam) / 16) + nVScroll[sTileset]) * 16;
			//memset(&IsTSA,0,sizeof(IsTSA));
			//MyTSAEditor.LoadTSA();


		}

		break;
	case WM_VSCROLL:

		UpdateScroll(hWnd, wParam, 1, vsbScroll, sTileset);
		//InvalidateRect(UiState::stateManager->GetTilesetWindow(),0,1);

		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		break;
	case WM_RBUTTONDOWN:
		if (LOWORD(wParam) == MK_RBUTTON) {

			if (!((mpTileset.Width > 1) || (mpTileset.Height > 1))) {
				mpTileset.uX = mpTileset.sX = (GetX(lParam) / 16);
				mpTileset.uX = mpTileset.sY = (GetY(lParam) / 16);
				mpTileset.cX = mpTileset.sX;
				mpTileset.cY = mpTileset.sY;
				mpTileset.Width = 1;
				mpTileset.Height = 1;

			}
			else {//Reset
				mpTileset.Width = 1;
				mpTileset.Height = 1;
			}
			InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		}

		break;
		/*case WM_RBUTTONUP:
			TP[1] = 0;
			break;*/
	case WM_MOUSEMOVE:

		if (wParam & (MK_RBUTTON)) {

			mpTileset.nX = GetX(lParam) / 16;
			mpTileset.nY = GetY(lParam) / 16;
			//Determine direction for length and width;
			if (mpTileset.nX > mpTileset.cX) {//increase width;
				mpTileset.Width += 1;
			}
			else if (mpTileset.nX < mpTileset.cX) {
				mpTileset.Width -= 1;
			}
			if (mpTileset.nY > mpTileset.cY) {//increase width;
				mpTileset.Height += 1;
			}
			else if (mpTileset.nY < mpTileset.cY) {
				mpTileset.Height -= 1;
			}
			mpTileset.cX = GetX(lParam) / 16;
			mpTileset.cY = GetY(lParam) / 16;
			if (mpTileset.Height < 1) mpTileset.Height = 1;
			if (mpTileset.Width < 1)mpTileset.Width = 1;

			mpMap.Width = mpTileset.Width;
			mpMap.Height = mpTileset.Height;


			//	if(mpTileset.Width>16)mpTileset.Width = 16;
			//	if(mpTileset.Height>16)mpTileset.Height = 16;
				//check for out of bounds 
			if (mpTileset.nX < 0) mpTileset.nX = 0;
			if (mpTileset.nX > (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData))->X)
				mpTileset.nX = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;
			if (mpTileset.nY < 0) mpTileset.nY = 0;
			if (mpTileset.nY > RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y) mpTileset.nY = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
		}
		else {
			mpTileset.sY = GetY(lParam) / 16;
			mpTileset.sX = GetX(lParam) / 16;
		}
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		

		break;


	case WM_PAINT:



		hdc = BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd, &coord);
	//	
		if (GlobalVars::gblVars->imgTileset)
		{
			if (GlobalVars::gblVars->imgTileset->_isGL)
			{
				GlobalVars::gblVars->imgTileset->glInstance->UpdateDisplay();
			}
			else
			{
				GlobalVars::gblVars->imgTileset->Blit(hdc, 0, 0, coord.right - coord.left, coord.bottom - coord.top, 0, nVScroll[sTileset] * 16);

			}
		}
		DrawTileRect(hdc, mpTileset, 16);
		
		 
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
		return 0;
		break;
	case WM_CREATE:
	//	gl = new COpenGL(hWnd);
		break;
	case WM_RBUTTONDBLCLK:
		///Reset the rects 

		mpMap.Height = 1;
		mpMap.Width = 1;
		mpTileset.Width = mpMap.Width;
		mpTileset.Height = mpMap.Height;
		
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		break;
	case WM_SIZE:
		if (RD1Engine::theGame&&RD1Engine::theGame->mgrTileset && GlobalVars::gblVars->imgTileset)
		{
			RD1Engine::theGame->mgrTileset->DrawTileset(&GlobalVars::gblVars->imgTileset);
		}
		break;
	case WM_DESTROY:

		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}