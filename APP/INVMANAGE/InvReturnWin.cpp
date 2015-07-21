/*! \file    InvReturnWin.cpp
\brief    awe4000r发票管理档的发票退回界面
\author   Yu Yan 
\version  1.0
\date     2008-2-14 
*/


#include "InvReturnWin.h"
#include "beep.h"
#include "CaProgressBar.h"
#include "CaMsgBox.h"
#include "InvManageFunc.h"
#include <string>
#include "SysMacDef.h"
#include "CGlobalArg.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CInvReturnWin::CInvReturnWin():CaWindow()
{
	m_pRtInvHead=NULL;
	
}

CInvReturnWin::~CInvReturnWin()
{

}


int CInvReturnWin::Create(int iX,int iY,int iW,int iH)
{
	DBG_PRINT(("进入CInvReturnWin::Create函数"));
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 16;        //控件的列宽
	
	m_pFrame->RegsiterWin(this,INV_RETURN_WIN);
	
	CaWindow::Create(iX,iY,iW,iH);
	
	NormalShow();
	
	//创建一个标签  第四行
	label[4] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH = SCREEN_TOP_OFFSET;
	label[4]->Create(0,curH, SCREEN_W, CHAR_H);
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));
	
	label[0] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	
	//创建一个标签  第二行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[3]->Create(0,curH, SCREEN_W, CHAR_H);
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	
	//创建一个标签  第三行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[1]->Create(0,curH, m_iBtnW+12, CHAR_H);
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	
	//创建一个标签  第三行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	label[2]->Create(m_iColW,curH, m_iBtnW-16, CHAR_H);
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	
	
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "负票");
	m_pBtn1 = new CaButton();
	curH += LINE_H;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	m_pBtn1->Visible(true); 
	
	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn + m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;
	m_pBtn2->Visible(true);
	
	OnActive = S_OnActive;
	
	this->End();
	
	DBG_PRINT(("退出CInvReturnWin::Create函数"));
	return 0;
}

int CInvReturnWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	UINT32 ii;
	char *content;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(INV_MANAGE_MENU);				
		return SUCCESS;
		break;
	case OPEN_CASH_BOX:
		MoneyBox_Open();
		break;
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}



int CInvReturnWin::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();
	
	return 1;
}


void CInvReturnWin::S_OnActive(CaWindow *obj)
{
	((CInvReturnWin *)obj)->DoActive();
}


void CInvReturnWin::DoActive()
{
	DBG_PRINT(("进入CInvReturnWin::DoActive函数"));
	UINT8 ret;	

	m_pQueryWin = (CInvRetQueryWin*)m_pFrame->GetWin(INV_RET_QUERY_WIN);//至关重要，将m_pQueryWin与INV_RET_QUERY_WIN界面绑定在一起 
    m_pRtInvHead =&(m_pQueryWin->m_InvHead);//m_InvHead存储着要退发票的信息，赋值给m_pRtInvHead
	DBG_PRINT(("m_pRtInvHead->m_fphm= %u",m_pRtInvHead->m_fphm));
	
	//修改屏幕显示才	
	ReturnShow(m_pRtInvHead);
	ChangeTitle();
	ReFresh();
	
	DBG_PRINT(("退出CInvReturnWin::DoActive函数"));
	
	return;
}

void CInvReturnWin::NormalShow()
{
	sprintf(title_array[0], "");
	sprintf(title_array[1], "");
	sprintf(title_array[2], "");
	sprintf(title_array[3], "");
	sprintf(title_array[4], "");
}

UINT8 CInvReturnWin::ChangeTitle()
{
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));
	
	return SUCCESS;
}

UINT8 CInvReturnWin::ReturnShow(CInvHead *pInvHead)
{
	DBG_PRINT(("进入CInvReturnWin::ReturnShow函数"));
	
	TDateTime retDateTime(pInvHead->m_kprq, pInvHead->m_kpsj);
	string strDateTime = retDateTime.FormatString(YYYYMMDDHHMMSS);
	
	double moneySum = ((double)(pInvHead->m_kphjje))/SUM_EXTENSION;
	sprintf(title_array[0], "所退发票号码:%08u", pInvHead->m_fphm);
	sprintf(title_array[1], "合计:-%.2lf", moneySum);
	sprintf(title_array[2], "商品行数目:%u", pInvHead->m_sphsl);
	sprintf(title_array[3], "开票时间:%s", strDateTime.c_str());
	sprintf(title_array[4], "当前发票号:%08u", g_globalArg->m_curInvVol->m_curInvNo);
	
	m_pBtn1->Visible(true); //可见
	m_pBtn2->Visible(true); //可见
	
	DBG_PRINT(("title_array[1] = %s", title_array[1]));
	DBG_PRINT(("title_array[2] = %s", title_array[2]));
	DBG_PRINT(("退出CInvReturnWin::ReturnShow函数"));
	return SUCCESS;	
}


void CInvReturnWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvReturnWin *win=(CInvReturnWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CInvReturnWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvReturnWin *win=(CInvReturnWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvReturnWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	if (0==g_globalArg->m_curInvVol->m_curInvNo)
	{
		//ErrMsgBox(INV_ZERO);
		return;	
	}
	
   UINT8 ret=SUCCESS;

   	CInvHead *pInvHead;
	pInvHead = m_pRtInvHead;

	//检查是否满足负票条件
	DBG_PRINT(("m_pRtInvHead->m_fphm= %u",m_pRtInvHead->m_fphm));
	ret=INVM_CheckIsNoRet(pInvHead);
	DBG_PRINT(("ret= %u",ret));
	if (ret !=SUCCESS)
	{
		INVM_ErrMsgBox(ret);
		return ;
	}
	DBG_PRINT(("m_pRtInvHead->m_fphm= %u",m_pRtInvHead->m_fphm));


	ret =INVM_InvRetProc(m_pRtInvHead);	//退本机发票,m_pRtInvHead存储着要退发票的信息
	DBG_PRINT(("ret= %u",ret ));

	if (ret!=SUCCESS)
	{
		INVM_ErrMsgBox(ret);
		ChangeWin(INV_RET_QUERY_WIN);
	}
	else
	{
		ChangeWin(INV_MANAGE_MENU);	 //返回发票管理界面				
	}
	
   return;
}


void CInvReturnWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(INV_MANAGE_MENU);	 //返回发票管理界面		
}






