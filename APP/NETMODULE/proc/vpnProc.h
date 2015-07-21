#ifndef __VPN_CONNECT_
#define __VPN_CONNECT_

#include <stdio.h>
#include "comdatatype.h"

#ifdef __cplusplus
extern "C"
{
#endif


//VPN加密协议有关的定义
#define VPN_ENCRYPT_PAP			1
#define VPN_ENCRYPT_CHAP		2
#define VPN_ENCRYPT_PROTOCOL	VPN_ENCRYPT_PAP

#if (VPN_ENCRYPT_PROTOCOL==VPN_ENCRYPT_PAP)
#define	VPN_FILE_SECRET			"/etc/ppp/pap-secrets"
#endif
#if (VPN_ENCRYPT_PROTOCOL==VPN_ENCRYPT_CHAP)
#define	VPN_FILE_SECRET			"/etc/ppp/chap-secrets"
#endif

#define	VPN_FILE_XL2TPD			"/etc/xl2tpd/xl2tpd.conf"


//VPN错误类型定义
#define VPN_SUCCESS								0
#define VPN_PARA_ER								-1
#define VPN_CHANGE_XL2TP_ER						-2
#define VPN_CHANGE_SERCRET_ER					-3
#define VPN_READ_FILE_ER						-5
#define VPN_DISCONNECT_ER						-6
#define VPN_RUN_XL2TPD_ER						-7
#define VPN_READ_ROUTE_ER						-8
#define VPN_RUN_L2TP_ER							-9
#define VPN_DISCONNECT					    	-10

#define VPN_FILE_EOF							FILE_EOF





typedef struct{
	UINT8 Iface[8];
	UINT8 Destination[4];
	UINT8 Gateway[4];
	INT32 Flags;
}RouteInfo;





/**
 * brief 将s_VPN_IP、s_VPN_username、加密协议等信息写入VPN拨号脚本文件
 * @param void
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int change_xl2tp_script();

/**
 * brief 将给定参数放入s_VPN_IP、s_VPN_username、s_VPN_passwd
 * @param char *ip：VPN的IP地址；
 * @param char *userName：VPN的用户名；
 * @param char *passwd：VPN的密码；
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int setLocalVar(const char *ip, const char *userName, const char* passwd); 

/**
 * brief 将VPN参数写入脚本文件
 * @param char *ip：VPN的IP地址；
 * @param char *userName：VPN的用户名；
 * @param char *passwd：VPN的密码；
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int setVPN_para(const char *ip, const char *userName, const char* passwd);


//////////////////////////////////////////////////////////////////////////
// 以下为提供给上一层调用的接口
//////////////////////////////////////////////////////////////////////////

/**
 * brief 获取当前路由表中的信息，放入s_ori_gw_IP、s_ori_host_IP、s_ori_host_type几个数组
 * @param void
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int VPN_getOri_route();

/**
 * brief 删除当前路由信息，并将s_ori_gw_IP、s_ori_host_IP、s_ori_host_type中的路由信息写入路由表
 * @param void
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int VPN_setOri_route();

/**
 * brief 删除当前路由信息，并将s_VPN_IP中存放的路由信息写入路由表
 * @param void
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int VPN_setVPN_route(int netmode);

/**
 * brief 建立VPN连接
 * @param char *ip：VPN的IP地址；
 * @param char *userName：VPN的用户名；
 * @param char *passwd：VPN的密码；
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int VPN_connent(const char *ip, const char *userName, const char* passwd);

/**
 * brief 断开VPN连接
 * @return 成功：VPN_SUCCESS；失败：对应错误码
 */
int VPN_close(int netmode);



#ifdef __cplusplus
}
#endif

#endif

