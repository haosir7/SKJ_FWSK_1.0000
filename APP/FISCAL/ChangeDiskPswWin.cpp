/*! \file    ChangeDiskPswWin.cpp
   \brief    awe4000r金税盘口令更改
   \author   Yu Yan 
   \version  1.0
   \date     2008-2-28
 */


#include "ChangeDiskPswWin.h"

#include "FiscalFun.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CChangeDiskPswWin::CChangeDiskPswWin():CaWindow()
{	
	m_OldPsw = "";
	m_NewPsw = "";	
}

CChangeDiskPswWin::~CChangeDiskPswWin()
{
	
}

int CChangeDiskPswWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,CHANGE_DISK_PSW_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "原口令:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(20);
	m_pInput1->m_InputType = CaInput::aPASSWORD; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "新口令:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = CaInput::aPASSWORD; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;

	//创建一个Input  第二行
	strcpy(title, "新口令确认:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(8);
	m_pInput3->m_InputType = CaInput::aPASSWORD; //该输入框只接受整型值
	m_pInput3->OnObject = S_OnInput3;
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;

	//创建一个Button  第五行
	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += LINE_H;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	//创建一个Button  第五行
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

void CChangeDiskPswWin::S_OnActive(CaWindow *obj)
{
	((CChangeDiskPswWin *)obj)->DoActive();
}

void CChangeDiskPswWin::DoActive()
{
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();
	m_pInput3->Clear();
	
	m_OldPsw = "";
	m_NewPsw = "";
	
	ReFresh();	
}

int CChangeDiskPswWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  
		ChangeWin(FISCAL_MAIN_MENU);			
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CChangeDiskPswWin::ReFresh()
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

void CChangeDiskPswWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CChangeDiskPswWin *win=(CChangeDiskPswWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CChangeDiskPswWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CChangeDiskPswWin *win=(CChangeDiskPswWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CChangeDiskPswWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CChangeDiskPswWin *win=(CChangeDiskPswWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CChangeDiskPswWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CChangeDiskPswWin *win=(CChangeDiskPswWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CChangeDiskPswWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CChangeDiskPswWin *win=(CChangeDiskPswWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CChangeDiskPswWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CChangeDiskPswWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CChangeDiskPswWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CChangeDiskPswWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 ret = CheckInput();
	if (ret !=SUCCESS)
	{
		return;
	}

	string strErr("");
    ret= FSC_ChangeDiskPsw(m_OldPsw, m_NewPsw,strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	ChangeWin(FISCAL_MAIN_MENU);	 //返回税控档菜单
}

void CChangeDiskPswWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(FISCAL_MAIN_MENU);	 //返回税控档菜单
}

UINT8 CChangeDiskPswWin::CheckInput(void)
{
	DBG_PRINT(("CheckInput begin"));
	
	UINT8 ret = SUCCESS;
	char *content1, *content2, *content3;
	INT32 tmp = 0;

	content1 = (char*)(m_pInput1->m_contentBuf); 
	content2 = (char*)(m_pInput2->m_contentBuf);
	content3 = (char*)(m_pInput3->m_contentBuf);

	//未输入老口令
	if (strlen(content1)==0)
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("请输入金税盘旧口令");
		return FAILURE;
	}

	//原证书口令未满八位
	if (strlen(content1)!=8)
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("原金税盘口令错误");
		return FAILURE;
	}

	//未输入新口令
	if (strlen(content2)==0)
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("请输入金税盘新口令");
		return FAILURE;
	}

	//新口令未满八位
	if (strlen(content2)!=8)
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("金税盘新口令未满8位");
		return FAILURE;
	}


	//未输入新口令
	if (strlen(content3)==0)
	{
		m_pInput3->SetFocus();
		CaMsgBox::ShowMsg("请再次输入金税盘新口令");
		return FAILURE;
	}

	if (0 != strcmp(content2, content3))
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("两次输入新口令不一致");
		return FAILURE;
	}
	
	m_OldPsw = content1;
	m_NewPsw = content2;

	return SUCCESS;
}

