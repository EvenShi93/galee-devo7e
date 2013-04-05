#ifndef _PAGE_H_
#define _PAGE_H_

#include "Macros.h"

//ҳ�洦����̶���
#define PV_RUN		0
#define PV_INIT		1
#define PV_END		2
#define PV_REDRAW	3
#define PV_TIMER	4
#define PV_USER		0x80

typedef u32 (*PAGEPROC)(u8 event);

//�˵����ݽṹ����
typedef const char * CSTR;
typedef struct{
	char *		Title;
	void *		SubItem;	//�ӹ���|�Ӳ˵�
	CSTR *		Values;		//ֵ�б�	 
	s16			Min,Max;	//�˵��������Сֵ
	s16			*pValue;	//�˵���Ӧ���ڴ�λ��
	void *		Data;	//����
	char *		Text;		//��ʾ�ַ���
}MENUITEM,*PMENUITEM;
typedef const MENUITEM CMENUITEM;
#define SUBMENU(X)	((CPMENUITEM)((X)->SubItem))
#define SUBPROC(X)	((PAGEPROC)((X)->SubItem))

//�˵�״̬����
typedef struct{
	u8 Init;
	u8 DrawMask,iFocus,iStart,Total;
	u8 Param;
}MENUSTAT;

#define PD_TITLE	1
#define PD_LIST		2
#define PD_IDX		4
#define PD_VALUE	8
#define PD_ALL		0xff

//ҳ����ƺ���
#define PAGELEVEL	5			//���������뼸��UI
extern PAGEPROC PageStack[PAGELEVEL];
extern u8 PageStackIdx;
void PageEnter(PAGEPROC page,u8 msg);
void PageGoto(PAGEPROC page,u8 msg);
void PageReturn(u8 msg);
void PageSet(PAGEPROC page,u8 msg);
void PageAlertModel(CSTR text,u32 delay);
void PageAlert(CSTR text,u32 delay,u8 msg);
void PageMenuProc(CMENUITEM *pmi,MENUSTAT *pms);

//��������ƿ���
#define PMD_BACK	1
#define PMD_ICON	2
#define PMD_NAME	4
#define PMD_BAT		8
#define PMD_TRIM	0X10
#define PMD_INFO	0x20
#define PMD_ALL		0xffffffff
extern u32 PageMainDrawMask;
u32 PageMain(u8 init);
void BatteryHandler(void);

//΢������
#define TRIM_NUM_HIDE 99
extern u32 PageMainTrimShowCnt;
void TrimProc(void);

//�˵�ϵ��
u32 PageMenuMain(u8 event);
u32 PageMenuSys(u8 event);
u32 PageMenuMod(u8 event);
u32 PageMenuRf(u8 event);
u32 PageStkCal(u8 event);
u32 PageMonitor(u8 event);
u32 PagePpmIn(u8 event);
u32 PageUsb(u8 event);
u32 PageBind(u8 event);
u32 PageIconSel(u8 event);
u32 PageEditName(u8 event);
u32 PageEditId(u8 event);
u32 PageModelSel(u8 event);
u32 PageHeliSet(u8 event);
u32 PageAeroSet(u8 event);
u32 PageMenuChMap(u8 event);
u32 PageTimerSet1(u8 event);
u32 PageTimerSet2(u8 event);
u32 PageTimerSet3(u8 event);

//ģ�Ͳ����˵�ϵ��
u32 PageNorRevSet(u8 event);
u32 PageEpaSet(u8 event);
u32 PageNeuSet(u8 event);
u32 PageDeltaSet(u8 event);
u32 PageVtailSet(u8 event);
u32 PageFlpAilSet(u8 event);
u32 PageAuxChSet(u8 event);
u32 PageGyroSet(u8 event);
u32 PageSwashSet(u8 event);
u32 PageDelaySet(u8 event);
u32 PageSwDef(u8 event);
u32 PageMixerSet(u8 event);
u32 PageThrMixSet(u8 event);
u32 PageYawMixSet(u8 event);

//�������ú���
extern CSTR *PageCurveTitle;
extern u8 PageCurveCnt;
extern s16 *PageCurveValue;
u32 PageCurveSet(u8 event);

//ģ�Ͳ�����ȡ����
void ModelLoad(u16 id,u8 txopen);
void ModelSave(u16 id);
void ModelFormat(void);

//����
u32 PageAbout(u8 init);

extern CSTR PageOptionNorRev[];
extern CSTR PageOptionOnOff[];
extern CSTR PageOptionStkType[];
extern CSTR PageOptionBatType[];
extern CSTR PageOptionOkExe[];
extern CSTR PageOptionModType[];
extern CSTR PageOptionSwDef[];
extern CSTR PageOptionProtocol[];
extern CSTR PageOptionRfPwr[];
extern CSTR PageOptionChSelA[];
extern CSTR PageOptionChSelH[];
extern CSTR PageOptionAuxSrc[];
extern CSTR PageOptionTmrType[];
extern CSTR PageOptionTmrSw[];
extern CSTR PageStkCurveTitle[];
extern CSTR PageThrCurveTitle[];
extern CSTR PagePitCurveTitle[];
extern CSTR PageOptionSwash[];
extern CSTR PageOptionLightOff[];
extern CSTR PageOptionAutoOff[];
extern CSTR PageOptionFreeWarn[];
#endif//_PAGE_H_