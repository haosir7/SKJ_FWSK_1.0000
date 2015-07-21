/**
 @file  CommonGetIPinfo.cpp
 @brief 本文件实现了获取IP信息（通用）的派生类。
 @author   zl
*/
#include "netManager.h"
#include "ipInfoProc.h"
#include "CommonGetIPinfo.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"




int CommonGetIPinfo::getIPinfo(IPinfo *info, string &strErr)
{	
	DBG_ASSERT_EXIT((info != NULL), ("info == NULL!"));

	INT32 res=0;
	INT32 netMode = curNetMode();
	string tmpStr("");
	CNetConnection *p = m_pConnection;

	if (netMode==NET_NONE || netMode==NET_M2M_WMMP )
	{
		DBG_PRINT(("getIPinfo. netMode=%d", netMode))
		strErr = "当前网络模式下无IP信息";
		return NET_MODE_ER;
	}
	
	if (p->performCheck() != CHECK_OK)
	{
		strErr = "网络尚未接通,无IP信息";
		return NET_CONNECT_FAIL;
	}

	//确定获取IP信息的对象
	if (netMode == NET_VPN)
	{
		if (g_netManager->GetConnectionMode() == NET_LAN)
			tmpStr = "ppp0";
		else
			tmpStr = "ppp1";
	}
	else if (netMode == NET_LAN)
	{
		tmpStr = "eth0";
	}
	else
	{
		tmpStr = "ppp0";
	}

	memset((void *)info, 0, sizeof(IPinfo));

	// 提取IP与子网掩码
	res = getIPandMASK(tmpStr.c_str(), info);
	if (res != GET_IPINFO_SUCCESS)
	{
		strErr = "获取网络IP出错";
		return NET_OTHER_ER;
	}

	// 提取网关信息
	res = getRoute(info);
	if (res != GET_IPINFO_SUCCESS)
	{
		strErr = "获取路由信息出错";
		return NET_OTHER_ER;
	}
	DBG_PRINT(("m_GateWay = %s", info->GeteWay));

	// 提取DNS信息
	res = getDNS(info);
	if (res != GET_IPINFO_SUCCESS)
	{
		strErr = "获取网络DNS出错";
		return NET_OTHER_ER;
	}

	return NET_SUCCESS;
}

