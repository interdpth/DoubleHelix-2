#include "cTextEditor.h"
#include "GlobalVars.h"
#include "GBAMethods.h"
extern GBAMethods GBA;


MetroidTextEditor::MetroidTextEditor() {
	tMain = NULL;
}

MetroidTextEditor::~MetroidTextEditor() {
	//	delete[] curtext;

}
int MetroidTextEditor::Draw() {
	int X = 0;
	int Y = 0;
	int cY = 0;
	int cX = 0;
	unsigned short tTile = 0;
	int i = 0;
	char Color = -1;
	Text->Clear();

	pixelflag = -1;
	for (i = 0; i<txtlength; i++) {

		tTile = binarystring[i];
		if (tTile == 0xFE00) {//New line
			Y += 0x10;
			X = 0;
		}
		else {
			//X += 7;

			//if( ((Tile > 0x4FF) || (Tile < 0x400))){
			if ((tTile >= 0x8100 && tTile < 0x8111)) {

				// Color = ((Tile)&0xf)*0x1000;
				Color = (tTile)& 0xf;
				if (Color == 0)Color = -1;

			}
			else if (tTile >= 0x200 && tTile < 0x1000) {
				X += 7;
				Text->Draw(Tiles, X, Y, tTile);
				Text->Draw(Tiles, X, Y + 8, tTile + 32);
				Text->Draw(Tiles, X + 8, Y, tTile + 1);
				Text->Draw(Tiles, X + 8, Y + 8, tTile + 33);
				X += 7;
			}
			else if (tTile < 0x100) {
				X += 7;
				Text->Draw(Tiles, X, Y, tTile, Color);
				Text->Draw(Tiles, X, Y + 8, tTile + 32, Color);

			}
			//	}

		}



	}
	InvalidateRect(textpic, 0, 1);
	return 0;
}



int MetroidTextEditor::CreateTextpicTileImage(HINSTANCE MainInstance, WNDPROC wnd ) {
	WNDCLASSEX blab5;
	memset(&blab5, 0, sizeof(blab5));
	blab5.cbSize = sizeof(blab5);		   // size of structure 
	blab5.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blab5.lpfnWndProc = wnd;	  // points to window procedure 
	blab5.cbClsExtra = NULL;				// no extra class memory 
	blab5.cbWndExtra = NULL; 			   // no extra window memory 
	blab5.hInstance = MainInstance;		   // handle to instance 
	blab5.hIcon = NULL; 			 // predefined app. icon 
	blab5.hCursor = NULL;				// predefined arrow 
										//blab5.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);				   // white background brush 
	blab5.lpszMenuName = NULL;    // name of menu resource 
	blab5.lpszClassName = "newpics";  // name of window class 


									  // Register the window class. 
	RegisterClassEx(&blab5);

	textpic = CreateWindowEx(NULL, "newpics", NULL, WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD, 0, 0, 512, 512, GetDlgItem(tMain, fraPic), 0, MainInstance, 0);


	ShowWindow(textpic, SW_SHOW);
	return 0;
}




int MetroidTextEditor::DecodeText(unsigned short* data, int dataLen)
{
	memset(curtext, 0, sizeof(curtext));
	for (int i = 0; i<dataLen; i++) {//Main copy loop


		if (StringTable.count(data[i]) == 1) {
			strcat(curtext, StringTable[data[i]].c_str());
		}
		else {
			int lamo = data[i];

			sprintf(curtext, "%s[RAW 0x%04X]", curtext, lamo);

		}
	}
	return 0;
}

int MetroidTextEditor::DecodeText(unsigned long offset) {
	//Find length of the text;
	unsigned short tv = 0;

	txtlength = 0;
	int i = 0;
	if (GBA.ROM) {
		MemFile::currentFile->seek(offset);


		while (!feof(GBA.ROM)) {
			MemFile::currentFile->fread(&tv, 2, 1, GBA.ROM);
			if (tv == 0xFF00 || tv == 0x41F) break;
			txtlength++;
		}
		txtlength++;



		MemFile::currentFile->seek(offset);
		MemFile::currentFile->fread(binarystring, 2, txtlength, GBA.ROM);

		memset(curtext, 0, 2048);
		DecodeText(binarystring, txtlength);
		SetWindowText(GetDlgItem(tMain, txtString), curtext);

	}

	return 0;
}


unsigned short  MetroidTextEditor::GetEncodedEntry(char* data)
{
	string disCompare = data;
	int retVal = 0;
	for (std::map<int, string>::iterator itr = StringTable.begin(), itr_end = StringTable.end(); itr != itr_end; ++itr) {
		if ((string)itr->second == data) {
			retVal = itr->first;
			break;
		}



	}
	
	
	sscanf(data, "[RAW 0x%hX]", &retVal);



	return retVal & 0xFFFF;
}


vector<unsigned short>*  MetroidTextEditor::EncodeString( char* txt)
{
	int len = strlen(txt);
	int i = 0;
	vector<unsigned short>* deseShorts = new vector<unsigned short>();
	unsigned short* tmpData = new unsigned short[4096];
	for (i = 0; i < len; i++)
	{
		char *disChar = new char[2];
		memset(disChar, 0, 2);
		*disChar=txt[i];
	
		if (*disChar == '[')
		{
			
			int tmplen = 0;
			for (tmplen = 0; tmplen < 32; tmplen++)
			{
				if (txt[i + tmplen] == ']') {
					//all done here
					//
					char tmpme[256] = { 0 };
					memcpy(&tmpme, &txt[i], tmplen+1);
					i += tmplen;
					unsigned short theVal = GetEncodedEntry(tmpme);

					deseShorts->push_back(theVal);
					break;
				}
			}

		}
		else
		{
			unsigned short theVal = GetEncodedEntry(disChar);

			deseShorts->push_back(theVal);

		}


	}

	return deseShorts;

}


//int cTextEditor::EncodeText(unsigned long offset) {
//	//Find length of the text;
//
//vector<unsigned short> encodedBytes = 
//
//
//
//
//	unsigned short tv = 0;
//	unsigned short* encodeString = new unsigned short[2048];
//
//	if (GBA.ROM) {
//		MemFile::currentFile->seek(offset);
//
//
//		while (!feof(GBA.ROM)) {
//			MemFile::currentFile->fread(&tv, 2, 1, GBA.ROM);
//			if (tv == 0xFF00 || tv == 0x41F) break;
//			txtlength++;
//		}
//		txtlength++;
//
//
//
//		MemFile::currentFile->seek(offset);
//		MemFile::currentFile->fread(binarystring, 2, txtlength, GBA.ROM);
//
//		memset(curtext, 0, 2048);
//
//		for (i = 0; i<txtlength; i++) {//Main copy loop
//
//			strcat(curtext, table[binarystring[i]]);
//		}
//		SetWindowText(GetDlgItem(tMain, txtString), curtext);
//
//	}
//
//	return 0;
//}
//
//


int MetroidTextEditor::BiCat(unsigned short* dest, char* entry) {
	char buf[256] = { 0 };
	int i = 0, dataplus = 0;//Tells how much to increase the text data
	//if ((entry[0] == '[')) {

	//	for (i = 0; i<strlen(entry); i++) {
	//		buf[i] = entry[i];
	//		if ((entry[i] == ']')) break;
	//	}


	//	dataplus = i;
	//}
	//else {
	//	buf[0] = entry[0];
	//	dataplus = 1;
	//}

	////Do a look up to match
	//for (i = 0; i<0xFFFF; i++) {
	//	//compare curtext with a entry from the table then put ii into the nb stream 
	//	//and increase count
	//	if (strcmp(table[i], buf) == 0) {
	//		//They're identical so put it into stream and break; 
	//		*dest = i;

	//		break;
	//	}

	//}

	return dataplus;

}
int MetroidTextEditor::EncodeText(int TitleChoice) {
	GetWindowText(GetDlgItem(this->tMain, txtString), this->curtext, 1024);
	vector<unsigned short>* encodedData = EncodeString(curtext);
	string teststr = curtext;
	DecodeText(encodedData->data(), encodedData->size());
	string test2 = curtext;

	if (teststr == test2)
	{
		teststr = "shit";
	}


	int i = 0;
	int ii = 0;//used for looking inside strings
	int iii = 0;
	int itemcount = 0;
	int strcounter = 0;


	unsigned short marker = 0xFF00;
	unsigned short nb[1024] = { 0 };
	unsigned long offset = 0;
	unsigned short returned = 0;
	//GetWindowText(GetDlgItem(tMain,txtString),stupidstring,1024);
	
	int a = strlen(curtext);

	//Just find it a new home.
	offset = GBA.FindFreeSpace(strcounter * 2, 0xFF);
	MemFile::currentFile->seek(offset);
	MemFile::currentFile->fwrite(encodedData->data(), 2, encodedData->size(), GBA.ROM);
	MemFile::currentFile->fwrite(&marker, 2, 1, GBA.ROM);
	//Now apply changes to lists and tables
	//Offset=
	GlobalVars::gblVars->TextEditor->ZMOffsets[GlobalVars::gblVars->TextEditor->cboTxt.GetListIndex()][GlobalVars::gblVars->TextEditor->cOffsets.GetListIndex()] = offset + 0x8000000;
	if (TitleChoice == 0) {
		MemFile::currentFile->seek(0x760914);
		for (i = 0; i<99; i++) {
			MemFile::currentFile->fwrite(&ZMOffsets[0][i], 4, 1, GBA.ROM);

		}
		MemFile::currentFile->seek(0x760660);
		for (i = 0; i<65; i++) {
			MemFile::currentFile->fwrite(&ZMOffsets[1][i], 4, 1, GBA.ROM);

		}

	}
	else if (TitleChoice == 1) {
		MemFile::currentFile->seek(0x79EA08);
		MemFile::currentFile->fwrite(&Offsets, 4, 99, GBA.ROM);
	}
	GBA.Reopen();
	iii = cOffsets.GetListIndex();
	ResetList(TitleChoice);
	cOffsets.SetListIndex(iii);

	SendMessage(tMain, WM_COMMAND, LOWORD(lstOffsets), 0);
	return 0;
}



int MetroidTextEditor::Create(int TitleChoice, HINSTANCE MainInstance, WNDPROC wnd) {
	int i = 0;
	GlobalVars::gblVars->TextEditor->pixelflag = 0;
	memset(GBA.GBAPal, 0, 512);
	memset(curtext, 0, 2048);
	cOffsets.Init(GetDlgItem(tMain, lstOffsets));
	cboTxt.Init(GetDlgItem(tMain, cboTextI));
	

	
	
		this->cboTxt.Clear();
		cboTxt.Additem("Main Text");
		//cboTxt.Additem("Item Text");
		cboTxt.SetListIndex(0);
	
		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("TextGFX")->Value);
		MemFile::currentFile->fread(&Graphics, 1, 0x8000, GBA.ROM);
		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("TextPal")->Value);
		MemFile::currentFile->fread(GBA.GBAPal, 2, 0x10, GBA.ROM);
	

		GBA.DecodePal(GBA.GBAPal, FontPal, 16, 0);
	

	ResetList(TitleChoice);
	LoadTable();
	Tiles.Load(Graphics, 1024);
	Text = new Image();
	Text->Create(512, 512);
	Text->SetPalette(FontPal);
	CreateTextpicTileImage(MainInstance,wnd);
	return 0;
}
int MetroidTextEditor::LoadTable() {
	unsigned long i = 0;
	unsigned long ii = 0;
	FILE* fp;
	unsigned long eoff;
	unsigned long Entry;

	char buf[256];
	char charbuf[256];
	char ThisFileString[2048] = { 0 };

	

	strcpy(ThisFileString, GlobalVars::gblVars->AppPath);
	strcat(ThisFileString, "\\DH.tbl");
	fp = fopen(ThisFileString, "r");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		eoff = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		while (ftell(fp)<eoff) {
			memset(buf, 0, 256);
			memset(charbuf, 0, 256);
			fgets(buf, 256, fp);

			//int blah=strlen(buf);
			//buf[strlen(buf)-2]=0;
			//buf[strlen(buf)-1]=0;
			sscanf(buf, "%X=%s", &Entry, &charbuf);
			//memcpy(&table[Entry], charbuf, strlen(charbuf));

			StringTable[Entry] = charbuf;
		}


		StringTable[0x40] = " ";
		

		fclose(fp);
	}
	//for (i = 0; i<0xFF00; i++) {


	//	if (StringTable.count(i) == 0) {

	//		char omg[1024];
	//		sprintf(omg, "[RAW 0x%X]", i);
	//		StringTable[i] = omg;
	//	}
	//}

	return 0;
}



int MetroidTextEditor::ResetList(int TitleChoice) {
	unsigned short i = 0;
	unsigned short max = 0;
	char buffer[256] = { 0 };
	GBA.Reopen();
	if (TitleChoice == 0) {
		max = 62;

		cOffsets.Clear();
		if (cboTxt.GetListIndex() == 0) {
			for (i = 0; i<99; i++) {

				sprintf(buffer, "%X", ZMOffsets[0][i]);
				cOffsets.Additem(buffer);
			}
		}
		else {
			for (i = 0; i<65; i++) {

				sprintf(buffer, "%X", ZMOffsets[1][i]);
				cOffsets.Additem(buffer);
			}
		}
	}
	else if (TitleChoice == 1) {
		max = 149;
		MemFile::currentFile->seek(0x79E6EC);
		for (i = 0; i<351; i++) {

			MemFile::currentFile->fread(&Offsets[i], 4, 1, GBA.ROM);
			sprintf(buffer, "%X", Offsets[i]);
			cOffsets.Additem(buffer);
		}


	}

	//for(i=0;i<62;i++){
	//	if(Offsets[i]==0) 
	//    Offsets[i]=Offsets[i];

	//}
	return 0;
}
