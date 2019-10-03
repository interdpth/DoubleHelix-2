#include "cStatEd.h"
#include <GBAMethods.h>
extern GBAMethods GBA;
cStatEd::cStatEd(int crf)
{
	SpritePreview = new SpriteObject();
	me = NULL;
	currentRomType = crf;
}


cStatEd::~cStatEd()
{
	delete SpritePreview;
}

int cStatEd::LoadSprite() {
	char buffer[256] = { 0 };
	unsigned char res = 0;

	if (currentRomType == 0) {
		//2b0d68 + (enID * 18)
		MemFile::currentFile->seek(0x2b0d68 + ((SpritePreview->id) * 18));
		MemFile::currentFile->fread(&ZMSprite, sizeof(ZMSprite), 1);
		sprintf(buffer, "%d", ZMSprite.health);
		SetWindowText(Health, buffer);
		sprintf(buffer, "%d", ZMSprite.Dmg);
		SetWindowText(Damage, buffer);
		res = ZMSprite.resistance;
		sprintf(buffer, "%d", ZMSprite.smallHP);
		SetWindowText(shp, buffer);
		sprintf(buffer, "%d", ZMSprite.largehp);
		SetWindowText(lhp, buffer);
		sprintf(buffer, "%d", ZMSprite.missiles);
		SetWindowText(m, buffer);
		sprintf(buffer, "%d", ZMSprite.supermissiles);
		SetWindowText(sm, buffer);
		sprintf(buffer, "%d", ZMSprite.PowerBombs);
		SetWindowText(pb, buffer);


		//Load Items

	}
	else if (currentRomType == 1) {
		MemFile::currentFile->seek(0x2E4D4C + ((SpritePreview->id) * 14));
		MemFile::currentFile->fread(&FusionSprite, sizeof(FusionSprite), 1);
		// MemFile::currentFile->seek(0x2b0d68 + ((SpritePreview->id)* 18));
		//MemFile::currentFile->fread(&FusionSprite,sizeof(FusionSprite),1);
		sprintf(buffer, "%d", FusionSprite.Health);
		SetWindowText(Health, buffer);
		sprintf(buffer, "%d", FusionSprite.Damage);
		SetWindowText(Damage, buffer);
		res = FusionSprite.resistance;
		sprintf(buffer, "%X", FusionSprite.unknown1);
		SetWindowText(u1, buffer);
		sprintf(buffer, "%X", FusionSprite.unknown2);
		SetWindowText(u2, buffer);
		sprintf(buffer, "%X", FusionSprite.unknown3);
		SetWindowText(u3, buffer);
		sprintf(buffer, "%X", FusionSprite.unknownDamage);
		SetWindowText(ud, buffer);
	}
	chkRes[0].value(res & 0x1 ? 1 : 0);
	chkRes[1].value(res & 0x2 ? 1 : 0);
	chkRes[2].value(res & 0x4 ? 1 : 0);
	chkRes[3].value(res & 0x8 ? 1 : 0);
	chkRes[4].value(res & 0x10 ? 1 : 0);
	chkRes[5].value(res & 0x20 ? 1 : 0);
	chkRes[6].value(res & 0x40 ? 1 : 0);
	chkRes[7].value(res & 0x80 ? 1 : 0);
	return 0;
}

int cStatEd::InitDialog() {
	int i = 0;
	char buff[256];
	SpriteList.Init(GetDlgItem(me, lstNumbers));
	SpriteList.Clear();
	for (i = 0x0; i < 0x100; i++) {
		sprintf(buff, "%X", i);
		SpriteList.Additem(buff);
	}
	Health = GetDlgItem(me, txtHealth);
	Damage = GetDlgItem(me, txtDMG);
	chkRes[0].SetCnt(GetDlgItem(me, chkBit1));
	chkRes[1].SetCnt(GetDlgItem(me, chkBit2));
	chkRes[2].SetCnt(GetDlgItem(me, chkBit4));
	chkRes[3].SetCnt(GetDlgItem(me, chkBit8));
	chkRes[4].SetCnt(GetDlgItem(me, chkBit10));
	chkRes[5].SetCnt(GetDlgItem(me, chkBit20));
	chkRes[6].SetCnt(GetDlgItem(me, chkBit40));
	chkRes[7].SetCnt(GetDlgItem(me, chkBit80));
	//ZM stuff
	shp = GetDlgItem(me, txtshp);
	lhp = GetDlgItem(me, txtlhp);
	m = GetDlgItem(me, txtm);
	sm = GetDlgItem(me, txtsm);
	pb = GetDlgItem(me, txtpb);
	//MF
	u1 = GetDlgItem(me, txtu1);
	u2 = GetDlgItem(me, txtu2);
	u3 = GetDlgItem(me, txtu3);
	ud = GetDlgItem(me, txtud);

	//GlobalVars::gblVars->StatEditor->SpritePreview->PreviewSprite=new clsPreviewSprite();

	//GlobalVars::gblVars->StatEditor->SpritePreview->PreviewSprite.Create(256, 256);
	SpritePreview->id = SpriteList.GetListIndex();
	LoadSprite();
	//Console.WriteLine("Fix LoadSprite ");
	SetupPreview();

	return 0;
}
int cStatEd::Switch() {
	if (currentRomType == 0) SetUpForZM();//change restance and disables MF editing
	if (currentRomType == 1) SetUpForMF();
	return 0;
}

int cStatEd::SetUpForMF() {



	//ZM stuff
	EnableWindow(shp, 0);
	EnableWindow(lhp, 0);
	EnableWindow(m, 0);
	EnableWindow(sm, 0);
	EnableWindow(pb, 0);
	EnableWindow(u1, 1);
	EnableWindow(u2, 1);
	EnableWindow(u3, 1);
	EnableWindow(ud, 1);
	//MF

	memset(&ZMSprite, 0, sizeof(ZMSprite));


	return 0;
}
int cStatEd::SetUpForZM() {



	//ZM stuff
	EnableWindow(shp, 1);
	EnableWindow(lhp, 1);
	EnableWindow(m, 1);
	EnableWindow(sm, 1);
	EnableWindow(pb, 1);
	EnableWindow(u1, 0);
	EnableWindow(u2, 0);
	EnableWindow(u3, 0);
	EnableWindow(ud, 0);
	//MF


	return 0;
}
int cStatEd::LoadEnemy() {
	char buffer[256] = { 0 };
	unsigned char res = 0;

	if (currentRomType == 0) {
		//2b0d68 + (enID * 18)
		MemFile::currentFile->seek(0x2b0d68 + ((SpritePreview->id) * 18));
		MemFile::currentFile->fread(&ZMSprite, sizeof(ZMSprite), 1);
		sprintf(buffer, "%d", ZMSprite.health);
		SetWindowText(Health, buffer);
		sprintf(buffer, "%d", ZMSprite.Dmg);
		SetWindowText(Damage, buffer);
		res = ZMSprite.resistance;
		sprintf(buffer, "%d", ZMSprite.smallHP);
		SetWindowText(shp, buffer);
		sprintf(buffer, "%d", ZMSprite.largehp);
		SetWindowText(lhp, buffer);
		sprintf(buffer, "%d", ZMSprite.missiles);
		SetWindowText(m, buffer);
		sprintf(buffer, "%d", ZMSprite.supermissiles);
		SetWindowText(sm, buffer);
		sprintf(buffer, "%d", ZMSprite.PowerBombs);
		SetWindowText(pb, buffer);


		//Load Items

	}
	else if (currentRomType == 1) {
		MemFile::currentFile->seek(0x2E4D4C + ((SpritePreview->id) * 14));
		MemFile::currentFile->fread(&FusionSprite, sizeof(FusionSprite), 1);
		// MemFile::currentFile->seek(0x2b0d68 + ((SpritePreview->id)* 18));
		//MemFile::currentFile->fread(&FusionSprite,sizeof(FusionSprite),1);
		sprintf(buffer, "%d", FusionSprite.Health);
		SetWindowText(Health, buffer);
		sprintf(buffer, "%d", FusionSprite.Damage);
		SetWindowText(Damage, buffer);
		res = FusionSprite.resistance;
		sprintf(buffer, "%X", FusionSprite.unknown1);
		SetWindowText(u1, buffer);
		sprintf(buffer, "%X", FusionSprite.unknown2);
		SetWindowText(u2, buffer);
		sprintf(buffer, "%X", FusionSprite.unknown3);
		SetWindowText(u3, buffer);
		sprintf(buffer, "%X", FusionSprite.unknownDamage);
		SetWindowText(ud, buffer);
	}
	chkRes[0].value(res & 0x1 ? 1 : 0);
	chkRes[1].value(res & 0x2 ? 1 : 0);
	chkRes[2].value(res & 0x4 ? 1 : 0);
	chkRes[3].value(res & 0x8 ? 1 : 0);
	chkRes[4].value(res & 0x10 ? 1 : 0);
	chkRes[5].value(res & 0x20 ? 1 : 0);
	chkRes[6].value(res & 0x40 ? 1 : 0);
	chkRes[7].value(res & 0x80 ? 1 : 0);
	return 0;
}
int cStatEd::SaveSprite() {
	char buffer[256] = { 0 };
	unsigned short blah = 0;
	unsigned char res = 0;
	res |= (chkRes[0].value() ? 1 : 0);
	res |= (chkRes[1].value() ? 2 : 0);
	res |= (chkRes[2].value() ? 4 : 0);
	res |= (chkRes[3].value() ? 8 : 0);
	res |= (chkRes[4].value() ? 0x10 : 0);
	res |= (chkRes[5].value() ? 0x20 : 0);
	res |= (chkRes[6].value() ? 0x40 : 0);
	res |= (chkRes[7].value() ? 0x80 : 0);
	if (currentRomType == 0) {
		//2b0d68 + (enID * 18)


		GetWindowText(Health, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.health);
		GetWindowText(Damage, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.Dmg);
		ZMSprite.resistance = res;

		GetWindowText(shp, buffer, 256);
		sprintf(buffer, "%d", &ZMSprite.smallHP);
		GetWindowText(lhp, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.largehp);
		GetWindowText(m, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.missiles);
		GetWindowText(sm, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.supermissiles);

		GetWindowText(pb, buffer, 256);
		sscanf(buffer, "%d", &ZMSprite.PowerBombs);

		MemFile::currentFile->seek(0x2b0d68 + ((SpritePreview->id) * 18));
		MemFile::currentFile->fwrite(&ZMSprite, 1, 18);
		fclose(GBA.ROM);
		GBA.ROM = fopen(GBA.FileLoc, "r+b");
		//		LoadSprite();
		//Load Items

	}
	else if (currentRomType == 1) {
		GetWindowText(Health, buffer, 256);
		sscanf(buffer, "%d", &FusionSprite.Health);
		GetWindowText(Damage, buffer, 256);
		sscanf(buffer, "%d", &FusionSprite.Damage);
		FusionSprite.resistance = (res);
		GetWindowText(u1, buffer, 256);
		sscanf(buffer, "%X", &FusionSprite.unknown1);
		GetWindowText(u2, buffer, 256);
		sscanf(buffer, "%X", &FusionSprite.unknown2);
		GetWindowText(u3, buffer, 256);
		sscanf(buffer, "%X", &FusionSprite.unknown3);
		GetWindowText(ud, buffer, 256);
		sscanf(buffer, "%X", &FusionSprite.unknownDamage);
		MemFile::currentFile->seek(0x2E4D4C + ((SpritePreview->id) * 14));
		//	fwrite(&FusionSprite,1,14);


		MemFile::currentFile->fwrite(&FusionSprite.Health, 2, 1);
		MemFile::currentFile->fwrite(&FusionSprite.Damage, 2, 1);
		MemFile::currentFile->fwrite(&FusionSprite.unknownDamage, 2, 1);
		MemFile::currentFile->fwrite(&res, 1, 1);
		MemFile::currentFile->fwrite(&FusionSprite.unknown6, 1, 1);
		MemFile::currentFile->fwrite(&FusionSprite.unknown1, 2, 1);
		MemFile::currentFile->fwrite(&FusionSprite.unknown2, 2, 1);
		MemFile::currentFile->fwrite(&FusionSprite.unknown3, 2, 1);
		// fwrite(&FusionSprite.end,2,1);

		GBA.Reopen();

	}

	return 0;
}
int cStatEd::SetupPreview()
{
	if (SpritePreview->id < 0x10)
	{
		return -1;
	}
	long GFXPnt = 0;
	long PalPnt = 0;
	long palsize = 0;
	long addybuf = 0;
	long size = 0;
	int i = 0;
	int ii = 0;
	unsigned char*  compBuffer = new unsigned char[64691];



	unsigned char *decompbuf = new unsigned char[32687];
	InvalidateRect(me, 0, 1);
	if (!GlobalVars::gblVars->frameTables->FramesExist(SpritePreview->id))
	{
		return 0;
	}
	memset(&SpritePreview->PreRAM[0x4000], 0, 0x4000);
	CompHeader thiscompheader;
	unsigned short transferpal[256] = { 0 };

	int  X = 0;
	long off = 0;
	int x = 0;
	memset(GBAGraphics::VRAM->SprPal, 0, sizeof(GBAGraphics::VRAM->SprPal));
	//	memset(& GBAGraphics::VRAM->GBASprPal[128],0,sizeof( GBAGraphics::VRAM->GBASprPal)-128);
	//    memset(& GBAGraphics::VRAM->SprPal,0,sizeof( GBAGraphics::VRAM->SprPal));
	if (currentRomType == 0) {
		GFXPnt = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpritePreview->id - 0x10) * 4;

		MemFile::currentFile->seek(0x75EBF8 + (SpritePreview->id - 0x10) * 4);
		MemFile::currentFile->fread(&off, 4, 1);
		MemFile::currentFile->seek(off - 0x8000000);
		MemFile::currentFile->fread(&thiscompheader.check_ten, 1, 1);       //Check byte should be 0x10 for lz
		MemFile::currentFile->fread(&thiscompheader.l3, 1, 1);
		MemFile::currentFile->fread(&thiscompheader.l2, 1, 1);
		MemFile::currentFile->fread(&thiscompheader.l1, 1, 1);

		PalPnt = 0x75EEF0 + (SpritePreview->id - 0x10) * 4;

		palsize = (thiscompheader.l2 & 0xF8) * 2;


	}
	else if (currentRomType == 1) {
		GFXPnt = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpritePreview->id - 0x10) * 4;



		PalPnt = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (SpritePreview->id - 0x10) * 4;

		palsize = ((RD1Engine::theGame->mgrOAM->MFSprSize[(SpritePreview->id - 0x10) << 1] / 2048) * 16);


	}



	MemFile::currentFile->seek(PalPnt);
	MemFile::currentFile->fread(&addybuf, 4, 1);
	MemFile::currentFile->seek(addybuf - 0x8000000);
	MemFile::currentFile->fread(&transferpal, 1, (RD1Engine::theGame->mainRoom->mgrSpriteObjects->paltransfer[x].Size) * 2);

	//	if(paltransfer[x][1] == 0) continue;
	memcpy(&GBAGraphics::VRAM->GBASprPal[128], &transferpal, (RD1Engine::theGame->mainRoom->mgrSpriteObjects->paltransfer[x].Size) * 2);

	GBA.DecodePal(GBAGraphics::VRAM->GBASprPal, SpritePreview->PreviewPal, 16, 0);



	
	RD1Engine::theGame->titleInstance->GetGFX(SpritePreview->id, &SpritePreview->PreRAM[0x4000]);

	SpritePreview->sprTileBuffer->Load(SpritePreview->PreRAM, 1023);
	RD1Engine::theGame->mgrOAM->DecodeOAM(GlobalVars::gblVars->OAMED, SpritePreview, GlobalVars::gblVars->frameTables->OAMFrameTable[SpritePreview->id].front());
	SpritePreview->PreviewSprite.GetFullImage()->Clear();
	RD1Engine::theGame->mgrOAM->DrawPSprite(SpritePreview);
	InvalidateRect(me, 0, 1);
	delete[] compBuffer;
	delete[] decompbuf;
	return 0;
}


