/**
 @file  NetWlenCardWin.cpp
 @brief 界面文件，运营商选择
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "netManager.h"
#include "3GConnection.h"
#include "NetWlenCardWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CNetWlenCardWin::CNetWlenCardWin():CaWindow()
{
}

CNetWlenCardWin::~CNetWlenCardWin()
{
}


int CNetWlenCardWin::Create(int iX,int iY,int iW,int iH)
{
	int curH=0;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	int inputW = SCREEN_W - left_offset*4;


	m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,NET_WLEN_CARD_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	//创建一个Input  第一行
	curH = SCREEN_TOP_OFFSET+2;
	strcpy(title, "原 值: ");
	m_pLbl1 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLbl1->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLbl1->SetTitle(title, strlen(title));

	//创建一个Input  第一行
	strcpy(title, "新 值: ");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H+2;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(1);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "1-电信 2-联通");
	m_pLbl2 = new CaLabel(false,CaObject::ALIGN_CENTER);
	curH += LINE_H+2;
	m_pLbl2->Create(0,curH, SCREEN_W, CHAR_H);
	m_pLbl2->SetTitle(title, strlen(title));
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽

	//创建一个Button  第五行
	strcpy(title, "修改");
	m_pBtn1 = new CaButton();
	curH += LINE_H+6;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn + m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;

	OnActive = S_OnActive;
	this->End();  
	return 0;
}

int CNetWlenCardWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ChangeWin(NET_3G_WLEN_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CNetWlenCardWin::ReFresh()
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


void CNetWlenCardWin::S_OnActive(CaWindow *obj)
{
	((CNetWlenCardWin *)obj)->DoActive();
}

void CNetWlenCardWin::DoActive()
{
	DispCurInfo();
	ReFresh();
}

void CNetWlenCardWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetWlenCardWin *win=(CNetWlenCardWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CNetWlenCardWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}

void CNetWlenCardWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetWlenCardWin *win=(CNetWlenCardWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CNetWlenCardWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetWlenCardWin *win=(CNetWlenCardWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CNetWlenCardWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeNetWlenCard();
}

void CNetWlenCardWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_3G_WLEN_MENU);
}

void CNetWlenCardWin::DispCurInfo()
{
	INT32 carrierType=0;
	INT8 chValue[16];
	string strErr("");

	CNetDBWork::load3gCarrier(&carrierType, strErr);
	sprintf(chValue, "原 值: %d", carrierType);
	m_pLbl1->SetTitle(chValue, strlen(chValue));
	m_pInput1->Clear();
}

void CNetWlenCardWin::ChangeNetWlenCard()
{
	int ret=0;
	INT32 carrierType = 0;
	string infoStr("");
	string strErr("");
	CaProgressBar proBar("");	
	
	proBar.SetText("修改设置中...");
	proBar.Show();

	if (g_netManager->GetConnectionMode() != NET_3G)
	{
		CaMsgBox::ShowMsg("当前非3G模式,无法修改");
		return;
	}
	if (g_netManager->checkConnect() != CONNECT_OFF)
	{
		CaMsgBox::ShowMsg("3G连接尚未断开,无法修改");
		return;
	}
	
	if( CheckInputValid(strErr) != SUCCESS )
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	
	carrierType = atoi((char*)(m_pInput1->m_contentBuf));
	switch(carrierType)
	{
	case CARRIER_CT:
		infoStr = "设置为电信3G网卡?";
		break;
	case CARRIER_CU:
		infoStr = "设置为联通3G网卡?";
		break;
	}
	CaMsgBox msgBox(infoStr.c_str(), CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if (msgBox.m_iStatus != OK_PRESSED)
	{
		ReFresh();
		return;
	}

	strErr = "";
	_3GConnection *p3G = (_3GConnection *)g_netManager->GetConnectionHandle();
	ret = p3G->set3Gcarrier(carrierType, strErr);
	if (ret != NET_SUCCESS)
	{
		DBG_PRINT(("ret = %d", ret))
		if (strErr=="")
			CaMsgBox::ShowMsg("设置失败");
		else
			CaMsgBox::ShowMsg(strErr);
	}
	else
	{
		CaMsgBox::ShowMsg("设置成功");
	//	CaMsgBox::ShowMsg("请重新输入3G帐号与密码");
		ChangeWin(NET_3GWLEN_SET_WIN);
	}

	return;
}

UINT8 CNetWlenCardWin::CheckInputValid(string &strErr)
{
	INT32 dbCardType=0;
	int cardType = atoi((char*)(m_pInput1->m_contentBuf));

	if( cardType != CARRIER_CT && cardType != CARRIER_CU )
	{
		strErr = "不支持此类型3G网卡";
		return FAILURE;
	}

	CNetDBWork::load3gCarrier(&dbCardType, strErr);
	if( cardType == dbCardType)
	{
		strErr = "当前类型,无需修改";
		return FAILURE;
	}

	return SUCCESS;
}

