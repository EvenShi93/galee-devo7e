#include "Tx.h"
#include "RF_CYRF6936.h"
#include "Page.h"

//ң���������ͨ��ֵ
s16 TxChValue[TX_CH_NUM]={0,0,0,0,0,0,0,0};

//���ID
u32 TxRndId=0;

//���������
u32 TxBindCnt;

//ͨ���ܺ�
s32 TxValueSum;

//���仺����
u8 TxPacket[32];

//PPMЭ��ӿ�
u32 TxPpmOpen(void);
u32 TxPpmBind(void);
void TxPpmClose(void);

//DSM2/DSMXЭ��ӿ�
u32 TxDsmOpen(void);
u32 TxDsmBind(void);
void TxDsmClose(void);

//DEVOЭ��ӿ�
u32 TxDevoOpen(void);
u32 TxDevoBind(void);
void TxDevoClose(void);

//WK2X01Э��ӿ�
u32 TxWkOpen(void);
u32 TxWkBind(void);
void TxWkClose(void);

//FLYSKYЭ��ӿ�
u32 TxFsOpen(void);
u32 TxFsBind(void);
void TxFsClose(void);

//HUBSANЭ��ӿ�
u32 TxHbsOpen(void);
u32 TxHbsBind(void);
void TxHbsClose(void);

//SkyartecЭ��ӿ�
u32 TxSkaOpen(void);
u32 TxSkaBind(void);
void TxSkaClose(void);

//J6ProЭ��ӿ�
u32 TxJ6pOpen(void);
u32 TxJ6pBind(void);
void TxJ6pClose(void);

typedef struct{
	u32 (*Open)(void);
	u32 (*Bind)(void);
	void (*Close)(void);
}TXMODULE;

u32 TxDummy(void)	{return 1;}
void TxDummyClose(void)	{}

const TXMODULE TxModule[]={
	{TxDummy,TxDummy,TxDummyClose},
	{TxPpmOpen,TxPpmBind,TxPpmClose},
	{TxDevoOpen,TxDevoBind,TxDevoClose},
	{TxDsmOpen,TxDsmBind,TxDsmClose},
	{TxDsmOpen,TxDsmBind,TxDsmClose},
	{TxWkOpen,TxWkBind,TxWkClose},
	{TxFsOpen,TxFsBind,TxFsClose},
	{TxHbsOpen,TxHbsBind,TxHbsClose},
	{TxSkaOpen,TxSkaBind,TxSkaClose},
	{TxJ6pOpen,TxJ6pBind,TxJ6pClose},
};
#define TX_MOD_NUM	sizeof(TxModule)/sizeof(TXMODULE)

u8 TxPrevId=0;
u32 TxLoad(u8 id)
{
	char msg[16];

	//����CYRF������̬ID��ÿ���ϵ������һ��
	if(TxRndId==0)
	{
		srand(AdcGetValue(0)+AdcGetValue(1)+AdcGetValue(2)+AdcGetValue(3)+AdcGetValue(4));

		CYRF_Reset();
		CYRF_GetMfgData(CYRF_MfgId);
		TxRndId = ((u32) ( rand() ^ CYRF_MfgId[0] ^ CYRF_MfgId[3]) << 16)
				 | ((u32)( rand() ^ CYRF_MfgId[1] ^ CYRF_MfgId[4]) << 8)
				 | ((u32)( rand() ^ CYRF_MfgId[2] ^ CYRF_MfgId[5]) << 0);
		TxRndId = TxRndId % 1000000;
	}

	if(id>=TX_MOD_NUM)
	{
		PageAlertModel("����ģ���ڲ�����!",0);
		return 0;
	}

	if(id!=TxPrevId)
	{
		strcpy(msg,PageOptionProtocol[id]);
		TxModule[TxPrevId].Close();
		if(TxModule[id].Open())
		{
			strcat(msg,"���سɹ�!");
			PageAlertModel(id?msg:"RF�ѹر�!",500);
			TxPrevId=id;
			return 1;
		}
		else
		{
			strcat(msg,"����ʧ��!");
			PageAlertModel(id?msg:"RF�ر�ʧ��!",0);
			TxPrevId=0;
			return 0;
		}
	}

	return 2;
}

u32 TxBind(void)
{
	return TxModule[TxPrevId].Bind();
}

void TxClose(void)
{
	TxModule[TxPrevId].Close();
	TxPrevId=0;
}


void TxSpiInit(void)
{
	/* Enable SPI2 */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_SPI2EN);
	/* Enable GPIOA */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	/* Enable GPIOB */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);

	/* SCK, MOSI */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO13 | GPIO15);
	/* MISO */
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
				  GPIO_CNF_INPUT_FLOAT, GPIO14);

	/*CYRF cfg */
	/* Reset and CS */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO11 | GPIO12);
	gpio_set(GPIOB, GPIO12);
	gpio_clear(GPIOB, GPIO11);


	/* A7105 */
	//Disable JTAG and SWD
	AFIO_MAPR = (AFIO_MAPR & ~AFIO_MAPR_SWJ_MASK) | AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF;
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set(GPIOA, GPIO13);

	/* CC2500 */
	//Disable JTAG and SWD
	AFIO_MAPR = (AFIO_MAPR & ~AFIO_MAPR_SWJ_MASK) | AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF;
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);
	gpio_set(GPIOA, GPIO14);

	/* Includes enable? */
	spi_init_master(SPI2,
					SPI_CR1_BAUDRATE_FPCLK_DIV_16,
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT,
					SPI_CR1_MSBFIRST);
	spi_enable_software_slave_management(SPI2);
	spi_set_nss_high(SPI2);
	spi_enable(SPI2);
}


///////////////////////////////////////////////////////////////////
//
//����EPA/REV/SUBTRIM����iͨ����������
//      (����˳���ܱ�)
//
//����: v -POSSIZE��+POSSIZE
//    : i ͨ��ID
//���: SigCh[i] 0~1200
void TxSetCh(u8 idx,s32 cv)
{
	static u8 DelayRun;

	//��ʱ����ʱ��
	if(idx==0xff)
	{
		static u32 TxDelayCnt;
		if(SysTimerClk>TxDelayCnt)
		{
			DelayRun=1;
			TxDelayCnt=SysTimerClk+5;//10msִ��һ����ʱ����
		}
		else	DelayRun=0;
		return;
	}

	///////////////////////////////////
	//1.EPA
	if(cv<0) cv*= Model.Epa[idx][0];
	else	 cv*= Model.Epa[idx][1];
	cv/=EPA_SIZE;

	//2.REV
	if(Model.Rev[idx])	cv=-cv;

	//3.NEU
	cv+=Model.Neu[idx]*NEU_STEP;

	//4.�޷�
	if(cv>TX_TRV)	cv=TX_TRV;
	if(cv<-TX_TRV)	cv=-TX_TRV;

	//5.д��ͨ��������
	if(Model.Delay[idx]>0 && Model.Delay[idx]<=DELAY_MAX)//��Ч���жϣ����������ļ�����
	{
		//��ʱͨ������
		if(DelayRun)
		{
			s32 dv,ds;
			dv=cv-TxChValue[idx];
			ds=DELAY_MAX-Model.Delay[idx]+1;
			if(dv<ds && dv>-ds)		TxChValue[idx]=cv;
			else
			{
				if(dv<0)	ds=-ds;
				TxChValue[idx]+=ds;
			}
		}
	}
	else TxChValue[idx]=cv;	//ֱ�Ӹ�ֵ
}

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ߺ���(����CURVE_NUM)
//
#define CURVE_SEG	(STK_TRV*2/(CURVE_NUM-1))	//9�����߾���8��
s32 TxMixCurve(s32 v,s16 *curve)
{
	//��ֵת��Ϊȫ��ֵ
	if(v<-STK_TRV)	v=-STK_TRV;
	if(v>+STK_TRV)	v=+STK_TRV;
	v+=STK_TRV;

	//Ѱ������
	s32	x1;
	u8	i;
	for(i=0,x1=0;i<CURVE_NUM-1;i++,x1+=CURVE_SEG)
	{
		//���Բ�ֵ
		// y-y1    y2-y1
		//------ = -----
		// x-x1    x2-x1
		//
		// y=y1+(x-x1)*(y2-y1)/(x2-x1)
		if(v>=x1 && v<=(x1+CURVE_SEG))
		{
			s32 y2,y1;
			y1=(s32)curve[i]*(STK_TRV*2)/CURVE_TRV;
			y2=(s32)curve[i+1]*(STK_TRV*2)/CURVE_TRV;
			v=y1+(v-x1)*(y2-y1)/CURVE_SEG;

			return v-STK_TRV;
		}
	}

	//���û��ת���ɹ�������ԭֵ
	return v;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ο���
//
s16 SwValue(s16 swpos)
{
	swpos=Model.SwDef[swpos];
	switch(swpos)
	{
		case 1://HOLD����
			if(SW(SW_HOLD))		return -1;
			return	1;
		case 2://FMOD����
			if(SW(SW_FMOD))		return 1;
			return -1;
		case 3://��չ1
			if(SW(SW_GEAR0))	return -1;
			if(SW(SW_GEAR1))	return 1;
			return 0;
		case 4://��չ2
			if(SW(SW_FLAP0))	return -1;
			if(SW(SW_FLAP1))	return 1;
			return 0;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
//
// ��غ���
//
s32 MixerValue[TX_CH_NUM];
u8 FlyMode=0;
u8 DrMode=0;
void TxMixer(void)
{
	static s16	ChRock=0,ChRockStep=CHROCKSTEP;
	static u32	ChRockDelay=0;

	//�����ö���ҡ��
	if(SysTimerClk>ChRockDelay)
	{
		ChRock+=ChRockStep;
		if(ChRock> STK_TRV)	ChRockStep=-CHROCKSTEP;
		if(ChRock<-STK_TRV)	ChRockStep=+CHROCKSTEP;
		ChRockDelay=SysTimerClk+CHROCKDELAY;
	}

	//������ʱ
	TxSetCh(-1,0);

	//����С��
	DrMode=SwValue(SWD_DR)+1;

	//�������
	FlyMode=(SwValue(SWD_HOLD)>0)?FM_HLD:FM_NOR;

	if(Model.Type==MT_AERO)//�̶���
	{
		//����4��ͨ��
		MixerValue[CH_AIL]=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		MixerValue[CH_ELE]=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		MixerValue[CH_THR]=StickTrim[2]+TxMixCurve(StickValue[2], Model.ThrCurve[0]);
		MixerValue[CH_RUD]=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);

		//����ܺͽ���
		MixerValue[CH_GEAR]=Model.Gear[SwValue(SWD_GEAR)+1]*STK_TRV/MAX_AUX;
		MixerValue[CH_FLAP]=Model.Flap[SwValue(SWD_FLAP)+1]*STK_TRV/MAX_AUX;

		//�������Ŵ���
		if(FlyMode==FM_HLD)		MixerValue[CH_THR]=(-100+Model.ThrHold)*STK_TRV/100;//��-100�������ۼ���������ֵ
		
		//���Ż�ؼ���
		{
			s16 Thr=MixerValue[CH_THR]+STK_TRV;
			MixerValue[CH_AIL]+=Thr*Model.ThrMix[0]/MAX_THM;
			MixerValue[CH_RUD]+=Thr*Model.ThrMix[1]/MAX_THM;
			MixerValue[CH_ELE]+=Thr*Model.ThrMix[2]/MAX_THM;		
		}
		
		//ת����
		{
			s32 ea,er;
			if(MixerValue[CH_AIL]>=0)	ea= MixerValue[CH_AIL]*Model.YawMix.Ar2E;
			else						ea=-MixerValue[CH_AIL]*Model.YawMix.Al2E;
			if(MixerValue[CH_RUD]>=0)	er= MixerValue[CH_RUD]*Model.YawMix.Rr2E;
			else						er=-MixerValue[CH_RUD]*Model.YawMix.Rl2E;
			MixerValue[CH_ELE]+=(ea+er)/MAX_YWM;			
		}
		
		//��������
		if(Model.Delta.Enable)
		{
			s32 a,e;
			a=MixerValue[CH_AIL]*Model.Delta.A2A + MixerValue[CH_ELE]*Model.Delta.E2A;
			e=MixerValue[CH_AIL]*Model.Delta.A2E + MixerValue[CH_ELE]*Model.Delta.E2E;
			MixerValue[CH_AIL]=a/MAX_DLT;
			MixerValue[CH_ELE]=e/MAX_DLT;
		}

		//Vβ���
		if(Model.Vtail.Enable)
		{
			s32 r,e;
			r=MixerValue[CH_RUD]*Model.Vtail.R2R + MixerValue[CH_ELE]*Model.Vtail.E2R;
			e=MixerValue[CH_RUD]*Model.Vtail.R2E + MixerValue[CH_ELE]*Model.Vtail.E2E;
			MixerValue[CH_RUD]=r/MAX_VTL;
			MixerValue[CH_ELE]=e/MAX_VTL;
		}

		//��������
		if(Model.FlpAil.Enable)
		{
			s32 a,f;
			a=MixerValue[CH_AIL]*Model.FlpAil.A2A + MixerValue[CH_FLAP]*Model.FlpAil.F2A;
			f=MixerValue[CH_AIL]*Model.FlpAil.A2F + MixerValue[CH_FLAP]*Model.FlpAil.F2F;
			MixerValue[CH_AIL]=a/MAX_FAM;
			MixerValue[CH_FLAP]=f/MAX_FAM;
		}
	}
	else//ֱ����
	{
		s32 Ail,Ele,Rud,Thr,Pit,Gyo;

		//����ؼ�����
		if(FlyMode==FM_NOR && SwValue(SWD_IDLE)<0) FlyMode=FM_IDL;

		//����6������ͨ��
		Thr=StickTrim[2]+StickValue[2];
		Ail=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		Ele=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		Rud=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);
		Pit=TxMixCurve(Thr , Model.PitCurve[FlyMode]);
		Thr=TxMixCurve(Thr , Model.ThrCurve[FlyMode==2]);
		Gyo=Model.Gyro[FlyMode==FM_IDL]*STK_TRV/MAX_GYRO;

		//�������Ŵ���
		if(FlyMode==FM_HLD)		Thr=(-100+Model.ThrHold)*STK_TRV/100;//��-100�������ۼ���������ֵ

		//б�̻�ر��ʵ���
		Ail=Ail*Model.Swash[0]/100;
		Ele=Ele*Model.Swash[1]/100;
		Pit=Pit*Model.Swash[2]/100;

		//б�̻��
		if(Model.SwashType==1)//120 CCPM		sin(120-90)=0.5
		{
			MixerValue[CH_AIL]	=Pit+Ele/2-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ele/2+Ail;
		}
		else if(Model.SwashType==2)//140 CCPM	sin(140-90)=0.766~=36/47
		{
			MixerValue[CH_AIL]	=Pit+Ele*36/47-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ele*36/47+Ail;
		}
		else if(Model.SwashType==3)//90 CCPM	sin(90-90)=0
		{
			MixerValue[CH_AIL]	=Pit-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ail;
		}
		else//�����б��
		{
			MixerValue[CH_AIL]	=Ail;
			MixerValue[CH_ELE]	=Ele;
			MixerValue[CH_FLAP]	=Pit;
		}

		//�趨����|����|������
		MixerValue[CH_THR]	=Thr;
		MixerValue[CH_RUD]	=Rud;
		MixerValue[CH_GEAR]	=Gyo;
	}

	//2������ͨ��	
	MixerValue[CH_AUX1]=Model.Aux[0][SwValue(SWD_AUX1)+1]*STK_TRV/MAX_AUX;
	MixerValue[CH_AUX2]=Model.Aux[1][SwValue(SWD_AUX2)+1]*STK_TRV/MAX_AUX;

	//ͨ��ӳ�����
	TxValueSum=0;
	for(u8 i=0;i<TX_CH_NUM;i++)
	{
		if(Model.ChMap[i]==CH_TEST)//����ͨ��
		{
			TxSetCh(i,ChRock);
		}
		else if(Model.ChMap[i]>=CH_PPMIN)//PPM IN
		{
			TxSetCh(i,PpmInValue[Model.ChMap[i]-8]);
		}
		else
		{
			TxSetCh(i,MixerValue[Model.ChMap[i]]);
		}
		TxValueSum+=TxChValue[i]/STKMOVTST;
	}
}