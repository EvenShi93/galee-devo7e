#include "Page.h"
#include "Func.h"

CMENUITEM MenuMain[]={
	{"GALEE DEVO-7E",0,0,0,6,0,0,0},
	{"ģ�Ͳ�������"	,PageAeroSet,0,0,0,0,0,0},
	{"ͨ��������"	,PageMonitor,0,0,0,0,0,0},
	{"ģ�͸߼�����"	,PageMenuMod,0,0,0,0,0,0},
	{"ң��������"	,PageMenuSys,0,0,0,0,0,0},
	{"USB����   "	,PageUsb,0,0,0,0,0,0},
	{"����...   "	,PageAbout,0,0,0,0,0,0},	
};
///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
u32 PageMenuMain(u8 event)
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
		ms.Total=MenuMain[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}	
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMain,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuMain[ms.iFocus+1].SubItem==PageAeroSet)//ģ������
		{
			BeepMusic(MusicEnter);
			if(Model.Type==MT_AERO)	PageEnter(PageAeroSet,PV_INIT);
			else					PageEnter(PageHeliSet,PV_INIT);
		}
		else if(MenuMain[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);
			PageEnter((PAGEPROC)MenuMain[ms.iFocus+1].SubItem,PV_INIT);
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