/**
 @file  trans_direct.cpp
 @brief 本文件实现了无应用层协议下的传输类。
 */
#include "trans_direct.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


trans_direct::trans_direct(Sending *p):trans_abstrac(p)
{
	DBG_PRINT(("trans_direct()"))
}

trans_direct::~trans_direct()
{
	DBG_PRINT(("~trans_direct()"))
}


int trans_direct::sendData(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
	int ret=0;
	INT32 tmpLen=0;
	
	ret = send(psBuf, sLen, strErr);
	DBG_PRINT(("ret = %d", ret))
	if (ret != NET_SUCCESS)
	{
		return ret;
	}

	ret = rev(m_revBuffLen, prBuf, &tmpLen, strErr);
	DBG_PRINT(("ret = %d", ret))
	if (ret != NET_SUCCESS)
	{
		*rLen = tmpLen;
		return ret;
	}

	*rLen = tmpLen;
	return NET_SUCCESS;
}






