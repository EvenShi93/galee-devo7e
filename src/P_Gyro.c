#include "Page.h"
#include "Func.h"

CMENUITEM MenuGyroSet[]={
	{"����������"	,0,0,0,2,0,0,0},
	{"����/����:"	,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[0],0,0},
	{"�ؼ�:"		,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[1],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  �����ǲ˵�����
//
u32 PageGyroSet(u8 event)
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
		ms.Total=MenuGyroSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuGyroSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}