/**
 @file  WirelessInfoWin.cpp
 @brief 界面文件，M2M模块信息
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "command_at.h"
#include "netManager.h"
#include "M2M_Common.h"
#include "WirelessInfoWin.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

CWirelessInfoWin::CWirelessInfoWin():CaWindow()
{
}

CWirelessInfoWin::~CWirelessInfoWin()
{
}

int CWirelessInfoWin::Create(int iX,int iY,int iW,int iH)
{
	int curH, lineinterval, left_offset;
	int inputW;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	m_pFrame->RegsiterWin(this, WIRELESS_INFO_WIN);    
	CaWindow::Create(iX,iY,iW,iH); 

	curH = SCREEN_TOP_OFFSET;
	lineinterval = LINE_H;
	left_offset = SCREEN_LEFT_OFFSET+2;
	inputW = SCREEN_W - left_offset*2;
	
	strcpy(title, "IMEI：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel1 = new CaLabel(false, CaObject::ALIGN_LEFT);
	m_pLabel1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;
	
	strcpy(title, "CCID：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel2->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;	
	
	strcpy(title, "固件版本：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel3 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel3->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel3->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;	
	
	strcpy(title, "软件版本：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel4 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel4->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel4->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;	

	strcpy(title, "返回");
	m_pBtn1 = new CaButton();
	m_pBtn1->Create(SCREEN_LEFT_OFFSET+100, curH, 48, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	OnActive = S_OnActive;
	
	this->End();

	return 0;
}

int CWirelessInfoWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{
	case RETURN_MAIN_MENU: 
		ChangeWin(M2M_MANAGE_MENU);
		return SUCCESS;
		break;		
	default: 
		break;		
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CWirelessInfoWin::IsCurWin()
{
	return true;
}

int CWirelessInfoWin::ReFresh()
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

void CWirelessInfoWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CWirelessInfoWin *win=(CWirelessInfoWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CWirelessInfoWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(M2M_MANAGE_MENU);
}

void CWirelessInfoWin::S_OnActive(CaWindow *obj)
{
	((CWirelessInfoWin *)obj)->DoActive();
}
void CWirelessInfoWin::DoActive()
{
	INT32 ret=0;
	INT8 title[64];
    INT32 titleLen=0;	
	M2Minfo info;
	M2MCommon *p = (M2MCommon *)g_netManager->GetConnectionHandle();

	memset((void *)&info, 0, sizeof(M2Minfo));
	ret = p->getModlueInfo(&info);
	DBG_PRINT(("ret = %d", ret))

	sprintf(title, "IMEI：%s", info.IMEI);
	titleLen = strlen(title);
	m_pLabel1->SetTitle(title, titleLen);	

	sprintf(title, "CCID：%s", info.CCID);
	titleLen = strlen(title);
	m_pLabel2->SetTitle(title, titleLen);	

	sprintf(title, "IMSI：%s", info.IMSI);
	titleLen = strlen(title);
	m_pLabel3->SetTitle(title, titleLen);	

	sprintf(title, "固件版本:%s", info.FirmVer);
	titleLen = strlen(title);

	sprintf(&title[titleLen], "  软件版本:%s", info.SoftVer);
	titleLen = strlen(title);
	m_pLabel4->SetTitle(title, titleLen);	
	DBG_PRINT(("titleLen=%d", titleLen));

	return;
}



