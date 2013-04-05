#include "Page.h"
#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  USBģʽ
//
u32 PageUsb(u8 event)
{
	if(event==PV_INIT)
	{
		USB_Enable(1);

		LcdClear(0);
		LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  
		LcdDrawText(3,0,"ң����������ΪU��");
		LcdDrawHLine(0,128,14,1);
		LcdDrawHLine(0,128,15,1);
		LcdDrawBmpFile(8,28,"res/tf.bmp");
		LcdDrawText(40,32,"��EXT���˳�");
		LcdDrawStop();
	
		PowerLight(20);//��дU��ʱ�򱳹�ƻ���˸�����ù̶�ֵ
		
		return 1;
	}	
	
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		USB_Disable(1);
		FS_Mount();
		
		PowerLight(TxSys.Light*8);
		
		PageAlert("U���ѶϿ�...",1000,PV_END);
	}
	
	if(event==PV_END)
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}