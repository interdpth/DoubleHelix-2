#include "resource.h"
#include "MemFile.h"
#include "FusionSamus.h"
#include "Image.h"
#include "GlobalVars.h"
#include "BackBuffer.h"
#include "SamusEditorClass.h"
#include <Windows.h>
#include "sCombo.h"
#include "sChecks.h"
#include "resource.h"

#include <map>
#define theTimer 22424242

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
void DrawSamus()
{
	if (samusBuffer)
	{
		delete samusBuffer;
	}
	SamusBase* base = mf;

	samusBuffer = new BackBuffer();
	samusBuffer->Create(512, 512);

	GlobalVars::gblVars->sec->SetupPreview(base);

	GFX = new Image();
	GFX->Create(256, 256);
	long pal[512] = { 0 };


	TileBuffer tb;
	tb.Load((unsigned char*)base->Sprite_tiles, 1023);

	GFX->SetPalette(GlobalVars::gblVars->sec->SpritePreview->PreviewPal);
	GFX->Clear();
	for (int i = 0; i < 1024; i++) {
		GFX->Draw(tb, ((i) % 32) * 8, ((i) / 32) * 8, i);
	}

}

#define lstSelchange 0x00010408
BOOL CALLBACK	SamusProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	int i = 0;
	char wndstr[256] = { 0 };

	int partIndex = 0;

	switch (message)
	{

	case WM_INITDIALOG:
		samusBuffer = NULL;
		curFrame = NULL;
		cboPose.Init(GetDlgItem(hWnd, cboPoseitem));
		InitUI();
		mf = new FusionSamus();

		GlobalVars::gblVars->sec->hwndSamusEditor = hWnd;
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
		switch (LOWORD(wParam))
		{
		case cboPoseitem:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				string myText = cboPose.GetText(cboPose.GetListIndex());
				mf->SamusPose = poseLookup[myText];
				mf->Logic();
				mf->Load();
				mf->PackSamus();
				DrawSamus();
				InvalidateRect(hWnd, 0, true);
			}
		}
		break;
	case WM_PAINT:
	{
		if (!GlobalVars::gblVars->sec->SpritePreview)
		{
			break;
		}

		hdc = BeginPaint(hWnd, &ps);
		int height = (GlobalVars::gblVars->sec->SpritePreview->Borders.bottom - GlobalVars::gblVars->sec->SpritePreview->Borders.top);
		int width = (GlobalVars::gblVars->sec->SpritePreview->Borders.right - GlobalVars::gblVars->sec->SpritePreview->Borders.left);
		//GlobalVars::gblVars->sec->SpritePreview->PreviewSprite.TransBlit
		//(hdc, 0, 0,
		//	width,
		//	height,
		//	0,
		//	0);

		StretchBlt(hdc, 50, 10, width * 2, height * 2, GlobalVars::gblVars->sec->SpritePreview->PreviewSprite.GetFullImage()->DC(), 0, 0, width, height, SRCCOPY);


		if (GFX)
		{
			BitBlt(hdc, 0, 100, 256, 256, GFX->DC(), 0, 0, SRCCOPY);
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
	HDC hdc;
	PAINTSTRUCT ps;

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

		//		oamEditor->SetupPreview(GBA.ROM, currentRomType, curFrame);
		//		oamEditor->LoadTiles(oamEditor->Tiles, curFrame);
		//		oamManager->DecodeOAM(GBA.ROM, globalVars->OAMED, curFrame->theSprite, curFrame->frameOffset - 0x8000000);
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



