/**
 @file  netHeaderDef.h
 @brief 本文件定义网络模块所需的若干宏定义和数据类型。
 */
#ifndef	__H_NET_MACRO_DEF_
#define	__H_NET_MACRO_DEF_




//////////////////////////////////////////////////////////////////////////
//网络连接的相关定义
//////////////////////////////////////////////////////////////////////////

/** @brief 枚举类型，网络模式 */
enum ENUM_NET_MODE
{
	NET_NONE = 0,		/**< 无网络模式		*/
	NET_LAN = 1,		/**< 局域网		*/
	NET_ADSL,			/**< ADSL		*/
	NET_3G,				/**< 3G			*/
	NET_M2M_WMMP,		/**< M2M_Wmmp		*/
	NET_M2M_APN,		/**< M2M_APN		*/
	NET_VPN = 10,		/**< VPN二次拨号		*/
};

/**
 *@struct _IPinfo 
 *@brief 网络IP信息定义
 */
typedef struct _IPinfo{
    char IP[16];
    char Mask[16];
    char GeteWay[16];
    char DNS[16];
}IPinfo;



//错误类型定义
#define	NET_SUCCESS							0
#define	NET_CONNECT_FAIL					-1
#define	NET_INIT_FAIL						-2
#define	NET_MODE_ER							-3
#define	NET_OPERATOR_FILE_ER				-4
#define	NET_OPERATOR_DB_ER					-5
#define	NET_INPUT_IP_ER						-6
#define	NET_M2M_MODULE_ER					-7
#define	NET_NOT_SUPPORT_ER					-8
#define	NET_POINT_NULL_ER					-9
#define	NET_BUFF_OVERFLOW_ER				-10
#define	NET_TRANS_FAIL						-11
#define	NET_SOCKET_FAIL						-12
#define	NET_WMMP_FAIL						-13
#define	NET_AT_CMD_FAIL						-14
#define	NET_CONDITION_ER					-15
#define	NET_OTHER_ER						-99



//CNetManager中checkConnect的返回结果定义
#define	CONNECT_ON_L2						2				//第二层网络已连接
#define	CONNECT_ON_L1						1				//第一层网络已连接，第二层未连接或未开启
#define	CONNECT_OFF							0				//网络未连接


//performCheck()的返回值定义
#define	CHECK_OK							0				//查询结果：接入
#define	CHECK_NOT							1				//查询结果：未接入
#define	CHECK_OFF							-1				//查询结果：接入后断线


//是否执行了建立连接
#define HAVE_DIALED				1
#define NOT_DIALED				0



//LAN模式相关定义
#define	LAN_DHCP_ON				1
#define	LAN_DHCP_OFF			0



//M2M模式相关定义
#define SIMCOM                  1   /**< 1: 支持新的SIMCOM的M2M 0: 支持旧的WAVECOM的M2M*/





//////////////////////////////////////////////////////////////////////////
//数据传输的相关定义
//////////////////////////////////////////////////////////////////////////

//子模块
#define	TRANS_BY_TRANSPROC				1
#define	TRANS_BY_CURL					2

//应用层协议
#define	APP_LAYER_HTTP					1
#define	APP_LAYER_NONE					2

//数据入口与出口的形式
#define	DATA_IN_BUFF					0
#define	DATA_IN_FILE					1





//////////////////////////////////////////////////////////////////////////
//为配置参数赋默认值。
//如果netModuleConfig.h文件中没有对某个宏做定义，则编译时会使用默认值。
//移植时不要修改这部分定义。
//////////////////////////////////////////////////////////////////////////
#include "netModuleConfig.h"


//接入方式 
#ifndef _CONFIG_LAN_MODULE
#define	_CONFIG_LAN_MODULE					1
#endif
#ifndef _CONFIG_ADSL_MODULE
#define	_CONFIG_ADSL_MODULE					1
#endif
#ifndef _CONFIG_3G_MODULE
#define	_CONFIG_3G_MODULE					1
#endif
#ifndef _CONFIG_M2M_MODULE
#define	_CONFIG_M2M_MODULE					0
#endif
#ifndef _CONFIG_VPN_MODULE
#define	_CONFIG_VPN_MODULE					0
#endif

//传输方式
#ifndef _TRANS_MODULE_CURL
#define	_TRANS_MODULE_CURL					0
#endif

//多线程支持
#ifndef _TRANS_THREAD_SUPPORT
#define	_TRANS_THREAD_SUPPORT				1
#endif

//默认传输方式
#ifndef TRANS_MODULE_DEF
#define	TRANS_MODULE_DEF						TRANS_BY_TRANSPROC
#endif
#ifndef TRANS_APP_LAYER_DEF
#define	TRANS_APP_LAYER_DEF						APP_LAYER_HTTP
#endif
#ifndef TRANS_DATA_TYPE_DEF
#define	TRANS_DATA_TYPE_DEF						DATA_IN_BUFF
#endif

//HTTP默认路径
#ifndef SERVER_FILE_DEF
#define SERVER_FILE_DEF							"/Servlet"
#endif
#ifndef SERVER_TIME
#define SERVER_TIME								"/TimeServlet"
#endif

//WMMP 的一些默认参数
#ifndef WMMP_FLOW_DEFAULT
#define WMMP_FLOW_DEFAULT						2
#endif
#ifndef WMMP_TRANS_PACK_LEN
#define WMMP_TRANS_PACK_LEN						2048
#endif

//3G 的一些默认参数
#ifndef _3G_AT_BUFF_LEN
#define _3G_AT_BUFF_LEN							512 
#endif

//SOCKET的一些默认参数
#ifndef MAX_SOCKET_CONNECT_RETRY
#define MAX_SOCKET_CONNECT_RETRY				7			//建立SOCKET连接失败时的最大重试次数			
#endif
#ifndef MAX_SOCKET_TRANS_RETRY
#define MAX_SOCKET_TRANS_RETRY					8			//SOCKET通讯错误时的最大重试次数		
#endif
#ifndef SOCKET_SEND_PACKLEN
#define SOCKET_SEND_PACKLEN						2048		//每次SOCKET发送的最大包长
#endif
#ifndef SOCKET_RECV_PACKLEN
#define SOCKET_RECV_PACKLEN						1024		//每次SOCKET接收的最大包长
#endif

//数据发送与接收是否共用一个缓存区	1-共用缓存；0-不共用缓存区
#ifndef SINGLE_TRANSBUFF_MODE
#define SINGLE_TRANSBUFF_MODE					0
#endif

//默认通讯缓存大小
#ifndef DEF_SENDBUFF_LEN
#define DEF_SENDBUFF_LEN						(1024*20)
#endif
#ifndef DEF_RECVBUFF_LEN
#define DEF_RECVBUFF_LEN						(1024*20)
#endif

//默认数据传输时的临时文件
#ifndef DEF_REQ_FILE
#define	DEF_REQ_FILE						"XmlReqFile.xml"
#endif
#ifndef DEF_RSP_FILE
#define	DEF_RSP_FILE						"XmlRspFile.xml"
#endif

//“连接测试”功能的执行方式	1-SOCKET方式	2-APP方式（需根据具体应用修改NetManageMenu.cpp中代码）
#ifndef TEST_CONNECT_TYPE
#define TEST_CONNECT_TYPE						1			
#endif

//数据传输中的LOG开关
#ifndef _DBG_PRINT_TRANSDATA
#define	_DBG_PRINT_TRANSDATA					0
#endif






//////////////////////////////////////////////////////////////////////////
//版本兼容性定义
//////////////////////////////////////////////////////////////////////////
//#define	_DEBUG_VERSION			


//ProgressBar定义
#ifdef APE_GUI
#define	PROBAR_DEF()			CaProgressBar info("")
#define	PROBAR_SHOW(x)			{info.SetText(x);info.Show();}

#else
#define	PROBAR_DEF()
#define	PROBAR_SHOW(x)

#endif


//临时
#ifdef _DEBUG_VERSION
#define	POWOFF_ENABLE()
#define	POWOFF_DISABLE()
#define	MASK_ALARM_SIGNAL()
#define	UN_MASK_ALARM_SIGNAL()
#endif





#endif

