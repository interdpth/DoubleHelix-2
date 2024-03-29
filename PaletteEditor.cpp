#include "MainHeader.h"
#include "GlobalVars.h"
#include "GBAGraphics.h"
#include "TilesetManager.h"
#include "BaseGame.h"

int DrawPal(HDC hdc, long* palette, int X, int Y, int palcol, int size = 16) {

	int i = 0;
	HBRUSH curbrush = NULL;
	RECT myrect = { 0,0,0,0 };
	//Blank the colors.
	for (i = 0; i < 256; i++) {

		curbrush = CreateSolidBrush(0);
		myrect.left = ((i) % 16) * size;
		myrect.top = ((i) / 16) * size;
		myrect.right = myrect.left + size;
		myrect.bottom = myrect.top + size;
		FillRect(hdc, &myrect, curbrush);
		DeleteObject(curbrush);

	}
	for (i = 0; i < 512; i++) {
		curbrush = CreateSolidBrush(palette[i]);
		myrect.left = ((i) % 16) *size;
		myrect.top = ((i) / 16) * size;
		myrect.right = myrect.left + size;
		myrect.bottom = myrect.top + size;
		FillRect(hdc, &myrect, curbrush);
		DeleteObject(curbrush);

	}
	return 0;

}

BOOL CALLBACK   LPProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam){

   HDC             hdc;
   int             i = 0;
   unsigned short             wndrs[3] = {hsbC1, hsbC2, hsbC2};
   PAINTSTRUCT     ps;

   switch (message)
   {

   case WM_INITDIALOG:
      hwndLPE = hwnd;

      break;

   case WM_PAINT:

      hdc = BeginPaint(hwnd, &ps);

      DrawPal(hdc, GBAGraphics::VRAM->PcPalMem, 0, 0, 0, 16);

      EndPaint(hwnd, &ps);

      break;
   case WM_TIMER:

      break;

   case WM_LBUTTONDOWN:
      // Check for the pal rect
      if ((GetX(lParam) >= 256) || (GetY(lParam) >= 256))
         return 0;
      LoadColor((GetX(lParam) / 16) + (GetY(lParam) / 16) * 16);
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case cmdSaveColors:

         GBA.EncodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);
         if (currentRomType == 0)
         {

             MemFile::currentFile->seek(RD1Engine::theGame->mgrTileset->RoomTilesets[RD1Engine::theGame->mainRoom->roomHeader->bTileset]->pTiles - 0x8000000); // Seek 
                                                                                            // the 
                                                                                            // endcoded 
                                                                                            // pal

			 MemFile::currentFile->fwrite(&GBA.GBAPal[32], sizeof(short), 0x1E0); // Read

                                                                    // pal

         } else if (currentRomType == 1)
         {

            MemFile::currentFile->seek(RD1Engine::theGame->mgrTileset->RoomTilesets[RD1Engine::theGame->mainRoom->roomHeader->bTileset]->pTiles - 0x8000000); // Seek 
                                                                                              // the 
                                                                                              // endcoded 
                                                                                              // pal

			MemFile::currentFile->fwrite(&GBA.GBAPal[32], sizeof(short), 0x1E0); // Read

                                                                    // pal c

         }
         if (currentRomType == 0)
         {
            MemFile::currentFile->seek(0x31C72E);
			MemFile::currentFile->fwrite(&GBA.GBAPal[1], sizeof(short), 15);

            MemFile::currentFile->seek(0x3ED51C);
			MemFile::currentFile->fwrite(&GBA.GBAPal[16], sizeof(short), 16);

         } else if (currentRomType == 1)
         {

            MemFile::currentFile->seek(0x40805E);
			MemFile::currentFile->fwrite(&GBA.GBAPal[1], 2, 47);

         }
         GBA.DecodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);
         InvalidateRect(hwndLPE, 0, 1);

         break;
      case cmdANC:
	  {
		  RD1Engine::theGame->mgrTileset->DrawTileset(&GlobalVars::gblVars->imgTileset);
		  MapManager * mgr = RD1Engine::theGame->mainRoom->mapMgr;
		 if(RD1Engine::theGame->mainRoom->roomHeader->bBg2!=0)mgr->GetLayer(MapManager::Backlayer)->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);
		 if (RD1Engine::theGame->mainRoom->roomHeader->bBg0 != 0)mgr->GetLayer(MapManager::ForeGround)->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);
		  
		 if (RD1Engine::theGame->mainRoom->roomHeader->bBg1 != 0)mgr->GetLayer(MapManager::LevelData)->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);
		 if (RD1Engine::theGame->mainRoom->roomHeader->lBg3 != 0)mgr->GetLayer(MapManager::BackgroundLayer)->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);

	//refresh thread	  RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
		  InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		  InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		  InvalidateRect(hwnd, 0, 1);
	  }
         break;
      case IDCANCEL:
         EndDialog(hwnd, 1);
         break;
      }

      break;
   case WM_HSCROLL:

      if ((HWND) lParam == GetDlgItem(hwnd, hsbC1))
      {
         GlobalVars::gblVars->scrColors[0].UpdateScroll(wParam);
      }
      if ((HWND) lParam == GetDlgItem(hwnd, hsbC2))
      {

         GlobalVars::gblVars->scrColors[1].UpdateScroll(wParam);
      }
      if ((HWND) lParam == GetDlgItem(hwnd, hsbC3))
      {
         GlobalVars::gblVars->scrColors[2].UpdateScroll(wParam);
      }
      ChangeColor();
      break;

   }
   return 0;
}

int             LoadColor(long Pal)
{
   RGBQUAD         Colors;
   int             i = 0;
   char            blahstr[32] = {0};
   HWND            hwndArray[3] = {GetDlgItem(hwndLPE, lblC1), GetDlgItem(hwndLPE, lblC2), GetDlgItem(hwndLPE, lblC3)};

//Hello welcome to load color. 
   curclr = Pal;
   Colors.rgbRed   = GetRValue(GBAGraphics::VRAM->PcPalMem[curclr]); /*
                                                       * get R, G, and B out
                                                       * of DWORD 
                                                       */
   Colors.rgbGreen = GetGValue(GBAGraphics::VRAM->PcPalMem[curclr]);
   Colors.rgbBlue  = GetBValue(GBAGraphics::VRAM->PcPalMem[curclr]);
   Colors.rgbReserved = 0;

   GlobalVars::gblVars->scrColors[0].curpos = Colors.rgbRed;
   GlobalVars::gblVars->scrColors[1].curpos = Colors.rgbGreen;
   GlobalVars::gblVars->scrColors[2].curpos = Colors.rgbBlue;
   SetScrollPos(GlobalVars::gblVars->scrColors[0].me, SB_CTL, GlobalVars::gblVars->scrColors[0].curpos, 1);
   SetScrollPos(GlobalVars::gblVars->scrColors[1].me, SB_CTL, GlobalVars::gblVars->scrColors[1].curpos, 1);
   SetScrollPos(GlobalVars::gblVars->scrColors[2].me, SB_CTL, GlobalVars::gblVars->scrColors[2].curpos, 1);
//Do labels
   for (i = 0; i < 3; i++)
   {
      sprintf(blahstr, "RGB: %d\nGBA:%d", GlobalVars::gblVars->scrColors[i].curpos, (GlobalVars::gblVars->scrColors[i].curpos >> 3) & 0x1F);

      SetWindowText(hwndArray[i], blahstr);

   }
   return 0;
}

int             ChangeColor()
{
   int             i = 0;
   char            blahstr[32] = {0};
   HWND            hwndArray[3] = {GetDlgItem(hwndLPE, lblC1), GetDlgItem(hwndLPE, lblC2), GetDlgItem(hwndLPE, lblC3)};

   for (i = 0; i < 3; i++)
   {
      sprintf(blahstr, "RGB: %d\nGBA:%d"
              ,GlobalVars::gblVars->scrColors[i].curpos

              ,(GlobalVars::gblVars->scrColors[i].curpos >> 3) & 0x1F);

      SetWindowText(hwndArray[i], blahstr);
      GBAGraphics::VRAM->PcPalMem[curclr] = RGB(GlobalVars::gblVars->scrColors[0].curpos, GlobalVars::gblVars->scrColors[1].curpos, GlobalVars::gblVars->scrColors[2].curpos);
      InvalidateRect(hwndLPE, 0, 0);
// DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);
   }
   return 0;

};
