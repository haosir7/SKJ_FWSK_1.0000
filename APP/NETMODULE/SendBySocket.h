/**
 @file  SendBySocket.h
 @brief 本文件定义了Socket方式的网络数据收发类。
 */
#ifndef __H_SEND_BY_SOCKEY_
#define __H_SEND_BY_SOCKEY_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "Sending.h"


/**
 @class SendBySocket 
 @brief Socket网络数据收发类。
 */
class SendBySocket : public Sending 
{
private:
	int m_socketFd;			/**< Socket号 */

public:
	SendBySocket();
	virtual ~SendBySocket();

	int connectServer(string &strErr);
	int disConnect();
	int send(UINT8 *psBuf, INT32 sLen, string &strErr);
	int rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr);

};



#endif

