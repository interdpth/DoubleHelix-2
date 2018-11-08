#include "MainHeader.h"
#include "TilesetManager.h"
#include "SpriteObjectManager.h"
struct layerhdr
{
   unsigned char              X;
   unsigned char              Y;
   unsigned short            *Layer;
};

struct exHeader
{
   RHeader         hdr;
   layerhdr        lhFore;
   layerhdr        lhLevel;
   layerhdr        lhBackLayer;
   layerhdr        lhBackground;
   nEnemyList      Sprite1;
   nEnemyList      Sprite2;
   nEnemyList      Sprite3;

};
int             ClearHeaderOffsets(RHeader * hdr)
{

   hdr->lForeground = 0;
   hdr->lBackgroundTSA = 0;
   hdr->lBackLayer = 0;
   hdr->lClipData = 0;
   hdr->lLevelData = 0;
   hdr->lSpritePointer3 = hdr->lSpritePointer2 = hdr->lSpritePointer = 0;
   return 0;

}
int             FillLayerInfo(layerhdr * dstLayer, nMapBuffer * srcLayer)
{
   dstLayer->X = srcLayer->X;
   dstLayer->Y = srcLayer->Y;
   dstLayer->Layer = srcLayer->TileBuf2D;

   return 0;
}
int             FillOutExHdr(exHeader * ex)
{
   ex->hdr = RoomHeader;
   ClearHeaderOffsets(&ex->hdr); // Clear the offsets

   FillLayerInfo(&ex->lhFore, MapManager::mgrMaps->GetLayer(MapManager::ForeGround));
   FillLayerInfo(&ex->lhLevel, MapManager::mgrMaps->GetLayer(MapManager::LevelData));
   FillLayerInfo(&ex->lhBackLayer, MapManager::mgrMaps->GetLayer(MapManager::Backlayer));
   FillLayerInfo(&ex->lhBackground, MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer));
   ex->Sprite1 = SpriteObjectManager::mgrSpriteObjects->SpriteObjects[0];
   ex->Sprite2 = SpriteObjectManager::mgrSpriteObjects->SpriteObjects[1];
   ex->Sprite3 = SpriteObjectManager::mgrSpriteObjects->SpriteObjects[2];
   return 0;

}
int             WriteALayer(FILE * fp, layerhdr * layer)
{
   fwrite(&layer->X, 1, 1, fp);
   fwrite(&layer->Y, 1, 1, fp);
   fwrite(layer->Layer, (layer->X * layer->Y) * 2, 1, fp); // The data at
                                                            // layer is unsigned short  

   return 0;
}
int             Export(char *FilePath)
{ // Exports a room to a file
   // Let's calculate how big it'll be 
   // FileTableHeader DH=2 + 8*4 = 32 = 34

   exHeader        Export;
   char            hdrr[3] = "DH";
   int             counter = 0;
   FILE           *fp = fopen(FilePath, "w+");
   unsigned long             Pointers[8] = {0};
   if(fp){
   fseek(fp, sizeof(Pointers) + 4, SEEK_SET);
   FillOutExHdr(&Export);
   Pointers[counter++] = ftell(fp);

   fwrite(&Export.hdr, 0x3c, 1, fp);
   // Pointers[counter++]=ftell(fp);
   // WriteALayer(fp,&Export.lhFore);
   Pointers[counter++] = ftell(fp);


   Pointers[counter++]=ftell(fp);
   WriteALayer(fp,&Export.lhBackLayer);
   Pointers[counter++]=ftell(fp);
   //WriteALayer(fp,&Export.lhBackground);
   // Pointers[counter++]=ftell(fp);
   fwrite(&Export.Sprite1,sizeof(nEnemyList),1,fp);
   Pointers[counter++]=ftell(fp);
   fwrite(&Export.Sprite2,sizeof(nEnemyList),1,fp);
   Pointers[counter++]=ftell(fp);
   fwrite(&Export.Sprite3,sizeof(nEnemyList),1,fp);

   fseek(fp, 0, SEEK_SET);
   fwrite(hdrr, 2, 1, fp);
   fwrite(Pointers, 8, 4, fp);
   fwrite(hdrr, 2, 1, fp);
   fclose(fp);
   }
   return 0;
}
int             ReadLayerData(FILE * fp, nMapBuffer * layer)
{
   fread(&layer->X, 1, 1, fp);
   fread(&layer->Y, 1, 1, fp);
   fread(&layer->TileBuf2D, (layer->X * layer->Y) * 2, 1, fp);
   layer->Dirty = 1;
   return 0;
}

int             Import(char *FilePath)
{
//imports a room to a file
   // Let's calculate how big it'll be 
   // FileTableHeader DH=2 + 8*4 = 32 = 34

   int             counter = 0;
   FILE           *fp = fopen(FilePath, "r+");
   unsigned long             Pointers[8] = {0};

   if(fp){
   fseek(fp, 2, SEEK_SET);
   // Read Pointers
   fread(Pointers, 8, 4, fp);
   // DATA TIME IS NOW
   fseek(fp, Pointers[counter++], SEEK_SET);
   fread(&RoomHeader, 0x3C, 1, fp);

    fseek(fp, Pointers[counter++], SEEK_SET);
   ReadLayerData(fp,MapManager::mgrMaps->GetLayer(MapManager::ForeGround));

   fseek(fp, Pointers[counter++], SEEK_SET);
   ReadLayerData(fp, MapManager::mgrMaps->GetLayer(MapManager::LevelData));
    
      fseek(fp, Pointers[counter++], SEEK_SET);
      ReadLayerData(fp,MapManager::mgrMaps->GetLayer(MapManager::Backlayer));  
	  fseek(fp, Pointers[counter++], SEEK_SET); 
	  ReadLayerData(fp,MapManager::mgrMaps->GetLayer(MapManager::BackgroundLayer));  
	  fseek(fp, Pointers[counter++], SEEK_SET);
	  fread(&SpriteObjectManager::mgrSpriteObjects->SpriteObjects[0],sizeof(nEnemyList),1,fp); 
      fseek(fp, Pointers[counter++], SEEK_SET);
      fread(&SpriteObjectManager::mgrSpriteObjects->SpriteObjects[1],sizeof(nEnemyList),1,fp); 
	  fseek(fp, Pointers[counter++], SEEK_SET);
	  fread(&SpriteObjectManager::mgrSpriteObjects->SpriteObjects[2],sizeof(nEnemyList),1,fp);
     
   fclose(fp);

   LoadHeaderControls();
   TilesetManager::mgrTileset->GetTileset(RoomHeader.bTileset);
  //  DrawRoom(GlobalVars::gblVars->CheckBoxes[ViewB].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewL].value() == BST_CHECKED, GlobalVars::gblVars->CheckBoxes[ViewF].value() == BST_CHECKED);
   SendMessage(Main,WM_COMMAND,0x10408,0);
   }
   return 0;
}
