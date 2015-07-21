/**
 @file  transfers_base.cpp
 @brief 本文件实现了网络数据传输基类CNetTransfers。
 */

#include "commonFunc.h"
#include "netManager.h"
#include "transfers_base.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



CNetTransfers::CNetTransfers()
{
	m_pSendBuff = NULL;
	m_pRevBuff = NULL;
	m_sendBuffLen = 0;
	m_revBuffLen = 0;

	m_serverIP.assign("");
	m_serverPort.assign("");
	m_serverFile.assign("");

	m_dataKeepType = DATA_IN_BUFF;
	m_sendFileName = DEF_REQ_FILE;
	m_revFileName = DEF_RSP_FILE;
}

CNetTransfers::~CNetTransfers()
{
}


void CNetTransfers::SetServerFile(const string file)
{
	m_serverFile.assign(file);
}

void CNetTransfers::SetSendBuff(UINT8 *pBuf, INT32 maxLen)
{
	m_pSendBuff = pBuf;
	m_sendBuffLen = maxLen;
}

void CNetTransfers::SetRevBuff(UINT8 *pBuf, INT32 maxLen)
{
	m_pRevBuff = pBuf;
	m_revBuffLen = maxLen;
}

UINT8* CNetTransfers::GetSendBuff(INT32 *bufLen)
{
	*bufLen = m_sendBuffLen;
	return m_pSendBuff;
}

UINT8* CNetTransfers::GetRevBuff(INT32 *bufLen)
{
	*bufLen = m_revBuffLen;
	return m_pRevBuff;
}


int CNetTransfers::TransToSever(INT32 sLen, INT32 *rLen, string &strErr)
{
	int ret=0;
	char tmpbuf[32];

	if (m_pSendBuff == NULL || m_sendBuffLen == 0)
	{
		strErr = "未设置发送缓存";
		return NET_POINT_NULL_ER;
	}
	if (m_pRevBuff == NULL || m_revBuffLen == 0)
	{
		strErr = "未设置接收缓存";
		return NET_POINT_NULL_ER;
	}

	//获取发送数据
	ret = getSendData(&sLen);
	if (ret != NET_SUCCESS)
	{
		sprintf(tmpbuf, "获取发送数据出错:%d", ret);
		strErr = tmpbuf;
		return NET_TRANS_FAIL;
	}

	//传输过程
#if (SINGLE_TRANSBUFF_MODE == 0)
	memset(m_pRevBuff, 0, m_revBuffLen);
#endif
	*rLen = 0;
	ret = trans_proc(m_pSendBuff, sLen, m_pRevBuff, rLen, strErr);
	if (ret != NET_SUCCESS)
	{
		DBG_PRINT(("ret = %d", ret));
		DBG_PRINT(("strErr=%s", strErr.c_str()));
		string tmpstr("网络通讯错:");
		tmpstr += strErr;
		strErr = tmpstr;
		return ret;
	}

	//放接收数据
	ret = putRevData(*rLen);
	if (ret != NET_SUCCESS)
	{
		sprintf(tmpbuf, "写接收数据出错:%d", ret);
		strErr = tmpbuf;
		return NET_TRANS_FAIL;
	}

	return NET_SUCCESS;
}


int CNetTransfers::getSendData(INT32 *sLen)
{
	int dataLen=0;
	if (m_dataKeepType == DATA_IN_FILE)
	{
		if (*sLen>0)
		{
			FILE *fp;	
			if( (fp = fopen(m_sendFileName.c_str(), "rb")) == NULL )
			{
				return NET_OPERATOR_FILE_ER;
			}
	
			fseek( fp, 0L, SEEK_END );	
			dataLen = ftell( fp );				
			DBG_PRINT(("Req filelen: %d", dataLen));
			if (dataLen >= m_sendBuffLen)
			{
				return NET_BUFF_OVERFLOW_ER;
			}
			
			fseek( fp, 0L, SEEK_SET );	
			fread( (void *)m_pSendBuff, dataLen, 1, fp);
			fclose(fp);
			*sLen = dataLen;
		}
	}
	else
	{
		if (*sLen >= m_sendBuffLen)
		{
			DBG_ASSERT_EXIT((*sLen>=m_sendBuffLen), ("send len overflow "));			
			return NET_BUFF_OVERFLOW_ER;
		}
	}

	return NET_SUCCESS;
}

int CNetTransfers::putRevData(INT32 len)
{
	if (len >= m_revBuffLen)
	{
		DBG_ASSERT_EXIT((len>=m_revBuffLen), ("rev len overflow "));			
		return NET_BUFF_OVERFLOW_ER;
	}

	if (m_dataKeepType == DATA_IN_FILE)
	{
		FILE *fp;	
		if( (fp = fopen(m_revFileName.c_str(), "wb")) == NULL )
		{
			return NET_OPERATOR_FILE_ER;
		}

		fwrite( (void *)m_pRevBuff, len, 1, fp);
		fclose(fp);
	}

	return NET_SUCCESS;
}

void CNetTransfers::SetDataKeepType(INT32 type)
{
	m_dataKeepType = type;
}

