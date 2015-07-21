/**
 @file  netManager.cpp
 @brief 本文件实现了网络模块中的网络管理类CNetManager。
 */
#if (_TRANS_THREAD_SUPPORT!=0)
#include <pthread.h>
#endif

#include "commonFunc.h"
#include "DoNothing.h"
#include "LANConnection.h"
#include "ADSLConnection.h"
#include "3GConnection.h"
#include "M2M_WMMP_Connection.h"
#include "M2M_APN_Connection.h"
#include "VPNConnection.h"

#include "SendBySocket.h"
#include "trans_direct.h"
#include "trans_http.h"
#include "SendByWmmp.h"
#include "trans_CURL.h"

#include "netManager.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"




CNetManager *CNetManager::m_pNetManager = NULL;
CNetManager *g_netManager = NULL;

unsigned char g_netSendBuf[DEF_SENDBUFF_LEN];
unsigned char g_netRevBuf[DEF_RECVBUFF_LEN];

#ifndef WIN32
static pthread_mutex_t	s_net_mutex = PTHREAD_MUTEX_INITIALIZER;		//网络线程的互斥不用考虑同一线程重复上锁而死锁的情况
#endif



CNetManager::CNetManager()
{
	m_pConnectionL1 = NULL;
	m_pConnectionL2 = NULL;
	
	m_defServerIP.assign("");
	m_defServerPort.assign("");
	m_defServerFile.assign("");
	
	m_pDefSendBuff = NULL;
	m_defSendBuffLen = 0;
	m_pDefRevBuff = NULL;
	m_defRevBuffLen = 0;

	defTransMode();
}

CNetManager::~CNetManager()
{
	if (m_pConnectionL1 != NULL)
	{
		delete m_pConnectionL1;
		m_pConnectionL1 = NULL;
	}
	if (m_pConnectionL2 != NULL)
	{
		delete m_pConnectionL2;
		m_pConnectionL2 = NULL;
	}
	if (m_pNetManager != NULL)
	{
		delete m_pNetManager;
		m_pNetManager = NULL;
	}
	
}


int CNetManager::SetConnectionMode(int mode, CNetConnection **handle, string &strErr)
{
	int ret=0;
	int connectState=0;

	if (mode >= NET_VPN)	
	{
		*handle = m_pConnectionL1;
		strErr = "给定的网络模式不正确";
		return NET_MODE_ER;
	}

	//若当前有未断开的连接，禁止切换
	connectState = checkConnect();
	if (connectState == CONNECT_ON_L2)	//VPN连接未断
	{
		*handle = m_pConnectionL1;
		strErr = "请先断开VPN连接";
		return NET_INIT_FAIL;		
	}
	else if (connectState == CONNECT_ON_L1)	//第一层连接未断
	{
		//LAN与WMMP没有建立与断开的过程，可跳过
		if (GetConnectionMode()!=NET_LAN && GetConnectionMode()!=NET_M2M_WMMP)	
		{
			*handle = m_pConnectionL1;
			strErr = "当前连接未断开,无法切换模式";
			return NET_INIT_FAIL;		
		}
	}

	//重新建立实体
	if (m_pConnectionL1 != NULL)
	{
		delete m_pConnectionL1;
		m_pConnectionL1 = NULL;
	}
	
	ret = buildConnectionInstance(mode, &m_pConnectionL1, strErr);
	*handle = m_pConnectionL1;
	if (ret != NET_SUCCESS)
	{
		return ret;
	}

	//第一层网络需调用initNetDevice()
	m_pConnectionL1->initNetDevice(strErr);

	return NET_SUCCESS;
}

int CNetManager::SetConnectionModeL2(int mode, CNetConnection **handle, string &strErr)
{
	int ret=0;
	
	if (mode<NET_VPN && mode>NET_NONE)
	{
		*handle = m_pConnectionL2;
		strErr = "给定的网络模式不正确";
		return NET_MODE_ER;
	}

	if (m_pConnectionL2 != NULL)
	{
		delete m_pConnectionL2;
		m_pConnectionL2 = NULL;
	}

	ret = buildConnectionInstance(mode, &m_pConnectionL2, strErr);
	*handle = m_pConnectionL2;
	if (ret != NET_SUCCESS)
	{
		return ret;
	}

	return NET_SUCCESS;
}

int CNetManager::GetConnectionMode()
{
	if (m_pConnectionL1 != NULL)
		return m_pConnectionL1->m_connectionMode;
	else
		return NET_NONE;
}

int CNetManager::GetConnectionModeL2()
{
	if (m_pConnectionL2 != NULL)
		return m_pConnectionL2->m_connectionMode;
	else
		return NET_NONE;
}

CNetConnection* CNetManager::GetConnectionHandle()
{
	return m_pConnectionL1;
}

CNetConnection* CNetManager::GetConnectionHandleL2()
{
	return m_pConnectionL2;
}

int CNetManager::checkConnect()
{
	CNetConnection *pL1=GetConnectionHandle();
	CNetConnection *pL2=GetConnectionHandleL2();

	//如果PL2为null（说明没有实体），则认为第二层未连接
	if (pL2 != NULL)
	{
		if ( pL2->performCheck() == CHECK_OK )
		{
			return CONNECT_ON_L2;
		}
	}
	//第一层类同
	if (pL1 != NULL)
	{
		if ( pL1->performCheck() == CHECK_OK )
		{
			return CONNECT_ON_L1;
		}
	}

	return CONNECT_OFF;
}

int CNetManager::getIPinfo(IPinfo *info, string &strErr)
{
	int ret = checkConnect();
	if (ret == CONNECT_ON_L2)
	{
		CNetConnection *p=GetConnectionHandleL2();
		return p->performGetIPinfo(info, strErr);
	}
	else if (ret == CONNECT_ON_L1)
	{
		CNetConnection *p=GetConnectionHandle();
		return p->performGetIPinfo(info, strErr);
	}
	else
	{
		strErr = "网络尚未接通,无IP信息";
		return NET_CONNECT_FAIL;
	}
}

int CNetManager::buildConnectionInstance(int mode, CNetConnection **handle, string &strErr)
{
	CNetConnection *p=NULL;

	switch (mode)
	{
	case NET_LAN:
#if (_CONFIG_LAN_MODULE != 0)
		p = new LANConnection();
#endif
		break;

	case NET_ADSL:
#if (_CONFIG_ADSL_MODULE != 0)
		p = new ADSLConnection();
#endif
		break;

	case NET_3G:
#if (_CONFIG_3G_MODULE != 0)
		p = new _3GConnection();
#endif
		break;

	case NET_M2M_WMMP:
#if (_CONFIG_M2M_MODULE != 0)
		p = new M2M_WMMP_Connection();
#endif
		break;

	case NET_M2M_APN:
#if (_CONFIG_M2M_MODULE != 0)
		p = new M2M_APN_Connection();
#endif
		break;

	case NET_VPN:
#if (_CONFIG_VPN_MODULE != 0)
		p = new VPNConnection();
#endif
		break;

	case NET_NONE:
	default:
		p = new NoneConnection();
		break;
	}

	if (p == NULL)
	{
		p = new NoneConnection();
		*handle = p;
		strErr = "当前版本不支持此模式";
		return NET_NOT_SUPPORT_ER;
	}

	*handle = p;
	return NET_SUCCESS;
}


CNetManager* CNetManager::GetInstance()
{
	if (m_pNetManager == NULL)
	{
		m_pNetManager = new CNetManager();
	}
	return m_pNetManager;
}

int CNetManager::init(string &strErr)
{
	INT32 ret=0;
	INT32 netMode=NET_NONE;
	INT32 netModeL2=NET_NONE;
	CNetConnection *handle=NULL;

	//设置当前网络模式
	if (CNetDBWork::loadNetMode(&netMode, strErr) != SQLITE_OK)
	{
		DBG_PRINT(("未设置网络模式, strErr=%s", strErr.c_str()))
		netMode=NET_NONE;
	}
	DBG_PRINT(("netMode=%d", netMode))
	if ((ret=SetConnectionMode(netMode, &handle, strErr)) != NET_SUCCESS)
	{
		return ret;
	}
	
	//第二层只支持VPN模式。
	//只要_CONFIG_VPN_MODULE宏开启，则强制设为VPN，否则强制设为NONE。by zl 20131015
	if (CNetDBWork::loadNetModeL2(&netModeL2, strErr) != SQLITE_OK)
	{
		DBG_PRINT(("未设置二次拨号网络模式, strErr=%s", strErr.c_str()))
		netModeL2=NET_NONE;
	}
#if (_CONFIG_VPN_MODULE != 0)	
	if (netModeL2 != NET_VPN)		
	{
		CNetDBWork::saveNetModeL2(NET_VPN, strErr);
		netModeL2=NET_VPN;
	}
#else
	if (netModeL2 != NET_NONE)		
	{
		CNetDBWork::saveNetModeL2(NET_NONE, strErr);
		netModeL2=NET_NONE;
	}
#endif
	DBG_PRINT(("netModeL2=%d", netModeL2))
	if ((ret=SetConnectionModeL2(netModeL2, &handle, strErr)) != NET_SUCCESS)
	{
		return ret;
	}

	//获取默认服务器IP
	CNetDBWork::loadAPPServerIP(m_defServerIP, m_defServerPort, strErr);
	if (m_defServerFile == "")
		m_defServerFile.assign(SERVER_FILE_DEF);

	//开机自动拨号过程
	if ((ret=autoDialProc(strErr)) != NET_SUCCESS)
	{
		return ret;
	}

	return NET_SUCCESS;
}

int CNetManager::autoDialProc(string &strErr)
{
	int ret=0;
	INT32 autoDialType=0;
	CNetConnection *pnet=NULL;

	CNetDBWork::loadAutoDialType(&autoDialType, strErr);
	DBG_PRINT(("autoDialType = %d", autoDialType));
	
	if (autoDialType <= 1)
	{
		pnet = g_netManager->GetConnectionHandle();
		ret = pnet->performConnect(strErr);
		if (ret != NET_SUCCESS)
		{
			return ret;
		}
	}
	
	if (autoDialType == 0)
	{
		CommonSleep(1500);
		pnet = g_netManager->GetConnectionHandleL2();
		ret = pnet->performConnect(strErr);
		if (ret != NET_SUCCESS)
		{
			return ret;
		}
	}

	return NET_SUCCESS;
}

CNetTransfers* CNetManager::GetTransHandle()
{
	CNetTransfers *pNetTrans = NULL;
	string strErr("");

	buildTransInstance(&pNetTrans, &m_transMode);

	if (m_pDefSendBuff!=NULL && m_defSendBuffLen!=0)
		pNetTrans->SetSendBuff(m_pDefSendBuff, m_defSendBuffLen);
	else
		pNetTrans->SetSendBuff(g_netSendBuf, sizeof(g_netSendBuf));
	
#if (SINGLE_TRANSBUFF_MODE == 0)
	if (m_pDefRevBuff != NULL && m_defRevBuffLen!=0)
		pNetTrans->SetRevBuff(m_pDefRevBuff, m_defRevBuffLen);
	else
		pNetTrans->SetRevBuff(g_netRevBuf, sizeof(g_netRevBuf));
#else
	if (m_pDefSendBuff!=NULL && m_defSendBuffLen!=0)
		pNetTrans->SetRevBuff(m_pDefSendBuff, m_defSendBuffLen);
	else
		pNetTrans->SetRevBuff(g_netSendBuf, sizeof(g_netSendBuf));
#endif

	pNetTrans->SetServerIP(m_defServerIP, m_defServerPort);
	pNetTrans->SetServerFile(m_defServerFile);

	return pNetTrans;
}

CNetTransfers* CNetManager::GetTransHandle(int module,int appProtocal,int transMethod,int dataKeepType)
{
	TransMode transMode;
	CNetTransfers *pNetTrans = NULL;
	string strErr("");

	transMode.module = module;
	transMode.appProtocal = appProtocal;
	transMode.dataKeepType = dataKeepType;
	buildTransInstance(&pNetTrans, &transMode);

	if (m_pDefSendBuff!=NULL && m_defSendBuffLen!=0)
		pNetTrans->SetSendBuff(m_pDefSendBuff, m_defSendBuffLen);
	else
		pNetTrans->SetSendBuff(g_netSendBuf, sizeof(g_netSendBuf));
	
#if (SINGLE_TRANSBUFF_MODE == 0)
	if (m_pDefRevBuff != NULL && m_defRevBuffLen!=0)
		pNetTrans->SetRevBuff(m_pDefRevBuff, m_defRevBuffLen);
	else
		pNetTrans->SetRevBuff(g_netRevBuf, sizeof(g_netRevBuf));
#else
	if (m_pDefSendBuff!=NULL && m_defSendBuffLen!=0)
		pNetTrans->SetRevBuff(m_pDefSendBuff, m_defSendBuffLen);
	else
		pNetTrans->SetRevBuff(g_netSendBuf, sizeof(g_netSendBuf));
#endif

	pNetTrans->SetSendBuff(g_netSendBuf, sizeof(g_netSendBuf));
	pNetTrans->SetRevBuff(g_netRevBuf, sizeof(g_netRevBuf));

	pNetTrans->SetServerIP(m_defServerIP, m_defServerPort);
	pNetTrans->SetServerFile(m_defServerFile);

	return pNetTrans;
}

void CNetManager::ReleaseTransHandle(CNetTransfers *p)
{
	if (p != NULL)
		delete p;
}

int CNetManager::buildTransInstance(CNetTransfers **pNetTrans, TransMode *transMode)
{
	CNetTransfers *pT=NULL;
	Sending *pS=NULL;
	int curl_flag=0;

	if (GetConnectionMode() == NET_M2M_WMMP)
	{
		DBG_ASSERT_EXIT(transMode->module!=TRANS_BY_CURL, ("CURL doesnot suport WMMP"));
		curl_flag = 0;
		pS = new SendByWmmp();
	}
	else
	{
		if (transMode->module == TRANS_BY_CURL)
		{
			curl_flag = 1;
		}
		else if (transMode->module == TRANS_BY_TRANSPROC)
		{
			pS = new SendBySocket();
		}
	}
	
	if (curl_flag == 0)
	{
		DBG_ASSERT_EXIT(pS!=NULL, ("pSending is NULL!!"));
		if (transMode->appProtocal == APP_LAYER_HTTP)
		{
			pT = new trans_http(pS);
		}
		else 
		{
			pT = new trans_direct(pS);
		}
	}
	else
	{
#if (_TRANS_MODULE_CURL != 0)
		pT = new trans_CURL();
#else
		DBG_ASSERT_EXIT(0, ("CURL not compile"));		
#endif
	}

	pT->SetDataKeepType(transMode->dataKeepType);

	*pNetTrans = pT;
	DBG_PRINT(("pT: %x", pT))

	return NET_SUCCESS;
	
}

void CNetManager::defTransMode()
{
	m_transMode.module = TRANS_MODULE_DEF;
	m_transMode.appProtocal = TRANS_APP_LAYER_DEF;
	m_transMode.dataKeepType = TRANS_DATA_TYPE_DEF;
}

void CNetManager::setTransMode(int module,int appProtocal,int transMethod,int dataKeepType)
{
	m_transMode.module = module;
	m_transMode.appProtocal = appProtocal;
	m_transMode.dataKeepType = dataKeepType;
}

void CNetManager::setDefServerIP(const string ip, const string port)
{
	string strErr("");
	m_defServerIP.assign(ip);
	m_defServerPort.assign(port);
	CNetDBWork::saveAPPServerIP(m_defServerIP, m_defServerPort, strErr);
}

void CNetManager::setDefServerFile(const string file)
{
	m_defServerFile.assign(file);
}

void CNetManager::SetDefSendBuff(UINT8 *pBuf, INT32 maxLen)
{
	m_pDefSendBuff = pBuf;
	m_defSendBuffLen = maxLen;
}

void CNetManager::SetDefRevBuff(UINT8 *pBuf, INT32 maxLen)
{
	m_pDefRevBuff = pBuf;
	m_defRevBuffLen = maxLen;
}



void CNetManager::NetTransMutexLock()
{
#if (_TRANS_THREAD_SUPPORT!=0)
	DBG_PRINT(("^^^^^^^^^^^Net Mutex lock^^^^^^^^"));
	while (pthread_mutex_trylock(&s_net_mutex) == EBUSY)
	{
		CommonSleep(800);
	}

/*	int ret = 0;	
	ret = pthread_mutex_lock(&s_net_mutex);
	DBG_PRINT(("^^^^^^^^^^^Net Mutex lock^^^^^^^^"));
	if (ret != 0)
	{
		DBG_PRINT(("===NET MUTEX LOCK %d====", ret));
	}
*/
#endif
}

void CNetManager::NetTransMutexUnlock()
{
#if (_TRANS_THREAD_SUPPORT!=0)
	int ret = 0;	
	DBG_PRINT(("^^^^^^^^^^^Net Mutex Unlock^^^^^^^^"));
	ret = pthread_mutex_unlock(&s_net_mutex);
	if (ret != 0)
	{
		DBG_PRINT(("===NET MUTEX UNLOCK %d====", ret));
	}
#endif
}

