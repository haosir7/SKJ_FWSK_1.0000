/**
 @file  trans_direct.h
 @brief 本文件定义了无应用层协议下的传输类。
 */
#ifndef	__H_TRANS_DIRECT_
#define	__H_TRANS_DIRECT_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "trans_abstrac.h"



/**
 @class trans_direct 
 @brief 实现无应用层协议的传输。
 */
class trans_direct : public trans_abstrac
{
public:
	trans_direct(Sending *p);
	virtual ~trans_direct();

private:
	int sendData(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr);


};





#endif

