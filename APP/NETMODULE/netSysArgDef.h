/**
 @file  netSysArgDef.h
 @brief 本文件定义网络模块所需的数据库SYSARG宏。
 */
#ifndef __NET_SYS_ARG_DEF_H
#define __NET_SYS_ARG_DEF_H


/*! 网络模式参数：0-无；1-局域网；2-ADSl；3-3g；4-M2MWMMP；5-M2MAPN */
#define SYS_NET_MODE						201	


/*! 第二层网络模式参数：0-无；10-VPN； */
#define SYS_NET_MODE_L2						202


/*! 局域网DHCP模式：0-DHCP关；1-DHCP开 */
#define SYS_LAN_DYNAMIC_IP					203

/*! 局域网IP */
#define SYS_LAN_IP							204

/*! 局域网子网掩码 */
#define SYS_LAN_MASK						205

/*! 局域网网关 */
#define SYS_LAN_GATEWAY						206

/*! 局域网DNS */
#define SYS_LAN_DNS							207



/*! ADSL 用户名 */
#define SYS_ADSL_USER						208

/*! adsl 密码 */
#define SYS_ADSL_PWD						209



/*! 3g 运营商 */
#define SYS_3G_CARRIER						210

/*! 3g 用户名 */
#define SYS_3G_USER							211

/*! 3g 密码 */
#define SYS_3G_PWD							212



/*! M2M 模块激活选项：0-不执行激活操作；1-执行激活操作 */
#define SYS_M2M_ACT_STATE					213

/*! M2M APN 接入点 */
#define SYS_M2M_APN_ACCPOINT				214

/*! M2M APN 号码 */
#define SYS_M2M_APN_NUMBER					215



/*! VPN 服务器IP */
#define SYS_VPN_SEVER_IP					216

/*! VPN 用户名 */
#define SYS_VPN_USER						217

/*! VPN 密码 */
#define SYS_VPN_PWD							218


/*! 开机自动拨号选项 0-自动VPN拨号；1-自动非VPN拨号；2-手动拨号*/
#define SYS_AUTO_DIAL						219



/*! 业务服务器IP */
#define SYS_APP_SERVER_IP					231

/*! 业务服务器PORT */
#define SYS_APP_SERVER_PORT					232

/*! 业务服务器的路径文件 */
#define SYS_APP_SERVER_FILE					233

/*! 时间服务器IP */
#define SYS_TIME_SERVER_IP					234

/*! 时间服务器PORT */
#define SYS_TIME_SERVER_PORT				235

/*! 时间服务器的路径文件 */
#define SYS_TIME_SERVER_FILE				236




#endif

