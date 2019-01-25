#include "MainHeader.h"

int LoadTrans(char* Directive, int DataType,HWND hwnd){
char FilePath[2048]={0};
//char MainMenu[]="[MainMenu]";
int lng=0;
char Files[2][1024]={"DH-LNG-EN.txt","DH-LNG-SP.txt"};
int i=0,j=0,k=0;
char menStr[256]={0};

sprintf(FilePath,"%s\\%s",GlobalVars::gblVars->AppPath,Files[lng]);
FILE* fp = fopen(FilePath,"r");

if(fp){
  MemFile::currentFile->seek(0);
  switch(DataType){
  case 0: //Menu
//      TransMenu(Directive,fp, hwnd);
      
	  break;
  }
 
  fclose(fp);
  

}


return 0;
}



//void TransMenu(char* MenuName,HWND hwnd){
//int isParsing=0;
//char FindMenu[256];
//char Sub[]="SubUp";
//char Popup[]="Popup";
//unsigned short  controls=0;
//int i=0,j=0;  //i will be menu, j will be submenu
//	while(!feof(fp)){
//		fgets(FindMenu, 256,fp);
//		if(!isParsing){
//			if(memcmp(FindMenu,MenuName,strlen(MenuName))){
//			 isParsing=1;
//          
//			}
//		} else {
//           //Determine if menu is sub or popup
//			if(memcmp(FindMenu,Popup,5)==0){
//                  controls=ApplyMenuString(0,&FindMenu[6],hwnd,controls);
//			}else if(memcmp(FindMenu,Sub,3)==0){
//				  controls=ApplyMenuString(1,&FindMenu[4],hwnd,controls);
//            }else if(memcmp(FindMenu,"[END]",5)==0){
//			DrawMenuBar(hwnd);
//            return;
//			}else{
//             DrawMenuBar(hwnd);
//            //return;
//			}
//
//		}
//	}
//
//
//}

unsigned short ApplyMenuString(int tMenu,char* string,HWND hwnd, unsigned short Counter ){
	int tMenuC=Counter>>8;
	int tSub=Counter&0xFF;
	//Quickly go through string get rid of " and ,
	int i =0;
	int len=strlen(string);
	for(i=0;i<len;i++){
		if((string[i]==',')) {
		
		string=&string[i+1];
		
		}
	
		if (string[i]=='"') {
          //string[i]=0x20;
			string=&string[i+1];
		}
			
		
	}
	switch(tMenu){
    case 0://Popup
		ModifyMenu(

			GetMenu(hwnd),
				// handle of menu
			tMenuC,
				// menu item to modify
			MF_STRING | MF_BYPOSITION,
				// menu item flags
			 0,
				// menu item identifier or handle of drop-down menu or submenu
			string
			);
		tMenuC++;
		tSub=0;//Reset it when the main menu changes
		break;
	case 1://Sub
   ModifyMenu(

		GetSubMenu(GetMenu(hwnd),tMenuC),
		// handle of menu
	tSub,
		// menu item to modify
	MF_STRING| MF_BYPOSITION,
		// menu item flags
	0,
		// menu item identifier or handle of drop-down menu or submenu
	string
	); 
   tSub++;
		break;


	}


 return ((tMenuC<<8) | tSub);
}