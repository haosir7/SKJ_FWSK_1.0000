/**
 @file  trans_CURL.h
 @brief 本文件定义了网络数据传输类trans_CURL，该类使用CURL库做数据传输。
 */

#ifndef	__H_TRANS_BY_CURL_
#define	__H_TRANS_BY_CURL_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "transfers_base.h"
#include "MycUrlHttp.h"



/**
 @class trans_CURL 
 @brief 本类使用CURL库做实现网络数据传输。
 */
class trans_CURL : public CNetTransfers
{
public:
	trans_CURL();
	virtual ~trans_CURL();

	void SetServerIP(const string IP, const string Port);
	int CheckServerLink(string &strErr);


private:
	TCUrlModel m_curModel;			/**< CURL类库操作句柄 */
	
	int trans_proc(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr);

	
};


#endif




