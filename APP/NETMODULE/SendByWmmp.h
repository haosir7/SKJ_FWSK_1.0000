/**
 @file  SendByWmmp.h
 @brief 本文件定义了WMMP方式的网络数据收发类。
 */
#ifndef __H_SEND_BY_WMMP_
#define __H_SEND_BY_WMMP_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "Sending.h"


/**
 @class SendByWmmp 
 @brief WMMP网络数据收发类。
 */
class SendByWmmp : public Sending 
{
private:
	int m_flow;					/**< 流序号 */

	UINT8 *m_pRevTmpBuf;		/**< WMMP专用接收缓存指针 */
	int m_revBufHead;			/**< 接收缓存数据头位置 */
	int m_revBufRear;			/**< 接收缓存数据尾位置 */

public:
	SendByWmmp();
	virtual ~SendByWmmp();

	int connectServer(string &strErr);
	int disConnect();
	int send(UINT8 *psBuf, INT32 sLen, string &strErr);
	int rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr);

	void SetServerIP(const string IP, const string Port);
	void setFlow(int flow);

private:
	/*!
	@brief 返回当前缓存BUFF内数据长度。
	@return   BUFF内数据长度
	*/
	int curBuffLength();

	/*!
	@brief 从缓存BUFF内取数据。
	@param[out] prBuf 取出的数据
	@param[in] conlen 取出数据长度
	*/
	void getDataFromBuff(UINT8 *prBuf, INT32 conlen);

};



#endif

