#include "Func.h"
#include "Page.h"

TXSYS TxSysBak;
///////////////////////////////////////////////////////////////////////////////////////
//
// ���ö�д����
//

int fputbuf(void *buf,int n,FILE *f)
{
	int i;
	u8 *p=(u8*)buf;
	for(i=0;i<n;i++)
	{
		if(fputc(*p++,f)==-1)		break;
	}
	return i;
}

void LoadCfg(void)
{
	FILE *fcfg=fopen("tx.bin","rb");	
	
	TxSysBak.Mark=0;
	if(fcfg)
	{
		setbuf(fcfg,0);	
		if(fread(&TxSysBak,sizeof(TxSysBak),1,fcfg)!=1)
		{
			TxSysBak.Mark=0;
		}
		fclose(fcfg);
	}	
	
	if(TxSysBak.Mark!=TXCFGMARK)
	{
		memcpy(&TxSys,&TxSysDef,sizeof(TxSys));
		PageAlertModel("ʹ��Ĭ������...",300);
	}	
	else
	{
		memcpy(&TxSys,&TxSysBak,sizeof(TxSys));
	}
}


void SaveCfg(void)
{	
	if(!memcmp(&TxSysBak,&TxSys,sizeof(TxSys)))	return;
	
	PageAlertModel("����ң��������...",300);
	
	FILE *fcfg=fopen("tx.bin","wb");	
	if(fcfg)
	{
		if(fputbuf(&TxSys,sizeof(TxSys),fcfg)!=sizeof(TxSys))
		{
			PageAlertModel("�޷�д��TX.BIN!",0);
		}
		fclose(fcfg);
	}
	else
	{
		PageAlertModel("�޷���TX.BIN!",0);
	}
	
	//���汸���Ա��Ƚ�
	memcpy(&TxSysBak,&TxSys,sizeof(TxSys));
}