#include "resource.h"
#include "..\MemFile\MemFile.h"
#include "FusionSamus.h"
#include "..\Win32GBAImageClasses\Image.h"
#include "GlobalVars.h"
#include "..\Win32GBAImageClasses\BackBuffer.h"
#include "uiSamusEditorClass.h"
#include <Windows.h>
#include "..\RNA_UI\sCombo.h"
#include "..\RNA_UI\sChecks.h"
#include "resource.h"

#include <map>
#define theTimer 22424242

vector<FusionSamus*> samusSprites;
FusionSamus* mf;

BackBuffer* samusBuffer;
Frame * curFrame;
FrameManager * fm;
Image* GFX;
#include "Frames.h"
sCombo cboDirection;
sCombo cboPose;
sCombo cboEvent;
sChecks chkMissiles;
//SuitStatus check
std::map<string, int> suitStatusLookup;
std::map<string, int> poseLookup;
std::map<string, int> eventLookup;
unsigned long GetPointer(unsigned long addr);
unsigned long Getlong(unsigned long addr);

unsigned char* GetData(unsigned long addr, unsigned long size);
SamusEditorClass* sec;
//Samus States.
signed int samusDirection;
int facingDirections; 
int CurrentPose; 
unsigned long GfxSizeTable;
void Logic()
{
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	int IsSamusFacingLeft; // r4

	int CopyIndex; // r2
	int PoseIndex; // r0
// r6
	unsigned long currentSizeTable = 0; // r1
	unsigned long* DifferentAnimTable = 0; // r2
	int TheDirectionIndex; // r1

	int someIndex; // r5
	SamusAnim* theCurrentAnim = NULL; // r6
	unsigned  long* tophalf; // r3
	unsigned long* legspointer = NULL; // r3
	unsigned long* CannonBottomTemp; // r0
	signed __int16 bottomgfxsize; // r0

	unsigned long* CannonBottomHalfFXOffset; // r0
	unsigned short* NewPalPointer = NULL; // r4
	unsigned int speedBoostIndex; // r1
	int indexByXTimer; // r1
	unsigned short* PalTablePointer = NULL; // r0
	unsigned short* FinalPalPointer = NULL; // r0
	int PalIndex; // r1
	int PalSize; // r2
	unsigned short* AnotherPalPointer = NULL; // r4
	unsigned int CurAnimState; // r1
	int SavingPalIndex; // r1
	unsigned short* APalTable = NULL; // r0
	unsigned short* APalPointer = { 0 }; // r1
	__int32 ChargeBeamStartPalCounter; // r1
	int chargeCounter; // r1
	unsigned short* PalPointer = NULL; // r4

	IsSamusFacingLeft = true;// isFacingLeft;
	if (mf->SamusPose != GettingHurt && mf->Invincibilitytimer)
		--mf->Invincibilitytimer;
	if (mf->AbsorbXTimer)
		--mf->AbsorbXTimer;
	if (mf->SpeedBoosting)
	{
		mf->SpeedBoostingindicator = 1;
		mf->SpeedBoostingindicatorCurrent = 16;
	}
	else if (mf->SpeedBoostingindicatorCurrent)
	{
		--mf->SpeedBoostingindicatorCurrent;
	}
	else
	{
		mf->SpeedBoostingindicator = mf->SpeedBoostingindicatorCurrent;
	}
	////backupOfLastpreIndex = lastPreviousPositionIndex;
	////previousPosIndex = 2 * (lastPreviousPositionIndex & 0x3F);
	////*(Previous_64_X_positions + previousPosIndex) = Previous_SamusX_position;
	////*(Previous_64_Y_positions + previousPosIndex) = Previous_SamusY_position;
	////lastPreviousPositionIndex = backupOfLastpreIndex + 1;
	////if (backupOfLastpreIndex << 16 > 0x1000000)
	////	lastPreviousPositionIndex = backupOfLastpreIndex - 127;
	////if (lastPreviousPositionIndex > 63)
	////	startNewPrevPositions = 1;
	CurrentPose = mf->SamusPose;
	mf->ArmCannonDirection = IsSamusFacingLeft;
	samusDirection = mf->ArmCannonDirection;
	facingDirections = samusDirection * 4;


	if (mf->SamusPose <= 0x3Fu)
	{
		switch (mf->SamusPose)
		{
		case Standing:
		{
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			unsigned long index = GetPointer(0x28D4C4 + PoseIndex);
			unsigned long pnta = GetPointer(index);


			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D4C4 + PoseIndex)];

			currentSizeTable = GetPointer(0x28DAE4);
		}
		goto SetGFXTablePointer;
		case TurningAround:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28d4ec + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB0C);
			goto SetGFXTablePointer;
		case LOOKFORTARGET:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D514 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB34);
			goto SetGFXTablePointer;
		case Running:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D53C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB5C);
			goto SetGFXTablePointer;
		case Jumping_Falling:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D55C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB7C);
			goto SetGFXTablePointer;
		case TurningAndJumpFall:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D58C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DBAC);
			goto SetGFXTablePointer;
		case Landing:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D5BC + PoseIndex)];
			currentSizeTable = GetPointer(0x28DBDC);
			goto SetGFXTablePointer;
		case Ducking:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D634 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC54);
			goto SetGFXTablePointer;
		case TurnAndDuck:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D60C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC2C);
			goto SetGFXTablePointer;
		case ShootAndDuck:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D634 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC54);
			goto SetGFXTablePointer;
		case HangLedge:
		/*	if ((mf->SamusDirection ^ 0x30) & Button_input)
				samusDirection = (ArmCannonDirection + 1) & 0xFF;*/
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * samusDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D65C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC7);
			goto SetGFXTablePointer;
		case Skidding:
			if (mf->MissilesSelected == 1)
				samusDirection = (mf->ArmCannonDirection + 1) & 0xFF;
			//DifferentAnimTable = ((SamusAnim**)rawFile[0x28D674]);
			goto LABEL_40;
		case ShineSparking:
			samusDirection = mf->Direction;
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->Direction;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D684 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC8C);
			goto SetGFXTablePointer;
		case DelayAfterShineSpark:
			samusDirection = mf->Direction;
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->Direction;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D69C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCA4);
			goto SetGFXTablePointer;
		case CannonLadder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D6B4 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCBC);
			goto SetGFXTablePointer;
		case ShootUpLadder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D6E4 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCEC);
			goto SetGFXTablePointer;
		case DelayAfterShootingHOrizladder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D714 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DD1C);
			goto SetGFXTablePointer;
		case ShootingOnHorizladder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D744 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DD4C);
			goto SetGFXTablePointer;
		case HitByOmega:
			samusDirection = (-(mf->Button_input & 0x70) | mf->Button_input & 0x70u) >> 31;
			DifferentAnimTable = (unsigned long*)&rawFile[0x28D784];
		LABEL_40:
			{
				TheDirectionIndex = 4 * IsSamusFacingLeft;

				mf->Animtable = (SamusAnim*)&DifferentAnimTable[2 * samusDirection + IsSamusFacingLeft];
				GfxSizeTable = GetPointer(0x28D8DC + ((2 * mf->SamusPose + IsSamusFacingLeft) * 4));
				someIndex = TheDirectionIndex;
				goto SetOAM;
			}
		default:
			break;
		}
	}
	CopyIndex = 4 * IsSamusFacingLeft;
	PoseIndex = 4 * IsSamusFacingLeft + 8 * mf->SamusPose;
	mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D2BC + PoseIndex)];
	currentSizeTable = GetPointer(0x28D8DC);
SetGFXTablePointer:
	GfxSizeTable = GetPointer(currentSizeTable + PoseIndex);
	someIndex = CopyIndex;

	 
	//loopie loop
	//Get the shit.


SetOAM:
	for each (auto a in samusSprites) delete a;
	samusSprites.clear();

	for (int i = 0; i < 255;i++)
	{
		SamusAnim* curAnim = &mf->Animtable[i];
		if (curAnim->OAMPointer == 0)
		{
			
			break;
		}
		FusionSamus* sprite = new FusionSamus();
		mf->Copy(sprite);
		samusSprites.push_back(sprite);


	}
	

}

void InitUI()
{
	suitStatusLookup["None"] = 0,
	suitStatusLookup["HiJump"] = 0x1;
	suitStatusLookup["SpeedBooster"] = 0x2;
	suitStatusLookup["SpaceJump"] = 0x4;
	suitStatusLookup["ScrewAttack"] = 0x8;
	suitStatusLookup["Varia"] = 0x10;
	suitStatusLookup["GravitySuit"] = 0x20;
	suitStatusLookup["MorphBall"] = 0x40;
	suitStatusLookup["SaxSuit"] = 0x80;


	poseLookup["Standing"] = 0x0;
	poseLookup["TurningAround"] = 0x1;
	poseLookup["LOOKFORTARGET"] = 0x2;
	poseLookup["Running"] = 0x3;
	poseLookup["Jumping_Falling"] = 0x4;
	poseLookup["TurningAndJumpFall"] = 0x5;
	poseLookup["Landing"] = 0x6;
	poseLookup["Ducking"] = 0x7;
	poseLookup["TurnAndDuck"] = 0x8;
	poseLookup["ShootAndDuck"] = 0x9;
	poseLookup["StartSpinJump"] = 0xA;
	poseLookup["Spinning"] = 0xB;
	poseLookup["Morphing"] = 0xC;
	poseLookup["MorphBall"] = 0xD;
	poseLookup["Rolling"] = 0xE;
	poseLookup["Unmorphing"] = 0xF;
	poseLookup["InAirMorpball"] = 0x10;
	poseLookup["GettingHurt"] = 0x11;
	poseLookup["GettingHurtInMorphball"] = 0x12;
	poseLookup["GettingKnockedBack"] = 0x13;
	poseLookup["GettingKnockedBackInMorph"] = 0x14;
	poseLookup["StartWalljump"] = 0x15;
	poseLookup["WallJump"] = 0x16;
	poseLookup["Elevator_"] = 0x17;
	poseLookup["HangLedge"] = 0x18;
	poseLookup["PullUpFromHang"] = 0x19;
	poseLookup["PullForwardFromHang"] = 0x1A;
	poseLookup["PullDownToHang"] = 0x1C;
	poseLookup["SpaceJumping"] = 0x1D;
	poseLookup["ScrewAttacking"] = 0x1E;
	poseLookup["Skidding"] = 0x1F;
	poseLookup["OnSave"] = 0x20;
	poseLookup["RechargeSecurity"] = 0x21;
	poseLookup["TurnAroundBeCharged"] = 0x22;
	poseLookup["ShineSparkDelay"] = 0x23;
	poseLookup["ShineSparking"] = 0x24;
	poseLookup["DelayAfterShineSpark"] = 0x25;
	poseLookup["HangingLadder"] = 0x26;
	poseLookup["BegingArmCannonLadder"] = 0x27;
	poseLookup["CannonLadder"] = 0x28;
	poseLookup["ShootUpLadder"] = 0x29;
	poseLookup["VertLadderCanonMove"] = 0x2A;
	poseLookup["HangingHorizLadder"] = 0x2B;
	poseLookup["MovingOnHorizlader"] = 0x2C;
	poseLookup["TurningHorizLAdder"] = 0x2D;
	poseLookup["DelayAfterShootingHOrizladder"] = 0x2E;
	poseLookup["ShootingOnHorizladder"] = 0x2F;
	poseLookup["Frozen"] = 0x30;
	poseLookup["FrozenAndFalling"] = 0x31;
	poseLookup["FrozenInMorphBall"] = 0x32;
	poseLookup["FrozenInMorphballFalling"] = 0x33;
	poseLookup["UnlockSecurity"] = 0x34;
	poseLookup["Saving"] = 0x35;
	poseLookup["OnNavPad"] = 0x36;
	poseLookup["DownloadAbility"] = 0x37;
	poseLookup["FacingForward"] = 0x39;
	poseLookup["FacingBackward"] = 0x3A;
	poseLookup["LoadSave"] = 0x3D;
	poseLookup["SamusDying"] = 0x3E;
	poseLookup["HitByOmega"] = 0x3F;
	poseLookup["GrabbedByYakuza"] = 0x40;
	for (map<string, int>::iterator it = poseLookup.begin(); it != poseLookup.end(); ++it) {
		cboPose.Additem((char*)(it->first.data()));
	}
}

bool SetFlagStatus(int id)
{
	switch (id)
	{
	
	case chkCharge: mf->BeamStatus ^= 1; break;
	case chkWide: mf->BeamStatus ^= 2; break;
	case chkPlasma: mf->BeamStatus ^= 4; break;
	case chkWave:mf->BeamStatus ^= 8;  break;
	case chkIce: mf->BeamStatus ^= 0x10; break;

	
	
	case chkMissil: mf->MissileBombStatus ^= 1; break;
	case chkSuperMissiles:  mf->MissileBombStatus ^= 2; break;
	case chkIceMissiles:  mf->MissileBombStatus ^= 4; break;
	case chkDiffusionMissiles:  mf->MissileBombStatus ^= 8; break;
	case chkBombs:  mf->MissileBombStatus ^= 0x10; break;
	case chkPBs: mf->MissileBombStatus ^= 0x20; break;


	case chkHiJump:  mf->SamusSuitStatus ^= 1;  break;
	case chkSpeedBooster:  mf->SamusSuitStatus ^= 2; break;
	case chkSpaceJump: mf->SamusSuitStatus ^= 4; break;
	case chkScrewAttack: mf->SamusSuitStatus ^= 8; break;
	case chkVaria: mf->SamusSuitStatus ^= 0x10; break;
	case chkGravity:mf->SamusSuitStatus ^= 0x20; break;
	case chkMorphball:mf->SamusSuitStatus ^= 0x40; break;
	case chkSaxSuit:mf->SamusSuitStatus ^= 0x80; break;


	case chkHoldMissiles: mf->MissilesSelected = !mf->MissilesSelected; break;
	case chkFaceRight:mf->SamusSuitStatus = !mf->SamusSuitStatus; break;
	default:
		return false;
	}
	return true;
}
void DrawSamus()
{
	if (samusBuffer)
	{
		delete samusBuffer;
	}
	SamusBase* base = mf;

	samusBuffer = new BackBuffer();
	samusBuffer->Create(1024, 1024);

	sec->SetupSamusPreview(base);

	GFX = new Image();
	GFX->Create(512, 512);
	long pal[512] = { 0 };


	TileBuffer tb;
	tb.Load((unsigned char*)base->Sprite_tiles, 1023);

	GFX->SetPalette(sec->SpritePreview->PreviewPal);
	GFX->Clear();
	for (int i = 0; i < 1023; i++) {
		GFX->Draw(tb, ((i) % 32) * 8, ((i) / 32) * 8, i);
	}

}


BOOL CALLBACK	SamusProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	int i = 0;
	char wndstr[256] = { 0 };

	int partIndex = 0;

	switch (message)
	{

	case WM_INITDIALOG:
		sec = new SamusEditorClass();
		SamusEditorClass::hwndSamusEditor = hWnd;
		samusBuffer = NULL;
		curFrame = NULL;
		cboPose.Init(GetDlgItem(hWnd, cboPoseitem));
		InitUI();
		mf = new FusionSamus();

		
		cboPose.SetListIndex(0);

		break;
	case WM_TIMER:
		switch (wParam)
		{
		case theTimer:

			break;
		}
		break;

	case WM_COMMAND:
		if (SetFlagStatus(LOWORD(wParam)))
		{
			string myText = cboPose.GetText(cboPose.GetListIndex());
			mf->theSprite->PreviewSprite.GetFullImage()->Clear();
			mf->SamusPose = poseLookup[myText];
			Logic();
			mf->Load();
			mf->PackSamus();
			DrawSamus();
			InvalidateRect(hWnd, 0, true);
		}
		switch (LOWORD(wParam))
		{

			
		case cboPoseitem:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				string myText = cboPose.GetText(cboPose.GetListIndex());
				mf->theSprite->PreviewSprite.GetFullImage()->Clear();
				mf->SamusPose = poseLookup[myText];
				Logic();
				mf->Load();
				mf->PackSamus();
				DrawSamus();
				InvalidateRect(hWnd, 0, true);
			}
		}
		break;
	case WM_PAINT:
	{
		if (!sec->SpritePreview)
		{
			break;
		}

		hdc = BeginPaint(hWnd, &ps);
		int height = (sec->SpritePreview->Borders.bottom - sec->SpritePreview->Borders.top);
		int width = (sec->SpritePreview->Borders.right - sec->SpritePreview->Borders.left);
		//sec->SpritePreview->PreviewSprite.TransBlit
		//(hdc, 0, 0,
		//	width,
		//	height,
		//	0,
		//	0);

		StretchBlt(hdc, 64, 1, width * 3, height * 3, sec->SpritePreview->PreviewSprite.GetFullImage()->DC(), 0, 0, width * 3, height * 3, SRCCOPY);

		if (GFX)
		{
			BitBlt(hdc, 18, 185, 256, 256, GFX->DC(), 0, 0, SRCCOPY);
		}

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_DESTROY:

		/// myBackBuffer.Destroy();
		break;
	}

	return 0;
}









LRESULT CALLBACK SamusSpriteProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};
	int currentPart = 0;
	switch (message)
	{
		/*case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc, 0, 0, 512, 512, samusBuffer->DC(), 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			break;*/

	case WM_LBUTTONDOWN:


		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

///Windos proc for handling the animation window
LRESULT CALLBACK AnimationSamusProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	/*Frame* curFrame = NULL;
	cOAMEdit* oamEditor = cOAMEdit::OamEditor;*/
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//if (cOAMEdit::OamEditor->currentFrames != NULL)
		//{

		//	curFrame = oamEditor->currentFrames->GetAnimatedFrame();
		//	if (!curFrame->frameInited) {
		//		globalVars->OAMED = true;

		//		oamEditor->SetupPreview(currentRomType, curFrame);
		//		oamEditor->LoadTiles(oamEditor->Tiles, curFrame);
		//		oamManager->DecodeOAM(globalVars->OAMED, curFrame->theSprite, curFrame->frameOffset - 0x8000000);
		//		globalVars->OAMED = false;
		//		oamManager->DrawPSprite(curFrame->theSprite);
		//		curFrame->frameInited = true;
		//		curFrame->animUpdated = true;
		//	}

		//	if (curFrame->animUpdated)
		//	{
		//		HBRUSH          br = CreateSolidBrush((COLORREF)RGB(255, 0, 0));
		//		RECT            rc;

		//		SetRect(&rc, 0, 0, curFrame->theSprite->PreviewSprite.Width, curFrame->theSprite->PreviewSprite.Height);
		//		FrameRect(hdc, &rc, br);

		//		DeleteObject(br);
		//		curFrame->animUpdated = false;
		//		curFrame->theSprite->PreviewSprite.Blit(hdc, 0, 0, 2048, 2048, 0, 0);
		//	}
		//	//BitBlt(hdc, 0, 0, 2048, 2048, animBuffer.DC(), 0, 0, SRCCOPY);


		//}
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}



