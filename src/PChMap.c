#include "Page.h"
#include "Func.h"

CMENUITEM *MenuChMap;
CMENUITEM MenuChMapA[]={
	{"�̶���ͨ��ӳ��"	,0,0,0,8,0,0,0},
	{"ͨ��1:",0,PageOptionChSelA		, 0,16,&Model.ChMap[0],0,0},
	{"ͨ��2:",0,PageOptionChSelA		, 0,16,&Model.ChMap[1],0,0},
	{"ͨ��3:",0,PageOptionChSelA		, 0,16,&Model.ChMap[2],0,0},
	{"ͨ��4:",0,PageOptionChSelA		, 0,16,&Model.ChMap[3],0,0},
	{"ͨ��5:",0,PageOptionChSelA		, 0,16,&Model.ChMap[4],0,0},
	{"ͨ��6:",0,PageOptionChSelA		, 0,16,&Model.ChMap[5],0,0},
	{"ͨ��7:",0,PageOptionChSelA		, 0,16,&Model.ChMap[6],0,0},
	{"ͨ��8:",0,PageOptionChSelA		, 0,16,&Model.ChMap[7],0,0},
};

CMENUITEM MenuChMapH[]={
	{"ֱ����ͨ��ӳ��"	,0,0,0,8,0,0,0},
	{"ͨ��1:",0,PageOptionChSelH		, 0,16,&Model.ChMap[0],0,0},
	{"ͨ��2:",0,PageOptionChSelH		, 0,16,&Model.ChMap[1],0,0},
	{"ͨ��3:",0,PageOptionChSelH		, 0,16,&Model.ChMap[2],0,0},
	{"ͨ��4:",0,PageOptionChSelH		, 0,16,&Model.ChMap[3],0,0},
	{"ͨ��5:",0,PageOptionChSelH		, 0,16,&Model.ChMap[4],0,0},
	{"ͨ��6:",0,PageOptionChSelH		, 0,16,&Model.ChMap[5],0,0},
	{"ͨ��7:",0,PageOptionChSelH		, 0,16,&Model.ChMap[6],0,0},
	{"ͨ��8:",0,PageOptionChSelH		, 0,16,&Model.ChMap[7],0,0},
};
///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ��ͨ��ӳ�䴦�����
//
u32 PageMenuChMap(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		if(Model.Type==MT_AERO)	MenuChMap=MenuChMapA;
		else					MenuChMap=MenuChMapH;
		
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuChMap[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuChMap,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuChMap[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuChMap[ms.iFocus+1].SubItem,PV_INIT);
		}		
	}
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}