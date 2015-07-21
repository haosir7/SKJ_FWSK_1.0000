/**
 @file  SendBySocket.cpp
 @brief 本文件实现了Socket方式的网络数据收发类。
 */
#include "socketClient.h"
#include "SendBySocket.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


SendBySocket::SendBySocket()
{
	DBG_PRINT(("SendBySocket()"))
	m_socketFd = -1;
}

SendBySocket::~SendBySocket()
{
	DBG_PRINT(("~SendBySocket()"))
}


int SendBySocket::connectServer(string &strErr)
{
	int i=0;
	int Max_Retry=MAX_SOCKET_CONNECT_RETRY;

	for (i=0; i<Max_Retry; i++)
	{
		SocketClose(&m_socketFd);
		if((m_socketFd=SocketOpen(m_serverIP.c_str(), m_serverPort.c_str())) == -1)
		{
			continue;
		}	
		break;
	}
	
	if (i==Max_Retry)
	{
		SocketClose(&m_socketFd);
		strErr = "建立Socket失败";
		return NET_SOCKET_FAIL;
	}
	else
	{
		return NET_SUCCESS;
	}
	
}
	
int SendBySocket::disConnect()
{
	SocketClose(&m_socketFd);	
	return NET_SUCCESS;
}
	
int SendBySocket::send(UINT8 *psBuf, INT32 sLen, string &strErr)
{
	int sendLen=0;

	sendLen = SocketSend(m_socketFd, psBuf, sLen);
	DBG_PRINT(("sendLen = %d", sendLen))
	if (sendLen < sLen)
	{
		DBG_PRINT(("send Data ERR: %d (%d)", sendLen, sLen));
		strErr = "数据发送失败:";
		return NET_TRANS_FAIL;
	}

	return NET_SUCCESS;
}
	
int SendBySocket::rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
	int revLen=0;
	int conLen=*rLen;

	revLen = SocketRecv(m_socketFd, rBufSize, prBuf, conLen);
	if (revLen <= 0)
	{
		DBG_PRINT(("rev Data ERR: %d", revLen));
		*rLen = 0;
		strErr = "数据接收失败:";
		return NET_TRANS_FAIL;
	}
	
	if (conLen>0)		//指定接收长度时，必须接收完成才返成功
	{
		if (revLen < conLen)
		{
			DBG_PRINT(("rev Data ERR: %d (%d)", revLen, conLen));
			strErr = "数据接收不完整:";
			if (revLen<0)
				*rLen = 0;
			else
				*rLen = revLen;
			return NET_TRANS_FAIL;
		}
	}
	
	*rLen = revLen;
	return NET_SUCCESS;
}
	


