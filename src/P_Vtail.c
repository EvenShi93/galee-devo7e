#include "Page.h"
#include "Func.h"

CMENUITEM MenuVtailSet[]={
	{" V β ���"	,0,0,0,5,0,0,0},
	{"��ؿ���:"	,0,PageOptionOnOff,0,1,&Model.Vtail.Enable,0,0},
	{"����\2����:"	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2E,0,0},
	{"����\2����:"	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2E,0,0},
	{"����\2����:"	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2R,0,0},
	{"����\2����:"	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2R,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageVtailSet(u8 event)
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
		ms.Total=MenuVtailSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.Vtail.Enable)	ms.Total=MenuVtailSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LcdClear(0);
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuVtailSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		//Vβ��غ��������ػ���
		if(Model.Vtail.Enable) Model.Delta.Enable=0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}