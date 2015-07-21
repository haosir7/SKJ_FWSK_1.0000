/**
 @file  NetworkingSetWin.cpp
 @brief 界面文件，IP信息设置
*/
#include "CMainFrame.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CaProgressBar.h"

#include "setToFileProc.h"
#include "netManager.h"
#include "LANConnection.h"
#include "NetworkingSetWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CNetworkingSetWin::CNetworkingSetWin():CMultiInputCommonWin(4)
{
	m_iNumOfBtn = 2;
}

CNetworkingSetWin::~CNetworkingSetWin()
{
}

int CNetworkingSetWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, NETWORKING_SET_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH); 

	SetObjPara(1, "网 络 IP：",	CaInput::aCHAR,15);
	SetObjPara(2, "子网掩码：",	CaInput::aCHAR,15);
	SetObjPara(3, "默认网关：",	CaInput::aCHAR,15);
	SetObjPara(4, "默认 DNS：",	CaInput::aCHAR,15);

	SetObjPara(10," 确认 ");
	SetObjPara(11," 返回 ");

	OnActive = S_OnActive;
	this->ReFresh();

	return 0;
}

int CNetworkingSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearNetworkingWin();
		ChangeWin(LOCAL_NETWORK_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}
   return CMultiInputCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

void CNetworkingSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
}

void CNetworkingSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CNetworkingSetWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput4);
	ReFresh(); 
}

void CNetworkingSetWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}

int CNetworkingSetWin::ReFresh()
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

void CNetworkingSetWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");

	if (g_netManager->GetConnectionMode() != NET_LAN)
	{
		CaMsgBox::ShowMsg("当前非局域网模式,无法设置");
		return;
	}
	
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	if (changeSettingProc(strErr) != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	CaMsgBox::ShowMsg("设置局域网IP信息成功");
	
	return ;
}

void CNetworkingSetWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearNetworkingWin();
	ChangeWin(LOCAL_NETWORK_MENU);
}

UINT8 CNetworkingSetWin::CheckInputValid(string &strErr)
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
		strErr = "未输入默认DNS";
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

void CNetworkingSetWin::DispNetworkingInfo()
{	
	m_pInput1->SetContentBuf((UINT8 *)m_ipInfo.IP, strlen(m_ipInfo.IP));
	m_pInput2->SetContentBuf((UINT8 *)m_ipInfo.Mask, strlen(m_ipInfo.Mask));
	m_pInput3->SetContentBuf((UINT8 *)m_ipInfo.GeteWay, strlen(m_ipInfo.GeteWay));
	m_pInput4->SetContentBuf((UINT8 *)m_ipInfo.DNS, strlen(m_ipInfo.DNS));
}

void CNetworkingSetWin::ClearNetworkingWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput4->Clear();
}

int CNetworkingSetWin::changeSettingProc(string &strErr)
{
	int ret = 0;
	LANConnection *pLan = NULL;
	CaProgressBar ProgressBar("网络配置修改中");
	ProgressBar.Show();

	memset((void *)&m_ipInfo, 0, sizeof(IPinfo));
	strncpy(m_ipInfo.IP, (INT8 *)m_pInput1->m_contentBuf, sizeof(m_ipInfo.IP));
	strncpy(m_ipInfo.Mask, (INT8 *)m_pInput2->m_contentBuf, sizeof(m_ipInfo.Mask));
	strncpy(m_ipInfo.GeteWay, (INT8 *)m_pInput3->m_contentBuf, sizeof(m_ipInfo.GeteWay));
	strncpy(m_ipInfo.DNS, (INT8 *)m_pInput4->m_contentBuf, sizeof(m_ipInfo.DNS));

	pLan = (LANConnection *)g_netManager->GetConnectionHandle();
	ret = pLan->setIPinfo(&m_ipInfo, strErr);
	return ret;
}

void CNetworkingSetWin::S_OnActive(CaWindow *obj)
{
	((CNetworkingSetWin *)obj)->DoActive();
}

void CNetworkingSetWin::DoActive()
{
	INT32 ret=0;
	string strErr("");
	CNetConnection *pNet = g_netManager->GetConnectionHandle();//只能设置LAN的IP
	CaProgressBar info("信息获取中...");
	info.Show();

	IPinfo tmpInfo;
	memset((void *)&m_ipInfo, 0, sizeof(IPinfo));
	ret = pNet->performGetIPinfo(&m_ipInfo, strErr);
	if (ret != NET_SUCCESS)
	{
		DBG_PRINT(("ret = %d", ret))
		CaMsgBox::ShowMsg(strErr);
		memset((void*)&m_ipInfo, 0, sizeof(IPinfo));
		ClearNetworkingWin();
	}
	else
	{
		ret = CNetDBWork::loadIPinfo(&tmpInfo, strErr);
		if (SQLITE_OK == ret)
		{
			strcpy(m_ipInfo.GeteWay, tmpInfo.GeteWay);
		}
		else
		{
			DBG_PRINT(("从数据库里获取Ip地址失败"));
		}

		DispNetworkingInfo();
	}

}

