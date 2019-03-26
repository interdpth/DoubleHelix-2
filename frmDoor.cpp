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
		
		cboDoorTyp.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoorType)));
		doorConnection.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboConn)));
		cboDoorOwner.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboOwner)));
		doorConnection.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboConnect)));
		cboDoorArea1.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboArea1)));
		cboDoorArea1.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoor1)));
		cboDoorDoor1.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboDoor2)));
		cboDoorArea3.Init(GetDlgItem(hwnd,(int)MAKEINTRESOURCE(cboArea2)));
		
		
		for(i = 0; i<0x100;i++){
			sprintf(cboBuf,"%X",i);
			cboDoorTyp.Additem(cboBuf);
			doorConnection.Additem(cboBuf);
			cboDoorOwner.Additem(cboBuf);
		}
		cboDoorTyp.SetListIndex(0);
		doorConnection.SetListIndex(0);
		cboDoorOwner.SetListIndex(0);
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
				curMgr->AddDoor(comboRoom.GetListIndex());
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			}
			//EndDialog(DoorWin,0);
		}
		if (LOWORD(wParam) == buttonDeleteDoor) {
			int Index = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId();
			editingStates CurState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();
			if (CurState == editingStates::DOOR && Index != -1) {
				curMgr->DeleteDoor(comboRoom.GetListIndex(), curMgr->CurrentRoomDoorIndexes[Index]);
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
			curMgr->Doors[curMgr->CurrentRoomDoorIndexes[CurDoor]].rawDoor.DestDoor = (unsigned char)doorConnection.GetListIndex();
			
			curMgr->ConnectDoor(curMgr->Doors[curMgr->CurrentRoomDoorIndexes[CurDoor]].rawDoor.DestDoor);
			
		}
		if(LOWORD(wParam) == cboConnect){
			SetConnections((unsigned char)doorConnection.GetListIndex());
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
									   cboDoorTyp.SetListIndex(Doors[CurrentRoomIndex].rawDoor.DoorType);
									   doorConnection.SetListIndex(Doors[CurrentRoomIndex].rawDoor.DestDoor);
									   cboDoorOwner.SetListIndex(Doors[CurrentRoomIndex].rawDoor.OwnerRoom);
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
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.DoorType = (unsigned char)cboDoorTyp.GetListIndex();
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.DestDoor= (unsigned char)doorConnection.GetListIndex();
	RD1Engine::theGame->mgrDoors->Doors[RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[DoorNo]].rawDoor.OwnerRoom=(unsigned char)cboDoorOwner.GetListIndex();
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
		cboDoorArea1.Enable();
		cboDoorArea1.Enable();
		cboDoorArea2.Enable();
		cboDoorArea3.Enable();
		cboDoorDoor1.Enable();
		cboDoorDoor2.Enable();
		doorConnection.Enable();
	}else{
		cboDoorArea1.Disable();
		cboDoorArea1.Disable();
		cboDoorArea2.Disable();
		cboDoorArea3.Disable();
		cboDoorDoor1.Disable();
		cboDoorDoor2.Disable();
		doorConnection.Disable();
	}
	return 0;
}

int SaveConnections(){
	int i = 0;
	long offset=0;

	int c = PSCE[doorConnection.GetListIndex()]+1;
	DoorConnections.DoorConnects[c].sA1 =DoorConnections.DoorConnects[c].dA2= cboDoorArea1.GetListIndex();
	DoorConnections.DoorConnects[c].D1=cboDoorArea1.GetListIndex();
	DoorConnections.DoorConnects[c].dA1=DoorConnections.DoorConnects[c].sA2=cboDoorArea2.GetListIndex();
    DoorConnections.DoorConnects[c].D2=cboDoorDoor1.GetListIndex();

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
