#include "MainHeader.h"
#include "resource.h"
#include "clsRoomScrolls.h"
void clsRoomScrolls::LoadScrollControls()
{
	int i = 0;
	char cBuf[100];
	cboScroll.Clear();
	if (Scrolls.size() >0) {
		cboScroll.Enable();
		for (i = 0; i < Scrolls.size(); i++) {
			sprintf(cBuf, "%X", i);
			cboScroll.Additem(cBuf);

		}
		cboScroll.SetListIndex(0);
	}
	else {

		cboScroll.Disable();
	}
}


void clsRoomScrolls::LoadScrollInfo(int s) {
	char txtbuffer[1024];
	
	unsigned short data = Scrolls[s]->unkData1;

	sprintf(txtbuffer, "%X", Scrolls[s]->unkData1);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte1), txtbuffer);

	sprintf(txtbuffer, "%X", Scrolls[s]->unkData2);
	SetWindowText(GetDlgItem(ScrollWIn, txtByte2), txtbuffer);	

}


