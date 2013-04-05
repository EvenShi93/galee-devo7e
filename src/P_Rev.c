#include "Page.h"
#include "Func.h"

CMENUITEM MenuRevSet[]={
	{"��������"	,0,0,0,8,0,0,0},
	{"ͨ��1:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[0],0,0},
	{"ͨ��2:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[1],0,0},
	{"ͨ��3:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[2],0,0},
	{"ͨ��4:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[3],0,0},
	{"ͨ��5:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[4],0,0},
	{"ͨ��6:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[5],0,0},
	{"ͨ��7:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[6],0,0},
	{"ͨ��8:"	,0,PageOptionNorRev	, 0,1,&Model.Rev[7],0,0},
};

///////////////////////////////////////////////////////////////////////////////////////
//
//  ģ�Ͳ������ڲ˵��������
//
u32 PageNorRevSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LcdClear(0);
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuRevSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuRevSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}