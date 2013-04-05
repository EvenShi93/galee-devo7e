#include "Page.h"
#include "Func.h"
#include "Tx.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
const char PageBindBmpFile[]="res/bind1.bmp";
u32 PageBind(u8 event)
{
	u16 x;
	char iconfile[16];
	static u32 secclk;
	static u8  iconidx;
	static u32 BindCnt;
	
	if(event==PV_INIT)
	{
		LcdClear(0);
		LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  
		x=LcdDrawText(3,0,(char*)PageOptionProtocol[Model.Protocol]);
		LcdDrawText(x+2,0,"\6\2���ڶ���");		
		LcdDrawHLine(0,128,14,1);
		LcdDrawHLine(0,128,15,1);
		LcdDrawBmpFile(52,22,PageBindBmpFile);
		LcdDrawStop();
		
		iconidx=1;
		BindCnt=TxBind();
		
		secclk=SysTimerClk+250;
		
		return 1;
	}	
	
	//����0.25�붨ʱ��
	if(SysTimerClk>secclk)
	{
		secclk+=250;
		
		iconidx++;
		if(iconidx>4) iconidx=1;
		strcpy(iconfile,PageBindBmpFile);
		iconfile[8]='0'+iconidx;
		LcdDrawBmpFile(52,22,iconfile);
		
		if(TxBindCnt!=TX_BIND_WAIT)
		{
			#define BINDBAR_W	100
			u32 per=BINDBAR_W-TxBindCnt*BINDBAR_W/BindCnt;
			
			LcdSetXy((128-BINDBAR_W)/2,6);
			for(u32 i=0;i<BINDBAR_W;i++)
			{
				LcdWriteLine(i>per?0x81:0xff);
			}
			LcdWriteLine(0xff);
		}
	}
	
	//����ʱ�����
	if(TxBindCnt==0)
	{
		PageReturn(PV_REDRAW);
	}

	//�˳�����	
	if(KeyTstDown(KEY_EXT))
	{
		TxBindCnt=0;//ǿ��ֹͣ���룬���Ƕ�����Э�鶼��Ч
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}