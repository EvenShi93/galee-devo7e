#include "Page.h"
#include "Func.h"

CMENUITEM MenuNeuSet[]={
	{"�е����"	,0,0,0,8,0,0,0},
	{"ͨ��1:"	,0,0	, -100,+100,&Model.Neu[0],0,0},
	{"ͨ��2:"	,0,0	, -100,+100,&Model.Neu[1],0,0},
	{"ͨ��3:"	,0,0	, -100,+100,&Model.Neu[2],0,0},
	{"ͨ��4:"	,0,0	, -100,+100,&Model.Neu[3],0,0},
	{"ͨ��5:"	,0,0	, -100,+100,&Model.Neu[4],0,0},
	{"ͨ��6:"	,0,0	, -100,+100,&Model.Neu[5],0,0},
	{"ͨ��7:"	,0,0	, -100,+100,&Model.Neu[6],0,0},
	{"ͨ��8:"	,0,0	, -100,+100,&Model.Neu[7],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageNeuSet(u8 event)
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
		ms.Total=MenuNeuSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuNeuSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}