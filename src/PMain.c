#include "Page.h"
#include "Func.h"
#include "Tx.h"

#define TRIM_NUM_HIDE 99

u32 PageMainDrawMask;

u16 BatVol=0;

///////////////////////////////////////////////////////////////////////////////////////
//
//  ���Ƶ�ص���״̬
//
#define LCD_BAT_ICON1	0x0e
#define LCD_BAT_ICON2	0x1b
#define LCD_BAT_ICON3	0x11
#define LCD_BAT_ICON4	0x1f
#define LCD_BAT_ICON5	0x1f
u16 BatLowCnt;
void DrawBattery(void)
{
	static u8 batflash;
	int i,BatPer;	
	u16 batfull,batlow;
	
	//����ٷֱ�,ȱʡ����:4V~5V
	batfull=500;
	batlow=400;
	if(TxSys.BatType==1)//����:4V~6V
	{
		batfull=600;
		batlow=400;
	}
	if(TxSys.BatType==2)//1S LI:3.3V~4.2V
	{
		batfull=410;
		batlow=330;
	}
	if(TxSys.BatType==3)//2S LI:6.6V~8.4V
	{
		batfull=830;
		batlow=660;
	}
	
	if(BatVol>batfull)	BatPer=LCD_BAT_FULL;
	else				BatPer=(BatVol-batlow)*LCD_BAT_FULL/(batfull-batlow);
	
	//��ʼ���Ƶ��״̬
    LcdDrawStart(0, 0,LCD_W-1, 8, DRAW_NWSE);
    
    //��ʾ��ص�ѹ
    if(BatLowCnt>=BAT_LOW_CNT)  batflash=!batflash;
    else						batflash=1;

    if(batflash)
    {
    	LcdDrawMiniInt(LCD_BAT_X+18,LCD_BAT_Y,BatVol,0,2,0,1);
    	LcdDrawMiniNum(LCD_BAT_X+18,LCD_BAT_Y,LCD_MN_CHAR('V'));
    }
    else LcdDrawRect(LCD_BAT_X,LCD_BAT_Y,LCD_BAT_X+22,LCD_BAT_Y+6,0);
    
    //��ʾ�����״ͼ
    LcdDrawMaskY(LCD_BAT_X+24,LCD_BAT_Y,LCD_BAT_ICON1,5);
    LcdDrawMaskY(LCD_BAT_X+25,LCD_BAT_Y,BatPer==LCD_BAT_FULL?LCD_BAT_ICON4:LCD_BAT_ICON2,5);
    for(i=0;i<LCD_BAT_FULL;i++)
    {
    	LcdDrawMaskY(LCD_BAT_X+26+i,LCD_BAT_Y,i>=(LCD_BAT_FULL-BatPer)?LCD_BAT_ICON4:LCD_BAT_ICON3,5);
    }
    LcdDrawMaskY(LCD_BAT_X+26+LCD_BAT_FULL,LCD_BAT_Y,LCD_BAT_ICON5,5);
    
    LcdDrawStop();
}

//��ص������͸澯
void BatteryHandler(void)
{
	static u32 batcnt;
	
	//��ؼ���Ƶ��
	if(SysTimerClk<batcnt)	return;
	batcnt=SysTimerClk+BAT_TST_CYCLE;
	
	//ȡ�õ�ѹ
	BatVol=(((u32)AdcGetValue(ADC_BAT))*VOLTAGE_NUMERATOR/100+VOLTAGE_OFFSET)/10;
	PageMainDrawMask|=PMD_BAT;
	
	//�����ﴦ���ص�ѹ�澯
	if(BatVol/10<TxSys.BatWarn)
	{
		static u8 BatLowMusicCnt;	
		if(BatLowCnt>BAT_LOW_CNT)//���˵�ص�ѹ����
		{
			BatLowMusicCnt++;
			if(BatLowMusicCnt>10)
			{
				BeepMusic(MusicBatLow);//�澯
				BatLowMusicCnt=0;
			}
		}
		else BatLowCnt++;
	}
	else BatLowCnt=0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ����΢����
//
#define LCD_TRIM_ICONL	0x1f
#define LCD_TRIM_ICONM	0x15
#define LCD_TRIM_ICONH	0x11
#define LCD_TRIM_ICONHB	0x1f
#define LCD_TRIM_ICONR	0x1f
void DrawTrimBarH(u16 x,u16 y,s8 trim)
{
	s16 i,p,s;
	    
	//������״����ֹλ�� 
    p=trim;
    p=p*LCD_TRIM_W/2/50;
    if(trim>0)
    {
    	if(p==0) p=1;//������ʾ1��
    	s=LCD_TRIM_W/2+1;
    	p+=LCD_TRIM_W/2+1;
    }
    else if(trim<0)
    {
    	if(p==0) p=-1;//������ʾ1��
    	s=p+LCD_TRIM_W/2;
    	p=LCD_TRIM_W/2;
    }
    else
    {
    	s=p=LCD_TRIM_W/2;
    }
    
    //������״��
    for(i=0;i<s          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONH ,LCD_TRIM_SIZE);
    for(   ;i<p          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONHB,LCD_TRIM_SIZE);
    for(   ;i<=LCD_TRIM_W;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONH ,LCD_TRIM_SIZE);
    LcdDrawMaskY(x,y,LCD_TRIM_ICONL,LCD_TRIM_SIZE);	//���
    LcdDrawMaskY(x+1+LCD_TRIM_W/2,y,LCD_TRIM_ICONM,LCD_TRIM_SIZE); //�е�
    LcdDrawMaskY(x+2+LCD_TRIM_W,y,LCD_TRIM_ICONR,LCD_TRIM_SIZE);   //�ұ�    
}

void DrawTrimBarV(u16 x,u16 y,s8 trim)
{
	s16 i,p,s;
	    
	//������״����ֹλ�� 
    p=-trim;
    p=p*LCD_TRIM_W/2/50;
    if(trim<0)
    {
    	if(p==0) p=1;//������ʾ1��
    	s=LCD_TRIM_W/2+1;
    	p+=LCD_TRIM_W/2+1;
    }
    else if(trim>0)
    {
    	if(p==0) p=-1;//������ʾ1��
    	s=p+LCD_TRIM_W/2;
    	p=LCD_TRIM_W/2;
    }
    else
    {
    	s=p=0;
    }

    //������״��
    for(i=0;i<s          ;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONH ,LCD_TRIM_SIZE);
    for(   ;i<p          ;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONHB,LCD_TRIM_SIZE);
    for(   ;i<=LCD_TRIM_W;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONH ,LCD_TRIM_SIZE);
    LcdDrawMaskX(x,y,LCD_TRIM_ICONL,LCD_TRIM_SIZE);	//�±�
    LcdDrawMaskX(x,y+1+LCD_TRIM_W/2,LCD_TRIM_ICONM,LCD_TRIM_SIZE); //�е�
    LcdDrawMaskX(x,y+2+LCD_TRIM_W,LCD_TRIM_ICONR,LCD_TRIM_SIZE);   //�ϱ�    
}

void DrawTrimBar(void)
{     
    DrawTrimBarH(LCD_TRIM_X1,LCD_TRIM_Y1,Model.Trim[3]);
    DrawTrimBarV(LCD_TRIM_X2,LCD_TRIM_Y2,Model.Trim[1]);
    DrawTrimBarH(LCD_TRIM_X3,LCD_TRIM_Y3,Model.Trim[0]);
    DrawTrimBarV(LCD_TRIM_X4,LCD_TRIM_Y4,Model.Trim[2]);
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��ʱ��
//
s8 PageMainTimerSel;
u8 PageMainTimerCol;
void DrawInfoTimer(u8 y,u8 idx)
{
	u16 m,s;
	TIMERRUN *pt=&TxTimer[idx];
	s32 timer=pt->Cnt;

	//�����ж�
	if(pt->Cfg->Type==TMR_HIDE)
	{
		LcdSetXy(4,y);
		LcdWriteSp(48);
		return;
	}
	
	//����ͼ��		
	LcdSetXy(4,y);
	LcdWriteChar(9);
	LcdWriteChar(10+idx);
		
	//���Ʒ���
	if(timer<0)
	{
		LcdWriteChar('-');
		timer=-timer;
	}
	else
	{
		LcdWriteChar(' ');
	}
	
	//����MS
	m=timer/60%60;
	s=timer%60;
	
	//��ʾ
	if(!PageMainTimerCol && PageMainTimerSel==idx+1)
	{
		LcdWriteSp(30);
	}
	else
	{
		char sp=pt->Run ? (PageMainTimerCol?0x36:0) : 0x36;//�����˸ͼ�����
		LcdWriteChar('0'+m/10);
		LcdWriteChar('0'+m%10);
		LcdWriteLine(0);
		LcdWriteLine(sp);
		LcdWriteLine(sp);//����ʱ�����˸
		LcdWriteChar('0'+s/10);
		LcdWriteChar('0'+s%10);		
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ������ʱ��
//
#define RUNTIMER_X	51
#define RUNTIMER_Y	1
void DrawRunTimer(u16 sec)
{
	u16 h,m,s;
	
	//����ʱ��
	h=sec/3600;
	m=sec/60%60;
	s=sec%60;
	
	//���ƿ���ʱ��
	LcdDrawMiniNum(RUNTIMER_X   ,RUNTIMER_Y,h/10);
	LcdDrawMiniNum(RUNTIMER_X+ 4,RUNTIMER_Y,h%10);
	LcdDrawMiniNum(RUNTIMER_X+ 8,RUNTIMER_Y,LCD_MN_COL);
	LcdDrawMiniNum(RUNTIMER_X+10,RUNTIMER_Y,m/10);
	LcdDrawMiniNum(RUNTIMER_X+14,RUNTIMER_Y,m%10);
	LcdDrawMiniNum(RUNTIMER_X+18,RUNTIMER_Y,LCD_MN_COL);
	LcdDrawMiniNum(RUNTIMER_X+20,RUNTIMER_Y,s/10);
	LcdDrawMiniNum(RUNTIMER_X+24,RUNTIMER_Y,s%10);
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��ʱ������Ϣ
//
u8 ChsIcon[6][40];
void DrawInfo(void)
{
	static u16 psec;
	u16 sec=SysTimerClk/250;
	
	//500
	if(psec==sec) return;
	psec=sec;
		
	//΢�������Զ�����������
	if(PageMainTrimShowCnt)	PageMainTrimShowCnt--;

    LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);   
    
	//���Ʒ���ģʽ
	LcdDrawIcon(6,15,32,10,ChsIcon[3+DrMode]);
	LcdDrawIcon(18,15,32,10,ChsIcon[FlyMode]);	
	
    
	//�����˸����
	PageMainTimerCol=!PageMainTimerCol;
	
	//���ƶ�ʱ��	
	DrawInfoTimer(4  ,0);
	DrawInfoTimer(4+1,1);
	DrawInfoTimer(4+2,2);
	
	//�����ʱ��3δ����������ʾ����
	if(TxTimer[2].Cfg->Type==TMR_HIDE)
	{
		char *pr=(char*)PageOptionProtocol[Model.Protocol];
		char *pw=(char*)PageOptionRfPwr[Model.RfPwr];
			
		if(Model.Protocol==0) pr="RF\xbOFF";
		if(Model.Protocol<=1) pw="";
		
		u16 l1,l2;
		l1=strlen(pr);
		l2=strlen(pw);
		LcdDrawMiniEng(30-(l1+1+l2)*2,50,pr);
		LcdDrawMiniEng(30-(l2-l1)*2,50,pw);
	}
	
	//���ƿ�ʼʱ��
	DrawRunTimer(sec/4);
	
    LcdDrawStop();
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
u32 PageMain(u8 event)
{
	if(event==PV_INIT)
	{
		FILE *ficon=fopen("res/flymode.bmp","rb");
		if(ficon)
		{
			setbuf(ficon,0);	
			fseek(ficon,62,SEEK_SET);
			fread(ChsIcon,sizeof(ChsIcon),1,ficon);
			fclose(ficon);
		}
		PageMainDrawMask=PMD_ALL;
		BatteryHandler();//����һ�ε�ѹ���
		return 1;
	}
	
	//�������
	if(PageMainDrawMask)
	{
    	LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);   
		if(PageMainDrawMask&PMD_BACK)	LcdDrawBmpFile(0,0,"res/main.bmp");	
		
		//ģ��ͼ��
		if(PageMainDrawMask&PMD_ICON)	
		{
			char file[20];
			strcpy(file,"icon/");
			strcat(file,Model.Icon);
			strcat(file,".bmp");
			LcdDrawBmpFile(74,9,file);
		}
		//ģ������
		if(PageMainDrawMask&PMD_NAME)	
		{
			u16 x=100-strlen(Model.Name)*6/2;
			LcdWriteStr(x,6,Model.Name);
		}
		
		//���Ƶ��
		if(PageMainDrawMask&PMD_BAT)	DrawBattery();
		if(PageMainDrawMask&PMD_TRIM)	DrawTrimBar();
				
    	LcdDrawStop();
		PageMainDrawMask=0;
	}
	
	//��ʱ�������
	DrawInfo();
	
	//��������
	if(KeyTstDown(KEY_ENT))
	{
		PageEnter(PageMenuMain,PV_INIT);
		PageMainDrawMask=PMD_ALL;
	}
	
	//��ʱ������
	if(PageMainTimerSel>0 && PageMainTimerSel<=3)
	{
		if(KeyTstDown(KEY_R))
		{
			TxTimer[PageMainTimerSel-1].Cnt+=60;
		}
		if(KeyTstDown(KEY_L))
		{
			TxTimer[PageMainTimerSel-1].Cnt-=60;
		}
	}
	if(KeyTstDown(KEY_DW))
	{
		PageMainTimerSel++;
	}
	if(KeyTstDown(KEY_UP))
	{
		PageMainTimerSel--;
	}
	if(PageMainTimerSel>3)	PageMainTimerSel=0;
	else if(PageMainTimerSel<0)	PageMainTimerSel=3;
	else
	{	
		if(KeyTstDown(KEY_EXT))
		{
			TxTimer[PageMainTimerSel-1].Reset=1;
		}
	}
	
	
	//���δ������
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}