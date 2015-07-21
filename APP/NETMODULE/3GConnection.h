/**
 @file  3GConnection.h
 @brief 本文件定义了与3G网络模式有关的几个派生类。
 @author   zl
*/

#ifndef	__H_NET_3G_CONNECTION_
#define	__H_NET_3G_CONNECTION_


#include "netConnection.h"


/** @brief 枚举，定义运营商类型 */
enum CARRIER_TYPE
{
	CARRIER_NONE = 0,
	CARRIER_CT,			/**< 电信 */
	CARRIER_CU,			/**< 联通 */
};


#define	SCRIPT_FILE_CT		"/etc/ppp/peers/evdo"
#define	SCRIPT_FILE_CU		"/etc/ppp/peers/unicom"

#define	SECRETS_FILE		"/etc/ppp/chap-secrets"


#define	DEF_USER_NAME_CT		"card"
#define	DEF_USER_PWD_CT			"card"

#define	DEF_USER_NAME_CU		"3gnet"
#define	DEF_USER_PWD_CU			"GSM"

#define _AT_BAUDRATE_3G			B115200 


/**
 @class connect3G 
 @brief 本类负责建立3G网络连接。
 */
class connect3G : public ConnectACT
{
public:
	int connect(string &strErr);

	connect3G(CNetConnection *p):ConnectACT(p){};
	virtual ~connect3G(){};
};

/**
 @class disconnect3G 
 @brief 本类负责断开3G网络。
 */
class disconnect3G : public DisConnectACT
{
public:
	int disConnect();

	disconnect3G(CNetConnection *p):DisConnectACT(p){};
	virtual ~disconnect3G(){};
};





/**
 *@class _3GConnection 
 *@brief 3G网络模式的派生类。
 
	本类实现了3G网络模式所需的各个操作。本类中组合了以下具体的操作对象：
	3G连接与断开对象connect3G、disconnect3G；		
	通用接入状态查询对象CommonCheck；
	通用IP信息获取对象CommonGetIPinfo。
 */
class _3GConnection : public CNetConnection
{
private:
	char *m_pATBuff;				/**< AT命令缓存指针 */

public:
	int setUserNamePwd(string name, string pwd, string &strErr);
	int getCSQ(int *rssi, int *ber);

	/*!
	@brief 设置3G运营商。	
	@param[in] type 运营商类型
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int set3Gcarrier(int type, string &strErr);
	


	_3GConnection();
	~_3GConnection();

private:
	/*!
	@brief 修改3G拨号脚本函数。
	@param[in] name 用户名。
	@param[in] pwd 密码
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int set3GScirptFunc(string &name, string &pwd, string &strErr);

	/*!
	@brief 获取默认的3G用户名与密码。
	@param[out] name 用户名。
	@param[out] pwd 密码
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	void getDefUserNamePwd(string &name, string &pwd);

	/*!
	@brief 开启3G端口文件。
	@return 3G端口文件操作符
	@note 会依次尝试打开每个可能的设备文件符。
	*/
	int open_at_port();

};



#endif

