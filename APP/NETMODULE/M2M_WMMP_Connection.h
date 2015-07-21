/**
 @file  M2M_WMMP_Connection.h
 @brief 本文件定义了与M2M-WMMP模式有关的几个派生类。
 @author   zl
*/
#ifndef	__H_NET_M2M_WMMP_CONNECTION_
#define	__H_NET_M2M_WMMP_CONNECTION_


#include "M2M_Common.h"


/**
 @class M2M_WMMP_Check 
 @brief 本类负责查询网络接入状态（WMMP模式专用）。
 @note 其实没有做任何实际的判断～
 */
class M2M_WMMP_Check : public CheckConnectionACT
{
public:
	int check();

	M2M_WMMP_Check(CNetConnection *p):CheckConnectionACT(p){};
	virtual ~M2M_WMMP_Check(){};
};



/**
 *@class M2M_WMMP_Connection 
 *@brief M2M_WMMP模式的派生类。
 
	本类实现了M2M_WMMP模式所需的各个操作。本类中组合了以下具体的操作对象：
	空的连接与断开对象connectNothing、disconnectNothing；		
	M2M_WMMP专用接入状态查询对象M2M_WMMP_Check；
	空的IP信息获取对象IPinfoDoNothing。
 */
class M2M_WMMP_Connection : public M2MCommon
{
public:
	int initNetDevice(string &strErr);

	M2M_WMMP_Connection();
	~M2M_WMMP_Connection();

};


#endif

