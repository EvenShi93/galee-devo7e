#include "Func.h"

#define PPMIN_HIGH	4000
#define PPMIN_NEU	1500
#define PPMIN_LOW	400

u32 PpmInCnt;
u8  PpmInIndex;
u8  PpmInCh;
s16 PpmInValue[PPMINNUM];

///////////////////////////////////////////////////////////////////////////////////////
//
//  PPM�����жϳ�ʼ��
//
// ����TIM4��Ϊ��ʱ����TIM4�Ƿ���ص�������ʱ��
void PpmInInit(void)
{	
	//�趨PPM IN���ⲿ�ж�
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);	
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);	
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);	//PPM IN ��PA10���ж�ΪEXTI15-10
	gpio_set_mode(PPM_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PPM_IN);	
	exti_select_source(EXTI10, PPM_PORT);	
	exti_set_trigger(EXTI10, EXTI_TRIGGER_FALLING);		
    
	PpmInCnt=0;
}

void PpmInCtl(u8 onoff)
{
	if(onoff)
	{ 
		exti_enable_request(EXTI10);
	}
	else
	{
		u8 i;
		for(i=0;i<PPMINNUM;i++) PpmInValue[i]=0;		
		exti_disable_request(EXTI10);
	}
}

//PPM IN ISR
void exti15_10_isr(void)
{
	static u16 preclk;
	u16 clk=timer_get_counter(TIM4);
	u16 width=clk-preclk;
	
	if(width>PPMIN_LOW)//�ų������ź�
	{		
		if(width>=PPMIN_HIGH)//֡��ʱ
		{
			PpmInCnt++;
			PpmInCh=PpmInIndex;
			PpmInIndex=0;
		}
		else
		{
			if(PpmInIndex<PPMINNUM)
			{
				PpmInValue[PpmInIndex++]=width-PPMIN_NEU;
			}
		}
		preclk=clk;
	}
	exti_reset_request(EXTI10);	
}