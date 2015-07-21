#ifndef __SYS_ARG_MAC_H
#define __SYS_ARG_MAC_H

//系统参数ID号-当前使用模板标号 @1
#define SYS_CUR_TEMPLATE_NO				0

//系统参数ID号-缺省付款人名称 @个人
#define SYS_DEFAULT_PAYER_NAME			1

//系统参数ID号-串口1波特率 @9600
#define SYS_COM1_BAUD_RATE				2

//系统参数ID号-转换器专用串口2波特率 @9600
#define SYS_COM2_BAUD_RATE				3

//系统参数ID号-默认快捷销售方式 0:总价销售 1:临时价销售 2:数量销售 @2	
#define SYS_SALE_MODE					4   
  
/*! 系统参数ID号-手写板类型 0：胜创通无屏， 1：胜创通有屏*/
#define SYS_HW_TYPE						5	
/*! 系统参数ID号-双向打印参数 @3*/
#define SYS_BIDIRECTION_PRINT			6

//单张发票限额-  @1000000分
#define SYS_INV_SUM_LIMIT				10

//系统参数ID号-转换器串口波特率 @9600
#define ZHQ_COM_BAUD_RATE				11


//系统参数ID号-收款机初始化标志 0:未初始化 1:初始化
#define SYS_MACHINE_INIT_FLAG			50

//系统参数ID号-收款机初始化日期
#define SYS_INIT_DATE					51	

//系统参数ID号-抄报标志 1:已抄报 0:未抄报
#define SYS_INV_DELIVER_FLAG			52

//系统参数ID号-申报标志 1:已申报 0:未申报
#define SYS_DECLARE_FLAG				53	

//系统参数ID号-发票缴销标志 1:已申报 0:未申报
#define SYS_INV_REVOKE_FLAG				54	

//系统参数ID号-锁死期 */
#define SYS_BLOCKAGE_DATE				55	

//系统参数ID号-注销状态 1:已注销 0:未注销 */
#define SYS_UNREGISTER_FLAG				56	

//系统参数ID号-分机号
#define SYS_FJH							57

//系统参数ID号-上次获取系统消息的时间
#define	SYS_MSG_TIME			    	58

//系统参数ID号-上次更新权限日期
#define	SYS_INFO_UPDATE_DATE			59



//系统参数ID号-证书口令(初始化时使用)
#define	SYS_CERTIF_PSW			100

//系统参数ID号-金税盘口令
#define	SYS_DISK_PSW			101



#endif
