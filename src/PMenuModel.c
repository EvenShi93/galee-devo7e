#include "Page.h"
#include "Func.h"
#include "Tx.h"

char PageMenuModelCur[10]="00��ģ��";
s16 PageMenuModelReset=0;
CMENUITEM MenuMod[]={
	{"ģ������"	,0,0,0,12,0,0,0},
	{"ѡ��:"	,PageModelSel			, 0,0,0,0,0,PageMenuModelCur},
	{"����:"	,PageEditName			, 0,0,0,0,0,Model.Name},
	{"����:"	,0,PageOptionModType	, 0,1,&Model.Type,0,0},
	{"ͼ��:"	,PageIconSel			, 0,0,0,0,0,Model.Icon},
	{"��Ƶ����" ,PageMenuRf,0,0,0,0,0,0},
	{"ͨ��ӳ��"	,PageMenuChMap,0,0,0,0,0,0},
	{"��ʱ��1"	,PageTimerSet1,0,0,0,0,0,0},
	{"��ʱ��2"	,PageTimerSet2,0,0,0,0,0,0},
	{"��ʱ��3"	,PageTimerSet3,0,0,0,0,0,0},
	{"PPM ����" ,PagePpmIn,0,0,0,0,0,0},
	{"����ѡ��" ,PageSwDef,0,0,0,0,0,0},
	{"����ģ��" ,0,PageOptionOkExe,0,2,&PageMenuModelReset,0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�ͻ������ò˵��������
//
void PageMenuModMarkNo(void)
{
	PageMenuModelCur[0]=TxSys.ModelNo/10+'0';
	PageMenuModelCur[1]=TxSys.ModelNo%10+'0';
}

u32 PageMenuMod(u8 event)
{
	static MENUSTAT ms;
	
	PageMenuModMarkNo();
		
	if(event==PV_INIT)
	{
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuMod[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
		
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMod,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{			
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuMod[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMod[ms.iFocus+1].SubItem,PV_INIT);
		}		
		
		//��λ����
		if(MenuMod[ms.iFocus+1].pValue==&PageMenuModelReset)
		{
			if(PageMenuModelReset==1)
			{
				PageMenuModelReset=0;
				memcpy(&Model,&ModelDef,sizeof(Model));
				Model.Name[5]=TxSys.ModelNo/10+'0';
				Model.Name[6]=TxSys.ModelNo%10+'0';
				Model.Name[7]=0;
				PageAlert("ģ��������!",1000,PV_INIT);
			}
			if(PageMenuModelReset==2)
			{
				PageMenuModelReset=0;
				ModelFormat();
				PageAlert("����ģ��������!",1000,PV_INIT);
			}
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