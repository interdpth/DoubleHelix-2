#include "MainHeader.h"
#include "TilesetManager.h"
#include "BaseGame.h"
#include "globals.h"
#define Tileset 0
#define BG 1




wndBGImport::wndBGImport(){
	    gfxbuffer = new unsigned char[64000];

	    tilemap= new unsigned short[32000];
}

wndBGImport::~wndBGImport()
{
	delete[] gfxbuffer;
	delete[] tilemap;
}

HMENU importMenu;

BOOL CALLBACK BGiProc(HWND hwnd,unsigned int message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i=0;
	char  FileStuff[1024]={0};
	wndBGImport* thisClass = GlobalVars::gblVars->BGi;
	switch (message)
	{
		
		case WM_INITDIALOG: 
			thisClass->me=hwnd;
			importMenu = LoadMenu(hGlobal, MAKEINTRESOURCE(mnuInsert));
			SetMenu(hwnd, importMenu);
			thisClass->picloaded=0;
			thisClass->cboPal.Init(GetDlgItem(hwnd,cboBGPalette));
			
			    thisClass->cboPal.SetListIndex(8);
			
			break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
            case cmdTS:

	           if(thisClass->picloaded){
				   thisClass->Type = Tileset;
            // sprintf(thisClass->FileString,"D:\\Documents and Settings\\Matt\\Desktop\\Romhacking\\tiles.png");
				 thisClass->MakeFiles();
				 thisClass->LoadFiles();
				 thisClass->DelFiles();
				 thisClass->Draw();
				}

				break;
			case cmdBG:
				if(thisClass->picloaded){
				thisClass->Type=BG;
            // sprintf(thisClass->FileString,"D:\\Documents and Settings\\Matt\\Desktop\\Romhacking\\tiles.png");
				 thisClass->MakeFiles();
				 thisClass->LoadFiles();
				 thisClass->DelFiles();
				 thisClass->Draw();
				}
				break;
          
			case cmdSelFile:
				if(GBA.ReturnFileName("Please Select a file\0*.*\0",FileStuff,1024)){
                     sprintf(thisClass->FileString,"%s",FileStuff);
					 for(i =strlen(FileStuff);i>0;i--){
                        if(FileStuff[i]==0x5C){
                            break;


						}
                      

					 }
                    SetWindowText(GetDlgItem(hwnd,lblFileName),&FileStuff[i+1]);
					thisClass->picloaded=1;
				}
				
				break;

            case cboBGPalette:
				   if(HIWORD(wParam) == CBN_SELCHANGE){
					   thisClass->palpos=(unsigned char)thisClass->cboPal.GetListIndex();
				   }
					   break;
			case cmdSave:
				switch(thisClass->Type){
				case Tileset:
					thisClass->SaveCustTileset();
					break;
				case BG:
					thisClass->SaveCustBGROM();
					break;
					
					
					
				}
				   break;
			case WM_DESTROY:
					 ShowWindow(hwnd, SW_HIDE);
				break;	
			}
			break;
	    case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		thisClass->Pic.Blit(hdc, 15,23,thisClass->pW*8,thisClass->pH*8,0,0);
        //DrawTileRect(hdc,mpSTS,8);
		EndPaint(hwnd, &ps);
		break;

	}
		return 0;
}
//
//bool LZVerify(unsigned char * uncompressData, unsigned char* comppeddata) 
//{
//	unsigned char decompBuf[0xFFFFFF] = { 0 };
//	GBA.LZ77Comp(0, uncompressData, 0xFFFFFF, )
//
//
//
//}

int wndBGImport::SaveCustBGROM(){
FILE* ft=NULL;
int radarray[3]={rdo0,rdo1,rdo2};
unsigned long offsets[4]={0};//4 Offsets
unsigned long csize=0;
unsigned short filler=0;
char stroff[10]={0};

unsigned char  *tempbuff = new unsigned char[32967];;
unsigned char *superbuff = new unsigned char[32967];
unsigned long i = 0;

unsigned short ntsz=(unsigned short)((0x7DE0)-tilesize)/32;
//Figure out palsize
for(i=0; i<256;i++){
		if(GPal[i]==0 && GPal[i+1]==0 && GPal[i+2]==0)  {
 
			 palsize = (unsigned short)i;
			 break;
			}

}
//Rewrite pal code later 
//Compress palette
//csize=GBA.LZ77Comp(palsize*2, (unsigned char*)GPal, tempbuff);
//for(i=0;i<palsize;i++){
//GPal[palpos<<5+i]=GPal[i];
//}
palpos= (unsigned char)cboPal.GetListIndex();

//Write it into gba.gbapal and pcpalmem 

memset(&GBA.GBAPal, 0, sizeof(GBA.GBAPal));
GBA.EncodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);
memcpy(&GBA.GBAPal[palpos * 16], &GPal, palsize*2);
RD1Engine::theGame->mgrTileset->SaveTilesetPal(currentRomType,RD1Engine::theGame->mainRoom->roomHeader.bTileset);
GBA.DecodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);

offsets[1]=GBA.FindFreeSpace(csize,0xFF);
MemFile::currentFile->seek(offsets[1]);
MemFile::currentFile->fwrite(tempbuff,1,csize);
memset(tempbuff,0,32967);
GBA.Reopen();
//Convert Buffer to use the pal

csize=GBA.LZ77Comp(tilesize, (unsigned char*)gfxbuffer, sizeof(gfxbuffer), tempbuff);

int asize = GBA.LZ77UnComp(tempbuff, superbuff);
offsets[3]=GBA.FindFreeSpace(csize,0xFF);
MemFile::currentFile->seek(offsets[3]);
MemFile::currentFile->fwrite(tempbuff,1,csize);
memset(tempbuff,0,32967);
GBA.Reopen();





long* gfxPointer = NULL;
gfxPointer = &RD1Engine::theGame->mgrTileset->RoomTilesets[RD1Engine::theGame->mainRoom->roomHeader.bTileset].gBackground;

*gfxPointer = offsets[3]+0x8000000;


//Left Will the position and right will be the size of the palette
           //top will be starting point of tiles in RAM

/*map.left  = palpos * 16;
map.right = palsize+map.left;
map.top   = ntsz;
MakeFiles(1);
LoadFiles(1);
ft = fopen("map1.bin", "w+b");
if(ft){
	fwrite(tilemap,1,mapsize*2,ft);
	fclose(ft);
}*/
for(i=0;i<mapsize;i++) {
  	tilemap[i] +=(palpos<<0xc)+ntsz;
}
/*ft = fopen("map2.bin", "w+b");
if(ft){
	fwrite(tilemap,1,mapsize*2,ft);
	fclose(ft);
}*/
unsigned long buff = 0;
csize=GBA.LZ77Comp(mapsize*2, (unsigned char*)tilemap, sizeof(tilemap), tempbuff);
offsets[2]=GBA.FindFreeSpace(csize,0xFF);
MemFile::currentFile->seek(offsets[2]);
//We need the size somehow :#
long selsize = 0;
for (selsize = 0; selsize<3; selsize++) { //Choses i val
	if (SendMessage(GetDlgItem(me, radarray[selsize]), BM_GETCHECK, 0, 0) == BST_CHECKED) {
		break;
	}

}




MemFile::currentFile->fwrite(&selsize, 4, 1);
MemFile::currentFile->fwrite(&tempbuff,1,csize);
RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA = offsets[2] + 0x8000000;//This might be +4

GBA.Reopen();
for(i=0;i<3;i++){ //Choses i val
		if(	SendMessage(GetDlgItem(me,radarray[i]),BM_GETCHECK,0,0)==BST_CHECKED)
                	break;
}

RD1Engine::theGame->mgrTileset->SaveTileset(RD1Engine::theGame->mainRoom->roomHeader.bTileset);
SaveHeader(2);
//BaseGame::theGame->mgrTileset->GetCBG();
RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
	
 delete[] tempbuff;
 delete[] superbuff;
 return 0;
}
int wndBGImport::MakeFiles(){
    char gritpath[1024]={0};
	char param[2048]={0};
	int  retval=0;
	int  raval=0;
	int  i=0;
	
	char radiostring[3][32]={"-Ah 32 -Aw 32","-Mh 64 -Aw 32","-Ah 32 -Aw 64"};
	int  psz[3][2]={  {32,32}, {64,32}, {32,64}};
	int radarray[3]={rdo0,rdo1,rdo2};
	for(retval=strlen(FileString);retval>0;retval--) 
		     if(FileString[retval] == 0x5c)
					break; 

	sprintf(picname,"%s",&FileString[retval+1]);
	sprintf(param,"%s\\%s",GlobalVars::gblVars->AppPath,picname);
    CopyFile(FileString,param,retval);
	sprintf(gritpath,"%s\\grit.exe",GlobalVars::gblVars->AppPath);    
	for(i=0;i<3;i++){ //Choses i val
		if(	SendMessage(GetDlgItem(me,radarray[i]),BM_GETCHECK,0,0)==BST_CHECKED)
                	break;
	}
		pW=psz[i][0];
		pH=psz[i][1];

	
	sprintf(param, "%s -gt -gu 8 -gB4 -mLs -MRp -mRtpf -ftb %s", picname, radiostring[i]);
    //We have the location of grit, and the parameter to call
	//So Call it
	ShellExecute( 0, "open", gritpath, param, GlobalVars::gblVars->AppPath, 1);
	Sleep(5000);
	//MakeFiles is done!

	return 0;
}
int wndBGImport::LoadFiles(){
//Now we make sure all of our files exist
//Should have about 4 files 

#define header  0
#define img     1
#define pal     2
#define map     3
int i=0,j=0;
char name[1024]={0};
char Files[4][1024]={0};
char FileTypes[4][16]={".h",".img.bin",".pal.bin",".map.bin"};
FILE* fp=NULL;
sprintf(name,"%s",picname);
for(i=strlen(picname);i>0;i--){
	if(name[i]=='.'){
	 name[i]=0;
	 break;
	}
	name[i]=0;


}
//have base name now let's fill out Files, this is stupid and coded bad
for (i = 0; i < 4; i++) 
{
	sprintf(Files[i], "%s\\%s%s", GlobalVars::gblVars->AppPath, name, FileTypes[i]);
}
for(i=0;i<3;i++){
j=i+1;
fp=fopen(Files[j],"r+b");
	if(!fp){
		sprintf(name,"File %s\n Could not be found aborting importation",Files[j]);
		MessageBox(0,name,"Error",MB_OK);
		for(i=0;i<4;i++)   DeleteFile(Files[i]);
		sprintf(Files[0],"%s\\%s",GlobalVars::gblVars->AppPath,name);
		DeleteFile(Files[0]);
		return 0;
	}else{
		MemFile::currentFile->seek(0);
		switch(j){
			case img:
			
					tilesize=GBA.ReturnEOF(fp);
					MemFile::currentFile->seek(0);
					MemFile::currentFile->fread(gfxbuffer,1,tilesize);
			
				
				break;
			case  pal:
				
					palsize=GBA.ReturnEOF(fp);
					MemFile::currentFile->seek(0);
					MemFile::currentFile->fread(GPal,1,palsize);
			
				break;

			case map:
				mapsize=GBA.ReturnEOF(fp);
				MemFile::currentFile->seek(0);
			    
				MemFile::currentFile->fread(&tilemap,1,mapsize);
				
			
				break;



		}

	}
	fclose(fp);
}

for(i=0;i<4;i++){  
	DeleteFile(Files[i]);
	sprintf(Files[0],"%s\\%s",GlobalVars::gblVars->AppPath,name);
	DeleteFile(Files[0]);
//Everything should be loaded
}
return 0;
}

int wndBGImport::DelFiles(){
//We don't need any of the newly created files everything is in memory :)
//DeleteFile(picname);//That all we do here? wtf


	return 0;
}

int wndBGImport::Draw(){
    int rex, rey,i , x, y, scrX, scrY;
	rex=rey=i=x=y=scrX=scrY=0;//
	Tiles.Load(gfxbuffer,tilesize/32);
	GBA.DecodePal(GPal,PCPal,16, 0) ; 
    Pic.Create(pW*8,pH*8);
	Pic.SetPalette(PCPal);
    //mX=pW;
	//mY=pH;
	//rex = (mX == 64 ? 2 :  1);
	//rey = (mY == 64 ? 2 :  1);
		
	//	for(scrY = 0; scrY < pH/32; scrY++)
	//	{	
	//		for(scrX = 0; scrX < pW/32; scrX++)
	//		{
				for(y = 0; y < pH; ++y)
				{
					for(x = 0; x < pW; ++x)
					{
						// Pic.Draw(Tiles,x*8 + scrX*256	,y*8+ scrY*256 ,  tilemap[i]);
						Pic.Draw(Tiles,x*8 	,y*8 ,  tilemap[i]);
						i++;
					}
				}
		//	}
	//	}
	InvalidateRect(me,0,1);
	return 0;
}


int wndBGImport::SaveCustTileset(){
	//FILE* ft = NULL;
	//int radarray[3] = { rdo0,rdo1,rdo2 };
	//unsigned long offsets[4] = { 0 };//4 Offsets
	//unsigned long csize = 0;
	//unsigned short filler = 0;
	//char stroff[10] = { 0 };
	//unsigned char  tempbuff[32967] = { 0 };
	//unsigned char superbuff[32967];
	//unsigned long i = 0;
	//if (tilesize> 0x7DE0 || ((((0x7DE0) - tilesize) / 32) > 0x400)) {

	//	MessageBox(me, "Cannot save to ROM, this image has too many non repeated tiles\nPlease choose a different image", "Error", MB_OK);
	//	return 0;
	//}
	//unsigned short ntsz = (unsigned short)((0x7DE0) - tilesize) / 32;
	////Figure out palsize
	//for (i = 0; i<256; i++) {
	//	if (GPal[i] == 0 && GPal[i + 1] == 0 && GPal[i + 2] == 0) {

	//		palsize = (unsigned short)i;
	//		break;
	//	}

	//}
	////Rewrite pal code later 
	////Compress palette
	////csize=GBA.LZ77Comp(palsize*2, (unsigned char*)GPal, tempbuff);
	////for(i=0;i<palsize;i++){
	////GPal[palpos<<5+i]=GPal[i];
	////}
	//palpos = (unsigned char)cboPal.GetListIndex();

	////Write it into gba.gbapal and pcpalmem 

	//memset(&GBA.GBAPal, 0, sizeof(GBA.GBAPal));
	//GBA.EncodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);
	//memcpy(&GBA.GBAPal[32], &GPal, palsize * 2);
	//BaseGame::theGame->mgrTileset->SaveTilesetPal(currentRomType, BaseGame::theGame->mainRoom->roomHeader.bTileset);
	//GBA.DecodePal(GBA.GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);

	////offsets[1] = GBA.FindFreeSpace(csize, 0xFF);
	////MemFile::currentFile->seek(offsets[1]);
	////fwrite(tempbuff, 1, csize);
	////memset(tempbuff, 0, 32967);
	////GBA.Reopen();
	////Convert Buffer to use the pal

	//csize = GBA.LZ77Comp(tilesize, (unsigned char*)gfxbuffer, sizeof(gfxbuffer), tempbuff);

	//int asize = GBA.LZ77UnComp(tempbuff, superbuff);
	//offsets[3] = GBA.FindFreeSpace(csize, 0xFF);
	//MemFile::currentFile->seek(offsets[3]);
	//fwrite(tempbuff, 1, csize);
	//memset(tempbuff, 0, 32967);
	//GBA.Reopen();





	//long* gfxPointer = NULL;
	//if (!currentRomType) gfxPointer = &BaseGame::theGame->mgrTileset->RoomTilesets[BaseGame::theGame->mainRoom->roomHeader.bTileset].gTiles;
	//if (currentRomType)  gfxPointer = &BaseGame::theGame->mgrTileset->RoomTilesets[BaseGame::theGame->mainRoom->roomHeader.bTileset].gTiles;// -0x8000000

	//*gfxPointer = offsets[3] + 0x8000000;


	////Left Will the position and right will be the size of the palette
	////top will be starting point of tiles in RAM

	///*map.left  = palpos * 16;
	//map.right = palsize+map.left;
	//map.top   = ntsz;
	//MakeFiles(1);
	//LoadFiles(1);
	//ft = fopen("map1.bin", "w+b");
	//if(ft){
	//fwrite(tilemap,1,mapsize*2,ft);
	//fclose(ft);
	//}*/
	//for (i = 0; i<mapsize; i++) {
	//
	//	tilemap[i] += 192;
	//}
	///*ft = fopen("map2.bin", "w+b");
	//if(ft){
	//fwrite(tilemap,1,mapsize*2,ft);
	//fclose(ft);
	//}*/
	//unsigned long buff = 0;

	//gfxPointer = NULL;
	//if (!currentRomType) gfxPointer = &BaseGame::theGame->mgrTileset->RoomTilesets[BaseGame::theGame->mainRoom->roomHeader.bTileset].TSAMap;
	//if (currentRomType)  gfxPointer = &BaseGame::theGame->mgrTileset->RoomTilesets[BaseGame::theGame->mainRoom->roomHeader.bTileset].TSAMap;// -0x8000000


	//offsets[2] = GBA.FindFreeSpace(mapsize * 2, 0xFF);//might be too much? 
	//MemFile::currentFile->seek(offsets[2]);
	////We need the size somehow :#

	//fwrite(&tilemap, 2, mapsize  );
	//unsigned short writeShit = 0x2;
	//fwrite(&writeShit, 2, 1);


	//*gfxPointer = offsets[2] + 0x8000000;//This might be +4


	//BaseGame::theGame->mgrTileset->SaveTileset(BaseGame::theGame->mainRoom->roomHeader.bTileset);
	//SaveHeader(2);
	////BaseGame::theGame->mgrTileset->GetCBG();
	//DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);
	//
return 0;
}


int wndBGImport::FillOutArray(unsigned char* array){
	unsigned char rawData[328] =
{
    0x02, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x59, 0x14, 0x59, 0x14, 0x79, 0x14, 
    0x79, 0x14, 0x59, 0x10, 0x59, 0x10, 0x79, 0x10, 0x79, 0x10, 0x59, 0x14, 0x59, 0x10, 0x79, 0x14, 
    0x79, 0x10, 0x40, 0x00, 0x5A, 0x14, 0x40, 0x00, 0x7A, 0x14, 0x5A, 0x10, 0x40, 0x04, 0x7A, 0x10, 
    0x40, 0x04, 0x5B, 0x14, 0x5A, 0x14, 0x7B, 0x14, 0x7A, 0x14, 0x5A, 0x10, 0x5B, 0x10, 0x7A, 0x10, 
    0x7B, 0x10, 0x5C, 0x14, 0x5A, 0x14, 0x7C, 0x14, 0x7A, 0x14, 0x5A, 0x10, 0x5C, 0x10, 0x7A, 0x10, 
    0x7C, 0x10, 0x5D, 0x14, 0x5A, 0x14, 0x7D, 0x14, 0x7A, 0x14, 0x5A, 0x10, 0x5D, 0x10, 0x7A, 0x10, 
    0x7D, 0x10, 0x5E, 0x14, 0x5A, 0x14, 0x7E, 0x14, 0x7A, 0x14, 0x5A, 0x10, 0x5E, 0x10, 0x7A, 0x10, 
    0x7E, 0x10, 0x5F, 0x14, 0x5A, 0x14, 0x7F, 0x14, 0x7A, 0x14, 0x5A, 0x10, 0x5F, 0x10, 0x7A, 0x10, 
    0x7F, 0x10, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x99, 0x14, 0x98, 0x14, 0xB9, 0x14, 
    0xB8, 0x14, 0x98, 0x10, 0x99, 0x10, 0xB8, 0x10, 0xB9, 0x10, 0x98, 0x14, 0x98, 0x10, 0xB8, 0x14, 
    0xB8, 0x10, 0x40, 0x00, 0x9A, 0x14, 0x40, 0x00, 0xBA, 0x14, 0x9A, 0x10, 0x40, 0x04, 0xBA, 0x10, 
    0x40, 0x04, 0x9B, 0x14, 0x9A, 0x14, 0xBB, 0x14, 0xBA, 0x14, 0x9A, 0x10, 0x9B, 0x10, 0xBA, 0x10, 
    0xBB, 0x10, 0x9C, 0x14, 0x9A, 0x14, 0xBC, 0x14, 0xBA, 0x14, 0x9A, 0x10, 0x9C, 0x10, 0xBA, 0x10, 
    0xBC, 0x10, 0x9D, 0x14, 0x9A, 0x14, 0xBD, 0x14, 0xBA, 0x14, 0x9A, 0x10, 0x9D, 0x10, 0xBA, 0x10, 
    0xBD, 0x10, 0x9E, 0x14, 0x9A, 0x14, 0xBE, 0x14, 0xBA, 0x14, 0x9A, 0x10, 0x9E, 0x10, 0xBA, 0x10, 
    0xBE, 0x10, 0x9F, 0x14, 0x9A, 0x14, 0xBF, 0x14, 0xBA, 0x14, 0x9A, 0x10, 0x9F, 0x10, 0xBA, 0x10, 
    0xBF, 0x10, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xB9, 0x1C, 0xB8, 0x1C, 0x99, 0x1C, 
    0x98, 0x1C, 0xB8, 0x18, 0xB9, 0x18, 0x98, 0x18, 0x99, 0x18, 0xB8, 0x1C, 0xB8, 0x18, 0x98, 0x1C, 
    0x98, 0x18, 0x40, 0x08, 0xBA, 0x1C, 0x40, 0x08, 0x9A, 0x1C, 0xBA, 0x18, 0x40, 0x0C, 0x9A, 0x18, 
    0x40, 0x0C, 0xBB, 0x1C, 0xBA, 0x1C, 0x9B, 0x1C, 0x9A, 0x1C, 0xBA, 0x18, 0xBB, 0x18, 0x9A, 0x18, 
    0x9B, 0x18, 0xBC, 0x1C, 0xBA, 0x1C, 0x9C, 0x1C, 
} ;
memcpy(array, rawData, 328);
return 0;
}