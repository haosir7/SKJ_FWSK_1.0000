/**
 @file  LocalNetworkMenu.cpp
 @brief 界面文件，IP模式菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "netManager.h"
#include "LANConnection.h"
#include "LocalNetworkMenu.h" 

CLocalNetworkMenu::CLocalNetworkMenu():CMultiBtnCommonWin(2)
{
}

int CLocalNetworkMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,LOCAL_NETWORK_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.选择IP获取模式");
	SetTitle(2,"B.手动设置IP");
	
	return 1;
}

void CLocalNetworkMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if(g_netManager->GetConnectionMode() != NET_LAN)
	{
		CaMsgBox::ShowMsg("当前非局域网模式,不能修改");
		return;
	}
	ChangeWin(NET_IP_MODEL_WIN);
}

void CLocalNetworkMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if(g_netManager->GetConnectionMode() != NET_LAN)
	{
		CaMsgBox::ShowMsg("当前非局域网模式,不能修改");
		return;
	}
	LANConnection *p=(LANConnection *)g_netManager->GetConnectionHandle();
	if(p->m_dhcpMode == LAN_DHCP_ON)
	{
		CaMsgBox::ShowMsg("当前自动获取IP模式,不能手动修改");
		return;
	}
		
	ChangeWin(NETWORKING_SET_WIN);	
}

int CLocalNetworkMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(NETWORK_SET_MENU);
		return SUCCESS;
		break;	
		
	default: break;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

