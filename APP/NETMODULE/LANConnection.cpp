/**
 @file  LANConnection.cpp
 @brief 本文件实现了与局域网模式有关的几个派生类。
 @author   zl
*/
#include "commonFunc.h"
#include "DoNothing.h"
#include "CommonGetIPinfo.h"
#include "setToFileProc.h"
#include "netManager.h"
#include "LANConnection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



int LANCheck::check()
{
	int sysRet=0;
	LANConnection *p = (LANConnection *)m_pConnection;
	
	if (p->m_dhcpMode == LAN_DHCP_ON)
	{
		sysRet = system("ifconfig eth0");
		if (sysRet != 0)
			return CHECK_OFF;
		else
			return CHECK_OK;
	}
	else		//手动输IP方式下，目前尚无法判断连接状态
	{
		return CHECK_OK;
	}

}


LANConnection::LANConnection()
{
	string strErr("");

	m_connectionMode = NET_LAN;
	m_dialState = HAVE_DIALED;	//LAN模式下直接处于已拨号的状态
	
	m_pConnect = new connectNothing(this);
	m_pDisConnect = new disconnectNothing(this);
	m_pCheck = new LANCheck(this);
	m_pGetIPinfo = new CommonGetIPinfo(this);

	if (CNetDBWork::loadDHCPMode(&m_dhcpMode, strErr) != SQLITE_OK)
	{
		m_dhcpMode = LAN_DHCP_OFF;
	}
}

LANConnection::~LANConnection()
{
}

int LANConnection::setDhcpMode(INT32 dhcpMode, string &strErr)
{
	if (g_netManager->checkConnect() == CONNECT_ON_L2)
	{
		strErr = "请先断开VPN连接";
		return NET_CONDITION_ER;
	}

	//修改/bin/init脚本
	if (updateInitFileForDHCP(dhcpMode) != NET_SUCCESS)
	{
		strErr = "修改init脚本错误";
		return NET_OPERATOR_FILE_ER;
	}
		
	//DHCP状态记入数据库
	if (CNetDBWork::saveDHCPMode(dhcpMode, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}

	m_dhcpMode = dhcpMode;

	//开启DHCP
	IPinfo info;
	if (CNetDBWork::loadIPinfo(&info, strErr) != SQLITE_OK)
	{
		DBG_PRINT(("%s", strErr.c_str()))
		DBG_PRINT(("数据库中无ip信息,"));
		return NET_SUCCESS;
	}
	restartDHCP(m_dhcpMode, &info);

	return NET_SUCCESS;
}

int LANConnection::setIPinfo(IPinfo *info, string &strErr)
{
	if (m_dhcpMode == LAN_DHCP_ON)
	{
		strErr = "自动获取IP模式下无法设置";
		return NET_MODE_ER;
	}
	if (g_netManager->checkConnect() == CONNECT_ON_L2)
	{
		strErr = "请先断开VPN连接";
		return NET_CONDITION_ER;
	}

	//修改/bin/init脚本及DNS配置文件
	if (setIPinfoToFile(info) != NET_SUCCESS)
	{
		strErr = "修改配置文件错误";
		return NET_OPERATOR_FILE_ER;
	}

	//新的IP设置记录入库
	if (CNetDBWork::saveIPinfo(info, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}

	//重启局域网
	restartLAN(info);

	return NET_SUCCESS;
}

int LANConnection::initNetDevice(string &strErr)
{
	IPinfo info;
	char tmpbuf[32];

	CNetDBWork::loadIPinfo(&info, strErr);
	system("ifconfig eth0 up");
	sprintf(tmpbuf, "route add default gw %s", info.GeteWay);
	DBG_PRINT(("%s", tmpbuf))
	system(tmpbuf);
	return NET_SUCCESS;
}

