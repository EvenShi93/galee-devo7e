#include "Func.h"
#include "Page.h"
#include "Tx.h"

//��ʱ��������
TIMERRUN TxTimer[TXTMR_NUM]={
	{0,0,0,1,&Model.Timer[0]},
	{0,0,0,1,&Model.Timer[1]},
	{0,0,0,1,&Model.Timer[2]},
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ��ʱ������
//
void TimerRun(u8 idx)
{	
	TIMERRUN *pt=&TxTimer[idx];
	
	//��λ�ж�
	if(pt->Reset)
	{
		pt->Reset=0;
		if(pt->Cfg->Type==TMR_CLK)	pt->Cnt=0;
		else						pt->Cnt=pt->Cfg->Time;
	}
	
	//�����ж�
	if(pt->Cfg->Sw==TMRSW_THR)
	{
		pt->Run=(MixerValue[CH_THR]*100/STK_TRV>=pt->Cfg->Thr);
	}
	else if(pt->Cfg->Sw==TMRSW_HOLD)
	{
		pt->Run=SW(SW_HOLD);
	}
	else if(pt->Cfg->Sw==TMRSW_FMOD)
	{
		pt->Run=SW(SW_FMOD);
	}
	else if(pt->Cfg->Sw==TMRSW_HOLDR)
	{
		pt->Run=!SW(SW_HOLD);
	}
	else if(pt->Cfg->Sw==TMRSW_FMODR)
	{
		pt->Run=!SW(SW_FMOD);
	}
	else pt->Run=0;
	
	//���ú������ж�
	if(pt->Cfg->Type<=TMR_HIDE)
	{
		pt->Run=0;
	}
	
	//�����еĲ�����
	if(!pt->Run)	return;
	
	//�������������
	s32 tr=0;
	if(pt->Cfg->Type==TMR_CLK)
	{
		pt->Cnt++;
		pt->Alert=(pt->Cnt>=pt->Cfg->Time);//��ʱ��
		tr=pt->Cfg->Time-pt->Cnt;
	}
	else if(pt->Cfg->Type==TMR_DCK)
	{
		pt->Cnt--;
		pt->Alert=(pt->Cnt<=0);
		tr=pt->Cnt;
	}
	
	//0~30����ǰԤ��
	if(tr<=30 && tr>=10 && tr%10==0)
	{
		tr=(tr-10)/10*8;
		BeepMusic(&MusicTimeRun[tr]);
	}
	
	//�澯����	
	if(pt->Alert && pt->Cnt%10==0)//ÿ10������һ��
	{
		BeepMusic(MusicTimeout);
		LightStartTmr=SysTimerClk;//����رռ���������
	}
}

void TimerProc(void)
{
	static u32 timercnt;
	
	//���Ƶ��
	if(SysTimerClk<timercnt)	return;
	timercnt=SysTimerClk+1000;
	
	TimerRun(0);
	TimerRun(1);	
	TimerRun(2);
}