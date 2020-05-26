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
	//////unsigned char bytes[4];

	////unsigned long data = Scrolls[s].unkData;
	////memcpy(&bytes, &data, 4);


	////sprintf(txtbuffer, "%X", bytes[0]);
	////SetWindowText(GetDlgItem(ScrollWIn, txtByte1), txtbuffer);

	////sprintf(txtbuffer, "%X", bytes[1]);
	////SetWindowText(GetDlgItem(ScrollWIn, txtByte2), txtbuffer);

	////sprintf(txtbuffer, "%X", bytes[2]);
	////SetWindowText(GetDlgItem(ScrollWIn, txtByte3), txtbuffer);

	////sprintf(txtbuffer, "%X", bytes[3]);
	////SetWindowText(GetDlgItem(ScrollWIn, txtByte4), txtbuffer);

	//sprintf(txtbuffer, "%X", data);
	//SetWindowText(GetDlgItem(ScrollWIn, txtLong), txtbuffer);

}


