#include "DoorManager.h"

#include "MainHeader.h"
#include "GlobalVars.h"
#include "resource.h"
#include "ConnectionsEditor.h"
#include "BaseGame.h"
BOOL CALLBACK DwProc (HWND hwnd,unsigned int message,WPARAM wParam,LPARAM lParam)
{
	int i;
	char cboBuf[100];
	DoorManager *curMgr = RD1Engine::theGame->mgrDoors;
	switch (message)
	{
		
	case WM_INITDIALOG:	// when dialog is first created
		DoorWin = hwnd;
		
		nMaxVScroll[sWidth] = 0xFF;
		nMaxVScroll[sHeight] = 0xFF;
		
		Combos[dType].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoorType)));
		Combos[dConn].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboConn)));
		Combos[dOwner].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboOwner)));
		Combos[cConnect].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboConnect)));
		Combos[cA1].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboArea1)));
		Combos[cD1].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoor1)));
		Combos[cD2].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoor2)));
		Combos[cA2].Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboArea2)));
		
		
		for(i = 0; i<0x100;i++){
			sprintf(cboBuf,"%X",i);
			Combos[dType].Additem(cboBuf);
			Combos[dConn].Additem(cboBuf);
			Combos[dOwner].Additem(cboBuf);
		}
		Combos[dType].SetListIndex(0);
		Combos[dConn].SetListIndex(0);
		Combos[dOwner].SetListIndex(0);
		ShowWindow(hwnd,SW_HIDE);
		break;
	case BN_CLICKED:
		
		break;
	case WM_LBUTTONDOWN:
		
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == btnAddDoor) {
			int Index = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId();
			editingStates CurState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();
			if (CurState == editingStates::DOOR) {
				curMgr->AddDoor(Combos[cRoom].GetListIndex());
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			}
			//EndDialog(DoorWin,0);
		}
		if (LOWORD(wParam) == buttonDeleteDoor) {
			int Index = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId();
			editingStates CurState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();
			if (CurState == editingStates::DOOR && Index != -1) {
				curMgr->DeleteDoor(Combos[cRoom].GetListIndex(), curMgr->CurrentRoomDoorIndexes[Index]);
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			}
			//EndDialog(DoorWin,0);
		}
		if(LOWORD(wParam) == IDCANCEL) {
			EndDialog(DoorWin,0);
		}
		if(LOWORD(wParam)==cmdSaveDoor){
			curMgr->SaveThisDoor(CurDoor);
		}
		if (LOWORD(wParam) == cmdSaveConn)
		{
			SaveConnections();
		}
		if(LOWORD(wParam)==cboConn){
			curMgr->Doors[curMgr->CurrentRoomDoorIndexes[CurDoor]].rawDoor.DestDoor = (unsigned char)Combos[dConn].GetListIndex();
			
			curMgr->ConnectDoor(curMgr->Doors[curMgr->CurrentRoomDoorIndexes[CurDoor]].rawDoor.DestDoor);
			
		}
		if(LOWORD(wParam) == cboConnect){
			SetConnections((unsigned char)Combos[cConnect].GetListIndex());			  
		}
		break;
		
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll
		
		


	case WM_MOVE:
	case WM_SIZE:
	{
		//RECT MainWindowRect;
		//RECT thisWindowRect;
		//GetWindowRect(GetDlgItem(Main, grpBoxObj), &MainWindowRect);
	
		//GetWindowRect(hwnd, &thisWindowRect);
		//thisWindowRect.bottom -= thisWindowRect.top;
		//thisWindowRect.right -= thisWindowRect.left;
		//
		//thisWindowRect.left = MainWindowRect.left + 12;
		//thisWindowRect.top = MainWindowRect.top + 10;

		///*thisWindowRect.bottom += thisWindowRect.top;
		//thisWindowRect.right += thisWindowRect.left;*/


		//MoveWindow(hwnd, thisWindowRect.left, thisWindowRect.top, thisWindowRect.right, thisWindowRect.bottom, true);
	}
		break;
	case WM_DESTROY:
		
		/// myBackBuffer.Destroy();
		break;
	}
	
	return 0;
}

int DoorManager::LoadThisDoor(int DoorNo) {

									   char	cboBuf[512] = {0};
									   if (DoorNo == -1) return 0;
									   int CurrentRoomIndex = CurrentRoomDoorIndexes[DoorNo];
									   sprintf(cboBuf,"Room Door: %d \n Actual Door: %X",DoorNo,CurrentRoomDoorIndexes[DoorNo]);
									   SetWindowText(GetDlgItem(DoorWin,lblDoor),cboBuf);
									   Combos[dType].SetListIndex(Doors[CurrentRoomIndex].rawDoor.DoorType);
									   Combos[dConn ].SetListIndex(Doors[CurrentRoomIndex].rawDoor.DestDoor);
									   Combos[dOwner].SetListIndex(Doors[CurrentRoomIndex].rawDoor.OwnerRoom);
									   sprintf(cboBuf,"%X",Doors[CurrentRoomIndex].rawDoor.xExitDistance);
									   SetWindowText(GetDlgItem(DoorWin,txtLength),cboBuf);
									   nHScroll[sWidth] = (Doors[CurrentRoomIndex].virtualDoor.Width - Doors[CurrentRoomIndex].virtualDoor.sX)+1;
									   nVScroll[sHeight] = (Doors[CurrentRoomIndex].virtualDoor.Height - Doors[CurrentRoomIndex].virtualDoor.sY)+1;
									   sprintf(cboBuf,"%d",(Doors[CurrentRoomIndex].virtualDoor.Width - Doors[CurrentRoomIndex].virtualDoor.sX)+1);
									   SetWindowText(GetDlgItem(DoorWin,txtDWidth),cboBuf);
									   sprintf(cboBuf,"%d",(Doors[CurrentRoomIndex].virtualDoor.Height - Doors[CurrentRoomIndex].virtualDoor.sY)+1);
									   SetWindowText(GetDlgItem(DoorWin,txtDHeight),cboBuf);


									   CurDoor = DoorNo;
									   ConnectDoor(CurrentRoomIndex);
									   if(ConnectExists) {
									   ConnControls(1);
									   }else{
									   ConnControls(0);
									   }
	return 0;
}

int DoorManager::ConnectDoor(unsigned char TD) {

	char Buf[512];
	unsigned char condoor = RD1Engine::theGame->mgrDoors->Doors[TD].rawDoor.DestDoor;

	sprintf(Buf, "Door: %X", TD);
	SetWindowText(GetDlgItem(DoorWin, lblCDoor), Buf);

	condoor = RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->Doors[TD].rawDoor.DestDoor].rawDoor.DoorType;
	sprintf(Buf, "%X", condoor);
	SetWindowText(GetDlgItem(DoorWin, lblDoorType), Buf);

	condoor = RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->Doors[TD].rawDoor.DestDoor].rawDoor.OwnerRoom;
	sprintf(Buf, "%X", condoor);
	SetWindowText(GetDlgItem(DoorWin, lblOwnerRoom), Buf);

	condoor = RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->Doors[TD].rawDoor.DestDoor].rawDoor.xExitDistance;
	sprintf(Buf, "%d", condoor);
	SetWindowText(GetDlgItem(DoorWin, lblCDWidth), Buf);

	/* unsigned char DoorType;
	unsigned char OwnerRoom;
	unsigned char XEntrance;
	unsigned char DWidth;
	unsigned char YEntrance;
	unsigned char DHeight;
	unsigned char DestDoor;
	unsigned char ExitDoorLength;
	unsigned char u1;
	unsigned char u2;
	unsigned char u3;
	unsigned char u4;*/
	return 0;
}

int DoorManager::SaveThisDoor(int DoorNo){
	char	cboBuf[512];
	int blah=0;
	
	sprintf(cboBuf,"Room Door: %d \n Actual Door: %d",DoorNo,RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]);
	SetWindowText(GetDlgItem(DoorWin,lblDoor),cboBuf);
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.DoorType = (unsigned char)Combos[dType].GetListIndex();
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.DestDoor= (unsigned char)Combos[dConn ].GetListIndex();
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.OwnerRoom=(unsigned char)Combos[dOwner].GetListIndex();
	GetWindowText(GetDlgItem(DoorWin,txtLength),cboBuf,3);
	sscanf(cboBuf,"%X",&blah);
	if(blah > 255) blah = 255;
	if(blah < 0) blah = 0;
	sprintf(cboBuf,"%X",RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.xExitDistance);
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.xExitDistance = blah;
	
	
	GetWindowText(GetDlgItem(DoorWin,txtDWidth),cboBuf,4);
	sscanf(cboBuf,"%d",&blah);
    RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].virtualDoor.Width = blah+ RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].virtualDoor.sX-1;
	

	GetWindowText(GetDlgItem(DoorWin,txtDHeight),cboBuf,4);
	sscanf(cboBuf,"%X",&blah);
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].virtualDoor.Height = blah + RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].virtualDoor.sY-1;
	blah =	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.DestDoor;
	RD1Engine::theGame->mgrDoors->Doors[blah].rawDoor.DestDoor = RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo];
	RD1Engine::theGame->mgrDoors->SaveDoors(RD1Engine::theGame->mainRoom->Area);
	RD1Engine::theGame->mgrDoors->LoadThisDoor(DoorNo);
	//DisplayDoors();
	//InvalidateRect(UiState::stateManager->GetMapWindow(),0,1);
	RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
	return 0;
	
}

int ConnControls(bool How){
	
	if(How){
		Combos[cA1].Enable();
		Combos[cD1].Enable();
		Combos[cdA1].Enable();
		Combos[cA2].Enable();
		Combos[cD2].Enable();
		Combos[cdA2].Enable();
		Combos[cConnect].Enable();
	}else{
		Combos[cA1].Disable();
		Combos[cD1].Disable();
		Combos[cdA1].Disable();
		Combos[cA2].Disable();
		Combos[cD2].Disable();
		Combos[cdA2].Disable();
		Combos[cConnect].Disable();
	}
	return 0;
}

int SaveConnections(){
	int i = 0;
	long offset=0;

	int c = PSCE[Combos[cConnect].GetListIndex()]+1;
	DoorConnections.DoorConnects[c].sA1 =DoorConnections.DoorConnects[c].dA2= Combos[cA1].GetListIndex();
	DoorConnections.DoorConnects[c].D1=Combos[cD1].GetListIndex();
	DoorConnections.DoorConnects[c].dA1=DoorConnections.DoorConnects[c].sA2=Combos[cdA1].GetListIndex();
    DoorConnections.DoorConnects[c].D2=Combos[cD2].GetListIndex();

	FILE* fp = fopen(GBA.FileLoc,"r+b");
	char max=0;
	
	
	if(currentRomType == 0){
		MemFile::currentFile->seek(0x05EEB8);
	}else if(currentRomType == 1){
		MemFile::currentFile->seek(0x6945c);
	}else{
		return 0;
	}
	MemFile::currentFile->fread(&offset,4,1);
	MemFile::currentFile->seek(offset-0x8000000);
	//for(int i = 0;;i+=3){
	
	
	
	MemFile::currentFile->seek(offset - 0x8000000);
	for(i = 0;i<DoorConnections.Connects;i++){
		fputc(DoorConnections.DoorConnects[i].sA1, fp);
		fputc(DoorConnections.DoorConnects[i].D1, fp);
		fputc(DoorConnections.DoorConnects[i].dA1,fp);
		fputc(DoorConnections.DoorConnects[i].sA2, fp);
		fputc(DoorConnections.DoorConnects[i].D2, fp);
		fputc(DoorConnections.DoorConnects[i].dA2, fp);
	}
	fclose(fp);
	EnableConnections();
	return 0;
}
