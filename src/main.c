#include "Macros.h"
#include "Func.h"
#include "Page.h"
#include "Tx.h"

//����USB���ӵ�ͼ��
const u8 USBLogo[]= {
    0x00, 0x77,//width:  119
    0x00, 0x35,//height: 53
    0x00, 0xb3,//size:   179
    0x47, 0x85, 0x70, 0x89, 0x6d, 0x8b, 0x6c, 0x8b, 0x6b, 0x8d, 0x5b, 0x9c, 0x59, 0x9e, 0x58, 0x9f,
    0x57, 0xa0, 0x56, 0x86, 0x0f, 0x8b, 0x56, 0x86, 0x10, 0x8b, 0x55, 0x86, 0x12, 0x89, 0x55, 0x86,
    0x14, 0x87, 0x55, 0x86, 0x70, 0x86, 0x70, 0x86, 0x4f, 0x88, 0x19, 0x86, 0x4e, 0x8c, 0x16, 0x86,
    0x4e, 0x8f, 0x13, 0x86, 0x4e, 0x91, 0x11, 0x86, 0x4e, 0x92, 0x11, 0x85, 0x41, 0x82, 0x0c, 0x93,
    0x0f, 0x85, 0x42, 0x84, 0x09, 0x94, 0x0e, 0x86, 0x42, 0x86, 0x07, 0x95, 0x0c, 0x86, 0x43, 0x87,
    0x06, 0x95, 0x09, 0x88, 0x44, 0x89, 0x03, 0xf6, 0x01, 0xff, 0xff, 0xe5, 0x03, 0x95, 0x1b, 0x87,
    0x33, 0x88, 0x05, 0x95, 0x1d, 0x86, 0x32, 0x87, 0x06, 0x94, 0x1f, 0x86, 0x31, 0x85, 0x09, 0x93,
    0x20, 0x85, 0x31, 0x83, 0x0b, 0x92, 0x22, 0x85, 0x30, 0x81, 0x0e, 0x90, 0x23, 0x86, 0x3f, 0x8e,
    0x25, 0x86, 0x40, 0x8b, 0x27, 0x86, 0x41, 0x86, 0x2b, 0x86, 0x72, 0x86, 0x72, 0x86, 0x72, 0x86,
    0x12, 0x8d, 0x53, 0x86, 0x11, 0x8d, 0x54, 0x86, 0x10, 0x8d, 0x55, 0x86, 0x0f, 0x8d, 0x56, 0x87,
    0x0d, 0x8d, 0x57, 0xa0, 0x58, 0x9f, 0x5a, 0x9d, 0x5c, 0x9b, 0x6a, 0x8d, 0x6a, 0x8d, 0x6a, 0x8d,
    0x6a, 0x8d, 0x16,
};

//ң��������
TXSYS	TxSys;
const TXSYS TxSysDef={
	TXCFGMARK,
	0,	//u8 StkCalied
	1,	//s16 ModelNo;
	0,	//s16 StkType; 
	{	//s16 StkCali[4][3];
		{1700,2048,1700},
		{1700,2048,1700},
		{1700,2048,1700},
		{1700,2048,1700},		
	},
	0,	//s16 StkDa;
	1,	//s16 Vibrator;
	1,	//s16 Music;
	5,	//s16 KeyBeep;
	20,	//s16 KeyTone;
	6,	//s16 Light;
	3,	//s16 Contrast;
	3,	//s16 LightOff;
	0,	//s16 BatTYpe;
	45,	//s16 BatWarn;
	0,	//s16 AutoOff;
	1,	//s16 FreeWarn;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �Զ��ػ���������λ
//
void AutoOffReset(void)
{
	AutoOffTime=SysTimerClk+TxSys.AutoOff*10*60*1000;
	ChStopTime=SysTimerClk+TxSys.FreeWarn*60000;//��ʼ��
}

///////////////////////////////////////////////////////////////////////////////////////
//
// ϵͳ�¼�
//
u32 AutoOffTime;
u32 ChStopTime;
void SysProc(void)
{
	static s16 AllChValue;
	static u8 InShutdown=0;
	
	SysTimerWatchDogRst();	
	
	//�����ϵͳ�¼�
	AdcHandler();	
	StickHandler();
	BeepHandler();
	KeyScanHandler();
	BatteryHandler();				
		
	//�źŻ�أ������ڽ���֮ǰ���Ա������ģʽ��˸��
	TxMixer();
		
	//��ʱ������
	TimerProc();
	
	//΢����ť����
	TrimProc();

	//���ҡ���Ƿ��ƶ����Զ��ػ���
	if(AllChValue!=TxValueSum)
	{
		AllChValue=TxValueSum;
		AutoOffReset();
	}
	
	//ҡ�˲���ÿ��������һ��
	if(TxSys.FreeWarn)
	{
		if(SysTimerClk>=ChStopTime && SysTimerClk>60000)
		{
			ChStopTime=SysTimerClk+TxSys.FreeWarn*60000;//
			BeepMusic(MusicStkStop);
			LightStartTmr=SysTimerClk;//����رռ���������
		}
	}
	
	//����Ѿ��ڹػ���������У����ٴ�������SysProc����
	if(!InShutdown)	
	{
		//��Դ������|�Զ��ػ�(1���Ӻ�����Զ��ػ�)
		if(PowerSw() || (TxSys.AutoOff && SysTimerClk>=AutoOffTime && SysTimerClk>60000))
		{
			InShutdown=1;
			TxClose();
			
			//�������ݲ��ػ�	
			ModelSave(TxSys.ModelNo);
			SaveCfg();	
			PowerDown();
		}	
	}	
}

///////////////////////////////////////////////////////////////////////////////////////
//
// ������
//
u32 LightStartTmr;
int main() 
{
	s16 txlight,txcontrast;
	int i;
	
    PowerInit();
	AdcInit();
    SysTimerInit();
    SPIFlash_Init(); 				//This must come before LCD_Init() for 7e (LCD share SPI with FLASH!)
    SPI_FlashBlockWriteEnable(1);	//Enable writing to all banks of SPIFlash
    TxSpiInit();
    LcdInit();
	KeyInit();
	BeepInit();
	PpmInInit();
				
	//����ļ�ϵͳ���󣬽���USB����ģʽ
	if(!FS_Mount() || KeyTstStat(KEY_ENT))
	{
		//��U�̣��ȴ��ػ�
		PowerLight(10);
		LcdDrawLogo(USBLogo);	
		USB_Enable(1);	
		KeyFlush();
		while(!KeyTstDown(KEY_EXT))
		{
			if(PowerSw())	PowerDown();
			KeyScanHandler();
			SysTimerWatchDogRst();
		}
		KeyFlush();
		USB_Disable(1);
		FS_Mount();
		SysTimerClk=0;//���ϵͳ��ʱ��
	}
	
	//��������
	LoadCfg();	
	
	//���ƿ������棬��Ļ����
	BeepMusicEnable=TxSys.Music;
	BeepVibrator=TxSys.Vibrator;
	BeepMusic(MusicStartup);
	LcdDrawBmpFile(0,0,"res/logo.bmp");	
	LcdDrawMiniEng(40,59,__DATE__);
	LcdDrawStop();
	for(i=0;i<8000;i++)
	{
		PowerLight(i/100);
		usleep(100);
		SysProc();
	}	

	//��ʾҡ��
	if(TxSys.StkCalied!=CALMARK)	PageAlertModel("ҡ��δУ׼!",0);
	
	//����ģ�ͣ���������
	ModelLoad(TxSys.ModelNo,0);
	
	//��ȫ���
	char *msg=0;
	do{
		PageAlertModel(msg,100);	//���ҡ�˺Ϳ���
		msg=0;
		if(FlyMode==FM_IDL)		msg="��ر��ؼ�ģʽ!";
		else if(TxSys.StkCalied==CALMARK && StickValue[2]>THRSAFE)	msg="����ҡ��δ��λ!";	//ֱ�Ӽ��ҡ��ֵ
	}while(msg);
	
	//���¼���ģ�ͣ��򿪷���
	ModelLoad(TxSys.ModelNo,1);		
	
	//Ԥ���ֵ
	txlight=txcontrast=-1;
	
	//��ѭ��
	KeyFlush();
	PageSet(PageMain,PV_INIT);
	
	while(1)
	{
		SysProc();		
		
		PageStack[PageStackIdx](PV_RUN);		//���洦�����
	
		//������Ļ���ȺͶԱȶ�
		if(TxSys.LightOff && SysTimerClk> (LightStartTmr+(u32)TxSys.LightOff*10000))
		{
			PowerLight(txlight=0);
		}
		else
		{
			if(txlight!=TxSys.Light)
			{
				txlight=TxSys.Light;
				PowerLight(TxSys.Light*8);
			}
		}
		if(txcontrast!=TxSys.Contrast)
		{
			txcontrast=TxSys.Contrast;
			LcdContrast(TxSys.Contrast);
		}
		BeepMusicEnable=TxSys.Music;
		BeepVibrator=TxSys.Vibrator;
		PpmInCtl(Model.PpmIn);
	}
}