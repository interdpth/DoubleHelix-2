/*int ShowSprites(unsigned char Number){
	int i = 0;
	int relX=0;
	int relY=0;
	int tSprite=0;
    int d=0;
	RECT blah = {0,0,0,0};
	
    if(Number == 0){
		for(i = 0; i < Enemy1.max;i++){
		 
			
		//	if(cOAMManager::mgrOAM->SpriteOAM[cOAMEdit::sprite_in[(Enemy1.Enemies[i].Creature & 0xF)-1].sprtype]==0) continue;
        tSprite=((Enemy1.Enemies[i].Creature) & 0xF)-1;
		
		if(cOAMManager::mgrOAM->SpriteOAM[cOAMEdit::sprite_in[tSprite].sprtype]){
		SpritePics[tSprite].TransBlit(
		
			ThisBackBuffer.DC(),
			(Enemy1.Enemies[i].X)*16-ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][0],
		
			(Enemy1.Enemies[i].Y)*16 - ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][1],
	    	(OverallSize[tSprite].right -OverallSize[tSprite].left),	
	        (OverallSize[tSprite].bottom-OverallSize[tSprite].top),
			0,
			0);
		
		}else{//Draw an S for sprite
		
			
				blah.left   =(Enemy1.Enemies[i].X)*16;
				blah.top    =(Enemy1.Enemies[i].Y)*16;
				blah.right  =blah.left+16;
				blah.bottom =blah.top +16;
					HBRUSH curbrush = CreateSolidBrush(RGB(255, 255, 255));
					
                    FrameRect(ThisBackBuffer.DC(), &blah,curbrush);
	                 DeleteObject(curbrush);
			
		
					 TextOut(ThisBackBuffer.DC(), Enemy1.Enemies[i].X*16+8,    
						     Enemy1.Enemies[i].Y*16,"S",1);
		}
	  
		}
	}else if (Number == 1){
        	for(i = 0; i < Enemy2.max;i++){
		tSprite=(Enemy2.Enemies[i].Creature & 0xF)-1;
	

     		if(cOAMManager::mgrOAM->SpriteOAM[cOAMEdit::sprite_in[tSprite].sprtype]){
		SpritePics[tSprite].TransBlit(
		
			ThisBackBuffer.DC(),
			(Enemy2.Enemies[i].X)*16-ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][0],
		
			(Enemy2.Enemies[i].Y)*16 - ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][1],
	    	(OverallSize[tSprite].right -OverallSize[tSprite].left),	
	        (OverallSize[tSprite].bottom-OverallSize[tSprite].top),
			0,
			0);
		
			}else{
              
				blah.left   =(Enemy2.Enemies[i].X)*16;
				blah.top    =(Enemy2.Enemies[i].Y)*16;
				blah.right  =blah.left+16;
				blah.bottom =blah.top +16;
					HBRUSH curbrush = CreateSolidBrush(RGB(255, 255, 255));
					
                    FrameRect(ThisBackBuffer.DC(), &blah,curbrush);
	                 DeleteObject(curbrush);
				
		
					 TextOut(ThisBackBuffer.DC(), Enemy2.Enemies[i].X*16+8,    
						     Enemy2.Enemies[i].Y*16,"S",1);

			}
	  
		}
	}else if (Number == 2){
	for(i = 0; i < Enemy3.max;i++){
		 
			
		//	if(cOAMManager::mgrOAM->SpriteOAM[cOAMEdit::sprite_in[(Enemy1.Enemies[i].Creature & 0xF)-1].sprtype]==0) continue;
        tSprite=(Enemy3.Enemies[i].Creature & 0xF-1);
		if(cOAMManager::mgrOAM->SpriteOAM[cOAMEdit::sprite_in[tSprite].sprtype]){
		SpritePics[tSprite].TransBlit(
		
			ThisBackBuffer.DC(),
			(Enemy3.Enemies[i].X)*16-ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][0],
		
			(Enemy3.Enemies[i].Y)*16 - ZMSpritePos[cOAMEdit::sprite_in[tSprite].sprtype][1],
	    	(OverallSize[tSprite].right -OverallSize[tSprite].left),	
	        (OverallSize[tSprite].bottom-OverallSize[tSprite].top),
			0,
			0);
		
		}else{
		
				blah.left   =(Enemy3.Enemies[i].X)*16;
				blah.top    =(Enemy3.Enemies[i].Y)*16;
				blah.right  =blah.left+16;
				blah.bottom =blah.top +16;
					HBRUSH curbrush = CreateSolidBrush(RGB(255, 255, 255));
					
                    FrameRect(ThisBackBuffer.DC(), &blah,curbrush);
	                 DeleteObject(curbrush);
			
		
					 TextOut(ThisBackBuffer.DC(), Enemy3.Enemies[i].X*16+8,    
						     Enemy3.Enemies[i].Y*16,"S",1);
		}
	}
	
	}
	return 0;
}
*/
/*/*
int GetClipdata(){
   	
	fseek(GBA.ROM,RoomHeader.lClipData-0x8000000,SEEK_SET);
	
	fread(MapManager::mgrMaps->GetLayer(MapManager::Clipdata).X,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X++;		// DISCH -- added inc
	fread(MapManager::mgrMaps->GetLayer(MapManager::Clipdata).Y,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y++;		// DISCH -- added inc
	// sprintf(buffer,"X is %x Y is %x",MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y);
	// sMessage(buffer); 
	fread(GBA.CompBuffer,sizeof(unsigned char),32691,GBA.ROM);

    memset(MapManager::mgrMaps->GetLayer(MapManager::Clipdata)->TileBuf2D,0,0x20000);
	GBA.ZMUncompRle((MapManager::mgrMaps->GetLayer(MapManager::Clipdata).X*MapManager::mgrMaps->GetLayer(MapManager::Clipdata).Y)*2, GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::Clipdata)->TileBuf2D);
	
	
	return 0;
}
int GetBackLayer(){
	
		unsigned char decompbuf[38592]={0};
	
	fseek(GBA.ROM,RoomHeader.lBackLayer-0x8000000,SEEK_SET);
	if(RoomHeader.bBg2 == 0x10){
		fread(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X++;		// DISCH -- added inc
		fread(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y++;		// DISCH -- added inc
		// sprintf(buffer,"X is %x Y is %x",MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y);
		// sMessage(buffer);
		fread(&GBA.CompBuffer,sizeof(unsigned char),32691,GBA.ROM);
		
		//GBA.ZMUncompRle(GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::LevelData)->TileBuf2D ,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y, 1);
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->BImage.Create(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X*16,MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y*16);
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
		memset(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->TileBuf2D,0,0x20000);
		GBA.ZMUncompRle(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X*MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y, GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->TileBuf2D);
	}else if(RoomHeader.bBg2==0x40){
        fseek(GBA.ROM,RoomHeader.lBackLayer-0x8000000,SEEK_SET);
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->size=fgetc(GBA.ROM);
		fseek(GBA.ROM,RoomHeader.lBackLayer-0x8000000+4,SEEK_SET);
		fread(&GBA.CompBuffer,sizeof(unsigned char),32691,GBA.ROM);
		memset(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->TileBuf2D,0,0x20000);
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->oSize=GBA.LZ77UnComp(GBA.CompBuffer, decompbuf);
	
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->BImage.Create(1024,1024);
		MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
		memcpy(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->TileBuf2D,decompbuf,MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->oSize);
	}
	
	//    GlobalVars::gblVars->img[iRoom].Clear();			// DISCH -- killed this
	
	
	//sprintf(buffer,"short 1 is: %x",(short)MapManager::mgrMaps->GetLayer(MapManager::LevelData)->TileBuf2D[195]);
	//sMessage(buffer);
	//fclose(fp);
	/*
	
	  
		for(thisY = 0; thisY<MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y;thisY++){
		for(thisX = 0; thisX < MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X;thisX++){
		TILE =(short) MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->TileBuf2D[thisX+(thisY * (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X))];
		GlobalVars::gblVars->img[iRoom].Draw(TileImage,thisX* 16, (thisY) * 16, TSA.nTSA[TILE*4]);
		GlobalVars::gblVars->img[iRoom].Draw(TileImage,(thisX * 16) + 8, (thisY) * 16, TSA.nTSA[(TILE*4)+1]);
		GlobalVars::gblVars->img[iRoom].Draw(TileImage,(thisX * 16), ((thisY) * 16)+8, TSA.nTSA[(TILE*4)+2]);
		GlobalVars::gblVars->img[iRoom].Draw(TileImage,(thisX * 16)+8, ((thisY) * 16)+8, TSA.nTSA[(TILE*4)+3]);
		
		  }
		  }
		  
	InvalidateRect(GetDlgItem(Main,picMap),0,0);*/
	//return 0;
//}
/*/*//*

int GetLevelLayer(){
	
	
	fseek(GBA.ROM,RoomHeader.lLevelData-0x8000000,SEEK_SET);
	fread(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X++;		// DISCH -- added inc
	fread(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y++;		// DISCH -- added inc

	fread(&GBA.CompBuffer,sizeof(unsigned char),32691,GBA.ROM);

	MapManager::mgrMaps->GetLayer(MapManager::LevelData)->BImage.Create(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*16,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16);
    MapManager::mgrMaps->GetLayer(MapManager::LevelData)->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
    memset(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->TileBuf2D,0,0x20000);
	//	GBA.ZMUncompRle(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y, GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::LevelData)->TileBuf2D);
    MapManager::mgrMaps->GetLayer(MapManager::LevelData)->oSize=GBA.RLEComp( GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::LevelData)->TileBuf2D);
	return 0;
}
int GetForeLayer(){
	unsigned char decompbuf[130050]={0};
	int i=0;
	if(RoomHeader.bBg0 == 0x10){
		fseek(GBA.ROM,RoomHeader.lForeground-0x8000000,SEEK_SET);
		fread(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X++;		// DISCH -- added inc
		fread(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y,sizeof(unsigned char),1,GBA.ROM);		//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y++;		// DISCH -- added inc
		// sprintf(buffer,"X is %x Y is %x",MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y);
		// sMessage(buffer);
		//for(i=0;i<32192;i++)GBA.CompBuffer[i]=0;
		//memset(GBA.CompBuffer,0,32192);
		fread(GBA.CompBuffer,1,32691,GBA.ROM);
	
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.Create(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X*16,MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y*16);
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
		memset(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D,0,0x20000);
		
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->oSize=GBA.ZMUncompRle(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X*MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y, GBA.CompBuffer, (unsigned char*)MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D);
		
	}else if(RoomHeader.bBg0==0x40){
        fseek(GBA.ROM,RoomHeader.lForeground-0x8000000,SEEK_SET);
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->size=fgetc(GBA.ROM);
		fseek(GBA.ROM,RoomHeader.lForeground-0x8000000+4,SEEK_SET);
		fread(&GBA.CompBuffer,sizeof(unsigned char),32691,GBA.ROM);
		//memset(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D,0,sizeof(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D));
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->oSize=GBA.LZ77UnComp(GBA.CompBuffer, decompbuf);
    
		memset(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D,0,0x20000);
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.Create(1024,1024);
		MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
		memcpy(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->TileBuf2D,decompbuf,MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->oSize);

		
		
		
	}
	return 0;
}
int GetBG(){

	unsigned char buffer[32192]={0};
	int decmpsize=0;
    fseek(GBA.ROM,RoomHeader.lBackgroundTSA-0x8000000,SEEK_SET);
	fread(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->size,1,1,GBA.ROM);
	if(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->size ==0){
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->X = 32; 
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->Y = 32;
	}else if((MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->size == 1)||(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->size >= 3)) {
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->X = 64;
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->Y = 32;
	}else if(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->size == 2){
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->X = 32;
		MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->Y = 64;
	}
    //fseek + 4
	memset(&GBA.CompBuffer,0,64691);
	fseek(GBA.ROM,RoomHeader.lBackgroundTSA-0x8000000+4,SEEK_SET);
	fread(GBA.CompBuffer,1,64691,GBA.ROM);


	decmpsize = GBA.LZ77UnComp(GBA.CompBuffer,buffer);
	MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->oSize = decmpsize;

	memset(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->TileBuf2D,0,0x20000);
	memcpy(MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->TileBuf2D,buffer,MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->oSize);
	return 0;
}
*/
/*
int  DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED){

	unsigned char cSize=0;
	unsigned char fd=0;
	int Width=GlobalVars::gblVars->img[1].Width,Height=GlobalVars::gblVars->img[1].Height;
	int k =0;

	
	if(!((crf == 0)||(crf==1))) return 0;
	
	
	Image ImgBuf;
	GlobalVars::gblVars->img[iRoom].Create(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*16,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16);
	
	ThisBackBuffer.Create(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*16,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16);
	SetBkMode(ThisBackBuffer.DC(), TRANSPARENT);
	SetTextColor(ThisBackBuffer.DC(),RGB(255,255,255));
	GlobalVars::gblVars->img[iRoom].SetPalette(GBAGraphics::VRAM->PcPalMem);
	
 	
	TileImage.Load( GBAGraphics::VRAM->fGbuf,1024);
	ImgBuf.Create(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*16,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16);
	ImgBuf.SetPalette(GBAGraphics::VRAM->PcPalMem);
	
	  if(RoomHeader.lBg3&&0x40 || RoomHeader.lBg3&& 0xF0 )	DrawBackGround(&ImgBuf); 
	
	//Do one nice loop
	if(RoomHeader.bBg2 == 0x40){//LZ 
		//this type uses the background set for tiles.
		fseek(GBA.ROM,RoomHeader.lBackLayer-0x8000000,SEEK_SET);
		fread(&cSize,1,1,GBA.ROM);
		if(cSize ==0){
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X  = 32; 
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y = 32;
		}else if((cSize == 1)||(cSize >= 3)) {
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X  = 64;
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y = 32;
		}else if(cSize == 2){
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->X  = 32;
			MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Y = 64;
		}
		
	}
   ImgBuf.Blit(ThisBackBuffer.DC(),0,0,Width,Height,0,0);
 
	if(GlobalVars::gblVars->CheckBoxes[ViewB].value()){
		if(MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Dirty){
			DrawLayer(MapManager::mgrMaps->GetLayer(MapManager::Backlayer), RoomHeader.bBg2);//Backlayer
		    MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->Dirty=0;
		} 
    	MapManager::mgrMaps->GetLayer(MapManager::Backlayer)->BImage.TransBlit(ThisBackBuffer.DC(),0,0,Width,Height,0,0);
	}
	
	if(GlobalVars::gblVars->CheckBoxes[ViewL].value()){
		if(MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Dirty){	
			DrawLayer(MapManager::mgrMaps->GetLayer(MapManager::LevelData), RoomHeader.bBg1);//Level
			MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Dirty=0;
			
		}
	MapManager::mgrMaps->GetLayer(MapManager::LevelData)->BImage.TransBlit(ThisBackBuffer.DC(),0,0,Width,Height,0,0);
	}
	
				
				
	
	if(RoomHeader.bBg0 == 0x40){//LZ 
		if(cSize ==0){
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X = 32; 
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y = 32;
		}else if((cSize == 1)||(cSize >= 3)) {
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X= 64;
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y  = 32;
		}else if(cSize == 2){
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->X = 32;
			MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Y  = 64;
		}
	}
	if(GlobalVars::gblVars->CheckBoxes[ViewF].value()){
		if(MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Dirty){
		  DrawLayer(MapManager::mgrMaps->GetLayer(MapManager::ForeGround), RoomHeader.bBg0);//Backlayer
		  MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->Dirty=0;
	   	}
        if(chkDoTrans.value() == 1)
		 MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.AlphaBlit(ThisBackBuffer.DC(),0,0,Width,Height,0,0,RoomHeader.TransForeground );
	    else
		 MapManager::mgrMaps->GetLayer(MapManager::ForeGround)->BImage.TransBlit(ThisBackBuffer.DC(),0,0,Width,Height,0,0);
	    
	}
	
	  
	
	
	
	
	GlobalVars::gblVars->img[1].TransBlit(ThisBackBuffer.DC(),0,0,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X*16,MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16,0,0);
	ShowSprites((unsigned char)Combos[cSpriteSet].GetListIndex());
	DisplayDoors();
	if(ViewClip.value()   ==1)DrawClipIdent();
	if(GlobalVars::gblVars->ScrollCheck.value()==1)DrawScrolls  ();
	
	InvalidateRect(hwndMap,0,1);
     
	//SendMessage(hwndMap,WM_SIZE,1,0);

	return 0;
}*/
/*int SaveSprites(){
	int i =0;
fseek(GBA.ROM,RoomHeader.lSpritePointer - 0x8000000,SEEK_SET);
if(Enemy1.max){
	for(i = 0;i<Enemy1.max;i++){
		fwrite(&Enemy1.Enemies[i].Y,1,1,GBA.ROM);
		fwrite(&Enemy1.Enemies[i].X,1,1,GBA.ROM);
		fwrite(&Enemy1.Enemies[i].Creature,1,1,GBA.ROM);
	}
	fputc(0xFF,GBA.ROM );
	fputc(0xFF,GBA.ROM );
	fputc(0xFF,GBA.ROM );

}
fseek(GBA.ROM,RoomHeader.lSpritePointer2 - 0x8000000,SEEK_SET);
if(Enemy2.max){
	for(i = 0;i<Enemy2.max;i++){
		fwrite(&Enemy2.Enemies[i].Y,1,1,GBA.ROM);
		fwrite(&Enemy2.Enemies[i].X,1,1,GBA.ROM);
		fwrite(&Enemy2.Enemies[i].Creature,1,1,GBA.ROM);
	}
fputc(0xFF,GBA.ROM );
fputc(0xFF,GBA.ROM );
fputc(0xFF,GBA.ROM );

}
fseek(GBA.ROM,RoomHeader.lSpritePointer3 - 0x8000000,SEEK_SET);
if(Enemy3.max){
	for(i = 0;i<Enemy3.max;i++){
		fwrite(&Enemy3.Enemies[i].Y,1,1,GBA.ROM);
		fwrite(&Enemy3.Enemies[i].X,1,1,GBA.ROM);
		fwrite(&Enemy3.Enemies[i].Creature,1,1,GBA.ROM);
	}
fputc(0xFF,GBA.ROM );
fputc(0xFF,GBA.ROM );
fputc(0xFF,GBA.ROM );


}
fclose(GBA.ROM);
GBA.ROM = fopen(GBA.FileLoc,"r+b");


	return 0;
}*/
/*int             EditThisLayer(nMapBuffer * Layer, LPARAM lParam, byte usens, unsigned short ToUse, int mul)
{ // Layer to use, XY value, if it's bigger then 16x16

   int             baseX = (GetX(lParam) / mul);

   baseX += (mul == 16 ? nHScroll[sHMap] : 0);
   int             baseY = (GetY(lParam) / mul);

   baseY += (mul == 16 ? nVScroll[sVMap] : 0);
   int             y = 0;
   int             x = 0;
   int             ny = 0;
   int             nx = 0;
   int             add = 0;
   int             width = Layer->X;

   if (mul == 8)
   { // Editing scenery
      // ns == switch between tileset and map copy?

      if (SCS == 0)
      { // Tilecopy

         for (y = 0; y < mpSTS.Height; y++)
         {
            for (x = 0; x < mpSTS.Width; x++)
            {
               add = 0;
               // Make sure it's in bounds
               if ((baseX + x) >= 31 && !(add & 0x3E0))
                  add += 0x3E0;
               // if(baseY>31) add+=1024;
               Layer->TileBuf2D[(baseX + x) + (baseY + y) * (width / 2) + add] = ToUse + (x) + (y) * (width / 2);

            }
         }
      } else if (SCS == 1)
      { // Map Copy

         for (y = 0; y < mpSTS.Height; y++)
         {
            for (x = 0; x < mpSTS.Width; x++)
            {
               add = 0;
               // Make sure it's in bounds
               if (baseX >= 31 || x >= 31 && !(add & 0x3E0))
                  add += 992;
               // if(baseY>31) add+=1024;
               Layer->TileBuf2D[(baseX + x) + (baseY + y) * (width / 2) + add] = Layer->TileBuf2D[ToUse + (x) + (y) * (width / 2)];

            }
         }
      }
		Layer->Dirty=Layer->SDirty=1; 
      return 0;
   }
   if (!MPToUse && GlobalVars::gblVars->CheckBoxes[Clip].value() == 0)
   { // Map

      for (y = 0; y < mpMap.Height; y++)
      {
         for (x = 0; x < mpMap.Width; x++)
         {
            if (MP[0] && (mpMap.Width >= 0 && mpMap.Height >= 0))
            {
               // Make sure it's in bounds

               Layer->TileBuf2D[(baseX + x) + (baseY + y) * (width)] = Layer->TileBuf2D[Tile + (x) + (y) * (width)];

            }
         }
      }
		Layer->Dirty=Layer->SDirty=1; 
      return 0;
   }
   if (GlobalVars::gblVars->CheckBoxes[Clip].value() == 1)
   {
      for (y = 0; y < mpMap.Height; y++)
      {
         for (x = 0; x < mpMap.Width; x++)
         {

            // Make sure it's in bounds

            Layer->TileBuf2D[(baseX + x) + (baseY + y) * (width)] = ToUse + x + (y * 16);

         }
      }
		Layer->Dirty=Layer->SDirty=1; 
      return 0;

   }
   if (MPToUse)
   { // Tileset 

      for (y = 0; y < mpTileset.Height; y++)
      {
         for (x = 0; x < mpTileset.Width; x++)
         {
            if ((((baseX + x) + (baseY + y) * (width)) <= (Layer->X * Layer->Y)))
            {
               Layer->TileBuf2D[(baseX + x) + (baseY + y) * (width)] = ToUse + x + (y * 16);
            } else
            {
               MessageBox(0, "Tile is out of bounds", "Error", MB_OK);
               return 0;
            }
         }
      }
   }
   Layer->Dirty=Layer->SDirty=1; 
   return 0;
}
*/
/*long            GBAMethods::ZMUncompRle(unsigned short arg0, unsigned char * arg1, unsigned char * arg2)
{

   unsigned char             *src = (unsigned char *) arg1, *dst = (unsigned char *) arg2;

   unsigned long             jj, ii, nn;
   unsigned long             ctrl, type;

   type = 0;

   // DMAFill(3, 0, arg2, nn, 16);

   for (ii = 0; ii < 2; ii++)
   {
      dst = (unsigned char *) & arg2[ii];
      if (*src++ == 1) // 8 control field

      {
         while (1)
         {
            ctrl = *src++;
            if (ctrl == 0)
               break;

            else if (ctrl & 0x80)
            {
               nn = ctrl & 0x7F;
               if (*src){
				   for (jj = 0; jj < nn; jj++){

                     dst[2 * jj] = *src;
				   }
			   }
               src++;
            } else
            {
               nn = ctrl;
               for (jj = 0; jj < nn; jj++)
                  dst[2 * jj] = src[jj];
               src += nn;
            }

            dst += 2 * nn;
         }
      } else // RLE16

      {
         while (1)
         {
            ctrl = src[0] << 8 | src[1];
            src += 2;
            if (ctrl == 0)
               break;

            if (ctrl & 0x8000)
            {
               nn = ctrl & 0x7FFF;
               if (*src)
                  for (jj = 0; jj < nn; jj++)
                     dst[2 * jj] = *src;
               src++;
            } else
            {
               nn = ctrl;
               for (jj = 0; jj < nn; jj++)
                  dst[2 * jj] = src[jj];
               src += nn;
            }
            dst += 2 * nn;
         }
      }
   }

   return type;
}*/
/*int DrawLayer(nMapBuffer* Map, unsigned char ctype){//img is the image to blit to 
int thisX=0,thisY=0,mX=0,mY=0;
unsigned short TILE = 0;
int i =0;
	//Map->BImage.Create(Map->X*16,Map->Y*16);
	//Map->BImage.SetPalette(GBAGraphics::VRAM->PcPalMem);
Map->BImage.Clear();
if(ctype==0) return 0;
if(ctype == 0x40){//LZ 
		//this type uses the background set for tiles.
		
		for(int ScenRep = 0;ScenRep<MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X/8;ScenRep++){
			for(thisX=0;thisX<Map->X;thisX++){
				for(thisY=0;thisY<Map->Y;thisY++){
					Map->BImage.Draw(BGImage, (ScenRep * (Map->X*8))+(thisX)* 8,(RoomHeader.bSceneryYPos-1)*16 + (thisY) * 8, Map->TileBuf2D[(thisX)+(thisY*Map->X )]);
				}
			}
		}
	
}else if(ctype == 0x10){

	for(thisY = 0; thisY<Map->Y;thisY++){
		for(thisX = 0; thisX < Map->X;thisX++){// from here if something is enabled then draw it 
			//BG layer
		
				
				//RLE
					
					TILE =(Map->TileBuf2D[thisX+(thisY * (Map->X))])<<2;
					Map->BImage.Draw(TileImage,thisX* 16, (thisY) * 16, TSA.nTSA[TILE++]);
					Map->BImage.Draw(TileImage,(thisX * 16) + 8, (thisY) * 16, TSA.nTSA[TILE++]);
					Map->BImage.Draw(TileImage,(thisX * 16), ((thisY) * 16)+8, TSA.nTSA[TILE++]);
					Map->BImage.Draw(TileImage,(thisX * 16)+8, ((thisY) * 16)+8, TSA.nTSA[TILE]);
					
			
			}
		}
	}



	return 0;
}

*/
/*
int cTED::EncodeText(unsigned long offset){
    
	int i=0;
	int ii=0;//used for looking inside strings
	int iii=0;
	int itemcount=0;
	int strcounter=0;
	char buf[256];
	char stupidstring[1024];
	unsigned short nb[1024]={0};

	GetWindowText(GetDlgItem(tMain,txtString),stupidstring,1024);
	GetWindowText(GetDlgItem(tMain,txtString),curtext,strlen(stupidstring));
	int a = strlen(curtext);

	for(i=0; i<strlen(curtext);i++){
		memset(&buf,0,256);
		if((curtext[i]=='[')){
			
			for(ii=0;ii<strlen(curtext)-i;ii++){
				buf[ii]=curtext[i+ii];
				if((curtext[i+ii]==']')) break;
			}
			
			i+=ii; 
			
		}else{
			buf[0]=curtext[i];
		}

		//Do a look up to match
		for(ii=0;ii<0xFFFF;ii++){
			//compare curtext with a entry from the table then put ii into the nb stream 
			//and increase count
			if(strcmp(table[ii],buf)==0){
				//They're identical so put it into stream and break; 
				nb[strcounter]=ii;
				strcounter++;
				break;
			}
		}

	}
	memcpy(binarystring,nb,strcounter*2);
	if(strcounter> txtlength){
		offset= GBA.FindFreeSpace(strcounter*2+16,0xFF);
		if(crf ==0){
		 ZMOffsets[cboTxt.GetListIndex()][GlobalVars::gblVars->TextEditor.cOffsets.GetListIndex()]=offset+0x8000000;
		}else if(crf==1){
		   Offsets[GlobalVars::gblVars->TextEditor.cOffsets.GetListIndex()]=offset+0x8000000;
		}
		if(	nb[strcounter]!=0xFF00) nb[strcounter]=0xFF00;
		
		fwrite(nb,2,strcounter,GBA.ROM);
		if(crf==0){
			if(GlobalVars::gblVars->TextEditor.cboTxt.GetListIndex()==0){
			  fseek(GBA.ROM,0x760914,SEEK_SET);
			    fwrite(&ZMOffsets[GlobalVars::gblVars->TextEditor.cOffsets.GetListIndex()],4,99,GBA.ROM);
			}else if(GlobalVars::gblVars->TextEditor.cboTxt.GetListIndex()==1){
              fseek(GBA.ROM,0x760660,SEEK_SET);
				for(i=0;i<65;i++){
		    	fwrite(&ZMOffsets[1][i],4,1,GBA.ROM);
				}
		

			}
	fseek(GBA.ROM,offset,SEEK_SET);
			
				if(	nb[strcounter]!=0xFF00) nb[strcounter]=0xFF00;
		    fwrite(nb,2,strcounter,GBA.ROM);
			
		}else if(crf==1){

			fseek(GBA.ROM,0x79EA08,SEEK_SET);
			fwrite(&Offsets,4,99,GBA.ROM);
		}
	}else{ 
			fseek(GBA.ROM,offset,SEEK_SET);
			
				if(	nb[strcounter]!=0xFF00) nb[strcounter]=0xFF00;
		    fwrite(nb,2,strcounter,GBA.ROM);
			
	}
	// fclose(fp);

	    iii=cOffsets.GetListIndex(); 
		ResetList();
		cOffsets.SetListIndex(iii); 
		
		SendMessage(tMain,WM_COMMAND,LOWORD(lstOffsets),0);
	return 0;
}
*//*
int LoadEnemies(){
//First we dim our need varibles
nEnemies tmparray[256];
unsigned char loopcounter=0;
unsigned char max=0;
int i=0,j=0;
int EnemySet=0;
long curroffset=0;
unsigned long offsetlist[3]={RoomHeader.lSpritePointer,RoomHeader.lSpritePointer2,RoomHeader.lSpritePointer3};
loopcounter = 0;
//Code for set 1
 

	Enemy1.max = 0;
	Enemy2.max = 0;
	Enemy3.max = 0;
	for(i =0; i< 0x100;i++){
	 Enemy1.Enemies[i].X=0;
	 Enemy1.Enemies[i].Y=0;
	 Enemy1.Enemies[i].Creature=0x10;
	 
	 Enemy2.Enemies[i].X=0;
	 Enemy2.Enemies[i].Y=0;
	 Enemy2.Enemies[i].Creature=0x10;
	 
	 Enemy3.Enemies[i].X=0;
     Enemy3.Enemies[i].Y=0;
	 Enemy3.Enemies[i].Creature=0x10;
	}
    for(i =0;i<3;i++){
		fseek(GBA.ROM,offsetlist[i]-0x8000000,SEEK_SET);
	
        max=0;
		for(j=0;j<256;j++) {
		   
			tmparray[j].Y=fgetc(GBA.ROM);
            tmparray[j].X=fgetc(GBA.ROM);
			tmparray[j].Creature=fgetc(GBA.ROM);
            if((tmparray[j].Creature == 0xFF)||(tmparray[j].X == 0xFF) || (tmparray[j].Y == 0xFF)) {
				break;
			}
            max++;
	   
		}
       
        switch(i){
			case 0:
				 memcpy(Enemy1.Enemies,tmparray,max*3);
				 Enemy1.max=max;  
			break;
        	case 1:
				 memcpy(Enemy2.Enemies,tmparray,max*3);
				 Enemy2.max=max;  
			break;
         
        	case 2:
				 memcpy(Enemy3.Enemies,tmparray,max*3);
				 Enemy3.max=max;  
			break;

		}

		
       



	}

  


   

			return 0;
}
*//*
int DrawScrolls(int ScrollToDraw){
int i; 
RECT blah; 



if(scroll.Room != 0xFF) {


		for(i = 0; i < scroll.Number+2;i++){
			

			
			if ((scroll.Scrolls[i].xStart != 0xFF) && (scroll.Scrolls[i].yEnd != 0xFF)){
   

				  
				
				  
			
				
     
                     HBRUSH curbrush = CreateSolidBrush(
														 RGB( 
														  255-((i+rand())<<2), 
														  255-((i+2+rand())<<4), 
														  255-((i+1+rand())<<8)
														 )
														 );
																
				
						for(int d=0;d<2;d++){
								blah.left = (scroll.Scrolls[i].xStart) * 16;+d;
								blah.top = (scroll.Scrolls[i].yStart)*16+d;
								blah.right=(scroll.Scrolls[i].xEnd) * 16+d;
								blah.bottom=(scroll.Scrolls[i].yEnd ) *16+d;
								FrameRect(ThisBackBuffer.DC(), &blah,curbrush);
							
						}
						DeleteObject(curbrush);

				

			}

		}

}
	return 0;
}//End Function
*//*
int cTED::LoadTable(){
int i=0;
int ii=0;
char mybuf[256];
memset(&table,0x0,sizeof(table));
for(i=0;i<0xFF00;i++)table[i][0]=0xFF;
strcpy(table[0x0040]," "); 
strcpy(table[0x0041],"!");
strcpy(table[0x0042],"\"");
strcpy(table[0x0043],"#");
strcpy(table[0x0044],"$");
strcpy(table[0x0045],"%");
strcpy(table[0x0046],"&");
strcpy(table[0x0047],"'");
strcpy(table[0x0048],"(");
strcpy(table[0x0049],")");
strcpy(table[0x004A],"*");
strcpy(table[0x004B],"+");
strcpy(table[0x004C],",");
strcpy(table[0x004D],"-");
strcpy(table[0x004E],".");
strcpy(table[0x004F],"/");
strcpy(table[0x0050],"0");
strcpy(table[0x0051],"1");
strcpy(table[0x0052],"2");
strcpy(table[0x0053],"3");
strcpy(table[0x0054],"4");
strcpy(table[0x0055],"5");
strcpy(table[0x0056],"6");
strcpy(table[0x0057],"7");
strcpy(table[0x0058],"8");
strcpy(table[0x0059],"9");
strcpy(table[0x005A],":");
strcpy(table[0x005B],";");
strcpy(table[0x005D],"=");
strcpy(table[0x005E],"1");
strcpy(table[0x005F],"?");
strcpy(table[0x0081],"A");
strcpy(table[0x0082],"B");
strcpy(table[0x0083],"C");
strcpy(table[0x0084],"D");
strcpy(table[0x0085],"E");
strcpy(table[0x0086],"F");
strcpy(table[0x0087],"G");
strcpy(table[0x0088],"H");
strcpy(table[0x0089],"I");
strcpy(table[0x008A],"J");
strcpy(table[0x008B],"K");
strcpy(table[0x008C],"L");
strcpy(table[0x008D],"M");
strcpy(table[0x008E],"N");
strcpy(table[0x008F],"O");
strcpy(table[0x0090],"P");
strcpy(table[0x0091],"Q");
strcpy(table[0x0092],"R");
strcpy(table[0x0093],"S");
strcpy(table[0x0094],"T");
strcpy(table[0x0095],"U");
strcpy(table[0x0096],"V");
strcpy(table[0x0097],"W");
strcpy(table[0x0098],"X");
strcpy(table[0x0099],"Y");
strcpy(table[0x009A],"Z");
strcpy(table[0x009B],"[");
strcpy(table[0x00C1],"a");
strcpy(table[0x00C2],"b");
strcpy(table[0x00C3],"c");
strcpy(table[0x00C4],"d");
strcpy(table[0x00C5],"e");
strcpy(table[0x00C6],"f");
strcpy(table[0x00C7],"g");
strcpy(table[0x00C8],"h");
strcpy(table[0x00C9],"i");
strcpy(table[0x00CA],"j");
strcpy(table[0x00CB],"k");
strcpy(table[0x00CC],"l");
strcpy(table[0x00CD],"m");
strcpy(table[0x00CE],"n");
strcpy(table[0x00CF],"o");
strcpy(table[0x00D0],"p");
strcpy(table[0x00D1],"q");
strcpy(table[0x00D2],"r");
strcpy(table[0x00D3],"s");
strcpy(table[0x00D4],"t");
strcpy(table[0x00D5],"u");
strcpy(table[0x00D6],"v");
strcpy(table[0x00D7],"w");
strcpy(table[0x00D8],"x");
strcpy(table[0x00D9],"y");
strcpy(table[0x00DA],"z");
strcpy(table[0x4601],"À");
strcpy(table[0x4701],"Á");
strcpy(table[0x4801],"Â");
strcpy(table[0x4901],"Ã");
strcpy(table[0x4A01],"Ç");
strcpy(table[0x4B01],"É");
strcpy(table[0x4C01],"Ê");
strcpy(table[0x4D01],"Í");
strcpy(table[0x4E01],"Ó");
strcpy(table[0x4F01],"Ô");
strcpy(table[0x5001],"Õ");
strcpy(table[0x5101],"Ú");
strcpy(table[0x5201],"Ü");
strcpy(table[0x5301],"à");
strcpy(table[0x5401],"á");
strcpy(table[0x5501],"â");
strcpy(table[0x5601],"ã");
strcpy(table[0x5701],"ç");
strcpy(table[0x5801],"é");
strcpy(table[0x5901],"ê");
strcpy(table[0x5A01],"í");
strcpy(table[0x5B01],"ó");
strcpy(table[0x5C01],"ô");
strcpy(table[0x5D01],"õ");
strcpy(table[0x5E01],"ú");
strcpy(table[0x5F01],"ü");
strcpy(table[0x41F],"?");
strcpy(table[0x02C8],"[Select]");
strcpy(table[0x02D2],"[LLSquare]");
strcpy(table[0x0300],"[UpArrow]");
strcpy(table[0x0302],"[DownArrow]");
strcpy(table[0x0304],"[LeftArrow]");
strcpy(table[0x0306],"[RightArrow]");
strcpy(table[0x0308],"[AButton]");
strcpy(table[0x030A],"[BButton]");
strcpy(table[0x030B],"[LButton]");
strcpy(table[0x030D],"[RButton]");
strcpy(table[0x8100],"[Color0]");
strcpy(table[0x8101],"[Color1]");
strcpy(table[0x8102],"[Color2]");
strcpy(table[0x8103],"[Color3]");
strcpy(table[0x8104],"[Color4]");
strcpy(table[0x8105],"[Color5]");
strcpy(table[0x8106],"[Color6]");
strcpy(table[0x8107],"[Color7]");
strcpy(table[0x8108],"[Color8]");
strcpy(table[0x8109],"[Color9]");
strcpy(table[0x810A],"[Color10]");
strcpy(table[0x810B],"[Color11]");
strcpy(table[0xFB00],"[FB]");
strcpy(table[0xFC00],"[NEXTTEXT]");
strcpy(table[0xFD00],"[NEWPAGE]");
strcpy(table[0xFE00],"[NEWLINE]");
strcpy(table[0xFF00],"[END]");
for(i =0;i<0xFF00;i++){
	if((table[i][0]==(char)0xFF)){
	
	
			table[i][0]=0;
		
		sprintf(mybuf,"[RAW 0x%X]",i);
		memcpy(&table[i],mybuf,strlen(mybuf));

	}
}

return 0;
}
*/

/*
LRESULT CALLBACK MapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int poo;
	int i=0;
	int dw,dh,x,y,nx,ny;
	dw =0;
	dh = 0;
	x = 0;
	y=0;
	nx=0;
	ny=0;
	RECT que;

	char cBuf[200];
		int td=0;
	switch (message) 
	{

		 
		
	case WM_PAINT:
		 /*HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = SelectObject(hdcMem, g_hbmBall);

        GetObject(g_hbmBall, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);*//*


		
		
		hdc = BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd,&que);
	    //MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer)->BImage.Blit(BB.DC(),0,0,que.right-que.left,que.bottom-que.top,0,0);
        //TransparentBlt(
		//BB.DC(), 0,0,que.right-que.left,que.bottom-que.top,
		//ThisBackBuffer.DC(),(nHScroll[sHMap]-(nHScroll[sHMap] ? 1:0))*16,(nVScroll[sVMap]-(nVScroll[sHMap] ? 1:0))*16,que.right-que.left,que.bottom-que.top, 0);
		
		//GlobalVars::gblVars->img[iRoom].Blit(hdc, 0,0,que.right-que.left,que.bottom-que.top,nHScroll[sHMap]*16,nVScroll[sVMap]*16);
	
		BitBlt(hdc, 0,0,que.right-que.left,que.bottom-que.top,ThisBackBuffer.DC(),nHScroll[sHMap]*16,nVScroll[sVMap]*16,SRCCOPY);
		//BitBlt(hdc, 0,0,que.right-que.left,que.bottom-que.top,BB.DC(),0,0,SRCCOPY);
		
		 DrawTileRect(hdc,mpMap);
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll
		UpdateScroll(hWnd,wParam,1,vsbMap,sVMap);
		// DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
		break;
	case WM_HSCROLL:
		
		UpdateScroll(hWnd,wParam,NULL,hsbMap,sHMap);
		// DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
		InvalidateRect(hWnd,0,1);
		break;
	case WM_LBUTTONDOWN:
		/*	if(wParam == (MK_LBUTTON| MK_CONTROL)){
				ClipBoard.CopyTiles(lParam);
				 DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
				//InvalidateRect(hwndMap,0,1);
				return 0;
			}*//*
		
		if(MP[1]==0){
			HandleMapLeftButton(message,wParam,lParam);
            return 0;
	}
		break;
	case WM_RBUTTONDOWN:
		if(LOWORD(wParam) == MK_RBUTTON){
			HandleRightClick(hWnd,  message,  wParam, lParam);
		}
		break;
	case WM_RBUTTONUP:
	if(GlobalVars::gblVars->ScrollCheck.value()==1){
			   //Checks where the coords are 
		if(MovingScroll[0]==1){
			if(((GetX(lParam)/16)+ nHScroll[sHMap])<(scroll.Scrolls[MovingScroll[1]].xStart) || 
		   ((GetY(lParam)/16)+ nVScroll[sVMap])<(scroll.Scrolls[MovingScroll[1]].yStart)){
			   MessageBox(hWnd,"Box was too small resetting box to old proptions.","Error",MB_OK);
			   MovingScroll[0]=0;
			   MovingScroll[1]=0;
			   return 0;
			}else{
                 scroll.Scrolls[MovingScroll[1]].xEnd=(GetX(lParam)/16)+1+nHScroll[sHMap]; 
				 scroll.Scrolls[MovingScroll[1]].yEnd=(GetY(lParam)/16)+1+nVScroll[sVMap];
				 MovingScroll[0]=0;
				 MovingScroll[1]=0;
				  DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
		   }
		}	
	}else if((MP[0]==1)&&(MP[1]==1)){//MP[0]=1;//Has been selected
			                   //MP[1]=1;//In use 	
		MP[1]=0;
	}else if(mpMap.Width ==1 && mpMap.Height ==1){

        MP[0]=MP[1]=0;
	}
		 
		
	
		break;
	case WM_LBUTTONUP:
		if(MovingSprite[0]){
		 	
			switch((unsigned char)Combos[cSpriteSet].GetListIndex()){
			case 0:
				Enemy1.Enemies[MovingSprite[1]].X=(GetX(lParam)/16)+nHScroll[sHMap];
				Enemy1.Enemies[MovingSprite[1]].Y=(GetY(lParam)/16)+nVScroll[sVMap];
			break;
			case 1:
				Enemy2.Enemies[MovingSprite[1]].X=(GetX(lParam)/16)+nHScroll[sHMap];
				Enemy2.Enemies[MovingSprite[1]].Y=(GetY(lParam)/16)+nVScroll[sVMap];
			break;
			case 2:
				Enemy3.Enemies[MovingSprite[1]].X=(GetX(lParam)/16)+nHScroll[sHMap];
				Enemy3.Enemies[MovingSprite[1]].Y=(GetY(lParam)/16)+nVScroll[sVMap];
			break;

			}
				MovingSprite[0]=0;
				MovingSprite[1]=0;
				 DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
				return 0;
				
			
		}
			if(MovingDoor[0]==1){
		 	
		
			
			
				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].DHeight-=	Doors[DoorBuf.DoorNotes[MovingDoor[1]]].YEntrance;
				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].DHeight+=	(GetY(lParam)/16)+nVScroll[sVMap];

				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].DWidth-=     Doors[DoorBuf.DoorNotes[MovingDoor[1]]].XEntrance; 
				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].DWidth+=	 (GetX(lParam)/16)+nHScroll[sHMap];//-Doors[DoorBuf.DoorNotes[i]].YEntrance)+1;;
				
				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].XEntrance=   (GetX(lParam)/16)+nHScroll[sHMap];
				Doors[DoorBuf.DoorNotes[MovingDoor[1]]].YEntrance=   (GetY(lParam)/16)+nVScroll[sVMap];
				
				MovingDoor[0]=0;
				MovingDoor[1]=0;
				 DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
				return 0;
			}
			if(GlobalVars::gblVars->ScrollCheck.value()==1){
				if(MovingScroll[0]==1){	
               
				scroll.Scrolls[MovingScroll[1]].xEnd-=scroll.Scrolls[MovingScroll[1]].xStart;
				scroll.Scrolls[MovingScroll[1]].xEnd+=(GetX(lParam)/16)+nHScroll[sHMap];
				scroll.Scrolls[MovingScroll[1]].yEnd-=scroll.Scrolls[MovingScroll[1]].yStart;
				scroll.Scrolls[MovingScroll[1]].yEnd+=(GetY(lParam)/16)+nVScroll[sVMap];
				scroll.Scrolls[MovingScroll[1]].xStart=(GetX(lParam)/16)+nHScroll[sHMap];
				scroll.Scrolls[MovingScroll[1]].yStart=(GetY(lParam)/16)+nVScroll[sVMap];
			
				MovingScroll[0]=0;
				MovingScroll[1]=0;
				 DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
				}
				return 0;
			}
		break;
	case WM_MOUSEMOVE:
		
	
		//Resize code 
		if(MP[1] ==0){
			
			
			mpMap.sX = GetX(lParam)/16;
			mpMap.sY = GetY(lParam)/16;
			if(mpMap.sX <0) mpMap.sX = 0;
			if(mpMap.sX>=MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X)				mpMap.sX = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X-1;
			if(mpMap.sY <0) mpMap.sY = 0;
			if(mpMap.sY>=MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y) mpMap.sY = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y-1;
		}else if(MP[1] == 1){
			mpMap.nX = GetX(lParam)/16;
			mpMap.nY = GetY(lParam)/16;
			//Determine direction for length and width;
			if(mpMap.nX>mpMap.cX){//increase width;
				mpMap.Width+=1;
			}else if(mpMap.nX<mpMap.cX){
				mpMap.Width-=1;
			}
			if(mpMap.nY>mpMap.cY){//increase width;
				mpMap.Height+=1;
			}else if(mpMap.nY<mpMap.cY){
				mpMap.Height-=1;
			}
			mpMap.cX = GetX(lParam)/16;
			mpMap.cY = GetY(lParam)/16;
			if(mpMap.Height<1) mpMap.Height =1;
			if(mpMap.Width<1)mpMap.Width = 1;
		//	if(mpMap.Width>16)mpMap.Width = 16;
		//	if(mpMap.Height>16)mpMap.Height = 16;
			//check for out of bounds 
			if(mpMap.nX <0) mpMap.nX = 0;
			if(mpMap.nX>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X) 
				mpMap.nX = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X;
			if(mpMap.nY <0) mpMap.nY = 0;
			if(mpMap.nY>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y) mpMap.nY = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y;
		}
		poo =	Combos[cClip].GetListIndex();
		Combos[cClip].SetListIndex(MapManager::mgrMaps->GetLayer(MapManager::Clipdata)->TileBuf2D[((GetX(lParam)/16)+ nHScroll[sHMap]) +  ((GetY(lParam)/16)+ nVScroll[sVMap])* (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X) ]);
		
		GetWindowText(Combos[cClip].thishwnd(),cBuf,200);
		SetWindowText(GetDlgItem(Main,lblClip),cBuf);
		Combos[cClip].SetListIndex(poo);
		sprintf(cBuf,"X: %X",((GetX(lParam)/16)+ nHScroll[sHMap]));
		SetWindowText(GetDlgItem(Main,lblX),cBuf);
		sprintf(cBuf,"Y: %X",((GetY(lParam)/16)+ nVScroll[sVMap]));
		SetWindowText(GetDlgItem(Main,lblY),cBuf);
     	if(wParam == MK_LBUTTON) SendMessage(hWnd, 0x201, wParam, lParam);
	
		InvalidateRect(hwndMap,0,0);
		
		break;
	case WM_SIZE:
		//Gotta Make sure the size isn't out of bounds.
		if(lParam!=0){
			GetWindowRect(hWnd,&que);
			que.right = que.right - que.left;
			que.bottom = que.bottom - que.top;
			if((que.right/16)>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X) que.right = (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X * 16);
			if((que.bottom/16)>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y) que.bottom = (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y*16)+20;
			MoveWindow(hWnd,que.left,que.top,que.right,que.bottom,1);
			nMaxHScroll[sHMap] = (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X - (que.right)/16);//MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X/16; // maximum H scroll
			nMaxVScroll[sVMap] = (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y - (que.bottom)/16 );
			UpdateScroll(hWnd,wParam,1,vsbMap,sVMap);
			
			UpdateScroll(hWnd,wParam,NULL,hsbMap,sHMap);
		}
		
		
		break;
	case WM_DESTROY:
		
		break;
		

		
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
*/
	/*case WM_MOUSEMOVE:
		if(wParam & (MK_LBUTTON))
	
		//Resize code 
		if(MP[1] ==0){
			
			
			mpMap.sX = GetX(lParam)/16;
			mpMap.sY = GetY(lParam)/16;
			if(mpMap.sX <0) mpMap.sX = 0;
			if(mpMap.sX>=MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X)				mpMap.sX = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X-1;
			if(mpMap.sY <0) mpMap.sY = 0;
			if(mpMap.sY>=MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y) mpMap.sY = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y-1;
		}else if(MP[1] == 1){
			mpMap.nX = GetX(lParam)/16;
			mpMap.nY = GetY(lParam)/16;
			//Determine direction for length and width;
			if(mpMap.nX>mpMap.cX){//increase width;
				mpMap.Width+=1;
			}else if(mpMap.nX<mpMap.cX){
				mpMap.Width-=1;
			}
			if(mpMap.nY>mpMap.cY){//increase width;
				mpMap.Height+=1;
			}else if(mpMap.nY<mpMap.cY){
				mpMap.Height-=1;
			}
			mpMap.cX = GetX(lParam)/16;
			mpMap.cY = GetY(lParam)/16;
			if(mpMap.Height<1) mpMap.Height =1;
			if(mpMap.Width<1)mpMap.Width = 1;
		//	if(mpMap.Width>16)mpMap.Width = 16;
		//	if(mpMap.Height>16)mpMap.Height = 16;
			//check for out of bounds 
			if(mpMap.nX <0) mpMap.nX = 0;
			if(mpMap.nX>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X) 
				mpMap.nX = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X;
			if(mpMap.nY <0) mpMap.nY = 0;
			if(mpMap.nY>MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y) mpMap.nY = MapManager::mgrMaps->GetLayer(MapManager::LevelData)->Y;
		}
		poo =	Combos[cClip].GetListIndex();
		Combos[cClip].SetListIndex(MapManager::mgrMaps->GetLayer(MapManager::Clipdata)->TileBuf2D[((GetX(lParam)/16)+ nHScroll[sHMap]) +  ((GetY(lParam)/16)+ nVScroll[sVMap])* (MapManager::mgrMaps->GetLayer(MapManager::LevelData)->X) ]);
		
		GetWindowText(Combos[cClip].thishwnd(),cBuf,200);
		SetWindowText(GetDlgItem(Main,lblClip),cBuf);
		Combos[cClip].SetListIndex(poo);
		sprintf(cBuf,"X: %X",((GetX(lParam)/16)+ nHScroll[sHMap]));
		SetWindowText(GetDlgItem(Main,lblX),cBuf);
		sprintf(cBuf,"Y: %X",((GetY(lParam)/16)+ nVScroll[sVMap]));
		SetWindowText(GetDlgItem(Main,lblY),cBuf);
     	if(wParam == MK_LBUTTON) SendMessage(hWnd, 0x201, wParam, lParam);
	
		InvalidateRect(hwndMap,0,0);
		
		break;*/