/**
 @file  M2M_APN_Connection.h
 @brief 本文件定义了与M2M-APN模式有关的几个派生类。
 @author   zl
*/
#ifndef	__H_NET_M2M_APN_CONNECTION_
#define	__H_NET_M2M_APN_CONNECTION_


#include "M2M_Common.h"


#define		APN_TAG		"0002"


/**
 @class connect_M2M_APN 
 @brief 本类负责建立M2M_APN网络连接。
 */
class connect_M2M_APN : public ConnectACT
{
public:
	int connect(string &strErr);

	connect_M2M_APN(CNetConnection *p):ConnectACT(p){};
	virtual ~connect_M2M_APN(){};
};

/**
 @class disconnect_M2M_APN 
 @brief 本类负责断开M2M_APN网络。
 */
class disconnect_M2M_APN : public DisConnectACT
{
public:
	int disConnect();

	disconnect_M2M_APN(CNetConnection *p):DisConnectACT(p){};
	virtual ~disconnect_M2M_APN(){};
};



/**
 *@class M2M_APN_Connection 
 *@brief M2M_APN模式的派生类。
 
	本类实现了M2M_APN模式所需的各个操作。本类中组合了以下具体的操作对象：
	3G连接与断开对象connect_M2M_APN、disconnect_M2M_APN；		
	通用接入状态查询对象CommonCheck；
	通用IP信息获取对象CommonGetIPinfo。
 */
class M2M_APN_Connection : public M2MCommon
{
public:
	int initNetDevice(string &strErr);
	
	/*!
	@brief 设置APN的接入点与号码。
	@param[in] Acc 接入点
	@param[in] Num 号码
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int setApn_Acc_Num(string &Acc, string &Num, string &strErr);

	M2M_APN_Connection();
	~M2M_APN_Connection();

private:
	/*!
	@brief 更新APN拨号脚本。
	*/
	void UpdateMobileChat(const INT8 *apnValue, const INT8 *apnNumber, UINT8 nameFlag);
	
};


#endif

