/**
 @file  M2MManageMenu.cpp
 @brief 界面文件，M2M管理菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"

#include "CGlobalArg.h"
#include "netManager.h"
#include "NetServerSetWin.h"
#include "NetModelEditWin.h"

#include "M2MManageMenu.h"

CM2MManageMenu::CM2MManageMenu():CMultiBtnCommonWin(2)
{
}

int CM2MManageMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, M2M_MANAGE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,  "A.模块信息");
	SetTitle(2,  "B.APN 专网");
//	SetTitle(2,  "B.无线监测");
	
	return 1;
}

void CM2MManageMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_netManager->GetConnectionMode()!=NET_M2M_WMMP &&
		g_netManager->GetConnectionMode()!=NET_M2M_APN )
	{
		CaMsgBox::ShowMsg("当前非无线通讯模式");
		return;
	}
	else if (g_netManager->GetConnectionMode()==NET_M2M_APN &&
			g_netManager->checkConnect()==CONNECT_ON_L1 )
	{
		CaMsgBox::ShowMsg("APN已连接,无法读取模块信息");
		return;
	}
	
	if( g_globalArg->m_sendInvFlag == 1 )
	{
		CaMsgBox::ShowMsg("发票上传中,请稍后再查");
	}
	else
	{
		ChangeWin(WIRELESS_INFO_WIN);
	}
}

void CM2MManageMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_netManager->GetConnectionMode()!=NET_M2M_WMMP &&
		g_netManager->GetConnectionMode()!=NET_M2M_APN )
	{
		CaMsgBox::ShowMsg("非M2M无线模式");
		return;
	}
	ChangeWin(APN_ACCPOINT_SET_WIN);
}

//void CM2MManageMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
//{
//	if (g_netManager->GetConnectionMode()!=NET_M2M_WMMP &&
//		g_netManager->GetConnectionMode()!=NET_M2M_APN )
//	{
//		CaMsgBox::ShowMsg("当前非无线通讯模式");
//	}
//	else
//	{
//		DBG_PRINT(("m_sendInvFlag = %d", g_globalArg->m_sendInvFlag));
//		if( g_globalArg->m_sendInvFlag == 1 )
//		{
//			CaMsgBox::ShowMsg("发票上传中,请稍后再查");
//		}
//		else
//		{
//			ChangeWin(WIRELESS_VIEW_WIN);
//		}
//	}
//}

int CM2MManageMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(NETWORK_SET_MENU);
		return SUCCESS;
	}
	
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

