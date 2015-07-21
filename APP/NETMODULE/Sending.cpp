/**
 @file  Sending.cpp
 @brief 本文件实现了网络数据收发的基类，其派生类为具体的收发方法类。
 */
#include "Sending.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


Sending::Sending()
{
	m_serverIP="";
	m_serverPort="";
}

Sending::~Sending()
{
}

int Sending::init()
{
	DBG_PRINT(("in Sending::init()"))
	return NET_SUCCESS;
}

void Sending::setFlow(int flow)
{
	DBG_ASSERT_EXIT((0), ("not WMMP mode. ERR!!"));
}

void Sending::SetServerIP(const string IP, const string Port)
{
	m_serverIP.assign(IP);
	m_serverPort.assign(Port);
}
	


