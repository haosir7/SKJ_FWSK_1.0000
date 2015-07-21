/**
 @file  trans_abstrac.cpp
 @brief 本文件实现了网络数据传输抽象类trans_abstrac。
 */
#include "trans_abstrac.h"
#include "commonFunc.h"
#include "netConnection.h"
#include "netManager.h"
#ifdef APE_GUI
#include "CaProgressBar.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



trans_abstrac::trans_abstrac(Sending *p)
{
	DBG_PRINT(("trans_abstrac()"))
	m_pSending = p;
}

trans_abstrac::~trans_abstrac()
{
	DBG_PRINT(("~trans_abstrac()"))
	if (m_pSending != NULL)
	{
		delete m_pSending;
		m_pSending = NULL;
	}
}

int trans_abstrac::trans_proc(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
	int ret=0;
	UINT8 vpn_reConnect_flag=0;
	UINT8 vpn_reConnect_times=0;
	INT32 rec_len=0;
	CNetConnection *pL2 = g_netManager->GetConnectionHandleL2();
	PROBAR_DEF();

#if (_DBG_PRINT_TRANSDATA==1)
	if( sLen > 0 )
	{
		DBG_PRINT(("sLen = %d", sLen));
		printf("-------------------sendBuf----------------------\n");
		printf("%s", psBuf);
		printf("\n------------------sendBuf_end-------------------\n");
	}
#endif
	
	vpn_reConnect_flag = 0;
	vpn_reConnect_times = 0;
	while(1)
	{		
		if (vpn_reConnect_flag != 0)		//是否进行VPN重连接	
		{
			if (vpn_reConnect_times++ >= 3)
			{			
				strErr = "重试多次,数据仍传输失败";
				return NET_TRANS_FAIL;
			}
			
			PROBAR_SHOW("重连接服务器中,请稍候...");
			DBG_PRINT(("***断开VPN连接***"));
			disConnect();
			
			DBG_PRINT(("***重新VPN连接***"));
			ret = pL2->performConnect(strErr);
			if(ret != NET_SUCCESS)
			{
				DBG_PRINT(("strErr= %s",strErr.c_str()));
				continue;
			}
			else
			{
		//		vpn_reConnect_times = 0;
				vpn_reConnect_flag = 0;
				strErr = "";
			}
		}

		
		DBG_PRINT(("建立与服务器的传输通道...."));
		PROBAR_SHOW("服务器连接建立中,请稍候...");
		
		if((ret=connectServer(strErr)) != NET_SUCCESS)
		{	
			if (pL2->m_connectionMode==NET_VPN && pL2->m_dialState==HAVE_DIALED)
			{
				DBG_PRINT(("建立服务器连接失败，准备重连..."));
				vpn_reConnect_flag = 1;
				//vpn_reConnect_times = 4;
				continue;
			}
			return ret;
		}
		CommonSleep(500);
		

		DBG_PRINT(("开始数据交互...."));
		PROBAR_SHOW("业务数据交互中...");
		
		ret = sendData(m_pSendBuff, sLen, prBuf, &rec_len, strErr);
		if (ret != NET_SUCCESS)
		{
			DBG_PRINT(("SendData FAILURE. "));
			disConnect();

			//vpn模式时要尝试重连接
			if (pL2->m_connectionMode==NET_VPN && pL2->m_dialState==HAVE_DIALED)
			{
				DBG_PRINT(("准备重连..."));
				vpn_reConnect_flag = 1;
				//vpn_reConnect_times = 4;
				continue;
			}

			return ret;
		}

		DBG_PRINT(("数据交互结束，rec_len=%d", rec_len));
		disConnect();		

#if (_DBG_PRINT_TRANSDATA==1)
		if( rec_len > 0 )
		{
			printf("-------------------revBuf----------------------\n");
			printf("%s", prBuf);
			printf("\n------------------revBuf_end-------------------\n");
		}		
#endif
		
		break;
	}

	*rLen = rec_len;
	DBG_PRINT(("TransToSever End"));
	return NET_SUCCESS;
}

void trans_abstrac::SetServerIP(const string IP, const string Port)
{
	m_serverIP.assign(IP);
	m_serverPort.assign(Port);
	m_pSending->SetServerIP(IP, Port);
}

int trans_abstrac::CheckServerLink(string &strErr)
{
	int ret=0;
	ret = connectServer(strErr);
	disConnect();
	return ret;
}


int trans_abstrac::connectServer(string &strErr) 
{
	return m_pSending->connectServer(strErr);
}
int trans_abstrac::disConnect() 
{
	return m_pSending->disConnect();
}
int trans_abstrac::send(UINT8 *psBuf, INT32 sLen, string &strErr) 
{
	return m_pSending->send(psBuf,sLen,strErr);
}
int trans_abstrac::rev(INT32 rBufSize, UINT8 *prBuf, INT32 *rLen, string &strErr) 
{
	return m_pSending->rev(rBufSize, prBuf,rLen,strErr);
}


