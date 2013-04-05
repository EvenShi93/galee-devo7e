#ifndef _MACROS_H_
#define _MACROS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "std.h"
#include "common.h"

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/f1/adc.h>
#include <libopencm3/stm32/exti.h>

//���Ͷ���
#ifndef LIBOPENCM3_CM3_COMMON_H
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#endif

//��������
#define VECTOR_TABLE_LOCATION 0x3000
#define LCD_W				128
#define LCD_H				64
#define LCD_MENULINES		3
#define	KEY_CONT_TRIG		(800/25)	//��������ʱ�䴥������
#define	KEY_CONT_FREQ		(140/25)	//����ʱ������һ��
#define LCD_BAT_X			90			//��ص�����ʾλ��
#define LCD_BAT_Y			1
#define LCD_BAT_FULL		8			//�����״ͼ�������
#define ADC_NUM				7			//ADCͨ����
#define	ADC_OVERSAMPLE		1			//ADC��������
#define ADC_AVG_NUM 		10			//ADCƽ��ϵ��
#define VOLTAGE_NUMERATOR 	210			//Compute voltage from y = 2.1592x + 0.2493
#define VOLTAGE_OFFSET    	249
#define BAT_TST_CYCLE		500			//��ؼ�����ڣ���λms
#define BAT_LOW_CNT			5			//��ص�ѹ�澯�˲�����
#define LCD_TRIM_X1			2
#define LCD_TRIM_Y1			59
#define LCD_TRIM_X2			57
#define LCD_TRIM_Y2			7
#define LCD_TRIM_X3			75
#define LCD_TRIM_Y3			59
#define LCD_TRIM_X4			66
#define LCD_TRIM_Y4			7
#define LCD_TRIM_W			48			//΢�����ȣ�ʵ����ʾ���Ϊ������+3
#define	LCD_TRIM_SIZE		5
#define TRIM_NUM_TIME		6			//΢�������Զ�����ʱ�䣬��λ0.25s
#define MAIN_FREQ			53			//1ms��ѭ������(����һЩ�����ȼ�HANDLER)
#define ADC_TRV				2048		//ADC����
#define	STK_TRV				512			//ҡ���г�
#define ICON_NUM			20			//���֧�ּ���ģ��ͼ��
#define MODCFG_NUM			30			//���֧�ּ���ģ��
#define CURVE_NUM			9			//��������
#define TXTMR_NUM			3
#define	THRSAFE				-495		//��ȫ���ţ�������⣩
#define STKMOVTST			6			//ҡ�˾�ֹ����ż�����ֵԽ��Լ�ٶ�		
#define CHROCKDELAY			1			//����Զ����Ե��ٶȣ���ֵԽ�󣬶���Խ��
#define CHROCKSTEP			3

//��ز�������
#define	MAX_EPA				120
#define EPA_SIZE			100			//EPA����
#define	NEU_STEP			3			//�����㲽��
#define	TRIM_STEP			5			//ҡ��΢������
#define	MAX_DLT				100			//���������г�
#define	MAX_VTL				100			//Vβ����г�
#define	MAX_FAM				100			//���������г�
#define	MAX_THM				100			//���Ż���г�
#define	MAX_YWM				100			//ת�����г�
#define	DELAY_MAX			20			//ͨ����������
#define	MAX_AUX				100			//AUXͨ������
#define	TRV_AUX				120			//AUXͨ������
#define	MAX_GYRO			100
#define	TRV_GYRO			120
#define CURVE_TRV			1000

//ҡ�˲�������
#define STK_AIL				0
#define STK_ELE				1
#define	STK_THR				2
#define STK_RUD				3
#define STK_NUM				4
extern u16 StickRaw[STK_NUM];
extern s16 StickCal[STK_NUM];
extern s16 StickValue[STK_NUM];
extern s16 StickTrim[STK_NUM];
void StickHandler(void);

//�Զ���ʾ���Զ��ػ�����
extern u32 AutoOffTime;
extern u32 ChStopTime;
void AutoOffReset(void);

//�������ܺ���
int fputbuf(void *buf,int n,FILE *f);
void LoadCfg(void);
void SaveCfg(void);
void TimerProc(void);
void SysProc(void);

//ģ�Ͳ���
#define TRIM_MAX			50

//ģ�����ݽṹ
typedef enum{
	MT_AERO,
	MT_HELI,
	MT_NUM
}MODELTYPE;

//ͨ��ӳ�����
#define CH_PPMIN	9
#define CH_TEST		8

////////////////////////////////////
//��ʱ������
#define TMR_DIS		0
#define	TMR_HIDE	1
#define TMR_CLK		2	//��ʱ��
#define TMR_DCK		3	//����ʱ

//��ʱ�����ƿ���
#define TMRSW_THR	0	
#define TMRSW_HOLD	1
#define TMRSW_HOLDR	2
#define TMRSW_FMOD	3
#define TMRSW_FMODR	4

typedef struct{
	s16 Type;
	s16	Sw;
	s16 Thr;
	s16 Time;
}TIMER;

typedef struct{
	s32	Cnt;
	u8	Run;
	u8	Alert;
	u8  Reset;
	TIMER *Cfg;
}TIMERRUN;
////////////////////////////////////
typedef struct{
	s16	Enable;
	s16 A2A,E2A,A2E,E2E;
}DELTAMIX;

typedef struct{
	s16	Enable;
	s16 E2E,R2E,E2R,R2R;
}VTAILMIX;

typedef struct{
	s16	Enable;
	s16 A2A,A2F,F2A,F2F;
}FLAPAILMIX;

typedef struct{
	s16 Rl2E,Rr2E,Al2E,Ar2E;
}YAWMIX;


#define CFGMARK 0x454c4130
#define CALMARK	0x43
#define MODELNL		9
#define MODELIDL	6

//��PageOptionModType���
#define	MT_AERO		0
#define	MT_HELI		1

#define	FM_HLD		0
#define FM_NOR		1
#define	FM_IDL		2

#define CURVE_DS	(CURVE_NUM+3)

//���ض��壨SwDef��
#define SWD_HOLD	0
#define SWD_IDLE	1
#define SWD_DR		2
#define SWD_GEAR	3
#define SWD_FLAP	4
#define SWD_AUX1	5
#define SWD_AUX2	6


//�°������ļ�
typedef struct{
	u32 	Mark;
	char 	Name[MODELNL+1];
	char 	Icon[8];
	s16		Type;
	s8		Trim[4];
	s16		Protocol;
	u32		RfId;
	char	RfIdStr[MODELIDL+1];
	s16		RfPwr;
	s16		RfChNum;
	s16		ChMap[12];
	TIMER	Timer[TXTMR_NUM];
	s16		Rev[12];
	s16		Epa[12][2];
	s16		Neu[12];
	s16		Delay[12];
	s16		ThrHold;
	DELTAMIX 	Delta;
	VTAILMIX 	Vtail;
	FLAPAILMIX	FlpAil;
	YAWMIX		YawMix;
	s16			ThrMix[3];
	s16		StkCurve[3][3][CURVE_DS];
	s16		ThrCurve[2][CURVE_DS];
	s16		PitCurve[3][CURVE_DS];
	s16		Gyro[2];
	s16		SwashType;
	s16		Swash[3];
	s16		Gear[3];
	s16		Flap[3];
	s16		Aux[6][3];
	s16		SwDef[8];
	u8		PpmIn;
}MODEL;

extern MODEL Model;
extern const MODEL ModelDef;

#define TXCFGMARK 0x454c4147
typedef struct{
	u32 Mark;
	u8  StkCalied;		//ҡ���Ƿ�У׼
	s16 ModelNo;
	s16 StkType;		//0�ձ� 1���� 2���ձ� 3������
	s16 StkCali[STK_NUM][3];
	s16 StkDa;			//ҡ������
	s16	Vibrator;
	s16	Music;
	s16 KeyBeep;
	s16 KeyTone;
	s16 Light;
	s16 Contrast;
	s16 LightOff;
	s16 BatType;
	s16 BatWarn;
	s16	AutoOff;
	s16 FreeWarn;
}TXSYS;
extern TXSYS TxSys;
extern const TXSYS TxSysDef;

extern u32 LightStartTmr;	//����رռ�ʱ��
extern TIMERRUN TxTimer[TXTMR_NUM];	//��ʱ��������

#endif//_MACROS_H_