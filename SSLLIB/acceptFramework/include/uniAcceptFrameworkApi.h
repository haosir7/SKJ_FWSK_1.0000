#if !defined(__AISINO_UNIACCEPTFRAMEWORKAPI__H__)
#define __AISINO_UNIACCEPTFRAMEWORKAPI__H__


#ifndef TYPE_HCRYPTPROV
#define TYPE_HCRYPTPROV 1

typedef void * HCRYPTPROV;
#endif

#ifndef BASETYPES
#define BASETYPES

#ifndef WIN32 
#undef  __stdcall
#define __stdcall
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef TYPE_HINSTANCE
#define TYPE_HINSTANCE 1
typedef void * HINSTANCE;
#endif 

#ifndef TYPE_DWORD
#define TYPE_DWORD 1

typedef char *LPTSTR;
typedef char *LPSTR, *PSTR;
typedef unsigned char UCHAR;

typedef unsigned int       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef char				*LPTSTR;
typedef const char			*LPCTSTR;

#define DECLARE_HANDLE(x) typedef WORD x
#endif

#define INET_SERVICE_HTTPS 1
#define INET_SERVICE_HTTP 2
#define INVALID_PORT_NUMBER 0


#ifdef MAX_PATH
#undef MAX_PATH
#endif
#define MAX_PATH          260

#endif  /* !BASETYPES */



#define UN_INITIALIZE  -100000000
#define SESSION_INVALIDATE_OR_NEED_REAUTH  -960010000

#define AES_ENCRYPT_OUT_BUFFER_TOO_SHORT  -960001010
#define OUT_BUFFER_TOO_SHORT  -960001011
#define AES_ENCRYPT_FILE_FAILURE  -960001020
#define AES_DECRYPT_FILE_FAILURE  -960001030

#define COMPRESS_PARA_EMPTY  -960001040
#define COMPRESS_ERROR  -960001041
#define COMPRESS_BEGIN_NEW_MEM  -960001042
#define AUTH_UNEXCEPTED_EXCEPTION  -960001043
#define COMPRESS_EXCEPTION   -960001049




#define DECOMPRESS_PARA_EMPTY  -960001050
#define DECOMPRESS_ERROR  -960001051
#define DECOMPRESS_BEGIN_NEW_MEM  -960001052

#define DECOMPRESS_EXCEPTION   -960001059



#define SHA_EXCEPTION   -960001060

#define PEM_ENCODE_FILE_FAILURE   -960001061
#define PEM_DECODE_FILE_FAILURE   -960001062
#define PEM_SOURCE_FILE_INVALID   -960001063
#define PEM_TARGET_FILE_INVALID   -960001064
#define PEM_MOVE_FILE_FAILURE   -960001065
#define PEM_DELETE_TARGET_FILE_FAILURE   -960001066
#define PEM_DELETE_SOURCE_FILE_FAILURE   -960001067

#define PEM_ENCODE_FAILURE   -960001069
#define PEM_DECODE_FAILURE   -960001070
#define GENGRATE_RANDOM_EXCEPTION  -960001071
#define SERVER_ENCRYPT_CERT_DECODE_PEM  -960001072
#define SERVER_ENCRYPT_CERT_EMPTY  -960001073
#define UA_OPEN_BIG_FILE -960001075
#define UA_OPEN_FILE_SHA1 -960001076

#define UA_OPEN_FILE -960001080
#define UA_READ_FILE -960001081

#define PARSE_COMMUNITY_MSG_TECH_IS_EMPTY -960001082
#define RESPONSE_DIGEST_LENGTH_NOT_EQUAL -960001083
#define RESPONSE_DIGEST_VALIDATE_FAILURE -960001084
#define PARSE_RESPONSE_TECH_MSG_FAILURE -960001086
#define DECRYPT_BIZ_MSG_FAILURE -960001087
#define RESPONSE_COMMUNITY_MSG_EMPTY_INVALID -960001088

#define NOT_SUITABLE_RESPONSE_CODE -960001090
#define UA_OPEN_FILE_WRITE -960001091
#define UA_OPEN_CONFIG_NOT_EXISTS -960001092
#define UA_OPEN_CONFIG_FILE -960001093

#define UA_OPEN_CA_AGENT_CONFIG_NOT_EXISTS -960001094
#define UA_AGENT_NOT_LOAD -960001095
#define UA_AGENT_NOT_SIGN -960001096
#define UA_AGENT_NOT_ENCRYPT -960001097
#define UA_AGENT_NOT_CERT_INFO -960001098
#define UA_AGENT_NOT_ISSUER -960001099

#define UA_HTTP_OPEN_REQUEST -960001100
#define UA_HTTP_GET_CONNECTION -960001101
#define UA_HTTP_OPEN_REQUEST_EXCEPTION -960011005
#define UA_HTTP_REQUEST_SEND -960011010
#define UA_HTTP_RECV_BUFFER_TOO_SMALL -960011015
#define UA_HTTP_DOWNLOAD_CREATE_FILE -960011020
#define UA_HTTP_GET_DOWNLOAD_FILE_LEN -960011025
#define UA_HTTP_UPLOAD_FILE_LEN_ZERO -960011026

#define UA_HTTP_DOWNLOAD_FILE_LEN_INCONSISTENT -960011030
#define UA_HTTP_VISIT_RESPONSE_EXCEPTION -960011035
#define UA_HTTP_VISIT_CONNECT_EXCEPTION -960011036

#define UA_HTTP_VISIT_RESPONSE_STATUS -960021000


#define PF_RZ_FTP_OPEN_CONN -960011040
#define PF_RZ_FTP_GET_FILE -960011041
#define PF_RZ_FTP_PUT_FILE -960011045
#define UA_HTTP_INVALID_URL -960011050
#define UA_HTTP_RESPONSE_NO_COOKIE -960011051
#define UA_HTTP_ERROR_MESSAGE_LENGTH_TOO_SHORT -960011055

#define UA_PARSE_RESPONSE_BIZ_MESSAGE	-960011056

#define UA_PARSE_REQEUST_TECH_SSL -960011057
#define UA_REQEUST_TECH_MESSAGE_EMPTY -960011058

#define UA_HTTP_RESPONSE_TOO_SHORT -960011060

#define UA_PARSE_REQEUST_TECH_MESSAGE -960011061
#define UA_PARSE_REQEUST_COMM_MESSAGE -960011062
#define UA_PARSE_RESPONSE_SERVER_CERT -960011063
#define UA_PARSE_RESPONSE_SERVER_RANDOM -960011064
#define UA_PARSE_RESPONSE_SESSION_ID -960011065

#define UA_PARA_RESPONSE_BUFFER_TOO_SHORT -960011067

#define CLIENT_NO_FOUND_ENCRYPT_ALGORITHM_EXCEPTION -960011068
#define REQUEST_FILE_NOT_EXISTS -960011070
#define REQUEST_FILE_CONTENT_EMPTY -960011071
#define MEM_ALLOC_FAILURE -960011072
#define GET_TEMP_DIR_FAILURE -960011073
#define GET_TEMP_FILE_NAME_FAILURE -960011074
#define RSA_ENCRYPT_FAILURE -960011080
#define RSA_ENCRYPT_BUFFER_TOO_SHORT -960011081
#define RSA_KEY_INVALID -960011082

#define UA_PARSE_REQEUST_TECH_URI -960011083
#define UA_PARSE_CONFIG_SERVICE_URI -960011084
#define UA_SERVICE_URI_EMPTY -960011085
#define UA_CLIENT_GBK_TO_UTF8 -960011086


#define UA_INIT_SSL -960011087
#define UA_SSL_OPEN_DOWNLOAD_FILE -960011089
#define UA_SSL_UPLOAD_FILE_FAILURE -960011090
#define UA_SSL_RECV_BUFFER_TOO_SHORT -960011091

#define UA_MISSING_BIG_FILE_TOKEN -960011092
#define UA_SSL_CONNECT_TIMEOUT -960011093

#define UA_NO_FOUND_ANY_SMART_KEY_CERT -960011100
#define UA_NO_FOUND_ANY_SMART_KEY_SLOT  -960011101
#define UA_NO_FOUND_ANY_SMART_KEY_TOKEN  -960011102
#define UA_SMART_KEY_ENUM_CERT  -960011103
#define UA_SMART_KEY_ENUM_CERT_NO_ANY  -960011104

#define UA_SMART_KEY_LOGIN  -960011105
#define UA_SMART_KEY_GEN_TMP_CONFIG  -960011106
#define UA_SMART_KEY_LOAD_PKCS11_DRIVER  -960011107
#define UA_SMART_KEY_PKCS11_DRIVER_NOT_EXISTS  -960011108
#define UA_SMART_KEY_SET_ENV_FAILURE  -960011109
#define UA_SMART_KEY_LOAD_ENCRYPT_ENGINE  -960011110

#define UA_SMART_KEY_PROPERTY_NOT_EXISTS  -960011111
#define UA_SMART_KEY_ENGINE_NOT_EXISTS   -960011112
#define UA_SMART_KEY_P11_NOT_EXISTS   -960011113
#define CONNECT_TEST_FAILURE   -960011114
#define UA_SMART_KEY_LOGIN_NO_ENGINE_CONFIG_FILE   -960011115

#define UA_AUTH_CLIENT_HELLO   -960032010
#define UA_AUTH_SERVER_HELLO   -960032015
#define UA_AUTH_CLIENT_AUTH   -960032020
#define UA_AUTH_SERVER_AUTH   -960032030

#define UA_SMART_KEY_LOGIN_PWD_ERROR   -960032031
#define UA_SMART_KEY_LOGIN_PWD_LOCK   -960032032

/*插入的税控设备同存根联录入的不是同一个，不能正常签名*/
#define UA_SMART_KEY_INSERT_NOT_SAME   -960032033

#define UA_SMART_KEY_CRYPT_API_LIB_NOT_FOUND   -960032034

#define UA_SMART_KEY_CERT_DECODE_LIB_NOT_FOUND   -960032035
#define UA_SMART_KEY_NOT_FOUND_LIB_API   -960032036

#define UA_SMART_NOT_SUPPORTED_PWD_DIALOG   -960032037

#define UA_AUTH_SERVER_RESPONSE_PROXY_FORMAT   -960032040
#define UA_READ_TRUST_CA_CERT   -960032041

#define UA_AUTH_OPEN_KEY   -960031000
#define UA_AUTH_CIPHER_SERVER_RESPONSE_CODE   -960033000
#define UA_CLIENT_SIGN_CODE   -960034000
#define UA_CLIENT_VERIFY_SIGN_CODE   -960035000
#define UA_AUTH_CODE   -960036000
/************************************************************************/
/* 0	成功
1	操作被取消
2	内存分配错误
3	句柄无效
4	数据指针错误
5	输入数据指针错误
6	输出数据指针错误
7	输入数据长度错误
8	输出数据长度错误
9	数据长度错误
10	数据格式错误
11	函数参数错误
12	函数调用失败
13	文件未找到
14	文件打开失败
15	文件读失败
16	文件写失败
17	模块装入失败
18	对象未找到
19	超时
21	数据库错误
22	许可证错误
23	密钥容器不存在
32	加密模块未初始化
33	加密模块内部错误
34	加密模块不存在
35	加密模块未打开
36	密钥容器不存在
37	未登录
38	已登录
39	签名验证失败
40	签名错误
41	分块长度错误
42	设备运行错误
43	无效的密钥
44	算法不匹配
45	私钥未找到
46	私钥已存在
47	密钥未创建
52	解编码时出错
53	解编码时给定的编码数据太短
54	非数字串
55	数据标志错误
56	解编码数据格式错误
57	密码算法错误
58	密码算法OID错误
60	函数参数错误
62	无证书
63	无效的证书
64	密钥文件写错误
65	密钥文件读错误
66	密钥文件已捐坏
67	密钥文件不存在
81	身份认证失败
82	非法用户
83	非法IP来源
84	数据包校验错误
85	数据包长度错误
86	数据包格式错误
87	无效的命令码
88	无效的命令参数
93	加密设备运行错误
95	网络通信错误
96	证书不存在
97	证书已作废
98	证书已挂失
99	证书已注销
100	证书未生效
101	证书已过期
102	证书已停用
103	证书状态未知
104	证书验证错误
105	证书已损坏
106	证书已存在
107	证书状态标记无效
108	操作权限不足
109	未经过身份认证
110	未建立连接
111	数据错误
112	认证数据已过期
113	认证服务器忙
114	CA未被信任
128	LDAP未初始化
129	对象已经存在
130	对象未找到
131	LDAP初始化失败
138	查找证书失败
139	查找黑名单失败
140	LDAP连接失败
141	OCSP连接失败
142	OCSP未初始化
143	发送至OCSP失败
144	接收自OCSP失败
145	接收自OCSP的数据包验证错误
160	端口号错误
161	操作失败
163	USB故障/冲突/通讯错误
164	卡片应答错误
165	卡片故障
166	卡片错误/锁死
167	未插USBKEY
168	口令错误
169	安全状态不满足
170	输入BUF错
171	卡片超时错误
176	卡片非法
181	无效的新口令
182	无效的口令
183	卡未初始化
187	没有安装USBKEY驱动                                 */
/************************************************************************/





#define UA_SSL_CONNECT_ERROR -960011120
#define UA_COULDNT_CONNECT  -960011121

#define UA_SIMPLEX_SSL  -960011130
#define UA_PATCH_COPY_P11  -960011140


#define PF_RZ_ENCRYPT_INIT -900010070
#define PF_RZ_ENCRYPT_UPDATE -900010071
#define PF_RZ_ENCRYPT_FINAL -900010072


#define PF_RZ_DECRYPT_INIT -900010075
#define PF_RZ_DECRYPT_UPDATE -900010076
#define PF_RZ_DECRYPT_FINAL -900010077


//  __attribute ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif



/**
* 
* @param requestTechnologyMsg
* @param requestTechnologyMsgLength
* @param requestBizMessage
* @param requestBizMessageLength
* @param responseBizMessage
* @return
*/
int  aisino_ssl_transfer_call(char* authCode, 
							char *requestTechnologyMsg,int requestTechnologyMsgLength, 
							unsigned char *requestBizMessage, int requestBizMessageLength, 
							unsigned char *responseBizMessage, int *responseBizMessageLength, unsigned char *errorMessage);

/**
* 
* @param requestTechnologyMsg
* @param requestTechnologyMsgLength
* @param requestBizMessage
* @param requestBizMessageLength
* @param responseBizMessageFilePath
* @param errorMessage
* @return
*/
int  aisino_ssl_transfer_call_responseFile(char* authCode, 
										 char *requestTechnologyMsg, int requestTechnologyMsgLength, 
										 unsigned char *requestBizMessage,  int requestBizMessageLength, 
										 unsigned char *responseBizMessageFilePath, unsigned char *errorMessage);

/**
* 
* @param requestTechnologyMsg
* @param requestTechnologyMsgLength
* @param requestBizMessageFilePath
* @param responseBizMessage
* @return
*/
int  aisino_ssl_transfer_call_requestFile(char* authCode, 
										char *requestTechnologyMsg, int requestTechnologyMsgLength, 
										unsigned char *requestBizMessageFilePath, 
										unsigned char *responseBizMessage,int *responseBizMessageLength, unsigned char *errorMessage);
/**
* 
* @param requestTechnologyMsg
* @param requestTechnologyMsgLength
* @param requestBizMessageFilePath
* @param responseBizMessageFilePath
* @param errorMessage
* @return
*/
int  aisino_ssl_transfer_call_file(char* authCode, 
												  char *requestTechnologyMsg, int requestTechnologyMsgLength, 
												  unsigned char *requestBizMessageFilePath, unsigned char *responseBizMessageFilePath, 
												  unsigned char *errorMessage);


int  aisino_download_file(char *downloadURL, char *saveDir,unsigned char *errorMessage);


int  aisino_Sign_pkcs1(char* keyPassword,unsigned char *pOrg, int cbOrg, 
								unsigned char *pOutSignData, int *cbOutSignData, int isVerifySign,int isSignResultDoBase64Encoded);

int  aisino_Sign_pkcs7_VerifyNSRSBH(char* keyPassword,const char* nsrsbh,unsigned char *pOrg, int cbOrg, 
								unsigned char *pOutSignData, int *cbOutSignData, int isVerifySign,int isSignResultDoBase64Encoded);

int  aisino_Sign_pkcs7(char* keyPassword,unsigned char *pOrg, int cbOrg, 
									unsigned char *pOutSignData, int *cbOutSignData, int isVerifySign,int isSignResultDoBase64Encoded);


int acceptFrameworkCleanup();
//int  aisino_change_key_password(char* pOldPassword,int oldPasswordLen,char *pNewPassword,int newPasswordLen);


#ifdef __cplusplus
}
#endif


#endif