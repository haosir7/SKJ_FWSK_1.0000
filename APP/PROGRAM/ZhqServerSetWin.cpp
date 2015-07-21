/**
 @file  ZhqServerSetWin.cpp
 @brief 界面文件，转换器连接的服务器设置
*/
#include "CMainFrame.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CaProgressBar.h"

#include "ZhqServerSetWin.h"
#include "setToFileProc.h"
#include "ZhqParaSetMenu.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CZhqServeripSetWin::CZhqServeripSetWin():CMultiInputCommonWin(3)
{
	m_iNumOfBtn = 2;
}

CZhqServeripSetWin::~CZhqServeripSetWin()
{
}

int CZhqServeripSetWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, ZHQ_IPADDR_SET_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH); 

	SetObjPara(1, "服务器IP：",	CaInput::aCHAR,15);
	SetObjPara(2, "服务器端口：",	CaInput::aCHAR,6);
	SetObjPara(3, "部署路径：",	CaInput::aCHAR,50);

	SetObjPara(10," 下一步 ");
	SetObjPara(11," 上一步 ");

	OnActive = S_OnActive;
	this->ReFresh();

	return 0;
}

int CZhqServeripSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		{
			CaMsgBox msgBox("确定放弃转换器网络设置?",CaMsgBox::MB_YESNO);
			msgBox.ShowBox();
			if (msgBox.m_iStatus == OK_PRESSED)
			{
		ClearZHQServWin();
		ChangeWin(ZHQ_PARA_SET_MENU);						
		return SUCCESS;
			}	
		}
		break;		
	default: 
		break;		
	}
   return CMultiInputCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

void CZhqServeripSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
}

void CZhqServeripSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CZhqServeripSetWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}

int CZhqServeripSetWin::ReFresh()
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

void CZhqServeripSetWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");

	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	if (changeZHQServInfoProc(strErr) != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	//CaMsgBox::ShowMsg("设置转换器服务器信息成功");
	
	ChangeWin(ZHQ_FTP_SET_WIN);
}

void CZhqServeripSetWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearZHQServWin();
	ChangeWin(ZHQ_IPADDR_SET_WIN);
}

UINT8 CZhqServeripSetWin::CheckInputValid(string &strErr)
{	
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入服务器IP地址";
		m_pInputList->SetFocusToObj(m_pInput1);
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput1->m_contentBuf) != NET_SUCCESS)
	{	
		strErr = "服务器IP地址输入无效";
		m_pInputList->SetFocusToObj(m_pInput1);
		return FAILURE;
	}

	if (m_pInput2->IsEmpty())
	{	
		strErr = "未输入服务器端口号";
		m_pInputList->SetFocusToObj(m_pInput2);
		return FAILURE;
	}

	if (m_pInput3->IsEmpty())
	{	
		strErr = "未输入服务器部署路径";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;
	}

	return SUCCESS;
}

void CZhqServeripSetWin::DispZHQServInfo()
{	
	m_pInput1->SetContentBuf((UINT8 *)m_netpara->m_ServIP.c_str(), m_netpara->m_ServIP.length());
	m_pInput2->SetContentBuf((UINT8 *)m_netpara->m_ServPort.c_str(), m_netpara->m_ServPort.length());
	m_pInput3->SetContentBuf((UINT8 *)m_netpara->m_ServAddr.c_str(), m_netpara->m_ServAddr.length());

	DBG_PRINT(("m_netpara: m_ServIP = %s", m_netpara->m_ServIP.c_str()));
	DBG_PRINT(("m_netpara: m_ServPort = %s", m_netpara->m_ServPort.c_str()));
	DBG_PRINT(("m_netpara: m_ServAddr = %s", m_netpara->m_ServAddr.c_str()));
}

void CZhqServeripSetWin::ClearZHQServWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
}

int CZhqServeripSetWin::changeZHQServInfoProc(string &strErr)
{
	int ret = SUCCESS;
	m_netpara->m_ServIP = (INT8 *)m_pInput1->m_contentBuf;
	m_netpara->m_ServPort = (INT8 *)m_pInput2->m_contentBuf;
	m_netpara->m_ServAddr = (INT8 *)m_pInput3->m_contentBuf;

	DBG_PRINT(("m_netpara: m_ServIP = %s", m_netpara->m_ServIP.c_str()));
	DBG_PRINT(("m_netpara: m_ServPort = %s", m_netpara->m_ServPort.c_str()));
	DBG_PRINT(("m_netpara: m_ServAddr = %s", m_netpara->m_ServAddr.c_str()));
	//更新数据库

	return ret;
}

void CZhqServeripSetWin::S_OnActive(CaWindow *obj)
{
	((CZhqServeripSetWin *)obj)->DoActive();
}

void CZhqServeripSetWin::DoActive()
{

	m_netpara =  ((CZhqParaSetMenu *)m_pFrame->GetWin(ZHQ_PARA_SET_MENU))->m_ComNetpara;

	DispZHQServInfo();
	this->ReFresh();

	return;
}
