#include "Page.h"
#include "Func.h"

s16 PageMenuSysReset=0;
s16 PageMenuSysAutoOff=0;
char PageMenuSysVol[6];
CMENUITEM MenuSys[]={
	{"ң��������",0,0,0,15,0,0,0},
	{"ҡ��У׼"	,PageStkCal,0,0,0,0,0,0},
	{"ҡ��ģʽ:"	,0,PageOptionStkType	, 0, 3,&TxSys.StkType,0,0},
	{"ҡ������:"	,0,0					, 0,50,&TxSys.StkDa,0,0},
	{"����ʾ:"	,0,PageOptionOnOff		, 0, 1,&TxSys.Vibrator,0,0},
	{"������ʾ:"	,0,PageOptionOnOff		, 0, 1,&TxSys.Music,0,0},
	{"��������:"	,0,0					, 0,10,&TxSys.KeyBeep,0,0},
	{"��������:"	,0,0					, 0,30,&TxSys.KeyTone,0,0},
	{"��������:"	,0,0					, 0,10,&TxSys.Light,0,0},
	{"�� �� ��:"	,0,0					, 0,10,&TxSys.Contrast,0,0},
	{"����ر�:"	,0,PageOptionLightOff	, 0, 6,&TxSys.LightOff,0,0},
	{"�������:"	,0,PageOptionBatType	, 0, 3,&TxSys.BatType,0,0},
	{"������ѹ:"	,0,0					,33,120,&TxSys.BatWarn,0,PageMenuSysVol},
	{"���о���:"	,0,PageOptionFreeWarn	, 0, 5,&TxSys.FreeWarn,0,0},
	{"�Զ��ػ�:"	,0,PageOptionAutoOff	, 0, 3,&PageMenuSysAutoOff,0,0},
	{"�ָ�Ĭ��ֵ"	,0,PageOptionOkExe		, 0, 1,&PageMenuSysReset,0,0},
};
///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
u32 PageMenuSys(u8 event)
{
	static MENUSTAT ms;
		
	//��ʼ���������
	if(event==PV_INIT)
	{
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuSys[0].Max;
		ms.DrawMask=PD_ALL;	
		PageMenuSysAutoOff=TxSys.AutoOff;
		return 1;
	}
	
	//����ѹתΪ�ַ���
	PageMenuSysVol[0]=TxSys.BatWarn>=100?('0'+TxSys.BatWarn/100):(' ');
	PageMenuSysVol[1]=TxSys.BatWarn>=10?('0'+TxSys.BatWarn/10%10):(' ');
	PageMenuSysVol[2]='.';	
	PageMenuSysVol[3]=TxSys.BatWarn%10+'0';	
	PageMenuSysVol[4]='V';
	PageMenuSysVol[5]=0;
		
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSys,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuSys[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuSys[ms.iFocus+1].SubItem,PV_INIT);
		}
		//��λ����
		if(MenuSys[ms.iFocus+1].pValue==&PageMenuSysReset)
		{
			if(PageMenuSysReset==1)
			{
				PageMenuSysReset=0;
				memcpy(&TxSys,&TxSysDef,sizeof(TxSys));
				PageAlert("ң�����ѻָ�Ĭ��ֵ!",1000,PV_REDRAW);
			}
		}
		
	}
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	//�Զ��ػ�����
	if(PageMenuSysAutoOff!=TxSys.AutoOff)
	{
		TxSys.AutoOff=PageMenuSysAutoOff;
		AutoOffReset();
	}	
	
	return 0;
}