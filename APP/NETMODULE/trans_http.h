/**
 @file  trans_http.h
 @brief 本文件定义了http协议传输类。
 */
#ifndef	__H_TRANS_HTTP_
#define	__H_TRANS_HTTP_

#include "IncludeMe.h"
#include "netHeaderDef.h"
#include "trans_abstrac.h"

#define	HTTP_HEAD_LEN_MAX		1024

#define	HTTP_TRANSFER_CHUNKED		1
#define	HTTP_TRANSFER_CONLENGTH		0


#define	HTTP_USE_POST_KEYWORD		0
#define	HTTP_USE_GET_KEYWORD		1



/**
 @class trans_http 
 @brief 实现http协议的传输。
 */
class trans_http : public trans_abstrac
{
private:
	char *m_pHttpHeadBuff;
	int m_transferType;
	int m_sendMode;

public:
	trans_http(Sending *p);
	virtual ~trans_http();

	/*!
	@brief 设置HTTP发送方式：POST或GET
	@param[in] mode HTTP_USE_POST_KEYWORD或者HTTP_USE_GET_KEYWORD
	*/
	void SetSendMode(int mode);

private:
	
	int sendData(UINT8 *psBuf, INT32 sLen, UINT8 *prBuf, INT32 *rLen, string &strErr);


	/*!
	@brief 接收并解析http包头
	@param[out] contentLen 解析Content-Length值，代表HTTP包内容长度（chunked模式输出为0）
	@return   接收到包头的字节数 
	*/
	int ReadHttpHead(INT32 *contentLen);

	/*!
	@brief 接收chunked方式下的HTTP包
	@param[out] revBuf 包内容
	@param[out] recContent_len 接收长度
	@return   NET_SUCCESS-成功；	其他值-失败 
	*/
	int RecvHttp_chunked(UINT8 *revBuf, INT32 *recContent_len, string &strErr);


	/*!
	@brief 设置http包头
	@param[in] msg POST或GET方式
	@param[in] file http路径
	@param[in] serverIP 服务器IP
	@param[in] serverPort 服务器端口
	@param[in] con_len HTTP包内容长度
	*/
	void sethttphead(char *msg,const char* file,const char * serverIP, const char *serverPort, UINT32 con_len);

	/*!
	@brief 获取包头中某字段值
	@param[in] sheader 包头BUFF指针
	@param[out] sKeyWord 字段名
	@return   >=0字段值	; <0无该字段     （返回值的定义似乎有问题问题！）
	*/
	int getValue(char *sheader,char *sKeyWord);
	/*!
	@brief 判断包头中某字段值是否存在
	@param[in] sheader 包头BUFF指针
	@param[out] sKeyWord 字段名
	@return   0-不存在	; 1-存在     
	*/
	int isExist(char *sheader,char *sKeyWord);

	/*!
	@brief 解析HTTP状态值
	@param[in] sheader 包头BUFF指针
	@return       状态值
	*/
	int getHttpState(char *sheader);
	/*!
	@brief 解析content-length值
	@param[in] sheader 包头BUFF指针
	@return   ontent-length值
	*/
	int getconlength(char* sheader);
	/*!
	@brief 解析resultType值
	@param[in] sheader 包头BUFF指针
	@return   resultType值
	*/
	int getserverstate(char * sheader);
	/*!
	@brief 解析是否为chunked状态
	@param[in] sheader 包头BUFF指针
	@return   HTTP_TRANSFER_CHUNKED或HTTP_TRANSFER_CONLENGTH
	*/
	int getTransferState(char* sheader);

};





#endif


