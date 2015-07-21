/**
 @file  DoNothing.cpp
 @brief 本文件定义了与非任何网络模式的派生类（提高代码兼容性）。
 @author   zl
*/
#include "DoNothing.h"


int connectNothing::connect(string &strErr)
{
	return NET_SUCCESS;
}

int disconnectNothing::disConnect()
{
	return NET_SUCCESS;
}

int checkNothing::check()
{
	return CHECK_NOT;
}

int IPinfoDoNothing::getIPinfo(IPinfo *info, string &strErr)
{
	strErr="当前网络模式下无IP信息"; 
	return NET_MODE_ER;
}


NoneConnection::NoneConnection()
{
	m_connectionMode = NET_NONE;
	m_dialState = NOT_DIALED;	
	m_pConnect = new connectNothing(this);
	m_pDisConnect = new disconnectNothing(this);
	m_pCheck = new checkNothing(this);	
	m_pGetIPinfo = new IPinfoDoNothing(this);
}

NoneConnection::~NoneConnection()
{
}

