#include "MainHeader.h"
#include "nMapBuffer.h"
#include "GlobalVars.h"
#include "DoorManager.h"
#include "MapEditingState.h"
#include "SpriteObjectManager.h"
#include "clsRoomScrolls.h"
//Scroller::Scroller()
//{
//	;
//}
//
//Scroller::~Scroller()
//{
//	;
//}



int  DrawTileRect(HDC hdc, long theColor, MousePointer mpointer, int mul)
{
	RECT            myrect;
	HBRUSH          curbrush = CreateSolidBrush(theColor);

	myrect.left = mpointer.sX * mul;
	myrect.top = mpointer.sY * mul;
	myrect.right = mpointer.Width * mul + myrect.left;
	myrect.bottom = myrect.top + mpointer.Height * mul;
	FrameRect(hdc, &myrect, curbrush);
	DeleteObject(curbrush);
	return 0;
}



int  DrawTileRect(HDC hdc, MousePointer mpointer, int mul)
{

	return DrawTileRect(hdc, RGB(255, 0, 0),  mpointer,  mul);
	 
}

LRESULT CALLBACK InputBoxProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	int             choice = -1;
	char            buf[200];

	switch (message)
	{

	case WM_SHOWWINDOW:
		SetWindowText(GetDlgItem(hWnd, lblEnter), procstring);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == cmdInputOK)
		{
			GetWindowText(GetDlgItem(hWnd, txtInput), buf, 4);
			sscanf(buf, "%x", &choice);
			ShowWindow(hWnd, SW_HIDE);

			EndDialog(hWnd, choice);
		}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int             LoadInput(long val)
{
	int             blah;

	blah = DialogBox(hGlobal, MAKEINTRESOURCE(frmInputBox), 0, (int(__stdcall *) (struct HWND__ *, unsigned int, unsigned int, long)) InputBoxProc);
	if (blah > -1)
		return blah;
	return val;
}





int             Gimmeasprite(int X, int Y)
{
	int             i = 0;
	int             x = 0;
	int             y = 0;
	int             width = 0;
	int             height = 0;
	nEnemyList *ThisEnemy = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[Combos[cSpriteSet].GetListIndex()];
	RECT* thisOverall = RD1Engine::theGame->mainRoom->mgrSpriteObjects->OverallSize;
		for (i = 0; i < ThisEnemy->Max(); i++)
		{

			width = thisOverall[(ThisEnemy->Enemies[i].Creature & 0xF) - 1].right - thisOverall[(ThisEnemy->Enemies[i].Creature & 0xF) - 1].left;

			height = thisOverall[(ThisEnemy->Enemies[i].Creature & 0xF) - 1].bottom - thisOverall[(ThisEnemy->Enemies[i].Creature & 0xF) - 1].top;
			x = ThisEnemy->Enemies[i].X;
			y = ThisEnemy->Enemies[i].Y;
			if (((x) <= X) &&
				((x + width / 16) >= X) &&
				((y) <= Y) &&
				((y + height / 16) >= Y))
			{

				
				return i;
			}
		}
	

	return -1;
}

int             DrawClipIdent()
{
	int             currclip = Combos[cClip].GetListIndex();
	RECT            blah;
	int             thisX = 0;
	int             thisY = 0;
	int             x = 0;
	int             y = 0;

	HBRUSH          curbrush = CreateSolidBrush(RGB(255, 255, 255));

	for (thisY = 0; thisY < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y; thisY++)
	{
		for (thisX = 0; thisX < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X; thisX++)
		{

			if (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->TileBuf2D[thisX + (thisY * (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->X))] == currclip)
			{

				blah.left = thisX * 16;
				blah.right = thisX * 16 + 16;
				blah.top = thisY * 16;
				blah.bottom = thisY * 16 + 16;

				FrameRect(RD1Engine::theGame->ThisBackBuffer.DC(), &blah, curbrush);
				

			}
		}
	}
	DeleteObject(curbrush);
	InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
	return 0;
}



int             EditThisLayer(nMapBuffer * Layer, WPARAM wParam, LPARAM lParam, byte usens, unsigned short ToUse)
{ // Layer to use, XY value, if it's bigger then 16x16

	int             baseX = (GetX(lParam) / 16) +nHScroll[sHMap];


	int             baseY = (GetY(lParam) / 16) + nVScroll[sVMap];

	int             y = 0;
	int             x = 0;
	int             ny = 0;
	int             nx = 0;
	int             add = 0;
	int             mwidth = Layer->X;
	int			   undo = 0;
	int			   cock = 0;
	unsigned short* TileBuf2D = Layer->TileBuf2D;
	int             Tiles = (mpMap.uX) + (mpMap.uY);
	int width = 0, height = 0;
	Layer->Dirty = true;
	if (keyno == 1) {
		y = GetMessagePos();
		baseX = (GetX(y) / 16) +nHScroll[sHMap];
		baseY = (GetY(y) / 16) + nVScroll[sVMap];
		

		switch (wParam) {
		case 0x5a:
		

			break;
		case 67://'c'

			break;

		case 86://'p'

		   /* Layer->DrawRect.top  = mpMap.sY;
			Layer->DrawRect.left = mpMap.sX;
			Layer->DrawRect.bottom = (mpMap.sY+( mpMap.Height));
			Layer->DrawRect.right = ( mpMap.sX+  mpMap.Width  );*/

			/*Layer->DrawRect.top = 0;
			Layer->DrawRect.left =0;
			Layer->DrawRect.bottom = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
			Layer->DrawRect.right = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;*/
			/* Layer->Dirty=Layer->SDirty=1;
			  undo=0;
			  DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);
			 InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);*/

			 // return 0;
			break;

		}

		keyno = 0;
	}
	else {	
		if (baseX >= Layer->X || baseY >= Layer->Y)
		{
			return 0;
		}
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

		Layer->UndoBuff->Set(Layer->X*Layer->Y * 2, Layer->TileBuf2D);//Set the undobuffer

		width = mpMap.Width; //max(   mpTileset.Width-mpMap.Width,  0);// + 1;
		height = mpMap.Height; //max(  mpTileset.Height-mpMap.Height, 0);// + 1;
		int val = 0;
		for (ny = 0; ny < height; ny++) {
			for (nx = 0; nx < width; nx++) {
				for (y = 0; y < mpTileset.Height; y++) {
					if (ny*mpTileset.Height + y >= mpMap.Height) break;
					for (x = 0; x < mpTileset.Width; x++) {
						if (nx*mpTileset.Width + x >= mpMap.Width) break;
						//ToUse is the tile to start at in the tile list
						
						val=ToUse + (Layer == RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata) ? 0 : x + (y * 16));
						TileBuf2D[(baseX + x) + (baseY + y) * (mwidth)+nx*mpTileset.Width + ny*mpTileset.Height *mwidth] = val;

					}
				}
			}

		}


	}
	/* if(undo!=1){
		//Set Rect up
	   Layer->DrawRect.top = baseY;
	   Layer->DrawRect.left =  baseX;
	   Layer->DrawRect.bottom = (baseY+(mpTileset.Height) +height);
	   Layer->DrawRect.right = (baseX+ mpTileset.Width  + width);
	 }*/
	 /*Layer->DrawRect.top  = mpMap.sY;
	 Layer->DrawRect.left = mpMap.sX;
	 Layer->DrawRect.bottom = (mpMap.sY+( mpMap.Height)+ nVScroll[sVMap]);
	 Layer->DrawRect.right = ( mpMap.sX+  mpMap.Width+nHScroll[sHMap]  );*/
	Layer->Dirty = Layer->SDirty = 1;
	RD1Engine::theGame->DrawStatus.dirty = true;
	RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
	//DrawLevel();
	SetWindowText(UiState::stateManager->GetMapWindow(), "Map");
	InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
	return 0;



}

int EditLayers(WPARAM wParam, LPARAM lParam) {

	int i = 0;
	int Layer[4] = { 0 };



	if (GlobalVars::gblVars->checkBoxForeground.value() == 1 || GlobalVars::gblVars->chkMC[0].value() == 1) {
		EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround), wParam, lParam, 1, Tile);
	}
	else if (GlobalVars::gblVars->checkBoxLevel.value() == 1 || GlobalVars::gblVars->chkMC[1].value() == 1)
	{
		EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData), wParam, lParam, 1, Tile);
	}
	else			if (GlobalVars::gblVars->checkBoxBackground.value() == 1 || GlobalVars::gblVars->chkMC[2].value() == 1)
	{
		EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer), wParam, lParam, 1, Tile);

	}
	else				if (GlobalVars::gblVars->checkBoxClip.value() == 1 || GlobalVars::gblVars->chkMC[3].value() == 1)
	{
		EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata), wParam, lParam, 0, Combos[cClip].GetListIndex());

	}
	return 0;
}


int DoScroll(LPARAM lParam) {
	if (!movingScroll) {
		movingScrollIndex = RD1Engine::theGame->mgrScrolls->Findmeascroll(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]), cboScroll.GetListIndex());
		if (movingScrollIndex) {
			movingScroll = 1;

		}
		else {
			movingScroll = movingScrollIndex = 0;
		}
	}
	return 0;

}