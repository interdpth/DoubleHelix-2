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
Frame* curFrame;
FrameManager* fm;
Image* GFX;
#include "Frames.h"
sCombo cboDirection;
sCombo cboPose;

sCombo cboCurFrame;
//State controls.
sCombo cboEvent;
sChecks chkFaceright;
sChecks chkHoldingMissiles;
sCombo cboAbsorbX;
//Beams
sChecks Beams_Wide;
sChecks Beams_Charge;
sChecks Beams_Plasma;
sChecks Beams_Wave;
sChecks Beams_Ice;

//Suits
sChecks Suits_HiJump;
sChecks Suits_SpeedBooster;
sChecks Suits_SpaceJump;
sChecks Suits_ScrewAttack;
sChecks Suits_Morphball;
sChecks Suits_Varia;
sChecks Suits_Gravity;
sChecks Suits_SaxSuit;
//Missiles

sChecks Missiles_Missiles;
sChecks Missiles_Ice;
sChecks Missiles_Super;
sChecks Missiles_Diffusion;
sChecks Missiles_Bombs;
sChecks Missiles_PowerBombs;
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
void Logic(int newPose)
{
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	int IsSamusFacingLeft; // r4

	int CopyIndex; // r2
	int PoseIndex; // r0
// r6
	unsigned long currentSizeTable = 0; // r1
	unsigned long DifferentAnimTable = 0; // r2
	int TheDirectionIndex; // r1

	int someIndex; // r5
	SamusAnim* theCurrentAnim = nullptr; // r6
	unsigned  long* tophalf; // r3
	unsigned long* legspointer = nullptr; // r3
	unsigned long* CannonBottomTemp; // r0
	signed __int16 bottomgfxsize; // r0

	unsigned long* CannonBottomHalfFXOffset; // r0
	unsigned short* NewPalPointer = nullptr; // r4
	unsigned int speedBoostIndex; // r1
	int indexByXTimer; // r1
	unsigned short* PalTablePointer = nullptr; // r0
	unsigned short* FinalPalPointer = nullptr; // r0
	int PalIndex; // r1
	int PalSize; // r2
	unsigned short* AnotherPalPointer = nullptr; // r4
	unsigned int CurAnimState; // r1
	int SavingPalIndex; // r1
	unsigned short* APalTable = nullptr; // r0
	unsigned short* APalPointer = { 0 }; // r1
	__int32 ChargeBeamStartPalCounter; // r1
	int chargeCounter; // r1
	unsigned short* PalPointer = nullptr; // r4

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
	CurrentPose = newPose;
	mf->SamusPose = CurrentPose;
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
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			unsigned long index = GetPointer(0x28D4C4 + PoseIndex);
			unsigned long pnta = GetPointer(index);


			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D4C4 + PoseIndex)];

			currentSizeTable = GetPointer(0x28DAE4);
		}
		goto SetGFXTablePointer;
		case TurningAround:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28d4ec + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB0C);
			goto SetGFXTablePointer;
		case LOOKFORTARGET:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D514 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB34);
			goto SetGFXTablePointer;
		case Running:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D53C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB5C);
			goto SetGFXTablePointer;
		case Jumping_Falling:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D55C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DB7C);
			goto SetGFXTablePointer;
		case TurningAndJumpFall:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D58C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DBAC);
			goto SetGFXTablePointer;
		case Landing:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D5BC + PoseIndex)];
			currentSizeTable = GetPointer(0x28DBDC);
			goto SetGFXTablePointer;
		case Ducking:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D634 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC54);
			goto SetGFXTablePointer;
		case TurnAndDuck:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D60C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC2C);
			goto SetGFXTablePointer;
		case ShootAndDuck:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D634 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC54);
			goto SetGFXTablePointer;
		case HangLedge:
			/*	if ((mf->SamusDirection ^ 0x30) & Button_input)
					samusDirection = (ArmCannonDirection + 1) & 0xFF;*/
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * samusDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D65C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC7);
			goto SetGFXTablePointer;
		case Skidding:
			if (mf->MissilesSelected == 1)
				samusDirection = (mf->ArmCannonDirection + 1) & 0xFF;
			DifferentAnimTable = 0x28D674;
			goto LABEL_40;
		case ShineSparking:
			samusDirection = mf->Direction;
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->Direction;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D684 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DC8C);
			goto SetGFXTablePointer;
		case DelayAfterShineSpark:
			samusDirection = mf->Direction;
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->Direction;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D69C + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCA4);
			goto SetGFXTablePointer;
		case CannonLadder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D6B4 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCBC);
			goto SetGFXTablePointer;
		case ShootUpLadder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D6E4 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DCEC);
			goto SetGFXTablePointer;
		case DelayAfterShootingHOrizladder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D714 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DD1C);
			goto SetGFXTablePointer;
		case ShootingOnHorizladder:
			CopyIndex = 4 * IsSamusFacingLeft;
			PoseIndex = CopyIndex + 8 * mf->ArmCannonDirection;
			mf->Animtable = (SamusAnim*)&rawFile[GetPointer(0x28D744 + PoseIndex)];
			currentSizeTable = GetPointer(0x28DD4C);
			goto SetGFXTablePointer;
		case HitByOmega:
			samusDirection = (-(mf->Button_input & 0x70) | mf->Button_input & 0x70u) >> 31;
			DifferentAnimTable = 0x28D784;
		LABEL_40:
			{
				TheDirectionIndex = 4 * IsSamusFacingLeft;
				DifferentAnimTable = *((unsigned long*)&rawFile[DifferentAnimTable + samusDirection * 4]);
				//Now pointing to samus animarray.
				mf->Animtable = (SamusAnim*)((unsigned long*)&rawFile[DifferentAnimTable - 0x8000000] + TheDirectionIndex);

				GfxSizeTable = GetPointer(0x28D8DC) + ((mf->SamusPose * 4 + IsSamusFacingLeft) * 4);
				someIndex = TheDirectionIndex;
				goto SetOAM;
			}
		default:
			break;
		}
	}
	CopyIndex = 4 * IsSamusFacingLeft;
	PoseIndex = CopyIndex + 8 * mf->SamusPose;
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
		SamusAnim* curAnim = &mf->Animtable[i];//if thrown value is single digit, the pointer getter is bad
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
	HWND hWnd = SamusEditorClass::hwndSamusEditor;

	cboPose.Init(GetDlgItem(hWnd, cboPoseitem));
	cboCurFrame.Init(GetDlgItem(hWnd, IDC_COMBO6));
	chkHoldingMissiles.SetCnt(GetDlgItem(hWnd, chkHoldMissiles));

	cboEvent.Init(GetDlgItem(hWnd, IDC_COMBO8));
	chkFaceright.SetCnt(GetDlgItem(hWnd, chkFacingRIght));

	cboAbsorbX.Init(GetDlgItem(hWnd, IDC_COMBO9));
	for (int i = 0; i < 256;i++) {

		char buf[4] = { 0 };
		sprintf(buf, "%X", i);
		cboAbsorbX.Additem(buf);
		cboEvent.Additem(buf);
	}


	//Beams
	Beams_Wide.SetCnt(GetDlgItem(hWnd, chkWide));
	Beams_Charge.SetCnt(GetDlgItem(hWnd, chkCharge));
	Beams_Plasma.SetCnt(GetDlgItem(hWnd, chkPlasma));
	Beams_Wave.SetCnt(GetDlgItem(hWnd, chkWave));
	Beams_Ice.SetCnt(GetDlgItem(hWnd, chkIce));

	//Suits
	Suits_HiJump.SetCnt(GetDlgItem(hWnd, chkHiJump));
	Suits_SpeedBooster.SetCnt(GetDlgItem(hWnd, chkSpeedBooster));
	Suits_SpaceJump.SetCnt(GetDlgItem(hWnd, chkSpaceJump));
	Suits_ScrewAttack.SetCnt(GetDlgItem(hWnd, chkScrewAttack));
	Suits_Morphball.SetCnt(GetDlgItem(hWnd, chkMorphball));
	Suits_Varia.SetCnt(GetDlgItem(hWnd, chkVaria));
	Suits_Gravity.SetCnt(GetDlgItem(hWnd, chkGravity));
	Suits_SaxSuit.SetCnt(GetDlgItem(hWnd, chkSaxSuit));
	//Missiles

	Missiles_Missiles.SetCnt(GetDlgItem(hWnd, chkMissil));
	Missiles_Ice.SetCnt(GetDlgItem(hWnd, chkIceMissiles));
	Missiles_Super.SetCnt(GetDlgItem(hWnd, chkSuperMissiles));
	Missiles_Diffusion.SetCnt(GetDlgItem(hWnd, chkDiffusionMissiles));
	Missiles_Bombs.SetCnt(GetDlgItem(hWnd, chkBombs));
	Missiles_PowerBombs.SetCnt(GetDlgItem(hWnd, chkPBs));


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
	if (samusSprites.size() == 0) return;
	if (samusBuffer)
	{
		delete samusBuffer;
	}

	SamusBase* base = samusSprites[cboCurFrame.GetListIndex()];

	samusBuffer = new BackBuffer();
	samusBuffer->Create(1024, 1024);
	//you are updating sprites don't forget thx loser.

	sec->SpritePreview = base->theSprite;

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
void LoadSamus()
{

	string myText = cboPose.GetText(cboPose.GetListIndex());
	int pose = poseLookup[myText];
	Logic(pose);
	cboCurFrame.Clear();
	for (int i = 0; i < samusSprites.size();i++)
	{

		FusionSamus* thisMf = samusSprites[i];

		thisMf->GetOAM(i, samusDirection, facingDirections, CurrentPose, GfxSizeTable);
		thisMf->Load();
		thisMf->UnpackSamus();
		thisMf->DrawMe();
		sec->SetupSamusPreview(thisMf);
		char num[4] = { 0 };
		sprintf(num, "%d", i);
		cboCurFrame.Additem(num);
	}
	cboCurFrame.SetListIndex(0);
}
#include <GritNative.h>
LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT* pNumArgs);
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

		InitUI();
		mf = new FusionSamus();


		cboPose.SetListIndex(0);
		cboEvent.SetListIndex(0);
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
			LoadSamus();
			DrawSamus();
			InvalidateRect(hWnd, 0, true);
			break;
		}
		switch (LOWORD(wParam))
		{

		case IDC_COMBO6:
			DrawSamus();
			InvalidateRect(hWnd, 0, true);
			break;
		case cboPoseitem:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				LoadSamus();
				DrawSamus();
				InvalidateRect(hWnd, 0, true);
			}
			break;
		case IDC_BUTTON1:
		{
			BackBuffer* newImage = new BackBuffer();//64 * samusSprites.size(), 64);
			int wd = 64 + 2;
			int ht = 64 + 2;
			newImage->Create(wd * samusSprites.size(), ht);
			SetBkMode(newImage->DC(), TRANSPARENT);
			SetTextColor(newImage->DC(), RGB(255, 255, 255));

			newImage->Clear();
			int i = 0;
			for each (auto obj in samusSprites)
			{
				RECT            myrect;
				HBRUSH          curbrush = CreateSolidBrush((long)RGB(255, 255, 0));

				myrect.left = i * 66;
				myrect.top = 0;
				myrect.right = 66 + myrect.left;
				myrect.bottom = 66 + i * 66;
				FrameRect(newImage->DC(), &myrect, curbrush);
				myrect.left = i * 66 + 1;
				myrect.top = 0;
				myrect.right = 66 + myrect.left;
				myrect.bottom = 66 + i * 66 + 1;
				FrameRect(newImage->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
				obj->theSprite->PreviewSprite.GetFullImage()->Blit(newImage->DC(), (i)*wd + 2, 2, wd, ht, 0, 0); i++;
			}
			FILE* fp = fopen("F:/lol.bmp", "w+b");
			newImage->SaveToFile(fp);
			fclose(fp);
			delete newImage;
		}
		break;
		case cmdImport: {
			//Import the image with grit
			//we just need the GFX and pal.
			char* file = "C:\\Users\\Endeavor3\\Documents\\GitHub\\ShrekTroid\\shrekstanding.png";
			GritInterface* newInterface = new GritInterface(NULL);
			char param[1024];
			int numargs = 4;
			sprintf_s(param, 1024, "%s %s -gt -gB 4 -pn 32", file, file);
			char** params = CommandLineToArgvA(param, &numargs);
			//char radiostring[3][32] = { "-Ah 32 -Aw 32","-Ah 64 -Aw 32","-Ah 32 -Aw 64" };


			auto rec = newInterface->run_main(numargs, params);

			LocalFree(params);
			FILE* fp = fopen("C:\\DHTest\\filedump.bin", "w+");
			fwrite(rec->_gfxRec.data, 1, rec->_gfxRec.height * rec->_gfxRec.width, fp);
			fclose(fp);
			//Get the info
			int numSprites = rec->areaRight / (64);

			for (int spriteCounter = 0; spriteCounter < numSprites; spriteCounter++)
			{
				unsigned char* gfxtop = rec->_gfxRec.data + 256 * spriteCounter;
				unsigned char* bottomtop = rec->_gfxRec.data + (256 * spriteCounter) + rec->_gfxRec.height /(2* numSprites);;
				//So first all of them share the same pal. 
				FusionSamus* thisMf = samusSprites[spriteCounter];
				std::memcpy(thisMf->Suit_color, rec->_palRec.data, 2 * rec->_palRec.height);

				SamusPiece* pieces[] = { thisMf->SamusTop, thisMf->SamusBottom };
				for (int i = 0;i < 2;i++)//3 one day
				{
					pieces[i]->bottomLen = 2 * 8;
					pieces[i]->topLen = 2 * 8;

					if (pieces[i]->topGFX)
					{
						delete[] pieces[i]->topGFX;
					}
					if (pieces[i]->bottomGFX)
					{
						delete[] pieces[i]->bottomGFX;
					}
					pieces[i]->bottomGFX = new unsigned char[32 * 8 *4 ];
					pieces[i]->topGFX = new unsigned char[32 * 8 * 4];
					memset(pieces[i]->bottomGFX, 0,32 * 8 * 4);
					memset(pieces[i]->topGFX, 0, 32 * 8 *4);
					//Draw from bitmap to memory


					unsigned char* datPointer = pieces[i]->topGFX;
					memcpy(datPointer, gfxtop, 32 * 8);gfxtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, gfxtop, 32 * 8);gfxtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, gfxtop, 32 * 8);gfxtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, gfxtop, 32 * 8);gfxtop += numSprites * 8 * 32;datPointer += 8 * 32;

					datPointer = pieces[i]->bottomGFX;
					memcpy(datPointer, bottomtop, 32 * 8);bottomtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, bottomtop, 32 * 8);bottomtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, bottomtop, 32 * 8);bottomtop += numSprites * 8 * 32;datPointer += 8 * 32;
					memcpy(datPointer, bottomtop, 32 * 8);bottomtop += numSprites * 8 * 32;datPointer += 8 * 32;
				}

				//Draw oam

			//4 3 by 2 tiles
				OverAllOAM part1;
				memset(&part1, 0, sizeof(OverAllOAM));
				OverAllOAM part2;
				memset(&part2, 0, sizeof(OverAllOAM));
				OverAllOAM part3;
				memset(&part3, 0, sizeof(OverAllOAM));
				OverAllOAM part4;
				memset(&part4, 0, sizeof(OverAllOAM));


				OverAllOAM part5;
				memset(&part5, 0, sizeof(OverAllOAM));
				OverAllOAM part6;
				memset(&part6, 0, sizeof(OverAllOAM));
				OverAllOAM part7;
				memset(&part7, 0, sizeof(OverAllOAM));
				OverAllOAM part8;
				memset(&part8, 0, sizeof(OverAllOAM));
				OverAllOAM part9;
				memset(&part9, 0, sizeof(OverAllOAM));
				OverAllOAM part10;
				memset(&part10, 0, sizeof(OverAllOAM));
				thisMf->theSprite->OAM.clear();
				thisMf->theSprite->maxparts = 3;
			/*	FrameManager::PackOAM(&part1, 0,      -8, -16, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part2, 8,      -8, -8, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part3, 48,     -8, 0, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part4, 64,     -8,  8, 2, 2, 0, 0, 0);*/


				/*int xLeftPos = 8;
				FrameManager::PackOAM(&part1, 0 , xLeftPos, -16, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part2, 8 , xLeftPos, 0, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part3, 19, xLeftPos,  16, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part4, 32 , xLeftPos,  32, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part5, 48, xLeftPos, 48, 2, 2, 0, 0, 0);*/


				//FrameManager::PackOAM(&part1, 0 ,  8, -16, 2, 2, 0, 0, 0);	
				//FrameManager::PackOAM(&part2, 8 ,  8, -8, 2, 2, 0, 0, 0);
			    FrameManager::PackOAM(&part3, 40,  8,  0, 2, 2, 0, 0, 0);
			    FrameManager::PackOAM(&part4, 20,  8,  8, 2, 2, 0, 0, 0);
				FrameManager::PackOAM(&part5, 64 , 8, 16, 2, 2, 0, 0, 0);	
			//	FrameManager::PackOAM(&part5, 40,  8, 24, 2, 2, 0, 0, 0);
				

				//FrameManager::PackOAM(&part6, 10, 24, -8, 2, 2, 0, 0, 0);
				//FrameManager::PackOAM(&part7, 21, 24, 0, 2, 2, 0, 0, 0);
				//FrameManager::PackOAM(&part8, 34, 24, 8, 2, 2, 0, 0, 0);
				//FrameManager::PackOAM(&part9, 44, 8, 16, 2, 2, 0, 0, 0);
				//FrameManager::PackOAM(&part10, 46, 24, 16, 2, 2, 0, 0, 0);
				//thisMf->theSprite->OAM.push_back(part1);
			//	thisMf->theSprite->OAM.push_back(part2);
				thisMf->theSprite->OAM.push_back(part3);
				thisMf->theSprite->OAM.push_back(part4);
			    thisMf->theSprite->OAM.push_back(part5);
				//thisMf->theSprite->OAM.push_back(part6);
				//thisMf->theSprite->OAM.push_back(part7);
				//thisMf->theSprite->OAM.push_back(part8);
				//thisMf->theSprite->OAM.push_back(part9);
				//thisMf->theSprite->OAM.push_back(part10);
				thisMf->theSprite->PreviewSprite.RefreshImage();

				thisMf->Load();
				thisMf->DrawMe();
				thisMf->theSprite->PreviewSprite.RefreshImage();
			}
			delete rec;

		}
					  break;
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
		RECT b;
		memset(&b, 0, sizeof(RECT));
		b.left = 64;
		b.top = 1;
		b.bottom = height*3;
		b.right = width*3;

		FillRect(hdc, &b, (HBRUSH)(COLOR_WINDOW + 1));
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



