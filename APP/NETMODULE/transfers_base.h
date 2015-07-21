/**
 @file  transfers_base.h
 @brief 本文件定义了网络数据传输基类CNetTransfers。
 */

#ifndef	__H_NET_TRANSFERS_
#define	__H_NET_TRANSFERS_


#include "IncludeMe.h"
#include "netHeaderDef.h"


/**
 @struct TransMode 
 @brief 定义了几个数据传输中的参数。
 */
typedef struct{
	int module;				/**< 传输模块选择 */
	int appProtocal;		/**< 应用层协议 */
//	int transMethod;		//传输方法选择 （已取消 140528）
	int dataKeepType;		/**< 数据暂存方式 */
}TransMode;



/**
 @class CNetTransfers 
 @brief 数据传输基类
 
  定义了数据传输基本接口。提供了服务器设置、缓存设置等函数。
 */
class CNetTransfers
{
private:
	string m_sendFileName;		/**< DATA_IN_FILE情况下的数据发送临时文件 */
	string m_revFileName;		/**< DATA_IN_FILE情况下的数据接收临时文件 */

	INT32 m_dataKeepType;		/**< 传输数据的暂存方式：DATA_IN_BUFF或DATA_IN_FILE */

protected:
	UINT8 *m_pSendBuff;			/**< 数据发送缓存指针 */
	UINT32 m_sendBuffLen;		/**< 数据发送缓存大小 */
	UINT8 *m_pRevBuff;			/**< 数据接收缓存指针 */
	UINT32 m_revBuffLen;		/**< 数据发送缓存大小 */

public:
	string m_serverIP;			/**< 服务器IP */
	string m_serverPort;		/**< 服务器端口 */
	string m_serverFile;		/**< 服务器路径 */


public:
	CNetTransfers();
	virtual ~CNetTransfers();

	/*!
	@brief 网络数据传输的主函数，供上层调用。
	@param[in] sLen 发送长度
	@param[out] rLen 接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int TransToSever(INT32 sLen, INT32 *rLen, string &strErr);

	/*!
	@brief 设置服务端IP\PORT
	@param[in] IP 服务端IP
	@param[in] Port 服务端端口号
	*/
	virtual void SetServerIP(const string IP, const string Port)=0;

	/*!
	@brief 设置服务端HTTP路径
	*/
	void SetServerFile(const string file);

	/*!
	@brief 设置发送用缓存
	*/
	void SetSendBuff(UINT8 *pBuf, INT32 maxLen);
	/*!
	@brief 设置接收用缓存
	*/
	void SetRevBuff(UINT8 *pBuf, INT32 maxLen);

	/*!
	@brief 获得发送缓存指针与长度
	*/
	UINT8* GetSendBuff(INT32 *bufLen);

	/*!
	@brief 获得接收缓存指针与长度
	*/
	UINT8* GetRevBuff(INT32 *bufLen);

	/*!
	@brief 设置传输数据的暂存方式
	@param[in] type-类型：DATA_IN_BUFF-数据暂存于BUFF；DATA_IN_FILE-数据暂存于文件
	*/
	void SetDataKeepType(INT32 type);

	/*!
	@brief 检查与服务器之间的连接。（SOCKET方式下只尝试建立SOCKET）
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int CheckServerLink(string &strErr)=0;


private:
	/*!
	@brief 根据不同暂存方式获得要发送的数据
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int getSendData(INT32 *sLen);
	/*!
	@brief 根据不同暂存方式写入接收的数据
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int putRevData(INT32 len);
	
	/*!
	@brief 传输过程
	@param[in] psBuf 待发送的数据
	@param[in] sLen 发送数据长度
	@param[out] prBuf 接收的数据
	@param[in,out] rLen 输入接收缓存大小，输出实际接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	virtual int trans_proc(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)=0;
	
};

#endif

