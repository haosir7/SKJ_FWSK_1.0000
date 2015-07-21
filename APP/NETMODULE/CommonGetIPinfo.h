/**
 @file  CommonGetIPinfo.h
 @brief 本文件定义了获取IP信息（通用）的派生类。
 @author   zl
*/
#ifndef	__H_NET_COMMON_IP_INFO_
#define	__H_NET_COMMON_IP_INFO_


#include "netConnection.h"



/**
 @class CommonGetIPinfo 
 @brief 本类负责获取当前网络的IP信息（适用于多数网络模式）。
 */
class CommonGetIPinfo : public GetIPinfoACT
{
public:
	int getIPinfo(IPinfo *info, string &strErr);

	CommonGetIPinfo(CNetConnection *p):GetIPinfoACT(p){};
	virtual ~CommonGetIPinfo(){};	
};



#endif

