#include "MainHeader.h"
#include "GlobalVars.h"
int lol;

BOOL CALLBACK  TextEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){

	unsigned char Width=0;
	unsigned char Height=0;
	unsigned long Offset=0;
	switch (message) 
	{
		case WM_INITDIALOG:
			GlobalVars::gblVars->TextEditor->tMain = hWnd;

			break;
		case WM_COMMAND:
            	switch(LOWORD(wParam)){
					case WM_DESTROY:
						
					  ShowWindow(hWnd,SW_HIDE);
					break;
					case lstOffsets:
						if(HIWORD(wParam)==LBN_SELCHANGE){//LBN_DBLCLK) 
							lol =  (int)GlobalVars::gblVars->TextEditor->Text->pixels;
							if(currentRomType==0){
                                Offset=GlobalVars::gblVars->TextEditor->ZMOffsets[GlobalVars::gblVars->TextEditor->cboTxt.GetListIndex()][GlobalVars::gblVars->TextEditor->cOffsets.GetListIndex()]-0x8000000;
							}else if(currentRomType ==1){
                                Offset=GlobalVars::gblVars->TextEditor->Offsets[GlobalVars::gblVars->TextEditor->cOffsets.GetListIndex()]-0x8000000;
							} 
							if(lol!=(int)GlobalVars::gblVars->TextEditor->Text->pixels)
								lol= lol;
					       GlobalVars::gblVars->TextEditor->DecodeText(Offset);
						   if(lol!=(int)GlobalVars::gblVars->TextEditor->Text->pixels)
								lol= lol;
                           GlobalVars::gblVars->TextEditor->Draw();
						}
						break;
					case cmdSaveText:
						/*	if(crf==0){
                                Offset=GlobalVars::gblVars->TextEditor->ZMOffsets[GlobalVars::gblVars->TextEditor->cboTxt.GetListIndex()][GlobalVars::gblVars->TextEditor->cOffsets.GetListIndex()]-0x8000000;
							}else if(crf ==1){
                                Offset=GlobalVars::gblVars->TextEditor->Offsets[GlobalVars::gblVars->TextEditor->cOffsets.GetListIndex()]-0x8000000;
							} */
						GlobalVars::gblVars->TextEditor->EncodeText(currentRomType);
					    SendMessage(hWnd,WM_COMMAND,0x0001047a,0);
						break;
					case cboTextI:
                         GlobalVars::gblVars->TextEditor->ResetList(currentRomType);
						break;
				}
			break;
	}
	
	//	return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

LRESULT CALLBACK TextPicProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc=NULL;
	PAINTSTRUCT ps;
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GlobalVars::gblVars->TextEditor->Text->Blit(hdc, 0,0,512 ,512,0,0);
		EndPaint(hWnd, &ps);
		break;
		
  		
	}
	
    return DefWindowProc(hWnd, message, wParam, lParam);
	
}
