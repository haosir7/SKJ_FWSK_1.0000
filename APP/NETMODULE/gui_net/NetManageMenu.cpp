/**
 @file  NetManageMenu.cpp
 @brief 界面文件，网络模块主菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"

#include "NetServerSetWin.h"
#include "NetModelEditWin.h"
#include "ADSLInfoSetWin.h"

#include "netManager.h"
#include "NetManageMenu.h"


#if (TEST_CONNECT_TYPE==2)
#include "NetCommunication.h"
#endif

CNetManageMenu::CNetManageMenu():CMultiBtnCommonWin(9,2)
{
}

int CNetManageMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, NET_MANAGE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);

	SetTitle(1,  "A.开票服务器");
	SetTitle(2,  "B.连接测试");
	SetTitle(3,  "C.网络模式");
	SetTitle(4,  "D.网络设置");
	SetTitle(5,  "E.ADSL连接");	
	SetTitle(6,  "F. 3G 连接");
	SetTitle(7,  "G.APN 连接");
	SetTitle(8,  "H.VPN 专网");
	SetTitle(9,  "I.开机拨号");

	
	return 1;
}

void CNetManageMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	((CNetServerSetWin *)m_pFrame->GetWin(NET_SERVER_SET_WIN))->m_servType = APP_SERV;
	ChangeWin(NET_SERVER_SET_WIN);
}

void CNetManageMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
//	string strErr;
	ConnectTest();
//	if( ret == SUCCESS )
//		CaMsgBox::ShowMsg(strErr);
}

void CNetManageMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	((CNetModelEditWin *)m_pFrame->GetWin(NETMODEL_EDIT_WIN))->m_pageFlag = 0;
	ChangeWin(NETMODEL_EDIT_WIN);
}

void CNetManageMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NETWORK_SET_MENU);
}

void CNetManageMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_ADSL_MODULE != 0)
	CaProgressBar info("检查ADSL状态...");
	info.Show();
	if (g_netManager->GetConnectionMode() != NET_ADSL)
	{
		CaMsgBox::ShowMsg("当前不是ADSL模式");
		return;
	}
	CNetConnection *pNet=g_netManager->GetConnectionHandle();
	netConnectProc(pNet);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetManageMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_3G_MODULE != 0)
	CaProgressBar info("检查3G状态...");
	info.Show();
	if (g_netManager->GetConnectionMode() != NET_3G)
	{
		CaMsgBox::ShowMsg("当前不是3G模式");
		return;
	}
	CNetConnection *pNet=g_netManager->GetConnectionHandle();
	netConnectProc(pNet);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetManageMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_M2M_MODULE != 0)
	CaProgressBar info("检查M2M状态...");
	info.Show();
	if (g_netManager->GetConnectionMode() != NET_M2M_APN)
	{
		CaMsgBox::ShowMsg("当前不是M2M模式");
		return;
	}
	CNetConnection *pNet=g_netManager->GetConnectionHandle();
	netConnectProc(pNet);

#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetManageMenu::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if (_CONFIG_VPN_MODULE != 0)
	CaProgressBar info("检查VPN状态...");
	info.Show();
	if (g_netManager->GetConnectionModeL2() != NET_VPN)
	{
		CaMsgBox::ShowMsg("当前未开启VPN模式");
		return;
	}
	CNetConnection *pNet=g_netManager->GetConnectionHandleL2();
	netConnectProc(pNet);
	
#else
	CaMsgBox::ShowMsg("当前版本不支持");
#endif
}

void CNetManageMenu::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_AUTO_DIAL_SET_WIN);
}


int CNetManageMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);
		return SUCCESS;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

void CNetManageMenu::netConnectProc(CNetConnection *pNet)
{
	int ret=0;
	string strErr("");
	CaProgressBar info("");

	if (pNet->performCheck() != CHECK_OK)
	{
		CaMsgBox msgBox("是否开始连接?", CaMsgBox::MB_YESNO);
		msgBox.ShowBox();
		if (msgBox.m_iStatus == OK_PRESSED)
		{	
			info.SetText("网络连接中,请稍候...");
			info.Show();
			ret = pNet->performConnect(strErr);
			if (ret != NET_SUCCESS)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg(strErr);
				return;
			}
			CaMsgBox::ShowMsg("网络连接成功");
			return;
		}
	}
	else
	{
		CaMsgBox msgBox("是否断开连接?", CaMsgBox::MB_YESNO);
		msgBox.ShowBox();
		if (msgBox.m_iStatus == OK_PRESSED)
		{	
			info.SetText("网络断开中...");
			info.Show();
			ret = pNet->performDisConnect();
			if (ret != NET_SUCCESS)
			{
				DBG_PRINT(("ret = %d", ret))
				CaMsgBox::ShowMsg(strErr);
				return;
			}
			CaMsgBox::ShowMsg("网络断开完成");
			return;
		}
	}
}

void CNetManageMenu::ConnectTest()
{
	//检查是否已建立连接
	if (g_netManager->checkConnect() == CONNECT_OFF)
	{
		CaMsgBox::ShowMsg("尚未建立网络连接");
		return ;
	}
	
	CaMsgBox msgBox("是否连接测试？", CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return; 
	}
	
	string strErr;
	CaProgressBar proBar("连接测试......");
	proBar.Show();

	//调用CheckServerLink测试，该函数只测试建立连接，不做数据传输。
#if (TEST_CONNECT_TYPE==2)
	CNetTrans NetTrans;
	if( NetTrans.ConnectServerTest(strErr) != SUCCESS )
	{
		strErr = "连接测试失败";	   
	}
	else
	{
		strErr = "连接测试成功";	   
	}
#endif

#if (TEST_CONNECT_TYPE==1)
	CNetTransfers *p=NULL;
	p=g_netManager->GetTransHandle();
	if (p->CheckServerLink(strErr) != NET_SUCCESS)
	{
		strErr = "连接测试失败";
	}	
	else
	{
		strErr = "连接测试成功";	   
	}
	g_netManager->ReleaseTransHandle(p);
#endif

	CaMsgBox::ShowMsg(strErr);
	return;
}
