#include "Tx.h"

#define PPM_PLUS	400	//PPM ����400us
#define PPM_MID		1500
#define PPM_FRAME	20000

//PPM�жϳ���
u8 TxPpmPin,TxPpmIdx;
static u16 TxPpmCallback(void)
{
	static u16 ppmtime;
	
	//����ߵ�ƽ
	if(TxPpmPin)	
	{
		PPM_H();
		TxPpmPin=0;
		return PPM_PLUS;
	}
	//����͵�ƽ
	else
	{
		u16 t;
		PPM_L();
		TxPpmPin=1;		
		
		if(TxPpmIdx>=TX_CH_NUM)	//��������һ������
		{
			t=PPM_FRAME-PPM_PLUS*TX_CH_NUM-ppmtime;	//����֡���ʱ��
			ppmtime=0;								//���ͨ���ۼ���
			TxPpmIdx=0;								//�ع�CH1
		}			
		else					//PPM��������
		{
			t=PPM_MID-PPM_PLUS+TxChValue[TxPpmIdx];	//���㱾ͨ��ʱ��		
			ppmtime+=t;								//�ۼӱ�ͨ��ʱ��
			TxPpmIdx++;								//CH����
		}
		
		return t;
	}
}
	
u32 TxPpmOpen(void)
{
	//��PPM�ܽ�����Ϊ����
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);    
    gpio_set_mode(PPM_PORT, GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL, PPM_OUT);
	PPM_L();
    
    //50ms ��ʼ���PPM�ź�
    SysTimerStart(50000, TxPpmCallback);
    
    TxPpmPin=0;
    TxPpmIdx=TX_CH_NUM+1;

	return 1;
}

void TxPpmClose(void)
{
	//��PPM�ܽ�����Ϊ����̬
    gpio_set_mode(PPM_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PPM_OUT);
}

u32 TxPpmBind(void)
{
	return 0;//���ض��������ʾ����
}

