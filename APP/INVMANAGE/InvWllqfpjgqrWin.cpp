/*! \file    InvWllqfpjgqrWin.cpp
   \brief    awe4000r网络领取发票结果确认界面
   \author   lzh
   \version  1.0
   \date     2015-06-03
 */
#if 0

#include "InvWllqfpjgqrWin.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include <string.h>
#include "stdlib.h"
#include "InvVolFunc.h" 
#include "SysMacDef.h"
#include "CGlobalArg.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CInvWllqfpjgqrWin::CInvWllqfpjgqrWin():CaWindow()
{
	m_invCode = "";
	m_invStartNo = 0;
	m_invEndNo = 0;
}

int CInvWllqfpjgqrWin::Create(int iX,int iY,int iW,int iH)
{
	int curH = 0;
	char title[OBJ_TITLE_MAX_LEN + 1];

    int titleLen = 0;
	m_pFrame->RegsiterWin(this,INV_WLLQFPJGQR_WIN);    
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	curH = SCREEN_TOP_OFFSET;
	m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽


	//创建一个Input  第一行
	strcpy(title, "发票代码:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(12);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受整型值
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "起始号码:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	curH += LINE_H;
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);
	m_pInput2->SetMaxLen(8);
    m_pInput2->OnObject = S_OnInput2;
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值

	strcpy(title, "终止号码:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	curH += LINE_H;
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);
	m_pInput3->SetMaxLen(8);
    m_pInput3->OnObject = S_OnInput3;
	m_pInput3->m_InputType = m_pInput3->aINT; //该输入框只接受整型值

	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += 2*LINE_H;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;	

	OnActive = S_OnActive;
	this->End();
	return 0;
}

int CInvWllqfpjgqrWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to INV_SEG_MENU
		ChangeWin(INV_MANAGE_MENU);	
		
		return SUCCESS;
		break;
		
	default: break;		
	}
   return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CInvWllqfpjgqrWin::ReFresh()
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


void CInvWllqfpjgqrWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWllqfpjgqrWin *win=(CInvWllqfpjgqrWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CInvWllqfpjgqrWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWllqfpjgqrWin *win=(CInvWllqfpjgqrWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}
void CInvWllqfpjgqrWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWllqfpjgqrWin *win=(CInvWllqfpjgqrWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CInvWllqfpjgqrWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWllqfpjgqrWin *win=(CInvWllqfpjgqrWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CInvWllqfpjgqrWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWllqfpjgqrWin *win=(CInvWllqfpjgqrWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvWllqfpjgqrWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;	

}
void CInvWllqfpjgqrWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;	
}
void CInvWllqfpjgqrWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;	
}

void CInvWllqfpjgqrWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
 	string strErr("");
 	
 	//检查发票代码和发票号码 张数
 	if (CheckInputValid(strErr) == FAILURE) 
 	{
 		return ;
 	}

	CaMsgBox msgBox("网络领取发票结果确认？", CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return; 
	}
   
	UINT8 ret=SUCCESS;

	CaProgressBar proBar("");
	proBar.SetText("网络领取发票结果确认中...");
    proBar.Show();

	ret= INV_NetInvVolVerify( m_invCode, m_invStartNo, m_invEndNo, strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	CaMsgBox::ShowMsg("网络领取发票结果确认");
	
}

void CInvWllqfpjgqrWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
		ChangeWin(INV_MANAGE_MENU);	
}


UINT8 CInvWllqfpjgqrWin::CheckInputValid(string &strErr)
{
 	UINT8 nRet = SUCCESS;
 
 	//输入不能为空
 	if(m_pInput1->IsEmpty())
 	{ 
 		CaMsgBox::ShowMsg("请输入发票代码");
 		m_pInput1->SetFocus();
 		this->ReFresh();
 		return FAILURE;
 	}
 
 	if(m_pInput2->IsEmpty())
 	{ 
 		CaMsgBox::ShowMsg("请输入起始号码");
 		m_pInput2->SetFocus();
 		this->ReFresh();
 		return FAILURE;
 	}
 
	if(m_pInput3->IsEmpty())
	{ 
		CaMsgBox::ShowMsg("请输入终止号码");
		m_pInput3->SetFocus();
		this->ReFresh();
		return FAILURE;
 	}

 	
 	UINT32 nLen = strlen((char*)(m_pInput1->m_contentBuf));
 	if (nLen != 2*INV_TYPE_CODE_LEN) 
 	{
 		CaMsgBox::ShowMsg("发票代码非法");
 		m_pInput1->SetFocus();
 		this->ReFresh();
 		return FAILURE;
 	}
 	else if (0 == atoi((INT8 *)m_pInput1->m_contentBuf)) 
 	{
 		DBG_PRINT(("发票代码非法"));
 		CaMsgBox::ShowMsg("发票代码非法");
 		m_pInput1->SetFocus();
 		this->ReFresh();
 		return FAILURE;
 	}

    m_invCode = (INT8 *)(m_pInput1->m_contentBuf);


 	//获取发票卷起始号码
 	m_invStartNo = atoi((INT8 *)m_pInput2->m_contentBuf);
 	if(m_invStartNo == 0)
 	{
 		CaMsgBox::ShowMsg("起始号码不能为零");
 		m_pInput2->SetFocus();
 		this->ReFresh();
 		return FAILURE;
 	}

	m_invEndNo = atoi((INT8 *)m_pInput3->m_contentBuf);
	
	if(m_invEndNo == 0)
	{
		CaMsgBox::ShowMsg("终止号码不能为零");
		m_pInput3->SetFocus();
		this->ReFresh();
		return FAILURE;
	}
 	
 	this->ReFresh();
	return nRet;
}

void CInvWllqfpjgqrWin::S_OnActive(CaWindow *obj)
{
	((CInvWllqfpjgqrWin *)obj)->DoActive();
}


void CInvWllqfpjgqrWin::DoActive()
{
	DBG_ENTER("CInvWllqfpjgqrWin::DoActive()");

	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();

	ReFresh();
}

#endif
