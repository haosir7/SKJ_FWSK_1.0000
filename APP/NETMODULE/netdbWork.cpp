/**
 @file  netdbWork.cpp
 @brief 本文件实现了网络模块中的数据库访问类CNetDBWork。
 */
#include "IncludeMe.h"
#include "CSysArg.h"
#include "netdbWork.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



CNetDBWork::CNetDBWork()
{
}

CNetDBWork::~CNetDBWork()
{
}


INT32 CNetDBWork::saveNetMode(INT32 mode, string &strErr)
{
	if (mode >= NET_VPN)
	{
		strErr = "给定的网络模式不正确";
		return NET_MODE_ER;
	}
	return saveSysArg(SYS_NET_MODE, mode, strErr);
}

INT32 CNetDBWork::loadNetMode(INT32 *mode, string &strErr)
{
	return loadSysArg(SYS_NET_MODE, mode, strErr);
}

INT32 CNetDBWork::saveNetModeL2(INT32 mode, string &strErr)
{
	if (mode<NET_VPN && mode>NET_NONE)
	{
		strErr = "给定的网络模式不正确";
		return NET_MODE_ER;
	}
	return saveSysArg(SYS_NET_MODE_L2, mode, strErr);
}

INT32 CNetDBWork::loadNetModeL2(INT32 *mode, string &strErr)
{
	return loadSysArg(SYS_NET_MODE_L2, mode, strErr);
}

INT32 CNetDBWork::saveAutoDialType(INT32 type, string &strErr)
{
	if (type<0 && type>2)
	{
		strErr = "给定类型不正确";
		return NET_OPERATOR_DB_ER;
	}
	return saveSysArg(SYS_AUTO_DIAL, type, strErr);
}

INT32 CNetDBWork::loadAutoDialType(INT32 *type, string &strErr)
{
	return loadSysArg(SYS_AUTO_DIAL, type, strErr);
}


INT32 CNetDBWork::saveDHCPMode(INT32 dhcpmode, string &strErr)
{
	return saveSysArg(SYS_LAN_DYNAMIC_IP, dhcpmode, strErr);
}

INT32 CNetDBWork::loadDHCPMode(INT32 *dhcpmode, string &strErr)
{
	return loadSysArg(SYS_LAN_DYNAMIC_IP, dhcpmode, strErr);
}

INT32 CNetDBWork::saveIPinfo(IPinfo *info, string &strErr)
{
	int ret=0;
	string ip(info->IP), mask(info->Mask), 
		gw(info->GeteWay), dns(info->DNS);

	if ((ret=saveSysArg(SYS_LAN_IP, ip, strErr)) != SQLITE_OK)
	{
		strErr += ":IP";
		return ret;
	}
	if ((ret=saveSysArg(SYS_LAN_MASK, mask, strErr)) != SQLITE_OK)
	{
		strErr += ":MASK";
		return ret;
	}
	if ((ret=saveSysArg(SYS_LAN_GATEWAY, gw, strErr)) != SQLITE_OK)
	{
		strErr += ":GW";
		return ret;
	}
	if ((ret=saveSysArg(SYS_LAN_DNS, dns, strErr)) != SQLITE_OK)
	{
		strErr += ":DNS";
		return ret;
	}

	return SQLITE_OK;
}

INT32 CNetDBWork::loadIPinfo(IPinfo *info, string &strErr)
{
	int ret=0;
	string ip(""), mask(""), gw(""), dns("");

	memset((void *)info, 0, sizeof(IPinfo));
	if ((ret=loadSysArg(SYS_LAN_IP, ip, strErr)) != SQLITE_OK)
	{
		strErr += ":IP";
		return ret;
	}
	if ((ret=loadSysArg(SYS_LAN_MASK, mask, strErr)) != SQLITE_OK)
	{
		strErr += ":MASK";
		return ret;
	}
	if ((ret=loadSysArg(SYS_LAN_GATEWAY, gw, strErr)) != SQLITE_OK)
	{
		strErr += ":GW";
		return ret;
	}
	if ((ret=loadSysArg(SYS_LAN_DNS, dns, strErr)) != SQLITE_OK)
	{
		strErr += ":DNS";
		return ret;
	}

	memcpy(info->IP, ip.c_str(), ip.length());
	memcpy(info->Mask, mask.c_str(), mask.length());
	memcpy(info->GeteWay, gw.c_str(), gw.length());
	memcpy(info->DNS, dns.c_str(), dns.length());

	return SQLITE_OK;
}

INT32 CNetDBWork::saveADSLUserNamePwd(string &name, string &pwd, string &strErr)
{
	return saveTwoSysArgFunc(SYS_ADSL_USER, name, SYS_ADSL_PWD, pwd, strErr);
}

INT32 CNetDBWork::loadADSLUserNamePwd(string &name, string &pwd, string &strErr)
{
	return loadTwoSysArgFunc(SYS_ADSL_USER, name, SYS_ADSL_PWD, pwd, strErr);
}

INT32 CNetDBWork::save3gUserNamePwd(string &name, string &pwd, string &strErr)
{
	return saveTwoSysArgFunc(SYS_3G_USER, name, SYS_3G_PWD, pwd, strErr);
}

INT32 CNetDBWork::load3gUserNamePwd(string &name, string &pwd, string &strErr)
{
	return loadTwoSysArgFunc(SYS_3G_USER, name, SYS_3G_PWD, pwd, strErr);
}

INT32 CNetDBWork::save3gCarrier(INT32 carrierType, string &strErr)
{
	return saveSysArg(SYS_3G_CARRIER, carrierType, strErr);
}

INT32 CNetDBWork::load3gCarrier(INT32 *carrierType, string &strErr)
{
	return loadSysArg(SYS_3G_CARRIER, carrierType, strErr);
}

INT32 CNetDBWork::save_M2M_State(INT32 actState, string &strErr)
{
	return saveSysArg(SYS_M2M_ACT_STATE, actState, strErr);
}

INT32 CNetDBWork::load_M2M_State(INT32 *actState, string &strErr)
{
	return loadSysArg(SYS_M2M_ACT_STATE, actState, strErr);
}

INT32 CNetDBWork::save_APN_Acc_Num(string &AccPoint, string &Number, string &strErr)
{
	return saveTwoSysArgFunc(SYS_M2M_APN_ACCPOINT, AccPoint, 
								SYS_M2M_APN_NUMBER, Number, strErr);
}

INT32 CNetDBWork::load_APN_Acc_Num(string &AccPoint, string &Number, string &strErr)
{
	return loadTwoSysArgFunc(SYS_M2M_APN_ACCPOINT, AccPoint, 
								SYS_M2M_APN_NUMBER, Number, strErr);
}

INT32 CNetDBWork::saveVPNUserNamePwd(string &name, string &pwd, string &strErr)
{
	return saveTwoSysArgFunc(SYS_VPN_USER, name, SYS_VPN_PWD, pwd, strErr);
}

INT32 CNetDBWork::loadVPNUserNamePwd(string &name, string &pwd, string &strErr)
{
	return loadTwoSysArgFunc(SYS_VPN_USER, name, SYS_VPN_PWD, pwd, strErr);
}

INT32 CNetDBWork::saveVPNServer(string &ip, string &strErr)
{
	return saveSysArg(SYS_VPN_SEVER_IP, ip, strErr);
}

INT32 CNetDBWork::loadVPNServer(string &ip, string &strErr)
{
	return loadSysArg(SYS_VPN_SEVER_IP, ip, strErr);
}

INT32 CNetDBWork::saveAPPServerIP(string &ip, string &port, string &strErr)
{
	return saveTwoSysArgFunc(SYS_APP_SERVER_IP, ip, SYS_APP_SERVER_PORT, port, strErr);
}

INT32 CNetDBWork::loadAPPServerIP(string &ip, string &port, string &strErr)
{
	return loadTwoSysArgFunc(SYS_APP_SERVER_IP, ip, SYS_APP_SERVER_PORT, port, strErr);
}

INT32 CNetDBWork::saveAPPServerFILE(string &file, string &strErr)
{
	return saveSysArg(SYS_APP_SERVER_FILE, file, strErr);
}

INT32 CNetDBWork::loadAPPServerFILE(string &file, string &strErr)
{
	return loadSysArg(SYS_APP_SERVER_FILE, file, strErr);
}

INT32 CNetDBWork::saveTIMEServerIP(string &ip, string &port, string &strErr)
{
	return saveTwoSysArgFunc(SYS_TIME_SERVER_IP, ip, SYS_TIME_SERVER_PORT, port, strErr);
}

INT32 CNetDBWork::loadTIMEServerIP(string &ip, string &port, string &strErr)
{
	return loadTwoSysArgFunc(SYS_TIME_SERVER_IP, ip, SYS_TIME_SERVER_PORT, port, strErr);
}

INT32 CNetDBWork::saveTimeServerFILE(string &file, string &strErr)
{
	return saveSysArg(SYS_TIME_SERVER_FILE, file, strErr);
}

INT32 CNetDBWork::loadTimeServerFILE(string &file, string &strErr)
{
	return loadSysArg(SYS_TIME_SERVER_FILE, file, strErr);
}




INT32 CNetDBWork::loadSysArg(INT32 id, INT32 *v_int, string &strErr)
{
	string text("");
	return loadproc(id, v_int, text, strErr);
}

INT32 CNetDBWork::loadSysArg(INT32 id, string &v_text, string &strErr)
{
	INT32 value=0;
	return loadproc(id, &value, v_text, strErr);
}
 
INT32 CNetDBWork::loadproc(INT32 id, INT32 *v_int, string &v_text, string &strErr)
{
	INT8 tmpChar[64];
	INT32 ret=0;
	CSysArg sysArg;
	CSysArg *pSysArg=&sysArg;

	sprintf(tmpChar,  "where SA_ID = %d", id);
	pSysArg->m_filter.assign(tmpChar);
	pSysArg->Requery();
	ret = pSysArg->LoadOneRecord();
	if (ret == SQLITE_OK)
	{
		*v_int = pSysArg->m_vInt;
		v_text = pSysArg->m_vText;
	}
	else
	{
		strErr = "查询参数错";
		sprintf(tmpChar, "(%d)", ret);
		strErr += tmpChar;
	}
	return ret;
}

INT32 CNetDBWork::saveSysArg(INT32 id, INT32 v_int, string &strErr)
{
	string text("");
	return saveproc(id, v_int, text, strErr);
}

INT32 CNetDBWork::saveSysArg(INT32 id, string &v_text, string &strErr)
{
	INT32 value=0;
	return saveproc(id, value, v_text, strErr);
}

INT32 CNetDBWork::saveproc(INT32 id, INT32 v_int, string &v_text, string &strErr)
{
	INT8 tmpChar[128];
	INT32 ret=0;
	CSysArg sysArg;
	CSysArg *pSysArg=&sysArg;

	sprintf(tmpChar,  "where SA_ID = %d", id);
	pSysArg->m_filter.assign(tmpChar);
	pSysArg->Requery();
	ret = pSysArg->LoadOneRecord();
	if (ret == SQLITE_DONE)	//表中尚无该ID的参数，调addnew()
	{
		pSysArg->m_said = id;
		pSysArg->m_name.assign("");
		pSysArg->m_vInt = v_int;
		pSysArg->m_vText.assign(v_text);
		memset(pSysArg->m_vBlob, 0, sizeof(pSysArg->m_vBlob));
		if ((ret=pSysArg->AddNew()) != SQLITE_OK)
		{
			strErr="添加参数错";
			sprintf(tmpChar, "(%d)", ret);
			strErr += tmpChar;
		}
	}
	else if (ret == SQLITE_OK)
	{
		sprintf(tmpChar, "update SYSARG set V_INT=%u, V_TEXT='%s' where SA_ID = %d",
							v_int, v_text.c_str(), id);
		pSysArg->SetSQL(tmpChar);
		ret = pSysArg->ExecuteSQL();
		if (ret != SQLITE_OK)
		{
			strErr="修改参数错";
			sprintf(tmpChar, "(%d)", ret);
			strErr += tmpChar;
		}
	}
	else
	{
		strErr="数据库错";
		sprintf(tmpChar, "(%d)", ret);
		strErr += tmpChar;
	}
	
	return ret;
}

INT32 CNetDBWork::saveTwoSysArgFunc(INT32 ID1, string &value1, INT32 ID2, 
									string &value2, string &strErr)
{
	int ret=0;
	if ((ret=saveSysArg(ID1, value1, strErr)) != SQLITE_OK)
	{
		strErr += ":参数1";
		return ret;
	}
	if ((ret=saveSysArg(ID2, value2, strErr)) != SQLITE_OK)
	{
		strErr += ":参数2";
		return ret;
	}
	return SQLITE_OK;
}

INT32 CNetDBWork::loadTwoSysArgFunc(INT32 ID1, string &value1, INT32 ID2, 
									string &value2, string &strErr)
{
	int ret=0;
	if ((ret=loadSysArg(ID1, value1, strErr)) != SQLITE_OK)
	{
		strErr += ":参数1";
		return ret;
	}
	if ((ret=loadSysArg(ID2, value2, strErr)) != SQLITE_OK)
	{
		strErr += ":参数2";
		return ret;
	}
	return SQLITE_OK;
}



