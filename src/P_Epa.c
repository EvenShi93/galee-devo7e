#include "Page.h"
#include "Func.h"

CMENUITEM MenuEpaSet[]={
	{"�г�����"	,0,0,0,16,0,0,0},
	{"ͨ��1 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[0][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[0][1],0,0},
	{"ͨ��2 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[1][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[1][1],0,0},
	{"ͨ��3 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[2][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[2][1],0,0},
	{"ͨ��4 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[3][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[3][1],0,0},
	{"ͨ��5 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[4][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[4][1],0,0},
	{"ͨ��6 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[5][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[5][1],0,0},
	{"ͨ��7 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[6][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[6][1],0,0},
	{"ͨ��8 ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[7][0],0,0},
	{"      ��:"	,0,0	, 0,+MAX_EPA,&Model.Epa[7][1],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageEpaSet(u8 event)
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
		ms.Total=MenuEpaSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuEpaSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}