/**
 @file  M2M_Common.h
 @brief 本文件定义了与M2M模块有关的派生类，会继续派生WMMP或APN类。
 @author   zl
*/
#ifndef	__H_NET_M2M_COMMOM_
#define	__H_NET_M2M_COMMOM_


#include "command_at.h"
#include "netConnection.h"


#define M2M_ACT_ON		1
#define M2M_ACT_OFF		0


/**
 @class M2MCommon 
 @brief 本类实现了一些M2M的基本操作，会继续派生WMMP或APN类。
 */
class M2MCommon : public CNetConnection
{
public:
	virtual int initNetDevice(string &strErr)=0;

	int getCSQ(int *rssi, int *ber);

	/*!
	@brief 获取M2M模块信息。
	@param[out] info M2M信息结构体。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	int getModlueInfo(M2Minfo *info);
	
	/*!
	@brief 设置M2M模块激活状态。
	@param[in] state 1-激活；0-撤销激活
	@return NET_SUCCESS - 成功；其他 - 失败
	@note 本函数目前只设置数据库标志位，其他什么也不做。
		  实际的模块设置在initNetDevice函数执行。
	*/
	static int setM2MState(int state);

	M2MCommon();
	virtual ~M2MCommon();

};


#endif

