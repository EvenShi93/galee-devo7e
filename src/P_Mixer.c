#include "Page.h"
#include "Func.h"

char MenuMixerDeltaStat[6];
char MenuMixerVtailStat[6];
char MenuMixerFlpAilStat[6];

CMENUITEM MenuMixerSet[]={
	{"�������"	,0,0,0,5,0,0,0},
	{"��������",PageDeltaSet,0,0,0,0,0,MenuMixerDeltaStat},
	{" V β ���",PageVtailSet,0,0,0,0,0,MenuMixerVtailStat},
	{"������",PageFlpAilSet,0,0,0,0,0,MenuMixerFlpAilStat},
	{"����\2ƽ����"  ,PageThrMixSet,0,0,0,0,0,0},
	{"ת��\2�������"  ,PageYawMixSet,0,0,0,0,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��ز˵��������
//
u32 PageMixerSet(u8 event)
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
		ms.Total=MenuMixerSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//���ø����״̬
	strcpy(MenuMixerVtailStat,PageOptionOnOff[Model.Vtail.Enable]);
	strcpy(MenuMixerDeltaStat,PageOptionOnOff[Model.Delta.Enable]);
	strcpy(MenuMixerFlpAilStat,PageOptionOnOff[Model.FlpAil.Enable]);
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMixerSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuMixerSet[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMixerSet[ms.iFocus+1].SubItem,PV_INIT);
		}				
	}	
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}