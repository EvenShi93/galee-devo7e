#include "Page.h"
#include "Func.h"

CMENUITEM MenuFlpAilSet[]={
	{"������"	,0,0,0,5,0,0,0},
	{"��ؿ���:"	,0,PageOptionOnOff,0,1,&Model.FlpAil.Enable,0,0},
	{"����\2����:"	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2A,0,0},
	{"����\2����:"	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2F,0,0},
	{"����\2����:"	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2A,0,0},
	{"����\2����:"	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2F,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageFlpAilSet(u8 event)
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
		ms.Total=MenuFlpAilSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.FlpAil.Enable)	ms.Total=MenuFlpAilSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LcdClear(0);
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuFlpAilSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		//�����غ��������ػ���
		if(Model.FlpAil.Enable) Model.Delta.Enable=0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}