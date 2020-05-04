#include "MainHeader.h"
/*class cTSAEditorClass{
public:
     cTSAEditorClass();
	 ~cTSAEditorClass();
	 tBlock  CurrentBlock[4];
	 sTilesetBlockStructure    blockset;
	
	 HWND TilesetPic;
	 HWND BlocksetPic;
	 HWND Preview;
	 HWND tMain;
	 Image Tileset;
	 Image iPreview;
	 Image TSA;
	 sChecks HorChk[4];
	 sChecks VerChk[4];
	 sCombo cboPals[4];
	 int Create();
	 int DrawThisTileset();
	 int DrawPreview();
	 int DrawTSA();
	 int LoadTSA();
	 int CreateTilesetWind();
	 int CreatePreviewWind();
	 int CreateTSAWind();
};   */

BOOL CALLBACK TSAProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{


	switch (message) 
	{
		case WM_INITDIALOG:
			MyTSAEditor.tMain = hWnd;
			MyTSAEditor.Create();
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_VSCROLL:
			break;
		case WM_RBUTTONDOWN:
	
			break;
				case WM_RBUTTONUP:
			
			break;
		case WM_MOUSEMOVE:
		
		
			break;

	
		case WM_PAINT:
			 
       

      
	
			break;
		case WM_SIZE:
            
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				//Handle Palettes first
				if(HIWORD(wParam)==CBN_SELCHANGE){
			case cbopallettetsa:
			
				MyTSAEditor.CurrentBlock[0].Pal=(unsigned short)MyTSAEditor.cboPals[0].GetListIndex();
				MyTSAEditor.DrawPreview();
				break;
			case cbopallettetsa2:
				MyTSAEditor.CurrentBlock[1].Pal=(unsigned short)MyTSAEditor.cboPals[1].GetListIndex();
				MyTSAEditor.DrawPreview();
				break;
			case cbopallettetsa3:
				MyTSAEditor.CurrentBlock[2].Pal=(unsigned short)MyTSAEditor.cboPals[2].GetListIndex();
				MyTSAEditor.DrawPreview();
				break;
			case cbopallettetsa4:
				MyTSAEditor.CurrentBlock[3].Pal=(unsigned short)MyTSAEditor.cboPals[3].GetListIndex();
				MyTSAEditor.DrawPreview();
				break;
				}
			case vertchk:
				MyTSAEditor.CurrentBlock[0].VFlip=MyTSAEditor.VerChk[0].value()*0x800;
				MyTSAEditor.DrawPreview();
				break;
			case vertchk2:
				MyTSAEditor.CurrentBlock[1].VFlip=MyTSAEditor.VerChk[1].value()*0x800;
				MyTSAEditor.DrawPreview();
				break;
			case vertchk3:
				MyTSAEditor.CurrentBlock[2].VFlip=MyTSAEditor.VerChk[2].value()*0x800;
				MyTSAEditor.DrawPreview();
				break;
			case vertchk4:
				MyTSAEditor.CurrentBlock[3].VFlip=MyTSAEditor.VerChk[2].value()*0x800;
				MyTSAEditor.DrawPreview();
				break;
			case chkHor:
				MyTSAEditor.CurrentBlock[0].HFlip=MyTSAEditor.HorChk[0].value()*0x400;
				MyTSAEditor.DrawPreview();				
				break;
			case chkHor2:
				MyTSAEditor.CurrentBlock[1].HFlip=MyTSAEditor.HorChk[1].value()*0x400;
				MyTSAEditor.DrawPreview();				
				break;
			case chkHor3:
				MyTSAEditor.CurrentBlock[2].HFlip=MyTSAEditor.HorChk[2].value()*0x400;
				MyTSAEditor.DrawPreview();				
				break;
			case chkHor4:
				MyTSAEditor.CurrentBlock[3].HFlip=MyTSAEditor.HorChk[3].value()*0x400;
				MyTSAEditor.DrawPreview();				
				break;
			case cmdSaveBlock:
			    MyTSAEditor.SaveBlock();
				break;
            case cboTP:
				if(HIWORD(wParam)==CBN_SELCHANGE) MyTSAEditor.DrawThisTileset();
				break;
	     	case WM_DESTROY:
					ShowWindow(MyTSAEditor.tMain,SW_HIDE);
				break;
			}
			break;
		

	 }		
            
   return 0;
  
}

LRESULT CALLBACK BlockPreviewProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc=NULL;
	PAINTSTRUCT ps;
	int a=0;
	int b=0;

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyTSAEditor.iPreview->Blit(hdc,0,0,16,16,0,0);
		EndPaint(hWnd, &ps);
		break;
		
  	case WM_LBUTTONDOWN:
	//	sprintf(buff,"Tile: %d",(GetX(lParam)+GetY(lParam))/8  );
	//	MessageBox(hWnd,buff,"None",MB_OK);
		a=GetX(lParam);
		b=GetY(lParam);
		MyTSAEditor.CurrentBlock[(a/8)+(b/8)*2 & 32].Tile=MyTSAEditor.curtsa;
		MyTSAEditor.DrawPreview();
			break;
   
	}
	
    return DefWindowProc(hWnd, message, wParam, lParam);
	
}


LRESULT CALLBACK BlockTilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc=NULL;
	PAINTSTRUCT ps;
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	
		MyTSAEditor.Tileset->Blit(hdc,0,0,288,1024,0,0);
		EndPaint(hWnd, &ps);
		break;
		
  		case WM_LBUTTONDOWN:
		MyTSAEditor.curtsa= (GetX(lParam)/8)+(GetY(lParam)/8)*16;
		
			break;
   
	}
	
    return DefWindowProc(hWnd, message, wParam, lParam);
	
}
