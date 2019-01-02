#include "MapUtils.h"

void LoadScrollInfo(int s, Scroller *scroll);

MapUtils::MapUtils(MapManager*  mgr)
{
	_mapMgr = RD1Engine::theGame->mainRoom->mapMgr;
}


MapUtils::~MapUtils()
{
}

void MapUtils::HandleRightClick(editingStates thisState, int mouseX, int mouseY, int objlist)
{
	_mapMgr = RD1Engine::theGame->mainRoom->mapMgr;
	int td = 0;
	if ((thisState != editingStates::SPRITE  && thisState != editingStates::DOOR) || (thisState == editingStates::DOOR &&  SendMessage(GetDlgItem(hwndMain(), chkResizeDoors), BM_GETCHECK, 0, 0) == 1))
	{
		_mapMgr->GetState()->SetAction(editingActions::RESIZE);
	}
	else
	{
		if (thisState == editingStates::DOOR)
		{
			int doornum = RD1Engine::theGame->mgrDoors->GetDoor(RD1Engine::theGame->mainRoom->Room, mouseX, mouseY);
			_mapMgr->GetState()->SetObjId(doornum);
			if (td != -1) {
				UiState::stateManager->ShowObj();
				RD1Engine::theGame->mgrDoors->LoadThisDoor(doornum);
				UiState::stateManager->ShowObj();
				return;
			}
		}
		else if (thisState == editingStates::SPRITE) {
			int spriteno = Gimmeasprite(mouseX,mouseY, objlist);
			_mapMgr->GetState()->SetObjId(spriteno);
			SpriteTabIndex = spriteno;
			UiState::stateManager->ShowObj();
			LoadCurSprite();
		}
		else if (thisState == editingStates::SCROLL) {
			int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(mouseX, mouseY, cboScroll.GetListIndex());
			if (scrollid != -1)
			{
				_mapMgr->GetState()->SetObjId(scrollid);


				UiState::stateManager->ShowObj();
				LoadScrollInfo(scrollid, RD1Engine::theGame->mgrScrolls->GetScrollInfo());
			}
		}
		//InvalidateRect(hWnd, 0, 1);
	}

}
void MapUtils::HandleLeftClick(editingStates thisState, int mouseX, int mouseY, int spritelistindex, int wParam, int lParam)
{
	_mapMgr = RD1Engine::theGame->mainRoom->mapMgr;
	if (thisState == editingStates::SPRITE) {
		int spriteno = Gimmeasprite(mouseX, mouseY, spritelistindex);
		_mapMgr->GetState()->SetObjId(spriteno);
		_mapMgr->GetState()->SetAction(editingActions::MOVE);
	}
	else if (thisState == editingStates::DOOR)
	{
		if (RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetAction() != editingActions::MOVE)
		{
			int doornum = RD1Engine::theGame->mgrDoors->GetDoor(RD1Engine::theGame->mainRoom->Room, mouseX, mouseY);
			if (doornum != -1)
			{
				_mapMgr->GetState()->SetObjId(RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[doornum]);
				_mapMgr->GetState()->SetAction(editingActions::MOVE);

			}
		}
	}
	else if (thisState == editingStates::SCROLL)
	{
		if (_mapMgr->GetState()->GetAction() != editingActions::MOVE)
		{
			int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(mouseX, mouseY, cboScroll.GetListIndex());
			if (scrollid != 0xffffffff)
			{
				_mapMgr->GetState()->SetObjId(scrollid);
				_mapMgr->GetState()->SetAction(editingActions::MOVE);
			}
		}
	}
	else if (thisState == editingStates::MAP)
	{
		EditLayers(wParam, lParam);
	}

}

int  MapUtils::Gimmeasprite(int X, int Y, int objlist)
{
	int             i = 0;
	int             x = 0;
	int             y = 0;
	int             width = 0;
	int             height = 0;
	nEnemyList *ThisEnemy = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[objlist];
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

int  MapUtils::EditThisLayer(nMapBuffer * Layer, WPARAM wParam, LPARAM lParam, byte usens, unsigned short ToUse)
{ // Layer to use, XY value, if it's bigger then 16x16

	int             baseX = (GetX(lParam) / 16) + MapHorizScroll->GetIndex();


	int             baseY = (GetY(lParam) / 16) + MapVertScroll->GetIndex();

	int             y = 0;
	int             x = 0;
	int             ny = 0;
	int             nx = 0;
	int             add = 0;
	int             mwidth = Layer->X;
	int			   undo = 0;
	unsigned short* TileBuf2D = Layer->TileBuf2D;
	int             Tiles = (mpMap.uX) + (mpMap.uY);
	int width = 0, height = 0;
	Layer->Dirty = true;
	///Copy and paste stuff, disabled for now
	int keyno = 0;
	if (keyno == 1) {
		y = GetMessagePos();
		baseX = (GetX(y) / 16) + MapHorizScroll->GetIndex();
		baseY = (GetY(y) / 16) + MapVertScroll->GetIndex();


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

						val = ToUse + (Layer == RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata) ? 0 : x + (y * 16));
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
	Layer->DrawRect.bottom = (mpMap.sY+( mpMap.Height)+ MapVertScroll->GetIndex());
	Layer->DrawRect.right = ( mpMap.sX+  mpMap.Width+MapHorizScroll->GetIndex()  );*/
	Layer->Dirty = Layer->SDirty = 1;
	RD1Engine::theGame->DrawStatus.dirty = true;
	RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
	//DrawLevel();
	SetWindowText(UiState::stateManager->GetMapWindow(), "Map");
	InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
	return 0;



}

int MapUtils::EditLayers(WPARAM wParam, LPARAM lParam) {

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
