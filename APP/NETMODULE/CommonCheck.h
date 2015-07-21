/**
 @file  CommonCheck.h
 @brief 本文件定义了网络接入状态查询（通用）的派生类。
 @author   zl
*/
#ifndef	__H_NET_COMMON_CHECK_
#define	__H_NET_COMMON_CHECK_

#include "netConnection.h"


/**
 @class CommonCheck 
 @brief 本类负责查询网络接入状态（适用于多数网络模式）。
 */
class CommonCheck : public CheckConnectionACT
{
public:
	int check();

	CommonCheck(CNetConnection *p):CheckConnectionACT(p){};
	virtual ~CommonCheck(){};	

private:

};



#endif

