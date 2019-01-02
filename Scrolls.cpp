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


