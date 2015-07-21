/**
 @file  DoNothing.h
 @brief 本文件定义了与非任何网络模式的派生类（提高代码兼容性）。
 @author   zl
*/
#ifndef	__H_NET_DO_NOTHING_
#define	__H_NET_DO_NOTHING_


#include "netConnection.h"


/**
 @class connectNothing 
 @brief 空的建立连接操作
 */
class connectNothing : public ConnectACT
{
public:
	int connect(string &strErr);

	connectNothing(CNetConnection *p):ConnectACT(p){};
	virtual ~connectNothing(){};
};

/**
 @class disconnectNothing 
 @brief 空的断开连接操作
 */
class disconnectNothing : public DisConnectACT
{
public:
	int disConnect();

	disconnectNothing(CNetConnection *p):DisConnectACT(p){};
	virtual ~disconnectNothing(){};
};

/**
 @class checkNothing 
 @brief 空的检查连接状态操作
 */
class checkNothing : public CheckConnectionACT
{
public:
	int check();

	checkNothing(CNetConnection *p):CheckConnectionACT(p){};
	virtual ~checkNothing(){};
};

/**
 @class IPinfoDoNothing 
 @brief 空的获取IP操作
 */
class IPinfoDoNothing : public GetIPinfoACT
{
public:
	int getIPinfo(IPinfo *info, string &strErr);

	IPinfoDoNothing(CNetConnection *p):GetIPinfoACT(p){};
	virtual ~IPinfoDoNothing(){};
};


/**
 @class NoneConnection 
 @brief 空的网络模式（提高代码兼容性）
 */
class NoneConnection : public CNetConnection
{
public:
	NoneConnection();
	~NoneConnection();
};

#endif

