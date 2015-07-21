/**
 @file  netManager.h
 @brief 本文件定义了网络模块中的网络管理类CNetManager。
 */

#ifndef	__H_NET_MANAGER_
#define	__H_NET_MANAGER_


#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "netConnection.h"
#include "transfers_base.h"

extern unsigned char g_netSendBuf[];
extern unsigned char g_netRevBuf[];


/**
 @class CNetManager 
 @brief 网络管理类

  用于管理网络连接与数据通讯两个子模块的构造、析构、设置等过程。
  这个类仅有一个全局性的实体对象。
 */
class CNetManager
{
private:
	CNetConnection *m_pConnectionL1;		/**< 第一层网络链接的指针		*/
	CNetConnection *m_pConnectionL2;		/**< 第二层网络链接的指针					*/

	TransMode m_transMode;					/**< 传输方式					*/

	string m_defServerIP;					/**< 默认的服务器IP				*/
	string m_defServerPort;					/**< 默认的服务器PORT			*/
	string m_defServerFile;					/**< 默认的HTTP路径				*/

	UINT8 *m_pDefSendBuff;					/**< 默认的发送BUF指针与长度	*/
	UINT32 m_defSendBuffLen;				
	UINT8 *m_pDefRevBuff;					/**< 默认的接收BUF指针与长度	*/
	UINT32 m_defRevBuffLen;

	static CNetManager *m_pNetManager;		/**< 指向自身对象的指针			*/
	
public:
	/*!
	@brief 设置网络连接方式（第一层）
	@param[in] mode 连接方式
	@param[out] handle 输出的网络连接句柄
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int SetConnectionMode(int mode, CNetConnection **handle, string &strErr);
	/*!
	@brief 设置网络连接方式（第二层）
	@param[in] mode 连接方式
	@param[out] handle 输出的网络连接对象指针
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int SetConnectionModeL2(int mode, CNetConnection **handle, string &strErr);


	/*!
	@brief 获取当前的网络连接方式
	@return 连接方式 
	*/
	int GetConnectionMode();
	/*!
	@brief 获取当前的网络连接方式（第二层）
	@return 连接方式 
	*/
	int GetConnectionModeL2();

	/*!
	@brief 获取当前网络连接对象的指针
	@return 连接方式 
	*/
	CNetConnection* GetConnectionHandle();
	/*!
	@brief 获取当前网络连接对象（第二层）的指针
	@return 连接方式 
	*/
	CNetConnection* GetConnectionHandleL2();
		

	/*!
	@brief 查询当前的连接状态（两层连接都进行检查）
	@return CONNECT_ON_L2 - 已建立第二层连接 
			CONNECT_ON_L1 - 已建立第一层连接  
			CONNECT_OFF   - 未建立连接
	@mark 当不存在连接实体时，认为没有建立连接
	*/
	int checkConnect();

	
	/*!
	@brief 获取当前的IP信息（如果第二层连接建立则输出第二层的IP）
	@param[out] info 输出的IP信息
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int getIPinfo(IPinfo *info, string &strErr);


	/*!
	@brief 网络模块的初始化函数，开机时调用。
		   根据数据库记录初始化默认的连接方式、服务器IP等。
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int init(string &strErr);
	
	/*!
	@brief 开机自动网络连接函数。
		   根据数据库的记录进行自动网络连接。
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int autoDialProc(string &strErr);

	

	/*!
	@brief 获取数据传输对象的指针，传输的类型使用默认值。
	@return   传输对象的指针	NULL-失败 
	*/
	CNetTransfers* GetTransHandle();
	/*!
	@brief 获取数据传输对象的指针，需给定传输类型参数。
	@param[in] module 传输子模块选择
	@param[in] appProtocal 应用层协议类型
	@param[in] transMethod 传输方式 （已取消）
	@param[in] dataKeepType 传输数据的暂存方式（文件或BUFF）
	@return   传输对象的指针	NULL-失败 
	*/
	CNetTransfers* GetTransHandle(int module,int appProtocal,int transMethod,int dataKeepType);
	
	/*!
	@brief 释放一个传输对象实体。
	@param[in] 传输对象的指针
	@return    
	*/
	void ReleaseTransHandle(CNetTransfers *p);
	

	/*!
	@brief 设置默认的传输参数。
	*/
	void setTransMode(int module,int appProtocal,int transMethod,int dataKeepType);
	/*!
	@brief 使用netHeaderDef.h文件中的宏定义作为默认的传输参数
	*/
	void defTransMode();


	/*!
	@brief 设置默认的传输服务器IP、port。
	*/
	void setDefServerIP(const string ip, const string port);
	/*!
	@brief 设置默认的传输HTTP路径。
	*/
	void setDefServerFile(const string file);
	/*!
	@brief 设置默认的发送缓存。
	*/
	void SetDefSendBuff(UINT8 *pBuf, INT32 maxLen);
	/*!
	@brief 设置默认的接收缓存。
	*/
	void SetDefRevBuff(UINT8 *pBuf, INT32 maxLen);


	/*!
	@brief 上互斥锁
	*/
	void NetTransMutexLock();
	/*!
	@brief 解互斥锁
	*/
	void NetTransMutexUnlock();


	/*!
	@brief 获取本类唯一对象的实体
	*/
	static CNetManager* GetInstance(void);


private:

	/*!
	@brief 建立一个网络连接的对象实体
	@param[in] mode 连接方式
	@param[out] handle 输出的网络连接对象指针
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int buildConnectionInstance(int mode, CNetConnection **handle, string &strErr);


	/*!
	@brief 建立一个数据传输的对象实体
	@param[out] pNetTrans 输出的数据传输对象指针
	@param[in] transMode 传输方式的参数
	*/
	int buildTransInstance(CNetTransfers **pNetTrans, TransMode *transMode);


	
	CNetManager();
	~CNetManager();


};


extern CNetManager *g_netManager;


#endif

