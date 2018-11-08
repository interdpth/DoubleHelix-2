#include "MainHeader.h"
#include "DoorManager.h"
int EnableConnections(){
	unsigned char blah[3];//Need three vars to test
	char max=0;
	long offset=0;
	int i = 0;

	MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("Connections")->Value);

	
	MemFile::currentFile->fread(&offset,4,1,GBA.ROM);
	MemFile::currentFile->seek(offset-0x8000000);

	while(1){
		MemFile::currentFile->fread(&blah,1,3,GBA.ROM);
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
	Area= (unsigned char)Combos[cArea].GetListIndex();
	Room =(unsigned char) Combos[cRoom].GetListIndex();
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
		Combos[cA1].Disable();
	    Combos[cD1].Disable();
	    Combos[cA2].Disable();
     	Combos[cD2].Disable();
	    Combos[cConnect].Disable();
		return 0;
	}
	EnableWindow(GetDlgItem(DoorWin,cmdSaveConn),1);
	Combos[cA1].Enable();
	Combos[cD1].Enable();
	Combos[cA2].Enable();
    Combos[cD2].Enable();
	Combos[cConnect].Enable();
	Combos[cConnect].Clear();
	for(i = 0; i < ConnectExists;i++){
		sprintf(blah,"%X",i);
		Combos[cConnect].Additem(blah);
	}
	Combos[cConnect].SetListIndex(0);
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
	Combos[cA1].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].sA1);
	Combos[cD1].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].D1);
	Combos[cA2].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].dA1);
	//Combos[cA2].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].sA2);
	Combos[cD2].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].D2);
	//Combos[cdA2].SetListIndex(DoorConnections.DoorConnects[PSCE[conn]].dA2);
	return 0;
}