#include "Page.h"
#include "Func.h"

CMENUITEM MenuSwashSet[]={
	{"б������"	,0,0,0,4,0,0,0},
	{"б������:"	,0,PageOptionSwash,0,3,&Model.SwashType,0,0},
	{"�����ر�:",0,0, -100,+100,&Model.Swash[0],0,0},
	{"������ر�:",0,0, -100,+100,&Model.Swash[1],0,0},
	{"�ݾ��ر�:",0,0, -100,+100,&Model.Swash[2],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  �����ǲ˵�����
//
u32 PageSwashSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuSwashSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSwashSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}