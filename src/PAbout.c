#include "Page.h"
#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
u32 PageAbout(u8 event)
{	
	if(event==PV_INIT)
	{
		LcdClear(0);
		LcdDrawBmpFile(0,0,"res/about.bmp");
		return 1;
	}	
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT) || KeyTstDown(KEY_ENT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}