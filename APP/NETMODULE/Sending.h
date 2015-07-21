/**
 @file  Sending.h
 @brief 本文件定义了网络数据收发的基类，其派生类为具体的收发方法类。
 */
#ifndef __H_SENDING_DATA_
#define __H_SENDING_DATA_

#include "IncludeMe.h"
#include "netHeaderDef.h"


/**
 @class Sending 
 @brief 网络数据收发基类，本类定义了与服务器建立连接、发送、接收数据的具体接口。
 */
class Sending 
{
protected:
	string m_serverIP;			/**< 目标服务器IP */
	string m_serverPort;		/**< 目标服务器端口 */

public:
	Sending();
	virtual ~Sending();

	/*!
	@brief 初始化。
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int init();

	/*!
	@brief 建立与目标服务器的连接。
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int connectServer(string &strErr)=0;

	/*!
	@brief 断开服务器。
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int disConnect()=0;

	/*!
	@brief 数据发送。
	@param[in] psBuf 待发送的数据
	@param[in] sLen 发送数据长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int send(UINT8 *psBuf, INT32 sLen, string &strErr)=0;

	/*!
	@brief 数据接收。
	@param[in] rBufSize 接收缓存最大BUFF
	@param[out] prBuf 接收的数据
	@param[out] rLen 实际接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr)=0;

	/*!
	@brief 设置目标服务器IP与端口。
	@param[in] IP 目标服务器IP
	@param[in] Port 目标服务器端口
	*/
	virtual void SetServerIP(const string IP, const string Port);

	/*!
	@brief M2M模块设置流。
	@param[in] flow 流序号
	*/
	virtual void setFlow(int flow);

	

};



#endif

