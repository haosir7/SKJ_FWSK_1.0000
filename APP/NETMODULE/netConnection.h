/**
 @file  netConnection.h
 @brief 本文件定义了与网络接入相关的几个基类。

	本文件定义了4个用于网络基本操作的基类：			
	ConnectACT、DisConnectACT、CheckConnectionACT、GetIPinfoACT。
	和一个网络模式的基类：
	CNetConnection。

 @author   zl

*/

#ifndef	__H_NET_CONNECTION_
#define	__H_NET_CONNECTION_


#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "netdbWork.h"
#ifndef _DEBUG_VERSION
#include "powoff_protect.h"
#endif

class CNetConnection;

/**
 @class ConnectACT 
 @brief 建立网络连接的基类。
 
	定义了建立网络连接功能的统一接口，派生出不同方式的建立网络连接类。
 */
class ConnectACT
{
protected:
	CNetConnection *m_pConnection;				/**< 指向组合了本对象的CNetConnection对象 */

public:
	/*!
	@brief 建立连接。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int connect(string &strErr)=0;
	
	/*!
	@brief 获取当前网络模式。	
	@return 网络模式值。在枚举类型ENUM_NET_MODE中定义。
	*/
	int curNetMode();

	ConnectACT(CNetConnection *p):m_pConnection(p){};
	virtual ~ConnectACT() {};
};

/**
 @class DisConnectACT 
 @brief 断开网络连接的基类。
 
   定义了断开网络连接功能的统一接口，派生出不同方式的断开网络连接类。
 */
class DisConnectACT
{
protected:
	CNetConnection *m_pConnection;				/**< 指向组合了本对象的CNetConnection对象 */

public:
	/*!
	@brief 断开连接。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int disConnect()=0;
	
	/*!
	@brief 获取当前网络模式。	
	@return 网络模式值。在枚举类型ENUM_NET_MODE中定义。
	*/
	int curNetMode();
	
	DisConnectACT(CNetConnection *p):m_pConnection(p){};
	virtual ~DisConnectACT() {};
};

/**
 @class CheckConnectionACT 
 @brief 检查网络接入状态的基类。
 
   定义了该功能的统一接口，派生出不同方式的状态查询操作类。
 */
class CheckConnectionACT
{
protected:
	CNetConnection *m_pConnection;			/**< 指向组合了本对象的CNetConnection对象 */

public:
	/*!
	@brief 网络接入状态查询。	
	@return CHECK_OK - 已连接；CHECK_NOT - 未建立连接；CHECK_OFF - 连接后断线
	*/
	virtual int check()=0;
	
	/*!
	@brief 获取当前网络模式。	
	@return 网络模式值。在枚举类型ENUM_NET_MODE中定义。
	*/
	int curNetMode();

	CheckConnectionACT(CNetConnection *p):m_pConnection(p){};
	virtual ~CheckConnectionACT() {};
};

/**
 @class GetIPinfoACT 
 @brief 获取IP信息的基类。
 
   定义了该功能的统一接口，派生出不同方式的获取IP信息操作类。
 */
class GetIPinfoACT
{
protected:
	CNetConnection *m_pConnection;			/**< 指向组合了本对象的CNetConnection对象 */

public:
	/*!
	@brief 获取IP信息。	
	@param[out] info IP信息结构体
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int getIPinfo(IPinfo *info, string &strErr)=0;
	
	/*!
	@brief 获取当前网络模式。	
	@return 网络模式值。在枚举类型ENUM_NET_MODE中定义。
	*/
	int curNetMode();

	GetIPinfoACT(CNetConnection *p):m_pConnection(p){};
	virtual ~GetIPinfoACT() {};	
};




/**
 *@class CNetConnection 
 *@brief 各种网络模式的基类。
 
   本类中包含了4种网络基本操作的基类指针，用于组合具体的派生类。
   本类为上层代码定义了网络操作接口。
   本类的派生类为具体的网络接入方式类（如3G、ADSL等）。
 */
class CNetConnection
{
public:
	int m_connectionMode;				/**< 网络连接模式 */
	int m_dialState;					/**< 是否已执行建立连接。HAVE_DIALED或NOT_DIALED */

protected:
	ConnectACT *m_pConnect;				/**< 指向具体建立连接对象的指针 */
	DisConnectACT *m_pDisConnect;		/**< 指向具体断开连接对象的指针 */
	CheckConnectionACT *m_pCheck;		/**< 指向具体状态查询对象的指针 */
	GetIPinfoACT *m_pGetIPinfo;			/**< 指向具体获取IP信息对象的指针 */

	
public:
	/*!
	@brief 执行建立网络连接操作，此函数是对业务层的接口。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int performConnect(string &strErr);

	/*!
	@brief 执行断开网络操作，此函数是对业务层的接口。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int performDisConnect();

	/*!
	@brief 执行接入状态查询操作，此函数是对业务层的接口。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int performCheck();

	/*!
	@brief 执行获取IP信息操作，此函数是对业务层的接口。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int performGetIPinfo(IPinfo *info, string &strErr);

	/*!
	@brief 网络设备初始化。	
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int initNetDevice(string &strErr);

	/*!
	@brief 设置用户名与密码。	
	@param[in] name 用户名
	@param[in] pwd 密码
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int setUserNamePwd(string name, string pwd, string &strErr);	

	/*!
	@brief 获取无线信号强度。	
	@param[out] rssi 信号强度值
	@param[out] ber 信号有效性
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	virtual int getCSQ(int *rssi, int *ber);

	/*!
	@brief 获取当前网络模式。	
	@return 网络模式值。在枚举类型ENUM_NET_MODE中定义。
	*/
	int curNetMode(){return m_connectionMode;};

	CNetConnection();
	virtual ~CNetConnection();


};

#endif

