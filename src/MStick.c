#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ADC���ݽṹ
//
u16 StickRaw[STK_NUM];
s16 StickCal[STK_NUM];
s16 StickValue[STK_NUM];
s16 StickTrim[STK_NUM];

	//ҡ��ģʽӳ��
const u8 StickMap[4][4]={
	{0,1,2,3},//�ձ�
	{0,2,1,3},//����
	{3,1,2,0},//���ձ�
	{3,2,1,0},//������
};
	
void StickHandler(void)
{
	u8 i;
	
	//ȡ��ԭʼ����
	StickRaw[0]=ADC_TRV*2-AdcGetValue(0);
	StickRaw[1]=AdcGetValue(1);
	StickRaw[2]=ADC_TRV*2-AdcGetValue(2);
	StickRaw[3]=AdcGetValue(3);
	
	//У׼
	for(i=0;i<STK_NUM;i++)
	{
		s32 v=StickRaw[i]-TxSys.StkCali[i][1];
		
		//��������
		if(v<0)
		{
			if(v<-TxSys.StkDa)	v+=TxSys.StkDa;
			else				v=0;
		}  
		else
		{
			if(v>TxSys.StkDa)	v-=TxSys.StkDa;
			else				v=0;
		}  
		
		v*=STK_TRV;		
		StickCal[i]= v/(TxSys.StkCali[i][v>0?2:0]-TxSys.StkDa);
		
		//ͨ��ӳ��
		u8 idx=StickMap[TxSys.StkType][i];
		StickValue[idx]=StickCal[i];
		StickTrim[idx]=Model.Trim[i]*TRIM_STEP;
	}
}