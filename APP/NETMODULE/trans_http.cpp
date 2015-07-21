/**
 @file  trans_http.cpp
 @brief 本文件实现了http协议传输类。
 */
#include "commonFunc.h"
#include "trans_http.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"
#include "pos_debug.h"




trans_http::trans_http(Sending *p):trans_abstrac(p)
{
	DBG_PRINT(("trans_http()"))
	m_pHttpHeadBuff = new char[HTTP_HEAD_LEN_MAX];
	m_transferType = 0;
	m_sendMode = HTTP_USE_POST_KEYWORD;
}

trans_http::~trans_http()
{
	DBG_PRINT(("~trans_http()"))
	if (m_pHttpHeadBuff != NULL)
	{
		delete m_pHttpHeadBuff;
		m_pHttpHeadBuff = NULL;
	}
}



int trans_http::sendData(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr)
{
	INT32 recHead_len = 0;
	INT32 recContent_len = 0;
	INT32 ret=0;
	INT32 i=0;
	
	//设置HTTP包头
	prn_log("SetHttpHead");
	memset(m_pHttpHeadBuff, 0, HTTP_HEAD_LEN_MAX);
	sethttphead(m_pHttpHeadBuff, m_serverFile.c_str(), 
				m_serverIP.c_str(), m_serverPort.c_str(), sLen);
	DBG_PRINT(("httpHeadLen = %u", strlen(m_pHttpHeadBuff)));
#if (_DBG_PRINT_TRANSDATA==1)
	printf("-----------------------HttpHead----------------------\n");
	printf("%s", m_pHttpHeadBuff);
	printf("---------------------HttpHead_end--------------------\n");
#endif

	//发送HTTP包头
	prn_log("SendHttpHead");
	if(send((UINT8 *)m_pHttpHeadBuff, strlen(m_pHttpHeadBuff), strErr) != NET_SUCCESS)
	{
		strErr = "发送HTTP包头错误";  
		return NET_TRANS_FAIL;  
	}

	//发送HTTP内容
	prn_log("SendHttpContent");
	if(send(psBuf, sLen, strErr) != NET_SUCCESS)
	{
		strErr = "发送HTTP包内容错误";  
		return NET_TRANS_FAIL;  
	}	

	CommonSleep(100);

	//读应答的HTTP包头
	prn_log("ReadHttpHead");
	for(i=0; i<1; i++)
	{
		recHead_len = ReadHttpHead(&recContent_len);	//recHead_len为http包头自身的长度；recContent_len为解析出HTTP内容长度	
		if(recHead_len<0)
		{
			continue;
		}
		break;
	}	
	if(recHead_len < 0)
	{
		strErr = "接收HTTP包头错误";  
		return NET_TRANS_FAIL;
	}
	if(recContent_len>=m_revBuffLen)
	{
		strErr = "接收HTTP内容超长";  
		return NET_TRANS_FAIL;
	}

	//读应答的HTTP内容
	prn_log("ReadHttpContent");
	if (m_transferType == HTTP_TRANSFER_CONLENGTH)
	{
		ret = rev(m_revBuffLen, prBuf, &recContent_len, strErr);
	}
	else
	{
		ret = RecvHttp_chunked(prBuf, &recContent_len, strErr);
	}
	if(ret != NET_SUCCESS)
	{
		if (strErr == "")
			strErr = "接收HTTP内容错误";  
		return NET_TRANS_FAIL;
	}
	prn_log2("rec_httpContent_len = %d", recContent_len); 
	
	*rLen = recContent_len;
	return NET_SUCCESS;
}


void trans_http::sethttphead(char *msg,const char* file,const char *serverIP, const char *serverPort, UINT32 con_len)
{
	char buf[64]; 

	if (m_sendMode == HTTP_USE_GET_KEYWORD)
		strcat(msg,"GET ");  
	else
		strcat(msg,"POST ");  
	strcat(msg,file);  

	strcat(msg," HTTP/1.1");  
	strcat(msg,"\r\n");  

	memset(buf,0,sizeof(buf));  
	sprintf(buf,"Content-Length:%lu",con_len);
	strcat(msg,buf);  
	strcat(msg,"\r\n");  

	strcat(msg,"Content-Type:application/octet-stream; Charset=gb18030");
	strcat(msg,"\r\n");  

	strcat(msg,"Host:");  
	strcat(msg,serverIP);  
	strcat(msg,":");
	strcat(msg,serverPort);
	strcat(msg,"\r\n");  
	strcat(msg,"\r\n");  
}


int trans_http::ReadHttpHead(INT32 *contentLen)
{
	int ret=0;
	int i=0;
	INT32 nBytes = 0;
	INT32 HeadHeadLen = 0;
	INT8 *buf = m_pHttpHeadBuff; 
	string strErr("");

	memset(buf, 0, HTTP_HEAD_LEN_MAX);
	HeadHeadLen = 16;
	ret = rev(m_revBuffLen, (UINT8 *)buf, &HeadHeadLen, strErr);  
	if (ret != NET_SUCCESS)
	{
		DBG_PRINT(("rev HttpHead ERR!"));
		return -1;  
	}
	DBG_ASSERT_WARNING(HeadHeadLen==16, ("HeadHeadLen=%d ! buf=%s ",HeadHeadLen,buf));
	if(HeadHeadLen != 16)
	{
		DBG_PRINT(("HttpHead 16Byte ERR"));
		return -1;  
	}
	i+=16;

	if(strstr((INT8 *)buf, "HTTP/1.1 200 OK") == 0)
	{
		int httpstate=getHttpState(buf);
		DBG_PRINT(("httpstate = %d", httpstate));
		if(httpstate==503)
		{
			return -2; 
		}
		if(httpstate == 400 ||
			httpstate == 404 ||
			httpstate == 500 || 
			httpstate < 0 )
		{
			return -3;
		}

		return -1;  
	}
	
	do
	{
		nBytes = 1;
		ret = rev(m_revBuffLen, (UINT8 *)(buf+i), &nBytes, strErr); 

		if(buf[i-3]=='\r' && buf[i-2]=='\n' && buf[i-1]=='\r' && buf[i]=='\n')
		{
			break;
		} 
		i += nBytes;
		if(i > HTTP_HEAD_LEN_MAX-2)
		{
			DBG_ASSERT_WARNING(FAILURE,("buf overflow ! "));
			return -1;
		}
	}while(nBytes > 0) ;
	prn_log2("httphead len = %d", i);

	m_transferType = getTransferState(buf);
	if (m_transferType == HTTP_TRANSFER_CHUNKED)
	{
		*contentLen = 0;
	}
	else
	{
		nBytes = getconlength(buf);
		*contentLen = nBytes;
		DBG_PRINT(("contentLen = %d", nBytes));
	}

#if (_DBG_PRINT_TRANSDATA==1)
	if( i > 0 )
	{
		printf("-------------------revHttpHead----------------------\n");
		printf("%s", buf);
		printf("-------------------revHttpHead_end------------------\n");
	}			
#endif

	return i;
}

int trans_http::RecvHttp_chunked(UINT8 *revBuf, INT32 *recContent_len, string &strErr)
{
	INT32 revLen=0;
	INT32 revLenPos=0;
	INT32 nTotalBytes=0;
	INT32 ret=0;
	UINT8 lengthBuf[8];
	
	while(1)
	{
		//接收长度信息
		revLenPos = 0;
		memset(lengthBuf, 0, sizeof(lengthBuf));
		do
		{
			revLen = 1;
			ret = rev(sizeof(lengthBuf), lengthBuf+revLenPos, &revLen, strErr);
			if(ret != NET_SUCCESS)
			{
				if (strErr == "")
					strErr = "未收到HTTP长度信息";  
				return NET_TRANS_FAIL;
			}
			revLenPos++;
		} while (!(lengthBuf[revLenPos-2]=='\r' && lengthBuf[revLenPos-1]=='\n'));
		sscanf((INT8 *)lengthBuf, "%x", &revLen);
		prn_log2("(RecvHttp) revLen = %d", revLen); 
		
		if (revLen == 0) //长度为0，表示结束
		{
			break;
		}
		else if (revLen < 0)
		{
			strErr = "HTTP包长度信息错误";  
			return NET_TRANS_FAIL;
		}
		
		//接收数据
		ret = rev(m_revBuffLen, revBuf+nTotalBytes, &revLen, strErr);
		if(ret != NET_SUCCESS)
		{
			if (strErr == "")
				strErr = "接收HTTP内容错误";  
			return NET_TRANS_FAIL;
		}
		nTotalBytes += revLen;

		//接收数据后面的“/r/n”
		revLen = 2;
		ret = rev(sizeof(lengthBuf), lengthBuf, &revLen, strErr);
		if(ret != NET_SUCCESS)
		{
			if (strErr == "")
				strErr = "接收HTTP内容错误";  
			return NET_TRANS_FAIL;
		}
	}
	
    prn_log2("(RecvHttp) Res = %d", nTotalBytes);
	*recContent_len = nTotalBytes;
	
	return NET_SUCCESS;
}

int trans_http::getValue(char *sheader,char *sKeyWord)
{
     char * cont_len_pos=NULL;  
     unsigned long cont_length=0;  
     const char* header = sheader;  

     cont_len_pos =(char *)strstr(header,sKeyWord);  
	 if(cont_len_pos == NULL)
	 {
		 DBG_ASSERT_WARNING(FAILURE,("not found %s: !",sKeyWord));
		 return -1;
	 }

	 cont_len_pos += strlen(sKeyWord);
	 while (*cont_len_pos == ' ')
		 cont_len_pos++;  
	 
     sscanf(cont_len_pos, "%ld", &cont_length);  
     return cont_length;  
}

int trans_http::isExist(char *sheader,char *sKeyWord)
{
     char * cont_len_pos=NULL;  
     const char* header = sheader;  

     cont_len_pos =(char *)strstr(header,sKeyWord);  
	 if(cont_len_pos == NULL)
	 {
		 return 0;
	 }
     return 1;  
}

int trans_http::getHttpState(char *sheader)
{
	return getValue(sheader,"HTTP/1.1");
}

int trans_http::getconlength( char* sheader)
{
	return getValue(sheader,"Content-Length:");
}

int trans_http::getTransferState( char* sheader)
{
	if (isExist(sheader,"Transfer-Encoding: chunked") == 1)
		return HTTP_TRANSFER_CHUNKED;
	else
		return HTTP_TRANSFER_CONLENGTH;
}

int trans_http::getserverstate(char * sheader)
{
	return getValue(sheader,"resultType:");
}

void trans_http::SetSendMode(int mode)
{
	m_sendMode = mode;
}



