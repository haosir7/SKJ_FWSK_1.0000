/*! \file  InvRetQueryWin.cpp
\brief    awe4000r发票管理档的红票开具查询界面
\author   zcy 
\version  1.0
\date     2015-5-11 
*/

#include "InvRetQueryWin.h"
#include "CaProgressBar.h"
#include "CaMsgBox.h"
#include "SysMacDef.h"
#include "CGlobalArg.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CInvRetQueryWin::CInvRetQueryWin():CaWindow()
{
	m_invCode = "";
	m_invNo = 0;
}

CInvRetQueryWin::~CInvRetQueryWin()
{
	
}


int CInvRetQueryWin::Create(int iX,int iY,int iW,int iH)
{
	DBG_PRINT(("进入CInvRetQueryWin::Create函数"));
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
	
	m_pFrame->RegsiterWin(this,INV_RET_QUERY_WIN);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//NormalShow();
	
	//创建一个标签  第一行
	label[0] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH = SCREEN_TOP_OFFSET;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	
	
	//创建一个Input  第二行
	strcpy(title, "所退发票代码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(20);
	m_pInput1->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第三行
	strcpy(title, "所退发票号码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;
	
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "查询");
	m_pBtn1 = new CaButton();
	curH += LINE_H;
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
	
	DBG_PRINT(("退出CInvRetQueryWin::Create函数"));
	return 0;
}

int CInvRetQueryWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(INV_MANAGE_MENU);					
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}



int CInvRetQueryWin::ReFresh()
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

void CInvRetQueryWin::S_OnActive(CaWindow *obj)
{
	((CInvRetQueryWin *)obj)->DoActive();
	
}


void CInvRetQueryWin::DoActive()
{
	DBG_PRINT(("进入CInvRetQueryWin::DoActive()函数"));
	
	NormalShow();
	
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();
	
	ChangeTitle();
	m_pInput1->SetContentBuf((UINT8*)g_globalArg->m_curInvVol->m_code.c_str(), g_globalArg->m_curInvVol->m_code.length());
	ReFresh();
	
	//每卷第一张时提醒用户换卷
	if (1 == g_globalArg->m_curInvVol->m_curInvNo%DEFAULTE_VOL_NUM)
	{
		CaMsgBox::ShowMsg("请确认纸质发票卷与当前发票号对应!");
	}

	DBG_PRINT(("退出CInvRetQueryWin::DoActive()函数"));
}

void CInvRetQueryWin::NormalShow()
{
	
	string strErr("");
	UINT8 ret=SUCCESS;
	UINT8 uFlag=0;
 	if (0 == g_globalArg->m_curInvVol->m_curInvNo) //避免反复读盘
 	{
 		ret = SALE_GetCurInv(g_globalArg->m_curInvVol,strErr);
 		if (ret ==FAILURE)
 		{
 			uFlag=1;
			if (strErr.length()>OBJ_TITLE_MAX_LEN)
			{
				strErr = strErr.substr(0,OBJ_TITLE_MAX_LEN);
			}
 			sprintf(title_array[0], strErr.c_str()); //错误
 		} 
 		
 	}

	if (0 == uFlag)
	{
		sprintf(title_array[0], "当前发票号码: %08u",  g_globalArg->m_curInvVol->m_curInvNo);
	}
	

}

UINT8 CInvRetQueryWin::ChangeTitle()
{
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	
	return SUCCESS;
}

void CInvRetQueryWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetQueryWin *win=(CInvRetQueryWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CInvRetQueryWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetQueryWin *win=(CInvRetQueryWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}


void CInvRetQueryWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetQueryWin *win=(CInvRetQueryWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CInvRetQueryWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetQueryWin *win=(CInvRetQueryWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CInvRetQueryWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CInvRetQueryWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	OnDownKey(); //切换焦点到下一个控件
	return;
}


void CInvRetQueryWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	DBG_PRINT(("进入CInvRetQueryWin::OnButton1函数"));

	UINT8 ret =SUCCESS;
	ret =CheckInputValid();
	if ( ret != SUCCESS) 
	{
		return ;
	}
	
	ret = INVM_CheckIsNoDB(m_invCode, m_invNo,&m_InvHead);

	DBG_PRINT(("ret= %u",ret));
	if(ret != SUCCESS)
	{
		INVM_ErrMsgBox(ret);
		return ;
	}

	ChangeWin(INV_RETURN_WIN);//切换到发票退回窗口
	DBG_PRINT(("退出CInvRetQueryWin::OnButton1函数"));
}


void CInvRetQueryWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{

	ChangeWin(INV_MANAGE_MENU);	 //返回发票管理界面		

}

//--------------------------------------------------------------------
//查询所退发票
//--------------------------------------------------------------------
UINT8 CInvRetQueryWin::CheckInputValid()
{
	DBG_PRINT(("进入CInvRetQueryWin::CheckInputValid"));
	
	UINT8 ret = SUCCESS;
	UINT8 errcode;
	char *content1, *content2;
	string strErr;
	
	content1 = (char*)(m_pInput1->m_contentBuf); 
	content2 = (char*)(m_pInput2->m_contentBuf);
	DBG_PRINT(("InvReturnProc begin content = %s", content1));
	DBG_PRINT(("InvReturnProc begin content = %s", content2));
	
	//未输入发票号码
	if (strlen(content2)==0)
	{
		m_pInput2->SetFocus();
		DBG_PRINT(("InvReturnProc begin content"));
		CaMsgBox::ShowMsg("请输入发票号码");
		return FAILURE;
	}
	m_invNo = atoi(content2);
	if (0 == m_invNo) 
	{
		m_pInput2->SetFocus();
		DBG_PRINT(("InvReturnProc begin content"));
		CaMsgBox::ShowMsg("发票号码非法");
		return FAILURE;
	}
	
	
	//未输入发票代码
	if( strlen(content1)==0 )
	{
		m_pInput1->SetFocus();
		DBG_PRINT(("InvReturnProc begin content"));
		CaMsgBox::ShowMsg("请输入发票代码");
		return FAILURE;
	}
	// 	else if (strlen(content1)!=20) 
	// 	{
	// 		DBG_PRINT(("发票代码非法"));
	// 		CaMsgBox::ShowMsg("发票代码非法");
	// 		return FAILURE;
	// 		
	// 	}
	else if (0 == atoi(content1)) 
	{
		m_pInput1->SetFocus();
		DBG_PRINT(("发票代码非法"));
		CaMsgBox::ShowMsg("发票代码非法");
		return FAILURE;
	}
	else //搜索发票代码关联号
	{
		m_invCode = content1;
	}
	
	DBG_PRINT(("退出CInvRetQueryWin::CheckInputValid"));
	return SUCCESS;	
}


