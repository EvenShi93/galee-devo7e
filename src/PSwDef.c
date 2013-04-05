#include "Page.h"
#include "Func.h"

CMENUITEM MenuSwDef[]={
	{"ѡ�񿪹�"	,0,0,0,7,0,0,0},
	{"��������:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_HOLD],0,0},
	{"�ؼ�����:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_IDLE],0,0},
	{"��С��:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_DR],0,0},
	{"�����:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_GEAR],0,0},
	{"��  ��:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_FLAP],0,0},
	{"����ͨ��1:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_AUX1],0,0},
	{"����ͨ��2:",0,PageOptionSwDef		, 0,4,&Model.SwDef[SWD_AUX2],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ϳ���ѡ�������
//
u32 PageSwDef(u8 event)
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
		ms.Total=MenuSwDef[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSwDef,&ms);
		
	//�س������� ���ؼ�����
	if(KeyTstDown(KEY_ENT) || KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}