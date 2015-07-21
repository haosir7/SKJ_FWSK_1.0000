#include "MycUrlHttp.h"

/************************************************************************
err
常见状态代码、状态描述、说明：
1.1xx：信息，请求收到，继续处理
2.2xx：成功，行为被成功地接受、理解和采纳
3.3xx：重定向，为了完成请求，必须进一步执行的动作
4.4xx：客户端错误，请求包含语法错误或者请求无法实现
5.5xx：服务器错误，服务器不能实现一种明显无效的请求
下表显示每个响应码及其含义：
100         继续
101         分组交换协
200         OK
201         被创建
202         被采纳
203         非授权信息
204         无内容
205         重置内容
206         部分内容
300         多选项
301         永久地传送
302         找到
303         参见其他
304         未改动
305         使用代理
307         暂时重定向
400         错误请求
401         未授权
402         要求付费
403         禁止
404         未找到
405         不允许的方法
406         不被采纳
407         要求代理授权
408         请求超时
409         冲突
410         过期的
411         要求的长度
412         前提不成立
413         请求实例太大
414         请求URI太大
415         不支持的媒体类型
416         无法满足的请求范围
417         失败的预期
500         内部服务器错误
501         未被使用
502         网关错误
503         不可用的服务
504         网关超时
505         HTTP版本未被支持
************************************************************************/

MemoryStruct::MemoryStruct()
{
	this->memory = NULL;
	this->size = 0;
}

WriteThis::WriteThis()
{
	this->readptr = NULL;
	this->sizeleft = 0;
}

curlconfig::curlconfig()
{
	this->proxy="";
	this->url="";
	this->username = "";
	this->userpwd = "";
	this->proxy_protocol = Proxy_protocol_NO;
	this->connect_timeout = 0;
}

static void *myrealloc(void *ptr, size_t size);

static void *myrealloc(void *ptr, size_t size)
{
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

static size_t 
ReadMemoryCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *pooh = (struct WriteThis *)userp;

  if(size*nmemb < 1)
    return 0;

  size_t res;
  if (pooh->sizeleft > 0)
  {
	  pooh->sizeleft -= nmemb;
	  if (pooh->sizeleft >= 0)
	  {
		res = nmemb;
		memcpy(ptr, pooh->readptr, res);
		pooh->readptr += res;
	  }
	  else
	  {
		res = pooh->sizeleft+nmemb;
		pooh->sizeleft = 0;
		memcpy(ptr, pooh->readptr, res);
	  }
	  
	  return res;
  }

  return 0;                          /* no more data left to deliver */
}

static size_t
GetContentParmCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	}
	return realsize;
}


static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}


//TCUrlModel* TCUrlModel::g_instance_curl = NULL;


TCUrlModel::TCUrlModel()
{
	m_pcurl = NULL;
	m_pslist = NULL;
	m_chunk.memory = NULL;
	m_chunk.size = 0;
	m_strSessionId = "";
	m_cookiefile = "/usr/app/cookie.txt";
}

TCUrlModel::~TCUrlModel()
{
	this->curlCleanUp();
}

const char * TCUrlModel::get_strerror(CURLcode error)
{
	switch (error) {
	case CURLE_OK:
	return "无错误";

	case CURLE_UNSUPPORTED_PROTOCOL:
	return "不支持的协议";

	case CURLE_FAILED_INIT:
	return "初始化失败";

	case CURLE_URL_MALFORMAT:
	return "网址格式非法";

	case CURLE_COULDNT_RESOLVE_PROXY:
	return "无法解析代理名称";

	case CURLE_COULDNT_RESOLVE_HOST:
	return "无法解析主机名称";

	case CURLE_COULDNT_CONNECT:
	return "无法连接到服务器";

	case CURLE_REMOTE_ACCESS_DENIED:
	return "拒绝访问到远程资源";

	case CURLE_HTTP_RETURNED_ERROR:
	return "HTTP响应代码表示错误";

	case CURLE_WRITE_ERROR:
	return "没有收到的数据写入到磁盘/应用";

	case CURLE_UPLOAD_FAILED:
	return "上传失败";

	case CURLE_READ_ERROR:
	return "无法打开/读取本地的数据文件/应用";

	case CURLE_OUT_OF_MEMORY:
	return "内存不足";

	case CURLE_OPERATION_TIMEDOUT:
	return "超时";

	case CURLE_RANGE_ERROR:
	return "请求超出了服务器提供的范围";

	case CURLE_BAD_DOWNLOAD_RESUME:
	return "不能恢复下载";

	case CURLE_TOO_MANY_REDIRECTS :
	return "重定向达到最大数";

	case CURLE_GOT_NOTHING:
	return "服务器返回无标题，无数据";

	case CURLE_SEND_ERROR:
	return "发送数据失败";

	case CURLE_RECV_ERROR:
	return "接受数据失败";

	case CURLE_BAD_CONTENT_ENCODING:
	return "无法识别的HTTP内容编码";

	case CURLE_LOGIN_DENIED:
	return "登录失败";

	case CURLE_AGAIN:
	return "Socket没有准备好发送/接收";

	/* error codes not used by current libcurl */
	default:
	break;
	}
	return curl_easy_strerror(error);
}

int TCUrlModel::curlInit()
{
	m_pcurl = curl_easy_init();
	if (NULL == m_pcurl)
	{
		return -1;
	}
	
	return 0;
}

void TCUrlModel::curlCleanUp()
{
	if (m_chunk.memory)
	{
		free(m_chunk.memory);
		m_chunk.memory = NULL;
	}
	m_chunk.size = 0;
	
	if (m_pslist)
	{
		curl_slist_free_all(m_pslist);
		m_pslist = NULL;
	}

	if (m_pcurl)
	{
		curl_easy_cleanup(m_pcurl);
		m_pcurl = NULL;
	}	
	
	return;
}

void TCUrlModel::SetCurlConfig(curlconfig config)
{
	this->m_curlconfig = config;
}

int TCUrlModel::dogetbuff(string &errstr)
{
	errstr = "";
	
	curlCleanUp();
	if (0 != curlInit())
	{
		errstr="CURL初始化失败!";
		return -1;
	}
	
	if (0 != curlSetOpt(errstr, NULL))
	{
		curlCleanUp();
		return -1;
	}
	
	CURLcode res = curl_easy_perform(m_pcurl);
	if (0 != procCurlRes(errstr, res))
	{
		errstr += ",请检查网络设置并重新连接!";
		curlCleanUp();
		return -1;
	}
	
	if (m_chunk.size <= 0)
	{
		errstr = "接收数据长度为空!";
		curlCleanUp();
		return -1;
	}
	
	return 0;
}

int TCUrlModel::dogetbuff(OUT unsigned char *outBuf, IN OUT int& outBufLen, string &errstr)
{
	errstr = "";
	
	curlCleanUp();
	if (0 != curlInit())
	{
		errstr="CURL初始化失败!";
		return -1;
	}

	if (0 != curlSetOpt(errstr, NULL))
	{
		curlCleanUp();
		return -1;
	}

	CURLcode res = curl_easy_perform(m_pcurl);
	if (0 != procCurlRes(errstr, res))
	{
		errstr += ",请检查网络设置并重新连接!";
		curlCleanUp();
		return -1;
	}

	if (m_chunk.size <= 0)
	{
		errstr = "接收数据长度为空!";
		curlCleanUp();
		return -1;
	}

	if (outBufLen <= 0 || m_chunk.size > (size_t)(outBufLen-1))
	{
		errstr = "接收数据长度大小超过缓存数据长度大小!";
		curlCleanUp();
		return -1;
	}

	outBufLen = m_chunk.size;
	memcpy(outBuf, m_chunk.memory, m_chunk.size);
	outBuf[outBufLen] = '\0';

	return 0;
}

int  TCUrlModel::SetSessionID(string& strSessionID)
{
	if(strSessionID.empty())
	{
		m_strSessionId = "";
		return 0;
	}
	char buf[1024];
	memset(buf,sizeof(buf),0);
	sprintf(buf,"JSESSIONID=%s\r\n", strSessionID.c_str());
	m_strSessionId = buf;
	return 0;
}

//获取sessionid
bool  TCUrlModel::GetSessionID(string& strSessionID)
{
	return 0;
}

//获取cookie文件名称
string TCUrlModel::GetCookieFileName()
{
	return this->m_cookiefile;
}
//
int  TCUrlModel::getValue(char *sheader,char *sKeyWord,char* sValue)
{
	char * cont_len_pos=NULL;  
	unsigned long cont_length=0;  
	const char* header = sheader;  
	
	cont_len_pos =(char *)strstr(header,sKeyWord);  
	if(cont_len_pos == NULL)
	{
		//		 DBG_ASSERT_WARNING(false,("not found %s: !",sKeyWord));
		return -1;
	}
	char* pTmp  = (char *)strchr(cont_len_pos,';');  
	//     cont_len_pos++;  
	//    sscanf(cont_len_pos, "%ld", &cont_length);  
	cont_length = strlen(cont_len_pos)-strlen(pTmp)-strlen(sKeyWord);
	memcpy((void*)sValue,(void*)&cont_len_pos[strlen(sKeyWord)],cont_length);
	// sValue = cont_len_pos -sValue;
	return cont_length;  
}


int TCUrlModel::curlSetOpt(string &errstr, struct WriteThis *pooh)
{
	errstr = "";
	if (m_curlconfig.url.length() <= 0)
	{
		errstr = "URL不能为空!";
		return -1;
	}
	//设置cookie 
	curl_easy_setopt(m_pcurl, CURLOPT_COOKIE,m_strSessionId.c_str());
	curl_easy_setopt(m_pcurl, CURLOPT_COOKIEJAR, m_cookiefile.c_str());		//接收	
	curl_easy_setopt(m_pcurl, CURLOPT_COOKIEFILE, m_cookiefile.c_str());	//发送

	/* First set the URL that is about to receive our POST. */
	curl_easy_setopt(m_pcurl, CURLOPT_URL,m_curlconfig.url.c_str());

	if (pooh != NULL)
	{
		/* Now specify we want to POST data */
		curl_easy_setopt(m_pcurl, CURLOPT_POST, 1L);
	}	

	if (m_curlconfig.proxy.length() != 0
		&& m_curlconfig.proxy_protocol != Proxy_protocol_NO)
	{
		curl_easy_setopt(m_pcurl, CURLOPT_PROXY, m_curlconfig.proxy.c_str());
			
		if (m_curlconfig.proxy_protocol == Proxy_protocol_HTTP)
		{
			//curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
		}
		else if(m_curlconfig.proxy_protocol == Proxy_protocol_SOCKS5)
		{
			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
		}			
		else
		{
			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
		}
		char tmpuserpwdbuff[512];
		if (m_curlconfig.username.length() != 0)
 		{
			sprintf(tmpuserpwdbuff, "%s:%s", m_curlconfig.username.c_str(), m_curlconfig.userpwd.c_str());
			curl_easy_setopt(m_pcurl, CURLOPT_PROXYUSERPWD, tmpuserpwdbuff);
	  	}	
		else
		{
			if(m_curlconfig.proxy_protocol == Proxy_protocol_SOCKS5)
			{
				sprintf(tmpuserpwdbuff, "%s",":");
				curl_easy_setopt(m_pcurl, CURLOPT_PROXYUSERPWD, tmpuserpwdbuff);
			}
		}	
	}

	if (NULL != pooh)
	{
		/* 上传数据*/
		/* we want to use our own read function */
		curl_easy_setopt(m_pcurl, CURLOPT_READFUNCTION, ReadMemoryCallback);
		/* pointer to pass to our read function */
		curl_easy_setopt(m_pcurl, CURLOPT_READDATA, pooh);
		/* Set the expected POST size. If you want to POST large amounts of data,
		   consider CURLOPT_POSTFIELDSIZE_LARGE */
		curl_easy_setopt(m_pcurl, CURLOPT_POSTFIELDSIZE, (curl_off_t)pooh->sizeleft);
	}

	/*取返回的内容*/
	curl_easy_setopt(m_pcurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(m_pcurl, CURLOPT_WRITEDATA, (void *)&m_chunk);

	/* get verbose debug output please */
	curl_easy_setopt(m_pcurl, CURLOPT_VERBOSE, 1L);

	if (0 != m_curlconfig.connect_timeout)
	{
		//curl_easy_setopt(m_pcurl, CURLOPT_CONNECTTIMEOUT, m_curlconfig.connect_timeout);
	//	curl_easy_setopt(m_pcurl, CURLOPT_TIMEOUT, m_curlconfig.connect_timeout*10);
		curl_easy_setopt(m_pcurl,CURLOPT_TIMEOUT,PROCTIMEOUT);
	}
	curl_easy_setopt(m_pcurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);

	m_pslist = curl_slist_append(m_pslist, "Content-Type:application/octet-stream;charset=gb18030"); 
	curl_easy_setopt(m_pcurl, CURLOPT_HTTPHEADER, m_pslist); 

	return 0;
}

int TCUrlModel::procCurlRes(string& errstr, CURLcode res)
{

	errstr = "";
	if (res == CURLE_OK)
	{
		long response;char errbuff[128];
		//未使用代理
		curl_easy_getinfo(m_pcurl, CURLINFO_RESPONSE_CODE, &response);
		//使用代理取得
		//curl_easy_getinfo(m_pcurl, CURLINFO_HTTP_CONNECTCODE, &response);
		switch(response/100)
		{
		case 1: 
			sprintf(errbuff, "%ld-信息，请求收到，继续处理", response);
			errstr = errbuff;
			break;
		case 2:
			sprintf(errbuff, "%ld-成功，行为被成功地接受、理解和采纳", response);
			errstr = errbuff;
			break;
		case 3:
			sprintf(errbuff, "%ld-重定向，为了完成请求，必须进一步执行的动作", response);
			errstr = errbuff;
			break;
		case 4:
			sprintf(errbuff, "%ld-客户端错误，请求包含语法错误或者请求无法实现", response);
			errstr = errbuff;
			break;
		case 5:
			sprintf(errbuff, "%ld-服务器错误，服务器不能实现一种明显无效的请求", response);
			errstr = errbuff;
			break;
		default:
			sprintf(errbuff, "%ld-其他错误", response);
			errstr = errbuff;
			break;
		}
		
		if (200 != response)
		{
			return -1;
		}
	}
	else
	{
		string err(get_strerror(res));
		errstr = err;
		return -1;
	}

	return 0;
}

int TCUrlModel::dopostbuff(IN unsigned char *postBuf, IN int postBufLen, string &errstr)
{
	struct WriteThis postbuff;
	postbuff.readptr = (char *)postBuf;
	postbuff.sizeleft = postBufLen;
	
	errstr = "";
	
	curlCleanUp();
	if (0 != curlInit())
	{
		errstr="CURL初始化失败!";
		return -1;
	}
	
	if (0 != curlSetOpt(errstr, &postbuff))
	{
		curlCleanUp();
		return -1;
	}
	
	CURLcode res = curl_easy_perform(m_pcurl);
	if (0 != procCurlRes(errstr, res))
	{
		errstr += ",请检查网络设置并重新连接!";
		curlCleanUp();
		return -1;
	}
	if (m_chunk.size <= 0)
	{
		errstr = "接收数据长度为空!";
		curlCleanUp();
		return -1;
	}
	
	return 0;
}

int TCUrlModel::dopostbuff(IN unsigned char *postBuf, IN int postBufLen,
		OUT unsigned char *outBuf, IN OUT int outBufLen, string &errstr)
{
	struct WriteThis postbuff;
	postbuff.readptr = (char *)postBuf;
	postbuff.sizeleft = postBufLen;

	errstr = "";
	
	curlCleanUp();
	if (0 != curlInit())
	{
		errstr="CURL初始化失败!";
		return -1;
	}
	
	if (0 != curlSetOpt(errstr, &postbuff))
	{
		curlCleanUp();
		return -1;
	}
	
	CURLcode res = curl_easy_perform(m_pcurl);
	if (0 != procCurlRes(errstr, res))
	{
		errstr += ",请检查网络设置并重新连接!";
		curlCleanUp();
		return -1;
	}
	
	if (m_chunk.size > outBufLen-1 || m_chunk.size <= 0)
	{
		errstr = "数据长度非法";
		curlCleanUp();
		return -1;
	}
	
	outBufLen = m_chunk.size;
	memcpy(outBuf, m_chunk.memory, m_chunk.size);
	outBuf[outBufLen] = '\0';
	
	return 0;
}

// int TCUrlModel::dopostbuff(void *pconfig)
// {
// 	CURLcode res;
// 	struct WriteThis pooh;
// 	bool errflag = false;
// 
// 	curlconfig *config_p = (curlconfig *)pconfig;
// 	config_p->res = 0;
// 	config_p->resstr="";
// 
// 	curlCleanUp();
// 	if (0 != curlInit())
// 	{
// 		config_p->resstr="CURL初始化失败!";
// 		return -1;
// 	}
// 
// 	pooh.readptr = config_p->m_writeThis.readptr;
// 	pooh.sizeleft = config_p->m_writeThis.sizeleft;
// 
// 	/* First set the URL that is about to receive our POST. */
// 	curl_easy_setopt(m_pcurl, CURLOPT_URL,config_p->url.c_str());
// 	/* Now specify we want to POST data */
// 	curl_easy_setopt(m_pcurl, CURLOPT_POST, 1L);
// 
// 	if (config_p->proxy.length() != 0
// 		&& config_p->proxy_protocol != Proxy_protocol_NO)
// 	{
// 		curl_easy_setopt(m_pcurl, CURLOPT_PROXY, config_p->proxy.c_str());
// 			
// 		if (config_p->proxy_protocol == Proxy_protocol_HTTP)
// 		{
// 			//curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
// 			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
// 		}
// 		else if(config_p->proxy_protocol == Proxy_protocol_SOCKS5)
// 		{
// 			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
// 		}			
// 		else
// 		{
// 			curl_easy_setopt(m_pcurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
// 		}
// 		char tmpuserpwdbuff[512];
// 		if (config_p->username.length() != 0)
//  		{
// 			sprintf(tmpuserpwdbuff, "%s:%s", config_p->username.c_str(), config_p->userpwd.c_str());
// 			//sprintf(tmpuserpwdbuff, "%s:", config_p->username.c_str());
// 			curl_easy_setopt(m_pcurl, CURLOPT_PROXYUSERPWD, tmpuserpwdbuff);
// 	  	}	
// 		else
// 		{
// 			if(config_p->proxy_protocol == Proxy_protocol_SOCKS5)
// 			{
// 				sprintf(tmpuserpwdbuff, "%s",":");
// 				curl_easy_setopt(m_pcurl, CURLOPT_PROXYUSERPWD, tmpuserpwdbuff);
// 			}
// 		}	
// 	}
// 
// 	m_pslist = curl_slist_append(m_pslist, "Content-Type:application/octet-stream;charset=gb18030"); 
// 	curl_easy_setopt(m_pcurl, CURLOPT_HTTPHEADER, m_pslist); 
// 
// 	/* 上传数据*/
// 	/* we want to use our own read function */
// 	curl_easy_setopt(m_pcurl, CURLOPT_READFUNCTION, ReadMemoryCallback);
// 	/* pointer to pass to our read function */
// 	curl_easy_setopt(m_pcurl, CURLOPT_READDATA, &pooh);
// 
// 	/*取返回的内容*/
// 	curl_easy_setopt(m_pcurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
// 	curl_easy_setopt(m_pcurl, CURLOPT_WRITEDATA, (void *)&m_chunk);
// 
// 	/* get verbose debug output please */
// 	curl_easy_setopt(m_pcurl, CURLOPT_VERBOSE, 1L);
// 
// 	/* Set the expected POST size. If you want to POST large amounts of data,
// 	   consider CURLOPT_POSTFIELDSIZE_LARGE */
// 	curl_easy_setopt(m_pcurl, CURLOPT_POSTFIELDSIZE, (curl_off_t)pooh.sizeleft);
// 
// 
// 	/* Perform the request, res will get the return code */
// 	res = curl_easy_perform(m_pcurl);
// 
// 	if (res == CURLE_OK)
// 	{
// 		long response;char errbuff[128];
// 		//未使用代理
// 		curl_easy_getinfo(m_pcurl, CURLINFO_RESPONSE_CODE, &response);
// 		//使用代理取得
// 		//curl_easy_getinfo(m_pcurl, CURLINFO_HTTP_CONNECTCODE, &response);
// 		config_p->res = response;
// 		
// 		switch(response/100)
// 		{
// 		case 1: 
// 			sprintf(errbuff, "%ld-信息，请求收到，继续处理", response);
// 			config_p->resstr = errbuff;
// 			break;
// 		case 2:
// 			sprintf(errbuff, "%ld-成功，行为被成功地接受、理解和采纳", response);
// 			config_p->resstr = errbuff;
// 			break;
// 		case 3:
// 			sprintf(errbuff, "%ld-重定向，为了完成请求，必须进一步执行的动作", response);
// 			config_p->resstr = errbuff;
// 			break;
// 		case 4:
// 			sprintf(errbuff, "%ld-客户端错误，请求包含语法错误或者请求无法实现", response);
// 			config_p->resstr = errbuff;
// 			break;
// 		case 5:
// 			sprintf(errbuff, "%ld-服务器错误，服务器不能实现一种明显无效的请求", response);
// 			config_p->resstr = errbuff;
// 			break;
// 		default:
// 			sprintf(errbuff, "%ld-其他错误", response);
// 			config_p->resstr = errbuff;
// 		  break;
// 		}
// 
// 		if (200 != response)
// 		{
// 			errflag = true;
// 			goto err_end;
// 		}
// 	}
// 	else
// 	{
// 		string err(get_strerror(res));
// 		config_p->resstr = err;errflag = true;
// 		goto err_end;
// 	}
// 
// 	if (m_chunk.size > G_RESBUFF_LEN || m_chunk.size <= 0)
// 	{
// 		config_p->resstr = "数据长度非法";
// 		errflag = true;
// 	}
// 	else
// 	{
// 		//////////////////////////////////////////////////////////////////////////
// 		//如果是指针那么先释放然后再申请
// 		//////////////////////////////////////////////////////////////////////////
// 		
// 		config_p->m_writeThis.sizeleft = m_chunk.size>(G_RESBUFF_LEN-1)?(G_RESBUFF_LEN-1):m_chunk.size;
// 		memcpy(config_p->m_writeThis.readptr, m_chunk.memory, config_p->m_writeThis.sizeleft);
// 		config_p->m_writeThis.readptr[config_p->m_writeThis.sizeleft] = '\0';
// 	}
// 	
// err_end:
//  	if(m_chunk.memory)
// 	{
// 		free(m_chunk.memory);
// 		m_chunk.memory = NULL;
// 	}
// 	
// 	/* always cleanup */
// 	curl_slist_free_all(m_pslist);
// 	curl_easy_cleanup(m_pcurl);
// 	m_pslist = NULL;
//  	m_pcurl = NULL;
// 	if (errflag)
// 	{
// 		config_p->resstr += ",请检查网络设置并重新连接!";
// 		return -1;
// 	}
// 
// 	return 0;
// }

//////////////////////////////////////////////////////////////////////////
/*
 * Parse URL and fill in the relevant members of the connection struct.
 */
//static CURLcode ParseURLAndFillConnection(struct SessionHandle *data,
//                                          struct connectdata *conn)
//{
//  char *at;
//  char *tmp;
//  char *path = data->state.path;
//  int rc;
//
//  /*************************************************************
//   * Parse the URL.
//   *
//   * We need to parse the url even when using the proxy, because we will need
//   * the hostname and port in case we are trying to SSL connect through the
//   * proxy -- and we don't know if we will need to use SSL until we parse the
//   * url ...
//   ************************************************************/
//  if((2 == sscanf(data->change.url, "%15[^:]:%[^\n]",
//                  conn->protostr,
//                  path)) && Curl_raw_equal(conn->protostr, "file")) {
//    if(path[0] == '/' && path[1] == '/') {
//      /* Allow omitted hostname (e.g. file:/<path>).  This is not strictly
//       * speaking a valid file: URL by RFC 1738, but treating file:/<path> as
//       * file://localhost/<path> is similar to how other schemes treat missing
//       * hostnames.  See RFC 1808. */
//
//      /* This cannot be done with strcpy() in a portable manner, since the
//         memory areas overlap! */
//      memmove(path, path + 2, strlen(path + 2)+1);
//    }
//    /*
//     * we deal with file://<host>/<path> differently since it supports no
//     * hostname other than "localhost" and "127.0.0.1", which is unique among
//     * the URL protocols specified in RFC 1738
//     */
//    if(path[0] != '/') {
//      /* the URL included a host name, we ignore host names in file:// URLs
//         as the standards don't define what to do with them */
//      char *ptr=strchr(path, '/');
//      if(ptr) {
//        /* there was a slash present
//
//           RFC1738 (section 3.1, page 5) says:
//
//           The rest of the locator consists of data specific to the scheme,
//           and is known as the "url-path". It supplies the details of how the
//           specified resource can be accessed. Note that the "/" between the
//           host (or port) and the url-path is NOT part of the url-path.
//
//           As most agents use file://localhost/foo to get '/foo' although the
//           slash preceding foo is a separator and not a slash for the path,
//           a URL as file://localhost//foo must be valid as well, to refer to
//           the same file with an absolute path.
//        */
//
//        if(ptr[1] && ('/' == ptr[1]))
//          /* if there was two slashes, we skip the first one as that is then
//             used truly as a separator */
//          ptr++;
//
//        /* This cannot be made with strcpy, as the memory chunks overlap! */
//        memmove(path, ptr, strlen(ptr)+1);
//      }
//    }
//
//    strcpy(conn->protostr, "file"); /* store protocol string lowercase */
//  }
//  else {
//    /* clear path */
//    path[0]=0;
//
//    if(2 > sscanf(data->change.url,
//                   "%15[^\n:]://%[^\n/]%[^\n]",
//                   conn->protostr,
//                   conn->host.name, path)) {
//
//      /*
//       * The URL was badly formatted, let's try the browser-style _without_
//       * protocol specified like 'http://'.
//       */
//      if(1 > (rc = sscanf(data->change.url, "%[^\n/]%[^\n]",
//                          conn->host.name, path)) ) {
//        /*
//         * We couldn't even get this format.
//         * djgpp 2.04 has a sscanf() bug where 'conn->host.name' is
//         * assigned, but the return value is EOF!
//         */
//#if defined(__DJGPP__) && (DJGPP_MINOR == 4)
//        if (!(rc == -1 && *conn->host.name))
//#endif
//        {
//          failf(data, "<url> malformed");
//          return CURLE_URL_MALFORMAT;
//        }
//      }
//
//      /*
//       * Since there was no protocol part specified, we guess what protocol it
//       * is based on the first letters of the server name.
//       */
//
//      /* Note: if you add a new protocol, please update the list in
//       * lib/version.c too! */
//
//      if(checkprefix("FTP.", conn->host.name))
//        strcpy(conn->protostr, "ftp");
//      else if(checkprefix("DICT.", conn->host.name))
//        strcpy(conn->protostr, "DICT");
//      else if(checkprefix("LDAP.", conn->host.name))
//        strcpy(conn->protostr, "LDAP");
//      else {
//        strcpy(conn->protostr, "http");
//      }
//
//      conn->protocol |= PROT_MISSING; /* not given in URL */
//    }
//  }
//
//  /* We search for '?' in the host name (but only on the right side of a
//   * @-letter to allow ?-letters in username and password) to handle things
//   * like http://example.com?param= (notice the missing '/').
//   */
//  at = strchr(conn->host.name, '@');
//  if(at)
//    tmp = strchr(at+1, '?');
//  else
//    tmp = strchr(conn->host.name, '?');
//
//  if(tmp) {
//    /* We must insert a slash before the '?'-letter in the URL. If the URL had
//       a slash after the '?', that is where the path currently begins and the
//       '?string' is still part of the host name.
//
//       We must move the trailing part from the host name and put it first in
//       the path. And have it all prefixed with a slash.
//    */
//
//    size_t hostlen = strlen(tmp);
//    size_t pathlen = strlen(path);
//
//    /* move the existing path plus the zero byte forward, to make room for
//       the host-name part */
//    memmove(path+hostlen+1, path, pathlen+1);
//
//     /* now copy the trailing host part in front of the existing path */
//    memcpy(path+1, tmp, hostlen);
//
//    path[0]='/'; /* prepend the missing slash */
//
//    *tmp=0; /* now cut off the hostname at the ? */
//  }
//  else if(!path[0]) {
//    /* if there's no path set, use a single slash */
//    strcpy(path, "/");
//  }
//
//  /* If the URL is malformatted (missing a '/' after hostname before path) we
//   * insert a slash here. The only letter except '/' we accept to start a path
//   * is '?'.
//   */
//  if(path[0] == '?') {
//    /* We need this function to deal with overlapping memory areas. We know
//       that the memory area 'path' points to is 'urllen' bytes big and that
//       is bigger than the path. Use +1 to move the zero byte too. */
//    memmove(&path[1], path, strlen(path)+1);
//    path[0] = '/';
//  }
//
//  if (conn->host.name[0] == '[') {
//    /* This looks like an IPv6 address literal.  See if there is an address
//       scope.  */
//    char *percent = strstr (conn->host.name, "%25");
//    if (percent) {
//      char *endp;
//      unsigned long scope = strtoul (percent + 3, &endp, 10);
//      if (*endp == ']') {
//        /* The address scope was well formed.  Knock it out of the hostname.  */
//        memmove(percent, endp, strlen(endp)+1);
//        if (!data->state.this_is_a_follow)
//          /* Don't honour a scope given in a Location: header */
//          conn->scope = (unsigned int)scope;
//      } else
//        infof(data, "Invalid IPv6 address format\n");
//    }
//  }
//
//  if (data->set.scope)
//    /* Override any scope that was set above.  */
//    conn->scope = data->set.scope;
//
//  /*
//   * So if the URL was A://B/C,
//   *   conn->protostr is A
//   *   conn->host.name is B
//   *   data->state.path is /C
//   */
//  (void)rc;
//  return CURLE_OK;
// }
