#include "Page.h"
#include "Func.h"
#include "Tx.h"

s16 PageMenuRfProto;
s16 PageMenuRfChNum;
extern u32 PageEditIdValue;
char PageMenuRfBindTitle[16];
CMENUITEM MenuRf[]={
	{"��Ƶ����",0,0,0,5,0,0,0},
	{PageMenuRfBindTitle,PageBind,0,0,0,0,0,0},
	{"ͨѶЭ��:",0,PageOptionProtocol	, 0,9,&PageMenuRfProto,0,0},
	{"ͨ �� ��:",0,0,4,8,&PageMenuRfChNum,0,0},
	{"���书��:",0,PageOptionRfPwr		, 0,7,&Model.RfPwr,0,0},
	{"ʶ �� ��:",PageEditId,0,0,0,0,0,Model.RfIdStr},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�ͻ������ò˵��������
//
void PageMenuRfIdStr(void)
{
	if(PageEditIdValue==0)
	{
		strcpy(Model.RfIdStr,"(�Զ�)");
		return;
	}
	
	u8 i;
	u32 mask=100000;
	for(i=0;i<MODELIDL;i++)
	{
		Model.RfIdStr[i]=PageEditIdValue/mask%10+'0';	
		mask/=10;
	}
}

u32 PageMenuRf(u8 event)
{
	static u8 CfgChanged;
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuRf[0].Max;
		ms.DrawMask=PD_ALL;	
		PageMenuRfProto=Model.Protocol;
		PageMenuRfChNum=Model.RfChNum;
		PageEditIdValue=Model.RfId;
		CfgChanged=1;
		return 1;
	}
	
	//������ñ仯
	if(PageMenuRfProto!=Model.Protocol || PageMenuRfChNum!=Model.RfChNum || PageEditIdValue!=Model.RfId)
	{
		if(!CfgChanged) 
		{
			strcpy(PageMenuRfBindTitle,"�����Ƶ����");
			ms.DrawMask=PD_ALL;	
		}
		CfgChanged=1;		
	}
	else
	{
		if(CfgChanged)
		{
			strcpy(PageMenuRfBindTitle,"��������");
			ms.DrawMask=PD_ALL;	
		}
		CfgChanged=0;
	}
	
	//��ID�ַ���
	PageMenuRfIdStr();
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuRf,&ms);
	
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{			
		//�������
		if(MenuRf[ms.iFocus+1].SubItem==PageBind)
		{
			if(CfgChanged)
			{
				Model.RfId=PageEditIdValue;
				Model.Protocol=PageMenuRfProto;
				Model.RfChNum=PageMenuRfChNum;
				TxLoad(Model.Protocol);
			}
			else
			{
				BeepMusic(MusicEnter);	
				PageEnter(PageBind,PV_INIT);
			}
		}		
		if(MenuRf[ms.iFocus+1].SubItem==PageEditId)
		{
			BeepMusic(MusicEnter);
			PageEnter(PageEditId,PV_INIT);
		}
	}	
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		if(CfgChanged)
		{
			PageAlert("û�б���Ķ�!",1000,PV_END);
		}
		else PageReturn(PV_INIT);
	}
	
	if(event==PV_END)
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}