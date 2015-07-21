/*! \file    NetServMenu.cpp
   \brief    awe4000r系统维护的网络维护菜单
   \author   Yu Yan
   \version  1.0
   \date     2010-10-9 16:53
 */

#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "NetModelEditWin.h"

#include "netManager.h"
#include "M2M_Common.h"
#include "NetServMenu.h"

CNetServMenu::CNetServMenu():CMultiBtnCommonWin(2)
{
}

int CNetServMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,NET_SERV_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.模块激活");
	SetTitle(2,"B.MAC下载");
//	SetTitle(3,"C.模块关闭");
	return 1;
}


void CNetServMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	CaMsgBox msgBox("是否激活？", CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if(msgBox.m_iStatus == OK_PRESSED)
	{
		EnableM2M();	
	}
	return;
}



void CNetServMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(DOWNLOAD_MAC_WIN);
	return;
}

int CNetServMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	 {	
		ChangeWin(MAINTENANCE_MENU);
		return SUCCESS;
	 }
//
//#if (0 == RELEASE_VER)
//	if (DISCOUNT_KEY == iEvent)
//	{
//		CaMsgBox msgBox("是否关闭模块？", CaMsgBox::MB_YESNO);
//		msgBox.ShowBox();
//		if(msgBox.m_iStatus == OK_PRESSED)
//		{
//			DisableM2M();	
//		}
//		return SUCCESS;
//	}
//#endif
		
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

INT8 CNetServMenu::EnableM2M(void)
{
	//无论当前为何种网络模式，均可执行M2M激活操作
	if (M2MCommon::setM2MState(M2M_ACT_ON) != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg("M2M模块激活出错");
		return FAILURE;		
	}

	CaMsgBox::ShowMsg("M2M模块激活成功");
	return SUCCESS;

}

INT8 CNetServMenu::DisableM2M(void)
{
	int netMode = g_netManager->GetConnectionMode();

	if (netMode == NET_M2M_APN || netMode == NET_M2M_WMMP)
	{
		CaMsgBox::ShowMsg("当前处于无线通讯模式,无法关闭");
		return SUCCESS;
	}
	
	CaProgressBar proBar("M2M模块关闭中.....");
	proBar.ReFresh();

	if (M2MCommon::setM2MState(M2M_ACT_OFF) != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg("M2M模块关闭出错");
		return FAILURE;		
	}

	CaMsgBox::ShowMsg("M2M模块关闭成功");
	return SUCCESS;

}

