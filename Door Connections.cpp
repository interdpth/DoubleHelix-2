#include "MainHeader.h"
#include "DoorManager.h"
int EnableConnections(){
	unsigned char blah[3];//Need three vars to test
	char max=0;
	long offset=0;
	int i = 0;

	MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("Connections")->Value);

	
	MemFile::currentFile->fread(&offset,4,1);
	MemFile::currentFile->seek(offset-0x8000000);

	while(1){
		MemFile::currentFile->fread(&blah,1,3);
		if((blah[0]==0xFF) && (blah[2]==0xFF)) {
			max = i - 3; 
			break;
		}
		i +=3;
	}
	max/=6;
	DoorConnections.Connects = max;
	MemFile::currentFile->seek(offset - 0x8000000);
	for(i = 0;i<DoorConnections.Connects;i++){
		DoorConnections.DoorConnects[i].sA1= MemFile::currentFile->fgetc();;
		DoorConnections.DoorConnects[i].D1= MemFile::currentFile->fgetc();
		DoorConnections.DoorConnects[i].dA1= MemFile::currentFile->fgetc();
		DoorConnections.DoorConnects[i].sA2= MemFile::currentFile->fgetc();
		DoorConnections.DoorConnects[i].D2= MemFile::currentFile->fgetc();
		DoorConnections.DoorConnects[i].dA2= MemFile::currentFile->fgetc();
	}
	
	
	return 0;
}


int DoesaBridgeExist(){//Will find any connection for this room only.
	unsigned char Area=0;
	unsigned char Door=0;
	unsigned char Room=0;
	char blah[5];
	int i=0;
	int ii = 0;
	Area= (unsigned char)comboArea.GetListIndex();
	Room =(unsigned char) comboRoom.GetListIndex();
	ConnectExists=0;
	for(i=0;i<0x100;i++) PSCE[i] =0;
	//How many doors are currently loaded
	for(i = 0;i<DoorConnections.Connects;i++){
		
		for(ii=0;ii<RD1Engine::theGame->mgrDoors->DoorCount;ii++){
			//First we'll check the area then the oor
			if((Area == DoorConnections.DoorConnects[i].sA1)  || (Area == DoorConnections.DoorConnects[i].sA2) ){
				if((DoorConnections.DoorConnects[i].D1 == RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[ii]) ||
				   (DoorConnections.DoorConnects[i].D2 == RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[ii]) ){
					sDoor* thisDoor = &RD1Engine::theGame->mgrDoors->Doors[DoorConnections.DoorConnects[i].D1].rawDoor;
					if((thisDoor->OwnerRoom == Room)){
						ConnectExists++;
						PSCE[ConnectExists] =i;
					}
				}
			}
		}
		
	} 
	if(ConnectExists==0) {
      	EnableWindow(GetDlgItem(DoorWin,cmdSaveConn),0);
		cboDoorArea1.Disable();
	    cboDoorArea1.Disable();
	    cboDoorArea3.Disable();
     	cboDoorDoor1.Disable();
		doorConnection.Disable();
		return 0;
	}
	EnableWindow(GetDlgItem(DoorWin,cmdSaveConn),1);
	cboDoorArea1.Enable();
	cboDoorArea1.Enable();
	cboDoorArea3.Enable();
    cboDoorDoor1.Enable();
	doorConnection.Enable();
	doorConnection.Clear();
	for(i = 0; i < ConnectExists;i++){
		sprintf(blah,"%X",i);
		doorConnection.Additem(blah);
	}
	doorConnection.SetListIndex(0);
	SendMessage(DoorWin,WM_COMMAND,HIWORD(CBN_SELCHANGE)|LOWORD(cboConnect),0);
	return 0;
}

int UserEnableConnections(){
	char strbuf[256];
	sprintf(strbuf,"Total Area Connects: %d",ConnectExists);
	SetWindowText(GetDlgItem(hwndMain(),lblC),strbuf);
	if(ConnectExists){
		EnableWindow(GetDlgItem(hwndMain(),cmdViewConnect),1);
	}else{
		EnableWindow(GetDlgItem(hwndMain(),cmdViewConnect),0);
	}
	return 0;
}

int SetConnections(unsigned char conn){
	conn = conn+1;
	cboDoorArea1.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].sA1);
	cboDoorArea1.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].D1);
	cboDoorArea3.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].dA1);
	//cboDoorArea3.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].sA2);
	cboDoorDoor1.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].D2);
	//cboDoorDoor2.SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].dA2);
	return 0;
}