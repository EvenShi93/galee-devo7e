#include "Page.h"
#include "Func.h"

CMENUITEM MenuAeroSet[]={
	{"�̶����������"	,0,0,0,9,0,0,0},
	{"��������"	,PageNorRevSet		, 0,0,0,0,0,0},
	{"�г�����"	,PageEpaSet			, 0,0,0,0,0,0},
	{"�е����"	,PageNeuSet			, 0,0,0,0,0,0},
	{"ҡ������"	,PageCurveSet		, 0,1,9,0,0,0},
	{"��������"	,PageCurveSet		, 0,2,1,0,0,0},
	{"��������:",0,0,-50,50,&Model.ThrHold,0,0},
	{"����ͨ��",PageAuxChSet,0,0,0,0,0,0},
	{"�������",PageMixerSet,0,0,0,0,0,0},
	{"�������"	,PageDelaySet			, 0,0,0,0,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageAeroSet(u8 event)
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
		ms.Total=MenuAeroSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuAeroSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuAeroSet[ms.iFocus+1].SubItem)
		{
			if(MenuAeroSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt	=MenuAeroSet[ms.iFocus+1].Max;
				if(MenuAeroSet[ms.iFocus+1].Min==1)
				{
					PageCurveTitle	=PageStkCurveTitle;
					PageCurveValue	=&Model.StkCurve[0][0][0];
				}
				else
				{
					PageCurveTitle	=PageThrCurveTitle;
					PageCurveValue	=&Model.ThrCurve[0][0];
				}
			}
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuAeroSet[ms.iFocus+1].SubItem,PV_INIT);
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