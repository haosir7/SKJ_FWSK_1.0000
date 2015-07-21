#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

#include "comdatatype.h"
#include "netHeaderDef.h"
#include "commonFunc.h"
#include "socketClient.h"
#ifndef _DEBUG_VERSION
#include "powoff_protect.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"




int SocketOpen(const char *serverIP, const char *serverPort)
{
	int res=0;
	int sockfd=0;
	struct sockaddr_in server_addr;		

	struct timeval timeo;
	socklen_t timeo_len=sizeof(timeo);
	
	int nZero=0;
	socklen_t para_len=sizeof(nZero);

	prn_log3("ip : %s , port : %s", serverIP, serverPort);

	//打开一个TCP的SOCKET
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		prn_log("(Socket) Failed to create socket");
		return -1;
	}

	//设置发送等待时间与发送缓存大小
	timeo.tv_sec=TIME_OUT_TIME;
	timeo.tv_usec=0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, timeo_len) == -1)	
	{
		prn_log("(Socket) setsockopt SENDTIMEOUT err!");
		return -1;
	}
	nZero=2048;
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, para_len) == -1)
	{
		prn_log("(Socket) setsockopt SO_SNDBUF  err!");
		return -1;
	}

	//设置接收等待时间与接收缓存大小
	timeo.tv_sec=TIME_OUT_TIME-2;
	timeo.tv_usec=0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeo, timeo_len) == -1)
	{
		prn_log("(Socket) setsockopt RCVTIMEOUT err!");
		return -1;
	}
	nZero=1024;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&nZero, para_len) == -1)
	{
		prn_log("(Socket) setsockopt SO_RCVBUF err!");
		return -1;
	}
	
	
	//与服务器端建立SOCKET连接
	memset((void *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverIP);
	server_addr.sin_port = htons(atoi(serverPort));
	bzero(&(server_addr.sin_zero), 8);

	MASK_ALARM_SIGNAL();
	res = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	UN_MASK_ALARM_SIGNAL();
	if( res == -1 )
	{
		if (errno == EINPROGRESS)
		{
			prn_log("connect time out");
		}
		prn_log("Failed to connect socket");
		return -1;   
	}

	struct timeval tm;
	fd_set set;
	tm.tv_sec = TIME_OUT_TIME;
	tm.tv_usec = 0;

	FD_ZERO(&set);     
	FD_SET(sockfd, &set);   
	int status=select(sockfd+1,(fd_set *)0, &set, (fd_set *)0, &tm);     

	switch(status)     
	{     
		case   -1:     
		{//select   error   
			prn_log("select   err");   
			return -1;
		}   

		case   0:     
		{//time   out   
			prn_log("time   out");   
			return -1;
		}   
		default:     
		{//success   
			if(   FD_ISSET(sockfd, &set)   )     
			{   
				int err=1;     
				int errlen=1;   
				//主要针对防火墙   
				getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);     
				if(err==0)   
				{//真的成功了   
					prn_log("(Socket) Connect SUCCESS!!!");
					FD_CLR(sockfd, &set);   
				}   
				else   
				{   
					prn_log("(Socket) Connect err");   
					return -1;   
				}   
			}   
		}   
	}  

	return sockfd;
}

int SocketClose(int *sockfd)
{
	if(*sockfd > -1)
	{
		close(*sockfd);
		*sockfd=-1;
	}
	return 0;
}



int SocketSend(IN int socHandle,IN const unsigned char* conBuf,IN int conLen)
{
	int nPackLen=SOCKET_SEND_PACKLEN;
	int retry_count=MAX_SOCKET_TRANS_RETRY;
	int nbytes=0;
	int nTotalBytes=0;
	int nSendLen=0;

	prn_log2("(Send) conLen = %d", conLen);
	while(retry_count-- > 0)
	{
		nSendLen=((conLen-nTotalBytes) > nPackLen) ? nPackLen : conLen-nTotalBytes;
#ifdef  SOCK_DBG
		prn_log2("(Send) nSendLen = %d", nSendLen);
#endif
		MASK_ALARM_SIGNAL();
		nbytes=send(socHandle, conBuf+nTotalBytes, nSendLen, MSG_NOSIGNAL);  
		UN_MASK_ALARM_SIGNAL();
		if(nbytes < 0)
		{
			prn_log3(" errno=%d,errstr=%s",errno,strerror(errno));
			prn_log3("nbytes=%d, nTotalBytes=%d",nbytes,nTotalBytes);
			if(retry_count <= 1)
			{
				return -1;
			}
			else
			{
				CommonSleep(1000);
				continue;
			}
		}
		else if(nbytes > 0)
		{
			retry_count=MAX_SOCKET_TRANS_RETRY;
			if (nbytes != nSendLen)
			{
				prn_log3(" errno=%d,errstr=%s",errno,strerror(errno));
				prn_log3("nbytes=%d, nSendLen=%d", nbytes, nSendLen);
				CommonSleep(1000);
			}
		}
		else
		{
			prn_log2("nbytes=%d",nbytes);
			prn_log2("nTotalBytes = %d", nTotalBytes);
		}

		nTotalBytes+=nbytes;
#ifdef  SOCK_DBG
		if (conLen>1)
			prn_log2("-------send: nTotalBytes = %d--------", nTotalBytes);
#endif

		if(nTotalBytes >= conLen)
		{
			break;
		}
	}	
	return nTotalBytes;
}


int SocketRecv(IN int socHandle,IN int buffSize, OUT_C unsigned char* conBuf,IN_OUT int conLen)
{
	const int MAX_RECV_LEN=SOCKET_RECV_PACKLEN;
	int retry_count=MAX_SOCKET_TRANS_RETRY;
	
	int nbytes=0;
	int nTotalBytes=0;
	int nrecvPackLen=0;
	int noLengthFlag=0;

	if (conLen<=0)	//未给定接收长度时，只能以超时来判定退出
	{
		conLen = buffSize;
		retry_count = MAX_SOCKET_TRANS_RETRY;
		noLengthFlag = 1;
	}

	while(retry_count-- > 0)
	{
		nrecvPackLen= ((conLen-nTotalBytes) > MAX_RECV_LEN) ? MAX_RECV_LEN : conLen-nTotalBytes;
#ifdef	SOCK_DBG
		prn_log2("(RecvHttp) nrecvPackLen = %d", nrecvPackLen);
#endif
		MASK_ALARM_SIGNAL();
		nbytes=recv(socHandle,conBuf+nTotalBytes,nrecvPackLen,0);  
		UN_MASK_ALARM_SIGNAL();
		if(nbytes <= 0)
		{
			prn_log3("(RecvHttp) nTotalBytes=%d,nbytes=%d",nTotalBytes,nbytes);
			prn_log3("(RecvHttp) errno=%d,errstr=%s",errno,strerror(errno)); 
			if(retry_count > 0)
			{
				CommonSleep(1000);
				continue;
			}

			break;
		}
		else if(nbytes>0)	//有收到数据时，可以增大重试次数
		{
			if (noLengthFlag==0)
				retry_count = MAX_SOCKET_TRANS_RETRY+3;	
			else
				retry_count = MAX_SOCKET_TRANS_RETRY;
		}

		nTotalBytes+=nbytes;
#ifdef  SOCK_DBG
		if (conLen>1)
			prn_log3("-------rev: nTotalBytes=%d,nbytes=%d-------",nTotalBytes,nbytes);
#endif
		if(nTotalBytes >= conLen)
		{
			break;
		}
	}	

	if (nTotalBytes>=0)
		return nTotalBytes;
	else
		return -1;
}

