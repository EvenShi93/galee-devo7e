#include "Page.h"
#include "Func.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//
char PageIconFiles[ICON_NUM][13];
u16	PageIconFileCnt,PageIconFileIdx,PageIconFileStart;;
u32 PageIconSel(u8 event)
{	
	static u32 DrawMask;
	
	if(event==PV_INIT)
	{
		LcdClear(0);
		LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  
		LcdDrawText(3,0,"ѡ��ģ��ͼ��");		
		LcdDrawHLine(0,128,14,1);
		LcdDrawHLine(0,128,15,1);
		LcdDrawStop();
		
		DrawMask=PD_ALL;
		
		PageIconFileCnt=0;
		PageIconFileIdx=0;
		PageIconFileStart=0;
		
		//ö������ģ��ͼ��
    	if(FS_OpenDir("icon"))
    	{
        	int type;
        	while((type = FS_ReadDir(PageIconFiles[PageIconFileCnt])) != 0 && PageIconFileCnt<ICON_NUM)
        	{
            	if (type == 1)
            	{
            		if(strncasecmp(PageIconFiles[PageIconFileCnt],Model.Icon,strlen(Model.Icon))==0)//Ѱ�ҵ�ǰͼ���ļ�
            		{
            			PageIconFileIdx=PageIconFileCnt;
            		}
            		PageIconFileCnt++;                
            	}
            }
        	FS_CloseDir();
        }
		
		//�ж��Ƿ���ͼ���ļ�
		if(PageIconFileCnt==0)
		{
			PageAlert("û��ͼ���ļ�...",1000,PV_END);
		}
		
		return 1;
	}	
	
	//���ƽ���
	if(DrawMask)
	{		
		//��ʼλ������
		if(PageIconFileIdx<PageIconFileStart) 					PageIconFileStart=PageIconFileIdx;
		if(PageIconFileIdx>PageIconFileStart+LCD_MENULINES-1)	PageIconFileStart=PageIconFileIdx-LCD_MENULINES+1;	
	
		LcdDrawStart(0, 0,LCD_W-1, LCD_H-1, DRAW_NWSE);  
		//�˵���������
		if(DrawMask&PD_IDX)
		{
			if(PageIconFileCnt>9)
			{
				LcdDrawMiniInt(110,4,PageIconFileIdx+1,2,0,0xff,1);
				LcdDrawMiniNum(110,4,LCD_MN_SPA);
				LcdDrawMiniInt(114,4,PageIconFileCnt,2,0,0xff,0);
			}
			else
			{
				LcdDrawMiniInt(120,4,PageIconFileIdx+1,2,0,0xff,1);
				LcdDrawMiniNum(120,4,LCD_MN_SPA);
				LcdDrawMiniInt(124,4,PageIconFileCnt,1,0,0xff,0);
			}
		}
		
		//�����ļ��б��ģ��ͼ��
		if(DrawMask&PD_LIST)
		{
			char file[20];
			
			//��ͼ��
			strcpy(file,"icon/");
			strcat(file,PageIconFiles[PageIconFileIdx]);
			LcdDrawBmpFile(74,22,file);
			
			//��ʾ�б�
			int i,y,idx;
			for(i=0,y=16;i<LCD_MENULINES;i++,y+=16)
			{
				idx=PageIconFileStart+i;
				strcpy(file,PageIconFiles[idx]);
				file[strlen(file)-4]=0;
				LcdBw=(idx==PageIconFileIdx);				
				LcdDrawRect(2,y,70,y+15,LcdBw);		//����ѡ�п�
				LcdDrawMiniInt(2,y+4,idx+1,2,0,0xff,0);	//�������
				LcdDrawMiniNum(10,y+4,LCD_MN_DOT);		//���Ʒָ���
				LcdDrawText(14,y,file);	//��������
				LcdBw=0;
			}
		}
		LcdDrawStop();
		DrawMask=0;
	}
	
	//���°�������
	if(KeyTstDown(KEY_UP))
	{
		if(PageIconFileIdx>0)	PageIconFileIdx--;
		else		 			PageIconFileIdx=PageIconFileCnt-1;
		DrawMask=PD_LIST|PD_IDX;
	}
	if(KeyTstDown(KEY_DW))
	{
		if(PageIconFileIdx<PageIconFileCnt-1)	PageIconFileIdx++;
		else		 							PageIconFileIdx=0;
		DrawMask=PD_LIST|PD_IDX;
	}
	
	//ѡ��λͼ
	if(KeyTstDown(KEY_ENT))
	{
		PageIconFiles[PageIconFileIdx][strlen(PageIconFiles[PageIconFileIdx])-4]=0;
		strcpy(Model.Icon,PageIconFiles[PageIconFileIdx]);
		PageReturn(PV_REDRAW);
	}
	
	//�˳���Ϣ
	if(KeyTstDown(KEY_EXT) || event==PV_END)	PageReturn(PV_REDRAW);
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}