/**
 @file  trans_CURL.cpp
 @brief 本文件实现了网络数据传输类trans_CURL，该类使用CURL库做数据传输。
 */

#include "commonFunc.h"
#include "netConnection.h"
#include "netManager.h"
#include "trans_CURL.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



trans_CURL::trans_CURL()
{
	DBG_PRINT(("trans_CURL()"))
}

trans_CURL::~trans_CURL()
{
	DBG_PRINT(("~trans_CURL()"))
}

int trans_CURL::trans_proc(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
#if (_TRANS_MODULE_CURL != 0)

	int re = 0;
	int rec_len = 0;
	curlconfig config;
	string urlstr("");
	INT8 urlbuf[64];
	
	DBG_PRINT(("In SendDataByCURL...."));
	DBG_PRINT(("send_len = %d", sLen));

	memset(urlbuf, 0, sizeof(urlbuf));
	sprintf(urlbuf, "http://%s:%s", m_serverIP.c_str(), m_serverPort.c_str());
	urlstr = urlbuf;
	urlstr += m_serverFile;
	
	DBG_PRINT(("urlstr = %s", urlstr.c_str()));
	config.url = urlstr;
	config.proxy_protocol = Proxy_protocol_NO;
	m_curModel.SetCurlConfig(config);
	
	re = m_curModel.dopostbuff(psBuf, sLen, strErr);
	if( re != 0)
	{
		if( strErr == "" )
			strErr = "CURL通讯错误";
		return NET_TRANS_FAIL;
	}
	
	rec_len = m_curModel.m_chunk.size;
	DBG_PRINT(("rec_len = %d", rec_len));
	memcpy(prBuf, m_curModel.m_chunk.memory, rec_len);
	prBuf[rec_len] = 0;
		
	*rLen = rec_len;

#if (_DBG_PRINT_TRANSDATA==1)
		if( rec_len > 0 )
		{
			printf("-------------------revBuf----------------------\n");
			printf("%s", prBuf);
			printf("\n------------------revBuf_end-------------------\n");
		}		
#endif


#else	//_TRANS_MODULE_CURL
	DBG_ASSERT_EXIT(0, ("CURL not compile"));			
#endif	//_TRANS_MODULE_CURL
	
	return NET_SUCCESS;
}


void trans_CURL::SetServerIP(const string IP, const string Port)
{
	m_serverIP.assign(IP);
	m_serverPort.assign(Port);
}

int trans_CURL::CheckServerLink(string &strErr)	//暂时未找到库中对应功能的调用方法
{
	return NET_SUCCESS;
}


