#include "Page.h"
#include "Func.h"

CMENUITEM MenuDelaySet[]={
	{"�������"	,0,0,0,8,0,0,0},
	{"ͨ��1:"	,0,0	, 0,DELAY_MAX,&Model.Delay[0],0,0},
	{"ͨ��2:"	,0,0	, 0,DELAY_MAX,&Model.Delay[1],0,0},
	{"ͨ��3:"	,0,0	, 0,DELAY_MAX,&Model.Delay[2],0,0},
	{"ͨ��4:"	,0,0	, 0,DELAY_MAX,&Model.Delay[3],0,0},
	{"ͨ��5:"	,0,0	, 0,DELAY_MAX,&Model.Delay[4],0,0},
	{"ͨ��6:"	,0,0	, 0,DELAY_MAX,&Model.Delay[5],0,0},
	{"ͨ��7:"	,0,0	, 0,DELAY_MAX,&Model.Delay[6],0,0},
	{"ͨ��8:"	,0,0	, 0,DELAY_MAX,&Model.Delay[7],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ������ٲ˵��������
//
u32 PageDelaySet(u8 event)
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
		ms.Total=MenuDelaySet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuDelaySet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}