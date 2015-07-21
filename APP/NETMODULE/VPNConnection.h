/**
 @file  VPNConnection.h
 @brief 本文件定义了与VPN网络模式有关的几个派生类。
 @author   zl
*/

#ifndef	__H_NET_VPN_CONNECTION_
#define	__H_NET_VPN_CONNECTION_


#include "netConnection.h"



/**
 @class connectVPN 
 @brief 本类负责建立VPN网络连接。
 */
class connectVPN : public ConnectACT
{
public:
	int connect(string &strErr);

	connectVPN(CNetConnection *p):ConnectACT(p){};
	virtual ~connectVPN(){};
};

/**
 @class disconnectVPN 
 @brief 本类负责断开VPN网络。
 */
class disconnectVPN : public DisConnectACT
{
public:
	int disConnect();

	disconnectVPN(CNetConnection *p):DisConnectACT(p){};
	virtual ~disconnectVPN(){};
};



/**
 *@class VPNConnection 
 *@brief VPN网络模式的派生类。
 
	本类实现了VPN网络模式所需的各个操作。本类中组合了以下具体的操作对象：
	VPN连接与断开对象connectVPN、disconnectVPN；		
	通用接入状态查询对象CommonCheck；
	通用IP信息获取对象CommonGetIPinfo。
 */
class VPNConnection : public CNetConnection
{
public:
	/*!
	@brief 设置VPN服务器IP。	
	@param[in] ip VPN服务器IP
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int setVPNip(string ip, string &strErr);

	int setUserNamePwd(string name, string pwd, string &strErr);

	VPNConnection();
	~VPNConnection();
};


#endif

