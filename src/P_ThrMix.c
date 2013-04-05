#include "Page.h"
#include "Func.h"

CMENUITEM MenuThrMixSet[]={
	{"����\2ƽ����"	,0,0,0,3,0,0,0},
	{"����Ť-����:"	,0,0	, -MAX_THM,+MAX_THM,&Model.ThrMix[0],0,0},
	{"��ƫ��-����:"	,0,0	, -MAX_THM,+MAX_THM,&Model.ThrMix[1],0,0},
	{"�����-����:"	,0,0	, -MAX_THM,+MAX_THM,&Model.ThrMix[2],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ���Ż�ز˵��������
//
u32 PageThrMixSet(u8 event)
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
		ms.Param=0xff;
		ms.Total=MenuThrMixSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
		
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuThrMixSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}