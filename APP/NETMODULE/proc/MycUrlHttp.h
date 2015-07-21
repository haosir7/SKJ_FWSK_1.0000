#ifndef MY_CURL_HTTP_H
#define MY_CURL_HTTP_H

//#define _WIN32_WINNT 0x0510
#include "curl.h"
#include "easy.h"

//#include "DataDesign.h"

#include <string>
using namespace std;

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

#define PROCTIMEOUT 15
#define	CONNECT_TIME_OUT	10		//连接超时(秒)

struct MemoryStruct
{
	char *memory;
	size_t size;
	MemoryStruct();
};

struct WriteThis 
{
	char *readptr;
	int sizeleft;
	WriteThis();
	WriteThis& operator = (const WriteThis writeinfo)
	{
		this->readptr = writeinfo.readptr;
		this->sizeleft = writeinfo.sizeleft;
		return *this;
	};
};

typedef enum PROXY_PROTOCOL_ENUM{Proxy_protocol_NO=0,Proxy_protocol_HTTP=1,Proxy_protocol_SOCKS5=2}PROXY_PROTOCOL_TYPE;

struct curlconfig
{
	PROXY_PROTOCOL_TYPE proxy_protocol;
	int connect_timeout;
	string proxy;
	string url;
	string username;
	string userpwd;
	curlconfig();
};
typedef struct curlconfig curlconfig;

class TCUrlModel 
{
public:
	TCUrlModel();

	virtual ~TCUrlModel();

	//int dopostbuff(void *pconfig, int (* lpfnprogress)(void *, double, double, double, double));
	//int dopostbuff(struct WriteThis &postbuff, struct MemoryStruct &outbuff, string &errstr);

	string GetCookieFileName();              //获取cookie文件名称
	int  SetSessionID(string& strSessionID);
	
	bool  GetSessionID(string& strSessionID); //获取sessionid

	int dopostbuff(IN unsigned char *postBuf, IN int postBufLen,
		OUT unsigned char *outBuf, IN OUT int outBufLen, string &errstr);
	int dopostbuff(IN unsigned char *postBuf, IN int postBufLen, string &errstr);


	int dogetbuff(OUT unsigned char *outBuf, IN OUT int& outBufLen, string &errstr);
	int dogetbuff(string &errstr);

	void SetCurlConfig(curlconfig config);

private:
	int curlInit();

	int curlSetOpt(string &errstr, struct WriteThis *pooh=NULL);	

	//void curlSetHttpHeader();

	int procCurlRes(string& errstr, CURLcode res);

	void curlCleanUp();

	const char *get_strerror(CURLcode error);

	int  getValue(char *sheader,char *sKeyWord,char* sValue);

private:
	CURL *m_pcurl;
	struct curl_slist *m_pslist;	
	curlconfig m_curlconfig;
	string  m_strSessionId;       // 设置sessionid

	string   m_cookiefile;

public:
	struct MemoryStruct m_chunk;
 };

#endif
