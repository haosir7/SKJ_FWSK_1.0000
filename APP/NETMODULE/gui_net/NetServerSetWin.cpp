/**
 @file  NetServerSetWin.cpp
 @brief 界面文件，服务器设置
*/
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "netManager.h"
#include "NetServerSetWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CNetServerSetWin::CNetServerSetWin():CaWindow()
{
	m_serverIP = "";
	m_serverPort = "";
	m_serverIP_flag = 0;
	m_serverPort_flag = 0;
}

CNetServerSetWin::~CNetServerSetWin()
{
}

int CNetServerSetWin::Create(int iX,int iY,int iW,int iH)
{
	int curH, left_offset;
	int inputW, leftoffset_btn;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	m_pFrame->RegsiterWin(this, NET_SERVER_SET_WIN);    
	CaWindow::Create(iX,iY,iW,iH); 

	curH = SCREEN_TOP_OFFSET;
	left_offset = SCREEN_LEFT_OFFSET+8;
	inputW = SCREEN_W - left_offset*2;

	strcpy(title, "服务器IP:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput1=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput1->SetTitle(title, titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(15);
	m_pInput1->m_InputType = CaInput::aCHAR;
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "服务器端口:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput2->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput2->SetTitle(title, titleLen / CHAR_W);	
 	m_pInput2->SetMaxLen(5);
	m_pInput2->m_InputType = CaInput::aINT;
	m_pInput2->OnObject = S_OnInput2;
	
 	m_iBtnW = (SCREEN_W - 40)/2 - 4; 
 	m_iColW = (SCREEN_W - 40)/2; 
	leftoffset_btn = left_offset + 14;

	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += LINE_H+5;
	m_pBtn1->Create(leftoffset_btn, curH, m_iBtnW, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW, curH, m_iBtnW, WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;	

	OnActive = S_OnActive;
	
	this->End();
	return 0;
}

int CNetServerSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{
	case RETURN_MAIN_MENU: 
		ClearNetServerWin();
		ChangeWin(NET_MANAGE_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CNetServerSetWin::IsCurWin()
{
	return true;
}

int CNetServerSetWin::ReFresh()
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


void CNetServerSetWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetServerSetWin *win=(CNetServerSetWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CNetServerSetWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetServerSetWin *win=(CNetServerSetWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}
void CNetServerSetWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetServerSetWin *win=(CNetServerSetWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CNetServerSetWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetServerSetWin *win=(CNetServerSetWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CNetServerSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInput2->SetFocus();
}

void CNetServerSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pBtn1->SetFocus();
}

void CNetServerSetWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");
	CaProgressBar proInfo("");

	proInfo.SetText("服务器参数设置中...");
	proInfo.Show();

	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	m_serverIP = (INT8 *)m_pInput1->m_contentBuf;
	m_serverPort = (INT8 *)m_pInput2->m_contentBuf;

	switch(m_servType)
	{
	case APP_SERV:
		g_netManager->setDefServerIP(m_serverIP, m_serverPort);
		break;

	case TIME_SERV:
		DBG_ASSERT_EXIT(0, ("not support TIME_SERV!!"));
		break;

	default:
		break;
	}

	m_serverIP_flag = 1;
	m_serverPort_flag = 1;
	
	CaMsgBox::ShowMsg("服务器参数设置完成");
	ClearNetServerWin();

	ChangeWin(NET_MANAGE_MENU);
}

void CNetServerSetWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearNetServerWin();
	ChangeWin(NET_MANAGE_MENU);
}

UINT8 CNetServerSetWin::isIPValid(INT8 *ip)
{
	INT32 res, i;	
	INT32 x[4];

	if (strlen(ip) > 15)
		return FAILURE;

	memset(x, 0, sizeof(x));
	res = sscanf(ip, "%d.%d.%d.%d", &x[0], &x[1], &x[2], &x[3]);
	DBG_PRINT(("res=%d, ip = %d.%d.%d.%d", res, x[0], x[1], x[2], x[3]));
	if (res != 4)
		return FAILURE;

	for (i=0; i<4; i++)
	{
		if (x[i] > 255)
			return FAILURE;
	}

	return SUCCESS;
}

UINT8 CNetServerSetWin::CheckInputValid(string &strErr)
{	
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入服务器IP地址";
		m_pInput1->SetFocus();
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput1->m_contentBuf) != SUCCESS)
	{	
		strErr = "服务器IP输入无效";
		m_pInput1->SetFocus();
		return FAILURE;
	}

	if (m_pInput2->IsEmpty())
	{	
		strErr = "未输入服务器端口号";
		m_pInput2->SetFocus();
		return FAILURE;
	}

	return SUCCESS;
}

void CNetServerSetWin::DispNetServerInfo()
{	
	m_pInput1->SetContentBuf((UINT8 *)m_serverIP.c_str(), m_serverIP.length());
	m_pInput2->SetContentBuf((UINT8 *)m_serverPort.c_str(), m_serverPort.length());
	m_pInput1->SetFocus();
}

void CNetServerSetWin::ClearNetServerWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput1->SetFocus();
	m_serverIP_flag = 0;
	m_serverPort_flag = 0;
}

void CNetServerSetWin::S_OnActive(CaWindow *obj)
{
	((CNetServerSetWin *)obj)->DoActive();
}

void CNetServerSetWin::DoActive()
{
	INT32 res;
	string strErr("");

	switch(m_servType)
	{
	case APP_SERV:
		CNetDBWork::loadAPPServerIP(m_serverIP, m_serverPort, strErr);
		break;

	case TIME_SERV:
		CNetDBWork::loadTIMEServerIP(m_serverIP, m_serverPort, strErr);
		break;

	default:
		break;
	}

	DispNetServerInfo();
}
