#include "MainHeader.h"
#include "cSSE.h"
#include "cOAMManager.h"

int InitPosArray(){
	int i = 0;
	for( i = 0; i < 256; i++){
		 ZMSpritePos[i][0] = 0;
		 ZMSpritePos[i][1] = 0;
	}
	//Poses are in tiles. 
     ZMSpritePos[0x24][1] = ZMSpritePos[0x23][1]=  ZMSpritePos[0x22][1] = 32;
    for(i = 0; i < 0xD;i++)
	ZMSpritePos[0x25+i][1] = (i%2 ? 32:64);
	ZMSpritePos[0x3A][0] = 16;
    ZMSpritePos[0x3A][1] = 96;
	ZMSpritePos[0x48][0] = 24;
    ZMSpritePos[0x4B][1] = 16;
	ZMSpritePos[0x4E][0] = 20;
    ZMSpritePos[0x95][0]=40;
	ZMSpritePos[0x95][1]=80;
	ZMSpritePos[0x50][0]=7;
	ZMSpritePos[0x50][1]=-16;
	ZMSpritePos[0x60][0]=16;
	ZMSpritePos[0x60][1]=72;
    ZMSpritePos[0x6E][0]=16;
	ZMSpritePos[0x8A][0]=48;
	ZMSpritePos[0x8A][1]=64;
	ZMSpritePos[0x8C][1]=32;
	ZMSpritePos[0x8C][0]=14;
	ZMSpritePos[0x8C][1]=32;
	ZMSpritePos[0xA3][0]=-16;
	ZMSpritePos[0xA3][1]=56;
	ZMSpritePos[0xA4][0]=90;
	ZMSpritePos[0xA4][1]=50;
	return 0;
}

int Preview(unsigned char Sprite){
//Lots of work goes on
	return 0;
}

