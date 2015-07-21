/**
 @file  M2M_Common.cpp
 @brief 本文件实现了M2M模块的一些基本操作，本类会继续派生WMMP或APN类。
 @author   zl
*/
#include "DoNothing.h"
#include "M2M_WMMP_Connection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


M2MCommon::M2MCommon()
{
}

M2MCommon::~M2MCommon()
{
}


int M2MCommon::getCSQ(int *rssi, int *ber)
{
	return at_getCSQ(rssi, ber);	
}

int M2MCommon::getModlueInfo(M2Minfo *info)
{
	int rc=0;

	rc = getM2MInfo(info);
	if (rc != 0)
	{
		DBG_PRINT(("rc = %d", rc))
		return NET_M2M_MODULE_ER;
	}
	
	return NET_SUCCESS;	
}

int M2MCommon::setM2MState(int state)
{
	int rc=0;
	INT32 curState=0;
	string strErr("");

	//只设置数据标志，其他什么也不做。
	//当网络模式切换为wmmp或apn时，会调用initNetDevice来设置模块的工作状态
	if (CNetDBWork::save_M2M_State(state, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}
	return NET_SUCCESS;	
}

