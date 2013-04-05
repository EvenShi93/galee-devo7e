#include "Page.h"
#include "Func.h"

CMENUITEM MenuDeltaSet[]={
	{"��������"	,0,0,0,5,0,0,0},
	{"��ؿ���:"	,0,PageOptionOnOff,0,1,&Model.Delta.Enable,0,0},
	{"����\2����:"	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2A,0,0},
	{"����\2����:"	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2A,0,0},
	{"����\2����:"	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2E,0,0},
	{"����\2����:"	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2E,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageDeltaSet(u8 event)
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
		ms.Total=MenuDeltaSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.Delta.Enable)	ms.Total=MenuDeltaSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LcdClear(0);
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuDeltaSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		//Vβ��غ�������/�����ػ���
		if(Model.Delta.Enable) 
		{
			Model.Vtail.Enable=0;
			Model.FlpAil.Enable=0;
		}
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}