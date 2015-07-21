/**
 @file  ADSLConnection.cpp
 @brief 本文件实现了ADSL网络接入的几个派生类。
 @author   zl
*/
#include "adslProc.h"
#include "CommonCheck.h"
#include "netManager.h"
#include "ADSLConnection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



int connectADSL::connect(string &strErr)
{
	int ret=0;
	string usrName(""), usrPwd("");
	CNetConnection *p = m_pConnection;

	DBG_PRINT(("ConnectADSL..."));
	if (p->performCheck() == CHECK_OK)
	{
		DBG_PRINT(("ADSL已连接"))
		return NET_SUCCESS;
	}

	//取ADSL用户名和密码，放入usrName,usrPwd
	if (CNetDBWork::loadADSLUserNamePwd(usrName,usrPwd,strErr) != SQLITE_OK)
	{
		if (strErr=="")
			strErr = "取用户名和密码失败";
		return NET_OPERATOR_DB_ER;
	}

	//拨号过程
	if ((ret=adslConnectProc(usrName,usrPwd,strErr)) != NET_SUCCESS)
	{
		if (strErr=="")
			strErr = "ADSL拨号失败";
		return ret;
	}

	p->m_dialState = HAVE_DIALED;
	return NET_SUCCESS;
}


int disconnectADSL::disConnect()
{
	if (g_netManager->checkConnect() == CONNECT_ON_L2)
	{
		CNetConnection *pL2 = g_netManager->GetConnectionHandleL2();
		pL2->performDisConnect();
	}

	CNetConnection *p = (CNetConnection *)m_pConnection;
	if (p->performCheck() == CHECK_OK) 
	{
		adslDisConnectProc();
	}
	else
	{
		DBG_PRINT(("ADSL不在连接状态: %d", p->m_dialState));
	}
	
	p->m_dialState = NOT_DIALED;
	return NET_SUCCESS;
}

int GetIPinfoADSL::getIPinfo(IPinfo *info, string &strErr)
{
	DBG_ASSERT_EXIT((info != NULL), ("info == NULL!"));

	FILE *pppd_fd;
	struct ppp_info my_pppd_info;
	CNetConnection *p = (CNetConnection *)m_pConnection;
	
	if (p->performCheck() != CHECK_OK)
	{
		strErr = "网络尚未接通,无IP信息";
		return NET_CONNECT_FAIL;
	}

	pppd_fd = fopen(CONFIG_FILE_PPPOE, "r");
	if(pppd_fd == NULL)
	{
		strErr = "打开ADSL配置文件错误";
		return NET_OPERATOR_FILE_ER;
	}
	
	memset((char *)(&my_pppd_info), 0, sizeof(struct ppp_info));
	fread((char*)(&my_pppd_info), sizeof(struct ppp_info), 1, pppd_fd);
	fclose(pppd_fd);

	memset(info, 0, sizeof(IPinfo));
	memcpy (info->IP, my_pppd_info.loc_addr, strlen(my_pppd_info.loc_addr));
	memcpy (info->Mask, my_pppd_info.netmask, strlen(my_pppd_info.netmask));
	memcpy (info->GeteWay, my_pppd_info.gateway, strlen(my_pppd_info.gateway));
	memcpy (info->DNS, my_pppd_info.dnsaddr[0], strlen(my_pppd_info.dnsaddr[0]));

	return NET_SUCCESS;
}


ADSLConnection::ADSLConnection()
{
	m_connectionMode = NET_ADSL;
	m_dialState = NOT_DIALED;
	
	m_pConnect = new connectADSL(this);
	m_pDisConnect = new disconnectADSL(this);
	m_pCheck = new CommonCheck(this);
	m_pGetIPinfo = new GetIPinfoADSL(this);
}

ADSLConnection::~ADSLConnection()
{
}

int ADSLConnection::setUserNamePwd(string name, string pwd, string &strErr)
{
	if (performCheck() != CHECK_NOT)
	{
		strErr = "请先断开ADSL连接";
		return NET_CONDITION_ER;
	}

	if (CNetDBWork::saveADSLUserNamePwd(name, pwd, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}
	return NET_SUCCESS;
}

