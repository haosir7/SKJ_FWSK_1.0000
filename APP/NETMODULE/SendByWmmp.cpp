/**
 @file  SendByWmmp.cpp
 @brief 本文件实现了WMMP方式的网络数据收发类。
 */
#include "wmmp.h"
#include "SendByWmmp.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


SendByWmmp::SendByWmmp()
{
	DBG_PRINT(("SendByWmmp()"));
	m_flow = WMMP_FLOW_DEFAULT;

	m_pRevTmpBuf = new UINT8[WMMP_RX_BUF_LEN];
	m_revBufHead = 0;
	m_revBufRear = 0;
}

SendByWmmp::~SendByWmmp()
{
	DBG_PRINT(("~SendByWmmp()"));
	if (m_pRevTmpBuf != NULL)
	{
		delete m_pRevTmpBuf;
		m_pRevTmpBuf = NULL;
	}
}


int SendByWmmp::connectServer(string &strErr)
{
#if (_CONFIG_M2M_MODULE == 1)
	int rc=0;

	if (m_flow<0)
		m_flow = WMMP_FLOW_DEFAULT;

	rc = wmmp_openAppFlow(m_flow);
	if( rc != 0 )
	{
		strErr = "开启WMMP流通道失败";
		return NET_WMMP_FAIL;
	}

	m_revBufHead = 0;
	m_revBufRear = 0;

	DBG_PRINT(("open flow successed"));
#endif
	return NET_SUCCESS;
}
	
int SendByWmmp::disConnect()
{
#if (_CONFIG_M2M_MODULE == 1)
	int rc=0;
	rc = wmmp_closeAppFlow(m_flow);
	if( rc != 0 )
	{
		DBG_PRINT(("close Flow failed"));
	}
#endif
	
	return NET_SUCCESS;
}
	
int SendByWmmp::send(UINT8 *psBuf, INT32 sLen, string &strErr)
{
#if (_CONFIG_M2M_MODULE == 1)
	int rc=0;
	unsigned int send_len = 0;
	unsigned int total_bytes = 0;
	unsigned int remain_bytes = sLen;
	
	do
	{
		send_len = (WMMP_TRANS_PACK_LEN < remain_bytes) ? WMMP_TRANS_PACK_LEN : remain_bytes;
		rc = wmmp_sendAppData(m_flow, (INT8 *)(psBuf+total_bytes), send_len);
		if (rc != 0)
		{
			strErr = "WMMP数据发送失败";
			return NET_WMMP_FAIL;
		}
		total_bytes += send_len;
		remain_bytes -= send_len;
	}while( remain_bytes > 0 );
#endif
		
	return NET_SUCCESS;
}
	
int SendByWmmp::rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
#if (_CONFIG_M2M_MODULE == 1)
	int rc=0;
	int revLen = rBufSize;
	int conLen = *rLen;
	int curRecLen = 0;
	
	if (conLen <= 0)
	{
		strErr = "wmmp未指定接收长度";
		return NET_WMMP_FAIL;
	}

	while(1)
	{
		curRecLen = WMMP_RX_BUF_LEN - m_revBufHead;	//将curRecLen赋值为可接收数据的最大长度

		//所需数据已经放入了缓存buff
		if (curBuffLength() >= conLen)
		{
			getDataFromBuff(prBuf, conLen);
			break;
		}

		//从M2M模块读取到缓存buff
		rc = wmmp_readAppData(m_flow, (INT8 *)(m_pRevTmpBuf+m_revBufHead), &curRecLen);
		if (rc != 0)
		{
			strErr = "WMMP数据接收出错";
			return NET_WMMP_FAIL;
		}
		DBG_PRINT(("curRecLen=%d", curRecLen));
		m_revBufHead += curRecLen;
	}

	*rLen = conLen;
#endif

	return NET_SUCCESS;
}

void SendByWmmp::SetServerIP(const string IP, const string Port)
{
#if (_CONFIG_M2M_MODULE == 1)
	DBG_PRINT(("in WMMP SetServerIP()"));
	m_serverIP.assign(IP);
	m_serverPort.assign(Port);

	wmmp_setAppServer(m_flow, IP.c_str(), Port.c_str());
#endif
}
		
void SendByWmmp::setFlow(int flow)
{
	m_flow = flow;
}

int SendByWmmp::curBuffLength()
{
	return (m_revBufHead - m_revBufRear);
}

void SendByWmmp::getDataFromBuff(UINT8 *prBuf, INT32 conlen)
{
#if (_CONFIG_M2M_MODULE == 1)
	memcpy(prBuf, m_pRevTmpBuf+m_revBufRear, conlen);
	m_revBufRear += conlen;
#endif
}

