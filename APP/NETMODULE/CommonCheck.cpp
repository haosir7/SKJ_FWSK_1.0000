/**
 @file  CommonCheck.cpp
 @brief 本文件实现了网络接入状态查询（通用）的派生类。
 @author   zl
*/
#include "ipInfoProc.h"
#include "netManager.h"
#include "CommonCheck.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



int CommonCheck::check()
{	
	INT32 sysRet = 0; 
	IPinfo info;
	string cmd("");
	INT32 netMode = curNetMode();
	CNetConnection *p = m_pConnection;

	//需要根据getIPandMASK()结果和dialState值来返回对应状态
	if (netMode == NET_VPN)
	{
		if (g_netManager->GetConnectionMode() == NET_LAN)
			cmd = "ppp0";
		else
			cmd = "ppp1";
	}
	else
	{
		cmd = "ppp0";
	}

	memset((void *)&info, 0, sizeof(info));
	sysRet = getIPandMASK(cmd.c_str(), &info);
	if (sysRet == GET_IP_FAIL)
	{
		if (p->m_dialState == HAVE_DIALED)
			return CHECK_OFF;
		else
			return CHECK_NOT;
	}

	return CHECK_OK;
}

