#include "MainHeader.h"
#include "resource.h"
#include "clsRoomScrolls.h"
void LoadScrollControls( Scroller *scroll)
{
	int i = 0;
	char cBuf[100];
	cboScroll.Clear();
	if (scroll->Number >0) {
		cboScroll.Enable();
		for (i = 0; i < scroll->Number; i++) {
			sprintf(cBuf, "%x", i);
			cboScroll.Additem(cBuf);

		}
		cboScroll.SetListIndex(0);
	}
	else {

		cboScroll.Disable();
	}
}


void LoadScrollInfo(int s,  Scroller *scroll) {
	char txtbuffer[1024];
	unsigned char bytes[4];

	unsigned long data = scroll->Scrolls[s].unkData;
	memcpy(&bytes, &data, 4);


	sprintf(txtbuffer, "%x", bytes[0]);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte1), txtbuffer);

	sprintf(txtbuffer, "%x", bytes[1]);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte2), txtbuffer);

	sprintf(txtbuffer, "%x", bytes[2]);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte3), txtbuffer);

	sprintf(txtbuffer, "%x", bytes[3]);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte4), txtbuffer);

	sprintf(txtbuffer, "%x", data);
	SetWindowText(GetDlgItem(ScrollWIn, txtLong), txtbuffer);

}
int DrawScrolls(int ScrollToDraw, Scroller *scroll) {

	RECT blah;

	MousePointer thisscroll;

	if (scroll->Room != 0xFF) {


		thisscroll = scroll->Scrolls[ScrollToDraw].rect;



		if ((thisscroll.sX != 0xFF) && (thisscroll.Height != 0xFF)) {








			HBRUSH curbrush = CreateSolidBrush(
				RGB(
					255 -
					((ScrollToDraw + 3) << 2) | 128,
					255 - ((ScrollToDraw + 2) << 4) | 128,
					255 - ((ScrollToDraw + 1) << 8) | 128
					)
				);


			for (int d = 0; d<2; d++) {
				blah.left = (thisscroll.sX) * 16 + d;
				blah.top = (thisscroll.sY) * 16 + d;
				blah.right = (thisscroll.Width) * 16 + d;
				blah.bottom = (thisscroll.Height) * 16 + d;
				FrameRect(RD1Engine::theGame->ThisBackBuffer.DC(), &blah, curbrush);

			}




			DeleteObject(curbrush);

			TextOut(RD1Engine::theGame->ThisBackBuffer.DC(),
				((thisscroll.sX)+(((thisscroll.Width - thisscroll.sX) / 2)))*(16),
				((thisscroll.sY)+(((thisscroll.Height - thisscroll.sY) / 2)))*(16), "Sc", 1);

		}



	}
	return 0;
}//End Function


