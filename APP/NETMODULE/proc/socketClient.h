#ifndef CLIENT_H
#define CLIENT_H


#ifndef IN
#define IN
#endif

#ifndef OUT_C
#define OUT_C
#endif

#ifndef IN_OUT
#define IN_OUT
#endif

#define TIME_OUT_TIME	5


#ifdef __cplusplus
extern "C"
{
#endif


int SocketOpen(const char *serverIP, const char *serverPort);
int SocketClose(int *sockfd);

int SocketSend(IN int socHandle,IN const unsigned char* conBuf,IN int conLen);
int SocketRecv(IN int socHandle,IN int buffSize, OUT_C unsigned char* conBuf,IN_OUT int conLen);


#ifdef __cplusplus
}
#endif

#endif
