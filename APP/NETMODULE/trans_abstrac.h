/**
 @file  trans_abstrac.h
 @brief 本文件定义了网络数据传输抽象类trans_abstrac，该类及其派生类实现了具体的传输过程。
 */
#ifndef	__H_ABSTRAC_TRANS_
#define	__H_ABSTRAC_TRANS_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "transfers_base.h"
#include "Sending.h"


/**
 @class trans_abstrac 
 @brief 数据传输抽象类。可派生不同的网络协议层子类。
 */
class trans_abstrac : public CNetTransfers
{
private:
	 Sending *m_pSending;			/**< 数据通讯的对象指针 */

public:
	trans_abstrac(Sending *p);
	virtual ~trans_abstrac();

	void SetServerIP(const string IP, const string Port);
	int CheckServerLink(string &strErr);
	
private:
	/*!
	@brief 数据收发函数
	@param[in] psBuf 待发送的数据
	@param[in] sLen 发送数据长度
	@param[out] prBuf 接收的数据
	@param[in,out] rLen 输入接收缓存大小，输出实际接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int sendData(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)=0;

	/*!
	@brief 传输过程
	@param[in] psBuf 待发送的数据
	@param[in] sLen 发送数据长度
	@param[out] prBuf 接收的数据
	@param[in,out] rLen 输入接收缓存大小，输出实际接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	@note 本函数内实现了传输失败后的重试逻辑
	*/
	int trans_proc(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr);

protected:
	/*!
	@brief 执行连接服务器操作
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int connectServer(string &strErr);
	/*!
	@brief 执行断开服务器操作
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int disConnect();
	/*!
	@brief 执行发送操作
	@param[in] psBuf 待发送的数据
	@param[in] sLen 发送数据长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int send(UINT8 *psBuf, INT32 sLen, string &strErr);
	/*!
	@brief 执行接收操作
	@param[in] rBufSize 接收缓存最大BUFF
	@param[out] prBuf 接收的数据
	@param[out] rLen 实际接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr);
	
};





#endif


