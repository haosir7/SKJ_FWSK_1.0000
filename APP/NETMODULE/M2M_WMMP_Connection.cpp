/**
 @file  M2M_WMMP_Connection.cpp
 @brief 本文件实现了与M2M-WMMP模式有关的几个派生类。
 @author   zl
*/
#include "drv_modem.h"
#include "wmmp.h"
#include "DoNothing.h"
#include "M2M_WMMP_Connection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


int M2M_WMMP_Check::check()
{
	INT32 m2m_state=0;
	string strErr("");

	CNetDBWork::load_M2M_State(&m2m_state, strErr);
	DBG_PRINT(("m2m_state = %d", m2m_state));
	if(0 == m2m_state)
	{
		return CHECK_NOT;
	}
	
	return CHECK_OK;
}

M2M_WMMP_Connection::M2M_WMMP_Connection()
{
	m_connectionMode = NET_M2M_WMMP;
	m_dialState = HAVE_DIALED;	//此模式下直接处于已拨号的状态

	m_pConnect = new connectNothing(this);
	m_pDisConnect = new disconnectNothing(this);
	m_pCheck = new M2M_WMMP_Check(this);
	m_pGetIPinfo = new IPinfoDoNothing(this);
}

M2M_WMMP_Connection::~M2M_WMMP_Connection()
{
}

int M2M_WMMP_Connection::initNetDevice(string &strErr)
{
	int rc=0;
	INT32 actState=0;
	DBG_PRINT(("enter wmmp initNetDevice"));

	//判断M2M激活选项
	if (CNetDBWork::load_M2M_State(&actState, strErr) != SQLITE_OK)
	{
		actState = 0;
	}
	DBG_PRINT(("actState = %d", actState));

	if (actState == M2M_ACT_ON)
	{
		modem_init();
		rc = wmmp_init();
		if( rc != 0 )
		{
			DBG_PRINT(("wmmp init fail. rc = %d", rc));
			strErr = "wmmp协议初始化失败";
			return NET_INIT_FAIL;
		}
	}
	DBG_PRINT(("wmmp init successed"));
	
	return NET_SUCCESS;		
}

