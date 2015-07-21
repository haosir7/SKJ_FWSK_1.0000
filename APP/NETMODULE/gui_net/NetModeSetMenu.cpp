/**
 @file  NetModeSetMenu.cpp
 @brief 界面文件，网络模式设置菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "netManager.h"
#include "NetModeSetMenu.h"

CNetModeSetMenu::CNetModeSetMenu():CMultiBtnCommonWin(2)
{
}

int CNetModeSetMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,NET_MODE_SET_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.网络模式设置");
	SetTitle(2,"B.开启VPN专网");
	
	return 1;
}

void CNetModeSetMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NETMODEL_EDIT_WIN);	
}

void CNetModeSetMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int ret = 0;
	CNetConnection *p=NULL;
	string strErr("");
	
	if(g_netManager->GetConnectionModeL2() == NET_NONE)
	{
		CaMsgBox msgBox("开启VPN专网模式?", CaMsgBox::MB_YESNO);
		msgBox.ShowBox();
		if (msgBox.m_iStatus == OK_PRESSED)
		{
			ret = g_netManager->SetConnectionModeL2(NET_VPN, &p, strErr);
			if (ret != NET_SUCCESS)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg("设置VPN模式出错");
				return;
			}
			ret = CNetDBWork::saveNetModeL2(NET_VPN, strErr);
			if (ret != SQLITE_OK)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg(strErr);
				return;
			}
		}
		else
		{
			ReFresh();
			return;
		}
	}
	else if (g_netManager->GetConnectionModeL2() == NET_VPN)
	{
		CaMsgBox msgBox("VPN专网已开启,是否关闭?", CaMsgBox::MB_YESNO);
		msgBox.ShowBox();
		if (msgBox.m_iStatus == OK_PRESSED)
		{
			ret = g_netManager->SetConnectionModeL2(NET_NONE, &p, strErr);
			if (ret != NET_SUCCESS)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg("设置VPN模式出错");
				return;
			}
			ret = CNetDBWork::saveNetModeL2(NET_NONE, strErr);
			if (ret != SQLITE_OK)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg(strErr);
				return;
			}
		}
		else
		{
			ReFresh();
			return;
		}
	}

	CaMsgBox::ShowMsg("设置完成");
}

int CNetModeSetMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(NET_MANAGE_MENU);
		return SUCCESS;
		break;	
		
	default: break;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

