/**
 @file  ADSLConnection.h
 @brief 本文件定义了ADSL网络模式有关的几个派生类。
 @author   zl
*/

#ifndef	__H_NET_ADSL_CONNECTION_
#define	__H_NET_ADSL_CONNECTION_

#include "netConnection.h"



/**
 @class connectADSL 
 @brief 本类负责建立ADSL网络连接。
 */
class connectADSL : public ConnectACT
{
public:
	int connect(string &strErr);

	connectADSL(CNetConnection *p):ConnectACT(p){};
	virtual ~connectADSL(){};
};

/**
 @class disconnectADSL 
 @brief 本类负责断开ADSL网络。
 */
class disconnectADSL : public DisConnectACT
{
public:
	int disConnect();

	disconnectADSL(CNetConnection *p):DisConnectACT(p){};
	virtual ~disconnectADSL(){};
};

/**
 @class GetIPinfoADSL 
 @brief 本类负责获取ADSL连接下的IP信息。
 */
class GetIPinfoADSL : public GetIPinfoACT
{
public:
	int getIPinfo(IPinfo *info, string &strErr);

	GetIPinfoADSL(CNetConnection *p):GetIPinfoACT(p){};
	virtual ~GetIPinfoADSL(){};
};




/**
 *@class ADSLConnection 
 *@brief ADSL网络模式的派生类。
 
	本类实现了ADSL网络模式所需的各个操作。本类中组合了以下具体的操作对象：
	ADSL连接与断开对象connectADSL、disconnectADSL；		
	通用接入状态查询对象CommonCheck；
	ADSL专用IP信息获取对象GetIPinfoADSL。
 */
class ADSLConnection : public CNetConnection
{
public:
	ADSLConnection();
	~ADSLConnection();
	
	int setUserNamePwd(string name, string pwd, string &strErr);

};



#endif

