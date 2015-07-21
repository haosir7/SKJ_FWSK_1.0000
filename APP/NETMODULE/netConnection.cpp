/**
 @file  netConnection.cpp
 @brief 本文件实现了网络接入基类。
 @author   zl
*/

#include "netConnection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


int ConnectACT::curNetMode()
{
	return m_pConnection->m_connectionMode;
}

int DisConnectACT::curNetMode()
{
	return m_pConnection->m_connectionMode;
}

int CheckConnectionACT::curNetMode()
{
	return m_pConnection->m_connectionMode;
}

int GetIPinfoACT::curNetMode()
{
	return m_pConnection->m_connectionMode;
}




CNetConnection::CNetConnection()
{
	m_connectionMode = 0;
	m_dialState = 0;
	
	m_pConnect = NULL;
	m_pDisConnect = NULL;
	m_pCheck = NULL;
	m_pGetIPinfo = NULL;
}

CNetConnection::~CNetConnection()
{
	if (m_pConnect != NULL)
	{
		delete m_pConnect;
		m_pConnect = NULL;
	}
	if (m_pDisConnect != NULL)
	{
		delete m_pDisConnect;
		m_pDisConnect = NULL;
	}
	if (m_pCheck != NULL)
	{
		delete m_pCheck;
		m_pCheck = NULL;
	}
	if (m_pGetIPinfo != NULL)
	{
		delete m_pGetIPinfo;
		m_pGetIPinfo = NULL;
	}
	
}


int CNetConnection::performConnect(string &strErr)
{
	DBG_ASSERT_EXIT((m_pConnect != NULL), (" m_pConnect == NULL!"));
	return m_pConnect->connect(strErr);
}

int CNetConnection::performDisConnect()
{
	DBG_ASSERT_EXIT((m_pDisConnect != NULL), (" m_pDisConnect == NULL!"));
	return m_pDisConnect->disConnect();
}

int CNetConnection::performCheck()
{
	DBG_ASSERT_EXIT((m_pCheck != NULL), (" m_pCheck == NULL!"));
	return m_pCheck->check();
}

int CNetConnection::performGetIPinfo(IPinfo *info, string &strErr)
{
	DBG_ASSERT_EXIT((m_pGetIPinfo != NULL), (" m_pGetIPinfo == NULL!"));
	return m_pGetIPinfo->getIPinfo(info, strErr);
}


int CNetConnection::initNetDevice(string &strErr) 
{
	return NET_SUCCESS;
}

int CNetConnection::setUserNamePwd(string name, string pwd, string &strErr) 
{
	return NET_SUCCESS;
}

int CNetConnection::getCSQ(int *rssi, int *ber) 
{
	DBG_ASSERT_EXIT((0), ("this mode can't get CSQ!")); 
	return NET_MODE_ER;
}


