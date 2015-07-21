/*
 * 网络模块配置头文件
 * 设置网络模块各参数。根据应用工程的具体需求配置。
*/

#ifndef	__H_NET_MODULE_CONFIG_
#define	__H_NET_MODULE_CONFIG_



//////////////////////////////////////////////////////////////////////////
//编译开关
//////////////////////////////////////////////////////////////////////////
//接入方式
#define	_CONFIG_LAN_MODULE					1
#define	_CONFIG_ADSL_MODULE					1
#define	_CONFIG_3G_MODULE					1
#define	_CONFIG_M2M_MODULE					1
#define	_CONFIG_VPN_MODULE					0

//传输方式
#define	_TRANS_MODULE_CURL					0

//多线程支持
#define	_TRANS_THREAD_SUPPORT				1




//////////////////////////////////////////////////////////////////////////
//配置项
//////////////////////////////////////////////////////////////////////////

//默认传输方式
#define	TRANS_MODULE_DEF						TRANS_BY_TRANSPROC
#define	TRANS_APP_LAYER_DEF						APP_LAYER_HTTP
#define	TRANS_METHOD_DEF						SOCKET_TYPE
#define	TRANS_DATA_TYPE_DEF						DATA_IN_BUFF


//HTTP默认路径
#define SERVER_FILE_DEF						"/inside/ReceiveServlet"
#define SERVER_TIME							"/wlkpsl/servlet/TimeService"


//WMMP 的一些默认参数
#define WMMP_FLOW_DEFAULT						2
#define WMMP_TRANS_PACK_LEN						2048


//3G 的一些默认参数
#define _3G_AT_BUFF_LEN							512 



//SOCKET的一些默认参数
#define MAX_SOCKET_CONNECT_RETRY				7			//建立SOCKET连接失败时的最大重试次数			
#define MAX_SOCKET_TRANS_RETRY					8			//SOCKET通讯错误时的最大重试次数		
#define SOCKET_SEND_PACKLEN						2048		//每次SOCKET发送的最大包长
#define SOCKET_RECV_PACKLEN						1024		//每次SOCKET接收的最大包长



//设置数据发送与接收是否共用一个缓存区	1-共用缓存；0-不共用缓存区
#define SINGLE_TRANSBUFF_MODE					0


//默认通讯缓存大小（不使用默认缓存时可设为1）
#define DEF_SENDBUFF_LEN					(1024*10)

#if (SINGLE_TRANSBUFF_MODE==0)
#define DEF_RECVBUFF_LEN					(1024*20)
#else
#define DEF_RECVBUFF_LEN						1		//此模式无需单独设置接收缓存大小
#endif



//默认数据传输时的临时文件
#define	DEF_REQ_FILE						"XmlReqFile.xml"
#define	DEF_RSP_FILE						"XmlRspFile.xml"



//“连接测试”功能的执行方式	1-SOCKET方式	2-APP方式（需根据具体应用修改NetManageMenu.cpp中代码）
#define TEST_CONNECT_TYPE						1			



//数据传输中的LOG开关
#define	_DBG_PRINT_TRANSDATA					1


#endif


