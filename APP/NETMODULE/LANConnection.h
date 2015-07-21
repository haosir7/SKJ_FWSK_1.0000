/**
 @file  LANConnection.h
 @brief 本文件定义了与局域网模式有关的几个派生类。
 @author   zl
*/
#ifndef	__H_NET_LAN_CONNECTION_
#define	__H_NET_LAN_CONNECTION_


#include "netConnection.h"



/**
 @class LANCheck 
 @brief 本类负责查询网络接入状态（局域网专用）。
 @warning 手工设置IP的情况下，目前无法查询网络状态，直接返成功。
 */
class LANCheck : public CheckConnectionACT
{
public:
	int check();

	LANCheck(CNetConnection *p):CheckConnectionACT(p){};
	virtual ~LANCheck(){};
};



/**
 *@class LANConnection 
 *@brief 局域网模式的派生类。
 
	本类实现了局域网模式所需的各个操作。本类中组合了以下具体的操作对象：
	空的连接与断开对象connectNothing、disconnectNothing（即不做任何事）；		
	局域网接入状态查询对象LANCheck；
	通用IP信息获取对象CommonGetIPinfo。
 */
class LANConnection : public CNetConnection
{
public:
	INT32 m_dhcpMode;

	int initNetDevice(string &strErr);

	/*!
	@brief 设置DHCP模式。	
	@param[in] dhcpMode LAN_DHCP_ON或LAN_DHCP_OFF
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int setDhcpMode(INT32 dhcpMode, string &strErr);

	/*!
	@brief 手工设置当前IP状态。	
	@param[in] info IP信息结构体
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int setIPinfo(IPinfo *info, string &strErr);

	LANConnection();
	~LANConnection();

};

#endif

