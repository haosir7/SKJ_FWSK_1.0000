/**
 @file  WirelessViewWin.cpp
 @brief 界面文件，无线信息监测
*/
#include "keypad.h"
#include "commonFunc.h"

#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "command_at.h"
#include "netManager.h"
#include "M2M_Common.h"
#include "trans_http.h"

#include "WirelessViewWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CWirelessViewWin::CWirelessViewWin():CaWindow()
{
}

CWirelessViewWin::~CWirelessViewWin()
{
}

int CWirelessViewWin::Create(int iX,int iY,int iW,int iH)
{
	int curH, lineinterval, left_offset;
	int inputW, leftoffset_btn;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	m_pFrame->RegsiterWin(this, WIRELESS_VIEW_WIN);    
	CaWindow::Create(iX,iY,iW,iH); 

	strcpy(title, "无线信号监测");
	m_pTitle = new CaLabel(false,CaObject::ALIGN_CENTER);
	m_pTitle->Create(SCREEN_LEFT_OFFSET,2, SCREEN_W, CHAR_H);
	m_pTitle->SetTitle(title, strlen(title));

	curH = SCREEN_TOP_OFFSET+4;
	lineinterval = LINE_H + 2;
	left_offset = SCREEN_LEFT_OFFSET+8;
	inputW = SCREEN_W - left_offset*2;
	curH += lineinterval;
	
	strcpy(title, "信号强度(0～31)：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel1 = new CaLabel(false, CaObject::ALIGN_LEFT);
	m_pLabel1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;
	
	strcpy(title, "信号等级：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel2->Create(left_offset, curH, inputW+1, LINE_H);
	m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
	curH += lineinterval;	
	
	strcpy(title, "ESC返回上级菜单");
	m_pBtn1 = new CaButton();
	m_pBtn1->Create(SCREEN_LEFT_OFFSET+66, 50, 115, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	OnActive = S_OnActive;
	
	this->End();

	return 0;
}

int CWirelessViewWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
   return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}
bool CWirelessViewWin::IsCurWin()
{
	return true;
}

int CWirelessViewWin::ReFresh()
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

void CWirelessViewWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CWirelessViewWin *win=(CWirelessViewWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CWirelessViewWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NETWORK_SET_MENU);
}

void CWirelessViewWin::S_OnActive(CaWindow *obj)
{
	((CWirelessViewWin *)obj)->DoActive();
}
void CWirelessViewWin::DoActive()
{
	INT32 rc=0;
	int rssi=0;
	int ber=0;
	UINT8 key=0;
	INT8 title[OBJ_TITLE_MAX_LEN + 1];
    INT32 titleLen=0;
	INT32 mode = g_netManager->GetConnectionMode();
	CNetConnection *p = (CNetConnection *)g_netManager->GetConnectionHandle();	
	
	g_netManager->NetTransMutexLock();
	
	do
	{
   		if( key = ReadKeypad() )
		{
			if (key == ESC_KEY)
			{
				break;
			}
			else if ( key==HELP_KEY && (mode==NET_M2M_WMMP || mode==NET_M2M_APN) )
			{
				M2Minfo info;
				memset((void *)&info, 0, sizeof(M2Minfo));
				((M2MCommon *)p)->getModlueInfo(&info);
				sprintf(title, "IMEI:%s", info.IMEI);
				CaMsgBox::ShowMsg(title);
			}
		}
		
		rc = p->getCSQ(&rssi, &ber);
		DBG_PRINT(("rc = %d", rc))
		if (rc != 0)
		{
			strcpy(title, "信号强度(0～31)：监测失败");
			titleLen = strlen(title) * CHAR_W + 1;
			m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
			
			strcpy(title, "信 号 等 级：    监测失败");
			titleLen = strlen(title) * CHAR_W + 1;
			m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
		}
		else
		{
			DBG_PRINT(("CSQ: %d, %d",rssi,ber));
			if (rssi==99 /*|| ber==99*/) //change for 3g. zl
			{
				strcpy(title, "信号强度(0～31)：无信号");
				titleLen = strlen(title) * CHAR_W + 1;
				m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
				
				strcpy(title, "信 号 等 级：    无信号");
				titleLen = strlen(title) * CHAR_W + 1;
				m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
			}
			else
			{
				if (rssi>20)
				{
					sprintf(title, "信号强度(0～31)：%d", rssi);
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
					
					strcpy(title,  "信 号 等 级：    强");
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
				}
				else if (rssi>10)
				{
					sprintf(title, "信号强度(0～31)：%d", rssi);
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
					
					strcpy(title,  "信 号 等 级：    中");
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
				}
				else
				{
					sprintf(title, "信号强度(0～31)：%d", rssi);
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel1->SetTitle(title, titleLen / CHAR_W);	
					
					strcpy(title,  "信 号 等 级：    弱");
					titleLen = strlen(title) * CHAR_W + 1;
					m_pLabel2->SetTitle(title, titleLen / CHAR_W);	
				}
			}
		}

		this->ReFresh();
		CommonSleep(800);
	}while (1);
	g_netManager->NetTransMutexUnlock();

	ChangeWin(NETWORK_SET_MENU);
	return;
}



