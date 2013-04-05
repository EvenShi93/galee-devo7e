#include "Page.h"
#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
// GUI���ƺ���
//
PAGEPROC PageStack[PAGELEVEL];
u8 PageStackIdx;
void PageEnter(PAGEPROC page,u8 msg)
{
	if(PageStackIdx>=PAGELEVEL-1)	return;
	PageStack[++PageStackIdx]=page;
	page(msg);
}

void PageGoto(PAGEPROC page,u8 msg)
{
	PageStack[PageStackIdx]=page;	
	page(msg);
}

void PageReturn(u8 msg)
{
	if(PageStackIdx>0)	PageStackIdx--;
	PageStack[PageStackIdx](msg);
}

void PageSet(PAGEPROC page,u8 msg)
{
	PageStack[PageStackIdx=0]=page;
	page(msg);
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ����Ի���
//
char *PageAlertText;
u32   PageAlertDelay;
u8	  PageAlertMsg;

void PageAlertDraw(void)
{
	int s,p,l;		
	l=6*strlen(PageAlertText);
	s=(LCD_W-l)/2;
	p=(LCD_W+l)/2;
	LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  

	if(PageAlertDelay==0)
	{
		LcdDrawRect(s-6, 9,p+6,56,0);
		LcdDrawRect(s-5,10,p+5,55,1);
		LcdDrawRect(s-4,11,p+3,53,0);
		LcdDrawText(s,18,PageAlertText);		//��������	
		LcdBw=1;
		LcdDrawText(LCD_W/2-16,36," EXT ");		//���ư�ť	
		LcdBw=0;
	}
	else
	{
		LcdDrawRect(s-6, 9,p+6,41,0);
		LcdDrawRect(s-5,10,p+5,40,1);
		LcdDrawRect(s-4,11,p+3,38,0);
		LcdDrawText(s,18,PageAlertText);		//��������	
	}
	LcdDrawStop();
}
	
u32 PageAlertProc(u8 event)
{
	static u32 starttime;
	if(event==PV_INIT)
	{
		PageAlertDraw();
		starttime=SysTimerClk;
		return 1;
	}
	
	if(PageAlertDelay==0)
	{
		//��������
		if(KeyTstDown(KEY_EXT))
		{
			PageReturn(PageAlertMsg);
		}
	}
	else
	{
		if(SysTimerClk-starttime>PageAlertDelay)
		{
			PageReturn(PageAlertMsg);
		}
	}
	return 0;
}

void PageAlert(CSTR text,u32 delay,u8 retmsg)
{
	if(!text)	return;
	PageAlertText=(char*)text;
	PageAlertDelay=delay;
	PageAlertMsg=retmsg;
	PageEnter(PageAlertProc,PV_INIT);
}

void PageAlertModel(CSTR text,u32 delay)
{
	if(!text)	return;
	PageAlertText=(char*)text;
	PageAlertDelay=delay;
	PageAlertDraw();

	if(PageAlertDelay==0)
	{
		while(!KeyTstStat(KEY_EXT)) 
		{
			SysProc();
			usleep(1000);
		}
	}
	else
	{
		while(delay--)
		{
			SysProc();
			usleep(1000);
		}
	}		
	KeyFlush();	
}
///////////////////////////////////////////////////////////////////////////////////////
//
//  �˵�ͨ�ù��̴���
//
void PageMenuProc(CMENUITEM *pmi,MENUSTAT *pms)
{	
	//��ʼ��ͼ
	if(pms->DrawMask)    
	{
		LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  
	
		//�������
		if(pms->DrawMask&PD_TITLE)
		{
			LcdDrawRect(0, 0,127,13,0);
			LcdDrawText(3,0,pmi[0].Title);
			LcdDrawHLine(0,127,14,1);
			LcdDrawHLine(0,127,15,1);
		}
		//�˵���������
		if(pms->DrawMask&PD_IDX)
		{
			if(pms->Total>9)
			{
				LcdDrawMiniInt(110,4,pms->iFocus+1,2,0,0xff,1);
				LcdDrawMiniNum(110,4,LCD_MN_SPA);
				LcdDrawMiniInt(114,4,pms->Total,2,0,0xff,0);
			}
			else
			{
				LcdDrawMiniInt(120,4,pms->iFocus+1,2,0,0xff,1);
				LcdDrawMiniNum(120,4,LCD_MN_SPA);
				LcdDrawMiniInt(124,4,pms->Total,1,0,0xff,0);
			}
		}
		
		//�˵��б�
		if(pms->DrawMask&(PD_LIST|PD_VALUE))
		{
			int i,s,p,y,idx;
			for(i=0,y=16;i<LCD_MENULINES && i<pms->Total;i++,y+=16)
			{
				idx=pms->iStart+i;
				LcdBw=(idx==pms->iFocus);
				idx++;
				
				//����ǻ��������б����Ҫ������������
				if(pms->DrawMask&PD_LIST || LcdBw)
				{
					LcdDrawRect(1,y,126,y+15,LcdBw);		//����ѡ�п�
					LcdDrawMiniInt(2,y+4,idx,2,0,0xff,0);	//�������
					LcdDrawMiniNum(10,y+4,LCD_MN_DOT);		//���Ʒָ���
					LcdDrawText(13,y,pmi[idx].Title);		//��������
					
					//���ӹ��ܵļӼ�ͷ��ʾ
					if(pmi[idx].SubItem) 
					{
						if(LcdBw)	LcdDrawText(114,y,"\6\2");//>>
						else		LcdDrawText(120,y,"\2");//>
					}
				
					//���ı���ѡ����ʾ�ı�
					if(pmi[idx].Text)
					{
						s=110-6*strlen(pmi[idx].Text);
						//LcdDrawMiniInt(60,y+4,s,2,0,0,0);	//�������
						//p=LcdDrawText(60,y,"test");
						p=LcdDrawText(s,y,pmi[idx].Text);
						LcdDrawHLine(s-1,p+1,y+14,0);//��һ���»���
					}
					//û���ı���ѡ����ʾֵ
					else if(pmi[idx].pValue)
					{
						if(pmi[idx].pValue)
						{
							s16 v=*pmi[idx].pValue;
							if(v>pmi[idx].Max) v=pmi[idx].Max;
							if(v<pmi[idx].Min) v=pmi[idx].Min;
							*pmi[idx].pValue=v;
							
							//����ѡ�����ʾ����
							if(pmi[idx].Values)
							{
								s=118-6*strlen(pmi[idx].Values[v]);
								p=LcdDrawText(s,y,(char*)pmi[idx].Values[v]);
							}
							//����ѡ�����ʾ����
							else
							{
								LcdDrawInt(s=90,y,v,3,(u32)pmi[idx].Data,0,0);
								p=90+24+2;
							}
							
							//ѡ�е�ʱ����ʾ�༭״̬
							if(LcdBw)
							{
								if(v==pmi[idx].Max)
									LcdDrawText(120,y,"\4");//SPIN
								else if(v==pmi[idx].Min)
									LcdDrawText(120,y,"\5");//SPIN
								else
									LcdDrawText(120,y,"\3");//SPIN
								LcdDrawHLine(s-1,p+1,y+14,0);//��һ���»���
							}
						}
					}
				}
				LcdBw=0;
			}
		}
		
		//��ɻ�ͼ		
		LcdDrawStop();
		pms->DrawMask=0;
	}
	
	//�Ӽ���������
	MENUITEM *mi=(MENUITEM *)&pmi[pms->iFocus+1];
	if(KeyTstDown(KEY_R))
	{
		if(mi->pValue)
		{
			if(*mi->pValue<mi->Max) 
			{
				(*mi->pValue)++;
				pms->DrawMask|=PD_VALUE;
			}
		}
	}
	if(KeyTstDown(KEY_L))
	{
		if(mi->pValue)
		{
			if(*mi->pValue>mi->Min) 
			{
				(*mi->pValue)--;
				pms->DrawMask|=PD_VALUE;
			}
		}
	}
	
	//���°�������
	if(KeyTstDown(KEY_UP))
	{
		if(pms->iFocus>0) pms->iFocus--;
		else		 pms->iFocus=pms->Total-1;
		pms->DrawMask=PD_LIST|PD_IDX;
	}
	if(KeyTstDown(KEY_DW))
	{
		if(pms->iFocus<pms->Total-1) pms->iFocus++;
		else		 		pms->iFocus=0;
		pms->DrawMask=PD_LIST|PD_IDX;
	}
	if(pms->iFocus>=pms->Total)	pms->iFocus=pms->Total-1;
	if(pms->iFocus<pms->iStart) 				pms->iStart=pms->iFocus;
	if(pms->iFocus>pms->iStart+LCD_MENULINES-1) pms->iStart=pms->iFocus-LCD_MENULINES+1;	
}	

//�˵�ѡ��
CSTR PageOptionOnOff[]={"�ر�","����"};
CSTR PageOptionNorRev[]={"�� --","-- ��"};
CSTR PageOptionOkExe[]={"��ִ��","ִ��","ȫ��"};
CSTR PageOptionStkType[]={"�ձ���","������","�й���","�ձ�����"};
CSTR PageOptionBatType[]={"4S ����","4S ����","1S ﮵�","2S ﮵�"};
CSTR PageOptionModType[]={"�̶���","ֱ����"};
CSTR PageOptionSwDef[]={"����","HOLD","FMOD","��3��","��3��"};
CSTR PageOptionProtocol[]={"�ر�","PPM","DEVO","DSM2","DSMX","WK2801","FlySky","Hubsan","SkyAtc","J6Pro"};
CSTR PageOptionRfPwr[]={"100uW","300uW","1mW","3mw","10mW","30mW","100mW","150mW"};
CSTR PageOptionChSelA[]={"����","����","����","����","�����","����","����1","����2","ҡ��","PPM CH1","PPM CH2","PPM CH3","PPM CH4","PPM CH5","PPM CH6","PPM CH7","PPM CH8"};
CSTR PageOptionChSelH[]={"����","����","����","����","����"  ,"�ݾ�","����1","����2","ҡ��","PPM CH1","PPM CH2","PPM CH3","PPM CH4","PPM CH5","PPM CH6","PPM CH7","PPM CH8"};
CSTR PageOptionAuxSrc[]={"0%","+100%","-100%","����X","����Y"};
CSTR PageOptionTmrType[]={"����","����","��ʱ","����ʱ"};
CSTR PageOptionTmrSw[]={"����","HOLD0","HOLD1","FMOD0","FMOD1"};
CSTR PageOptionSwash[]={"�����","120\7CCPM","140\7CCPM","90\7CCPM"};
CSTR PageOptionLightOff[]={"����","10��","20��","30��","40��","50��","60��"};
CSTR PageStkCurveTitle[]={"����DR0����","����DR1����","����DR2����","����DR0����","����DR1����","����DR2����","����DR0����","����DR1����","����DR2����"};
CSTR PageThrCurveTitle[]={"������������","�ؼ���������"};
CSTR PagePitCurveTitle[]={"�����ݾ�����","�����ݾ�����","�ؼ��ݾ�����"};
CSTR PageOptionAutoOff[]={"����","10��","20��","30��"};
CSTR PageOptionFreeWarn[]={"����","1��","2��","3��","4��","5��"};