#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ����ɨ���ʼ��
//
//�������ɨ������ͼ�����Ǵ��
//       B.5         B.6         B.7           B.8
//C.6  Rudder TL   Rudder TR   Elevator TU   Elevator TD        
//C.7  NC          Ent         R+            L-
//C.8  Throttle TU Throttle TD Aileron TR    Aileron TL
//C.9  NC          DN-         UP+           Ext  
//
//Comment:
//TL:  Trim Left   TR: Trim Right
//TU:  Trim Up     TD: Trim down
//
//OP:
// ��PB5 ��PB8 ��PB6 ��PB7 ��PC6

void KeyInit(void)
{
	/* Enable AFIO */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);
	
	/* Remap GPIO_Remap_SWJ_JTAGDisable */
	AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON;
	
	/* Enable GPIOB & GPIOE */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
	
	// PortB 5 6 7 8 ��©���
	gpio_set_mode(KEY_COL_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO5 | GPIO6 | GPIO7 | GPIO8);
	gpio_set(KEY_COL_PORT, GPIO5 | GPIO6| GPIO7 | GPIO8);
	
	// PortC 6 7 8 9 ��������
	gpio_set_mode(KEY_ROW_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6 | GPIO7 | GPIO8 | GPIO9);
	gpio_set(KEY_ROW_PORT, GPIO6 | GPIO7 | GPIO8 | GPIO9);
	
	//PortC 10 11�������� HOLD FMOD
	gpio_set_mode(KEY_SW_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO10 | GPIO11);
	gpio_set(KEY_SW_PORT, GPIO10 | GPIO11);
	
	KeyFlush();
	KeyStat=KeyScanOnce();//Ϊ������ⰴ��ȡ�ð���״̬
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ����ɨ��һ��
//
volatile u16 KeySwExt;//��չ����
u16 KeyScanOnce(void)
{
    u16 result=0;
    u16 col;
    
    gpio_set(KEY_COL_PORT, KEY_COL_PORT_MASK);
    for(col = 1<<5; col <= 1<<8; col<<=1)
    {
        gpio_clear(KEY_COL_PORT, col);
        result<<=4;
        result|= 0x0f&(gpio_port_read(KEY_ROW_PORT)>>6);
        gpio_set(KEY_COL_PORT, col);
    }

    //���û���κμ����£��Ϳ��Խ���OP����ɨ��
    if(result==KEY_MASK)
    {    	
    	//PC6 ��©���������
		gpio_set_mode(KEY_ROW_PORT,GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO6);
		gpio_clear(KEY_ROW_PORT, GPIO6);
		
		KeySwExt=(~(gpio_port_read(KEY_COL_PORT)>>5))&0xf;
		
		//��ԭPC6
		gpio_set_mode(KEY_ROW_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);
		gpio_set(KEY_ROW_PORT, GPIO6);
	}
	
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ����ɨ���жϷ������(sys_tick_handler��1ms����1��)
//
volatile u16 KeyStat,KeyDown,KeyHold,KeyBuf;
volatile u16 KeyPressCnt;
volatile u16 SwStat,SwBuf;
void KeyScanHandler(void)
{
	static u32 KeyScanDivider;
	u16 k;
	u16 s,sw;
		
	//����ɨ���Ƶ��25msɨ��һ��
	if(SysTimerClk-KeyScanDivider<25)	return;	
	KeyScanDivider=SysTimerClk;
	
	k=KeyScanOnce();		
	
	//��������
	if(k==KeyBuf)	
	{
		KeyStat=k;
		k&=KEY_MASK;
		if(k!=KEY_MASK)
		{
			if(KeyPressCnt==0)
			{
				if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);				
				LightStartTmr=SysTimerClk;//����رռ���������
				AutoOffReset();
				KeyDown&=KeyStat;
			}
			else
			{
				//��������
				if(KeyPressCnt>=KEY_CONT_TRIG)
				{
					u16 kd=KeyDown;					
					//�����ĸ�ֵҪ��&,���⽫��δ�����"��������"��ȥ
					KeyHold&=KeyBuf;
					KeyDown&=KeyBuf |KEY_EXT|KEY_ENT;//***�����������ļ��ڴ˴�����
					KeyPressCnt=KEY_CONT_TRIG-KEY_CONT_FREQ;
					
					//�����б仯������
					if(KeyDown!=kd)
					{
						if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);
						LightStartTmr=SysTimerClk;//����رռ���������
						AutoOffReset();
					}
				}
			}
			KeyPressCnt++;
		}
		else
		{
			KeyHold|=KeyBuf;		//����һ�ɿ�,KEYHOLD��ҪΪ0
			KeyPressCnt=0;
		}
	}
	else
	{
		KeyBuf=k;
	}
	
	//��ȡ����	
    s=gpio_port_read(KEY_SW_PORT);
    sw=0;
    if(s&(1L<<10))	sw|=SW_FMOD;
    if(s&(1L<<11))	sw|=SW_HOLD;
    if(KeySwExt&1)	sw|=SW_FLAP0;
    if(KeySwExt&2)	sw|=SW_FLAP1;
    if(KeySwExt&4)	sw|=SW_GEAR0;
    if(KeySwExt&8)	sw|=SW_GEAR1;
	if(sw==SwBuf)	SwStat=sw;
	else			SwBuf=sw;	
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  �������
//
u8 KeyTstDown(u16 mask)
{
	if((KeyDown&mask)==0)
	{
		KeyDown |= mask;
		return 1;
	}
	return 0;
}
void KeyClearDown(u16 mask)
{
	KeyDown |= mask;	
}

u8 KeyTstStat(u16 mask)
{
	return !(KeyStat&mask);
}

u8 KeyTstHold(u16 mask)
{
	if((KeyHold&mask)==0)
	{
		KeyHold |= mask;
		return 1;
	}
	return 0;
}

void KeyFlush(void)
{
	KeyDown=KeyStat=KeyHold=KeyBuf=KEY_MASK;
	KeyPressCnt=0;
}

void KeyStopCnt(void)
{
	KeyPressCnt=1;
}