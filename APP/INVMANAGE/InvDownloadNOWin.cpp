// InvInfoDownloadWin.cpp: implementation of the CInvDownloadNOWin class.
//
//////////////////////////////////////////////////////////////////////

#include "InvDownloadNOWin.h"
#include "CMainFrame.h"
#include "LOGCTRL.h"
#include "pos_debug.h"
#include "CaMsgBox.h"
#include "CGlobalArg.h"
#include "InvManageFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvDownloadNOWin::CInvDownloadNOWin() : CaWindow()
{

}

CInvDownloadNOWin::~CInvDownloadNOWin()
{

}

int CInvDownloadNOWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
	
	m_pFrame->RegsiterWin(this, INV_DOWNLOAD_NO_WIN);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "发票代码:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(20);
	m_pInput1->m_InputType = m_pInput2->aCHAR; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "发票号码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;

//	curH += LINE_H;
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "查询");
	m_pBtn1 = new CaButton();
	curH += 2*LINE_H;
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

int CInvDownloadNOWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(INV_DOWNLOAD_MENU);					
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CInvDownloadNOWin::ReFresh()
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

void CInvDownloadNOWin::S_OnActive(CaWindow *obj)
{
	((CInvDownloadNOWin *)obj)->DoActive();
}


void CInvDownloadNOWin::DoActive()
{
	DBG_PRINT((" CInvDownloadNOWin::DoActive()!"));
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput1->SetContentBuf((UINT8*)g_globalArg->m_curInvVol->m_code.c_str(), g_globalArg->m_curInvVol->m_code.length());
	ReFresh();	
}


void CInvDownloadNOWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvDownloadNOWin *win=(CInvDownloadNOWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CInvDownloadNOWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvDownloadNOWin *win=(CInvDownloadNOWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}


void CInvDownloadNOWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvDownloadNOWin *win=(CInvDownloadNOWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CInvDownloadNOWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvDownloadNOWin *win=(CInvDownloadNOWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CInvDownloadNOWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CInvDownloadNOWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CInvDownloadNOWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 ret = 0;
	string strErr(""), strtmp("");
	char buf[16];

	CInvHead invhead;		
	CInvHead invheadtmp;
	char sqlbuf[256];
	//发票查询方式
	UINT8 cxfs = 0;//0为号码段查询,1为时间段查询

	ret = CheckInput(strErr);	
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	ret = INVM_InvDetailNOQuery(m_code, m_invStartNo);
	if (ret != SUCCESS)
	{
	return;
	}
//	ChangeWin(INV_DOWNLOAD_MENU);	 //返回发票管理界面		
	
}


void CInvDownloadNOWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(INV_DOWNLOAD_MENU);	 //返回发票管理界面		
}



UINT8 CInvDownloadNOWin::CheckInput(string& strErr)
{
	if (m_pInput1->IsEmpty())
	{
			m_pInput1->SetFocus();
			strErr = "请输入发票代码!";
			return FAILURE;	
	}
	else
	{
		m_invhead.m_fpdm = (char *)m_pInput1->m_contentBuf;
	}
	
	if (m_pInput2->IsEmpty())
	{
	    m_pInput2->SetFocus();
		strErr = "请输入发票号码!";
		return FAILURE;
	}
	m_invhead.m_fphm = atoi( (char *)m_pInput2->m_contentBuf );

	m_code = m_invhead.m_fpdm;
	m_invStartNo = m_invhead.m_fphm;
	m_invEndNo = m_invhead.m_fphm;
	
	return SUCCESS;
}