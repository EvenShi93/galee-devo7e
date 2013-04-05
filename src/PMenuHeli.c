#include "Page.h"
#include "Func.h"

CMENUITEM MenuHeliSet[]={
	{"ֱ������������"	,0,0,0,11,0,0,0},
	{"��������"	,PageNorRevSet		, 0,0,0,0,0,0},
	{"�г�����"	,PageEpaSet			, 0,0,0,0,0,0},
	{"�е����"	,PageNeuSet			, 0,0,0,0,0,0},
	{"������"	,PageGyroSet		, 0,0,0,0,0,0},
	{"ҡ������"	,PageCurveSet		, 0,1,9,0,0,0},
	{"��������"	,PageCurveSet		, 0,2,2,0,0,0},
	{"�ݾ�����"	,PageCurveSet		, 0,3,3,0,0,0},
	{"��������:",0,0,-50,50,&Model.ThrHold,0,0},
	{"б�̻��"	,PageSwashSet		, 0,0,0,0,0,0},
	{"����ͨ��" ,PageAuxChSet,0,0,0,0,0,0},
	{"�������"	,PageDelaySet		, 0,0,0,0,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageHeliSet(u8 event)
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
		ms.Total=MenuHeliSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuHeliSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuHeliSet[ms.iFocus+1].SubItem)
		{
			if(MenuHeliSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt	=MenuHeliSet[ms.iFocus+1].Max;
				if(MenuHeliSet[ms.iFocus+1].Min==1)
				{
					PageCurveTitle	=PageStkCurveTitle;
					PageCurveValue	=&Model.StkCurve[0][0][0];
				}
				else if(MenuHeliSet[ms.iFocus+1].Min==2)
				{
					PageCurveTitle	=PageThrCurveTitle;
					PageCurveValue	=&Model.ThrCurve[0][0];
				}
				else
				{
					PageCurveTitle	=PagePitCurveTitle;
					PageCurveValue	=&Model.PitCurve[0][0];
				}
			}
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuHeliSet[ms.iFocus+1].SubItem,PV_INIT);
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