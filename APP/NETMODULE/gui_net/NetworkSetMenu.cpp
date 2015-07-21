/**
 @file  NetworkSetMenu.cpp
 @brief 界面文件，网络设置菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CGlobalArg.h"

#include "netHeaderDef.h"
#include "netManager.h"
#include "NetworkSetMenu.h"


CNetworkSetMenu::CNetworkSetMenu():CMultiBtnCommonWin(7,2)
{
}

int CNetworkSetMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, NETWORK_SET_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);

	SetTitle(1,  "A.局 域 网");
	SetTitle(2,  "B.ADSL宽带");
	SetTitle(3,  "C. 3G 无线");
	SetTitle(4,  "D.M2M 无线");
	SetTitle(5,  "E.VPN 专网");	
	SetTitle(6,  "F.本机IP查询");
	SetTitle(7,  "G.无线检测");

	return 1;
}

void CNetworkSetMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_LAN_MODULE != 0)
	if (g_netManager->GetConnectionMode() != NET_LAN)
	{
		CaMsgBox::ShowMsg("当前非局域网模式");
		return;
	}
	ChangeWin(LOCAL_NETWORK_MENU);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetworkSetMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_ADSL_MODULE != 0)
	if (g_netManager->GetConnectionMode() != NET_ADSL)
	{
		CaMsgBox::ShowMsg("当前非ADSL模式");
		return;
	}
	ChangeWin(ADSLINFO_SET_WIN);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetworkSetMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_3G_MODULE != 0)
	if (g_netManager->GetConnectionMode() != NET_3G)
	{
		CaMsgBox::ShowMsg("当前非3G模式");
		return;
	}
	ChangeWin(NET_3G_WLEN_MENU);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetworkSetMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_M2M_MODULE != 0)
	if (g_netManager->GetConnectionMode() != NET_M2M_APN &&
		g_netManager->GetConnectionMode() != NET_M2M_WMMP)
	{
		CaMsgBox::ShowMsg("当前非M2M模式");
		return;
	}
	ChangeWin(M2M_MANAGE_MENU);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetworkSetMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_VPN_MODULE != 0)
	if (g_netManager->GetConnectionModeL2() != NET_VPN)
	{
		CaMsgBox::ShowMsg("当前未开启VPN");
		return;
	}
	ChangeWin(VPN_INFO_SET_WIN);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetworkSetMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NETWORKING_SHOW_WIN);
}

void CNetworkSetMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_netManager->GetConnectionMode()!=NET_M2M_WMMP &&
		g_netManager->GetConnectionMode()!=NET_M2M_APN &&
		g_netManager->GetConnectionMode()!=NET_3G )
	{
		CaMsgBox::ShowMsg("当前非无线通讯模式");
		return;
	}
	else if (g_netManager->GetConnectionMode()==NET_M2M_APN &&
			g_netManager->checkConnect()==CONNECT_ON_L1 )
	{
		CaMsgBox::ShowMsg("APN已连接,无法获取无线信息");
		return;
	}
	else
	{
		DBG_PRINT(("m_sendInvFlag = %d", g_globalArg->m_sendInvFlag));
		if( g_globalArg->m_sendInvFlag == 1 )
		{
			CaMsgBox::ShowMsg("发票上传中,请稍后再查");
		}
		else
		{
			ChangeWin(WIRELESS_VIEW_WIN);
		}
	}
}

int CNetworkSetMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(NET_MANAGE_MENU);
		return SUCCESS;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

