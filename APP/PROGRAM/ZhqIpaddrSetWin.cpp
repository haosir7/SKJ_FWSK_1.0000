/**
 @file  NetworkingSetWin.cpp
 @brief 界面文件，IP信息设置
*/
#include "CMainFrame.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CaProgressBar.h"

#include "ZhqIpaddrSetWin.h"
#include "setToFileProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CZhqIpaddrSetWin::CZhqIpaddrSetWin():CMultiInputCommonWin(4)
{
	m_iNumOfBtn = 2;
}

CZhqIpaddrSetWin::~CZhqIpaddrSetWin()
{
}

int CZhqIpaddrSetWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, ZHQ_IPADDR_SET_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH); 

	SetObjPara(1, "本机 IP：",	CaInput::aCHAR,15);
	SetObjPara(2, "子网掩码：",	CaInput::aCHAR,15);
	SetObjPara(3, "默认网关：",	CaInput::aCHAR,15);
	SetObjPara(4, "DNS 地址：",	CaInput::aCHAR,15);

	SetObjPara(10," 下一步 ");
	SetObjPara(11," 返回 ");

	OnActive = S_OnActive;
	this->ReFresh();

	return 0;
}

int CZhqIpaddrSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		{
			CaMsgBox msgBox("确定放弃转换器网络设置?",CaMsgBox::MB_YESNO);
			msgBox.ShowBox();
			if (msgBox.m_iStatus == OK_PRESSED)
			{
		ClearIpAddrWin();
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

void CZhqIpaddrSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
}

void CZhqIpaddrSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CZhqIpaddrSetWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput4);
	ReFresh(); 
}

void CZhqIpaddrSetWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}

int CZhqIpaddrSetWin::ReFresh()
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

void CZhqIpaddrSetWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");
	//非动态获取IP时，检验输入
	if (0 == m_netpara->m_IsDhcp)
	{
		if (CheckInputValid(strErr) == FAILURE) 
		{
			CaMsgBox::ShowMsg(strErr);
			this->ReFresh();
			return ;
		}
	}

	if (changeIpAddrProc(strErr) != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

//	CaMsgBox::ShowMsg("设置本机IP信息成功");
	
	ChangeWin(ZHQ_SERVERIP_SET_WIN);
}

void CZhqIpaddrSetWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearIpAddrWin();
	ChangeWin(ZHQ_PARA_SET_MENU);
}

UINT8 CZhqIpaddrSetWin::CheckInputValid(string &strErr)
{	
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入IP地址";
		m_pInputList->SetFocusToObj(m_pInput1);
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput1->m_contentBuf) != NET_SUCCESS)
	{	
		strErr = "网络IP输入无效";
		m_pInputList->SetFocusToObj(m_pInput1);
		return FAILURE;
	}

	if (m_pInput2->IsEmpty())
	{	
		strErr = "未输入子网掩码";
		m_pInputList->SetFocusToObj(m_pInput2);
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput2->m_contentBuf) != NET_SUCCESS)
	{	
		strErr = "子网掩码输入无效";
		m_pInputList->SetFocusToObj(m_pInput2);
		return FAILURE;
	}

	if (m_pInput3->IsEmpty())
	{	
		strErr = "未输入默认网关";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput3->m_contentBuf) != NET_SUCCESS)
	{	
		strErr = "网关输入无效";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;
	}

	if (m_pInput4->IsEmpty())
	{	
		strErr = "未输入DNS地址";
		m_pInputList->SetFocusToObj(m_pInput4);
		return FAILURE;
	}
	if (isIPValid((INT8 *)m_pInput4->m_contentBuf) != NET_SUCCESS)
	{	
		strErr = "DNS输入无效";
		m_pInputList->SetFocusToObj(m_pInput4);
		return FAILURE;
	}

	return SUCCESS;
}

void CZhqIpaddrSetWin::DispIpAddrInfo()
{	
	m_pInput1->SetContentBuf((UINT8 *)m_netpara->m_LocalIP.c_str(), m_netpara->m_LocalIP.length());
	m_pInput2->SetContentBuf((UINT8 *)m_netpara->m_LocalMask.c_str(), m_netpara->m_LocalMask.length());
	m_pInput3->SetContentBuf((UINT8 *)m_netpara->m_LocalGate.c_str(), m_netpara->m_LocalGate.length());
	m_pInput4->SetContentBuf((UINT8 *)m_netpara->m_LocalDNS.c_str(), m_netpara->m_LocalDNS.length());

	DBG_PRINT(("m_netpara: m_IDDHCP = %u", m_netpara->m_IsDhcp));
	DBG_PRINT(("m_netpara: m_LocalIP = %s", m_netpara->m_LocalIP.c_str()));
	DBG_PRINT(("m_netpara: m_LocalMask = %s", m_netpara->m_LocalMask.c_str()));
	DBG_PRINT(("m_netpara: m_LocalGate = %s", m_netpara->m_LocalGate.c_str()));
	DBG_PRINT(("m_netpara: m_LocalDNS = %s", m_netpara->m_LocalDNS.c_str()));
}

void CZhqIpaddrSetWin::ClearIpAddrWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput4->Clear();
}

int CZhqIpaddrSetWin::changeIpAddrProc(string &strErr)
{
	int ret = SUCCESS;
	m_netpara->m_LocalIP = (INT8 *)m_pInput1->m_contentBuf;
	m_netpara->m_LocalMask = (INT8 *)m_pInput2->m_contentBuf;
	m_netpara->m_LocalGate = (INT8 *)m_pInput3->m_contentBuf;
	m_netpara->m_LocalDNS = (INT8 *)m_pInput4->m_contentBuf;

	DBG_PRINT(("m_netpara: m_IDDHCP = %u", m_netpara->m_IsDhcp));
	DBG_PRINT(("m_netpara: m_LocalIP = %s", m_netpara->m_LocalIP.c_str()));
	DBG_PRINT(("m_netpara: m_LocalMask = %s", m_netpara->m_LocalMask.c_str()));
	DBG_PRINT(("m_netpara: m_LocalGate = %s", m_netpara->m_LocalGate.c_str()));
	DBG_PRINT(("m_netpara: m_LocalDNS = %s", m_netpara->m_LocalDNS.c_str()));
	//更新数据库

	return ret;
}

void CZhqIpaddrSetWin::S_OnActive(CaWindow *obj)
{
	((CZhqIpaddrSetWin *)obj)->DoActive();
}

void CZhqIpaddrSetWin::DoActive()
{

	m_netpara =  ((CZhqParaSetMenu *)m_pFrame->GetWin(ZHQ_PARA_SET_MENU))->m_ComNetpara;

	CaMsgBox msgBox("是否动态获取本机IP地址?",CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if (msgBox.m_iStatus == OK_PRESSED)
	{
		m_pInput1->m_bReadOnly = true;
		m_pInput2->m_bReadOnly = true;
		m_pInput3->m_bReadOnly = true;
		m_pInput4->m_bReadOnly = true;
		m_netpara->m_IsDhcp = 1;
		ClearIpAddrWin();
		ChangeWin(ZHQ_SERVERIP_SET_WIN);
		this->ReFresh();
	}
	else
	{
		m_pInput1->m_bReadOnly = false;
		m_pInput2->m_bReadOnly = false;
		m_pInput3->m_bReadOnly = false;
		m_pInput4->m_bReadOnly = false;
		m_netpara->m_IsDhcp = 0;
		DispIpAddrInfo();
		this->ReFresh();
	}
	return;
}

