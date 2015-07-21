#ifndef __SYSMACDEF_H
#define __SYSMACDEF_H

#include "VersionConfig.h"

#if (CORP_NAME_LEN_TPYE == 1)
#define CORP_NAME_LEN					200		//企业名称长度
#define PAYER_NAME_LEN					200		//付款单位名称长度
#else
#define CORP_NAME_LEN					80		
#define PAYER_NAME_LEN					80		
#endif

#define TAX_ID_NUM						6		//税种税目总数
#define MAX_INV_ITEM_NUM				32
#define TAX_CODE_LEN					20		//企业税号长度
#define GOODS_NAME_LEN					20		//商品名称长度
#define GOODS_CODE_LEN					16//13		//商品编码长度
#define CLENT_CODE_LEN                  3           //客户编码长度
#define INV_TAX_CTRL_CODE_LEN			20//32//8		//发票税控码长度
#define INV_TYPE_CODE_LEN				6//10		//发票代码长度(BCD)
#define DAY_SUM_SIGN_LEN				128		//日汇总电子签名长度hex
#define INV_VOL_MAC_LEN					4		//发票领用卷信息MAC长度
#define DECLARE_MAC_LEN					4		//申报MAC(BCD)的长度
#define DEC_SIGN_LEN					128		//申报电子签名长度hex
#define OPERATOR_NAME_LEN				10		//操作员名称长度
#define SOFT_VER_LEN					18		//软件版本长度
#define HARD_VER_LEN					18		//硬件版本长度
#define FCARD_NO_LEN					8		//税控卡编号(BCD)长度
#define FCARD_PWD_LEN					8		//税控卡口令(BCD)长度
#define CORP_NO_LEN						8		//纳税人编码(BCD)长度
#define MACHINE_NO_LEN					16		//机器编号(ASC)长度
#define CORP_ADDR_LEN					100		//企业地址长度
#define CORP_PHONE_LEN					24		//企业电话长度

#define CORP_SBH_LEN					30		//纳税人识别号
#define CORP_DZDAH_LEN					20		//纳税人电子档案号
#define CORP_HYDM_LEN					6		//纳税人行业代码
#define CORP_HYMC_LEN                   200      //纳税人行业名称
#define CORP_JYXMZWMC_LEN				200		//纳税人经营项目中文名称
#define CORP_SWJGDM_LEN					40		//纳税人税务机关代码
#define CORP_ZGSWGY_LEN					20		//纳税人主管税务官员
#define CORP_KPGLY_LEN					50		//开票管理员
#define CORP_KPGLY_PWD_LEN				8		//开票管理员密码
#define CORP_KPMSMC_LEN					20		//开票模式名称
#define CORP_JSPBH_LEN					12		//金税盘编号
#define CORP_QYLX_LEN					16		//企业类型
#define CORP_QYSJ_LEN					14		//启用时间
#define CORP_ZFBZ_LEN					8		//主分机  

//发票种类信息
#define INV_KIND_CODE_LEN                20       //发票种类代码
#define INV_KIND_NAME_LEN                 70       //发票种类名称         

#define PRICE_EXTENSION					100 	//价格放大倍数
#define SUM_EXTENSION					100		//金额放大倍数
#define DEPT_TAX_EXTENSION				100 	//商品税率放大倍数

#define NORMAL_INV						1		//正常发票
#define RETURN_INV						2		//已退发票
#define WASTE_INV						3		//作废发票
#define WASTE_NOR						4       //正废
#define WASTE_RET                       5       //负废
#define RET_MANUAL_INV                  6       //负数手工发票（退货）
#define	RET_SPECIAL_INV                 7       //负数特殊发票


#define SYSTEM_ADMINISTRATOR			1		//系统管理员
#define DIRECTOR_OPERATOR				2		//主管操作员
#define NORMAL_OPERATOR					3		//普通操作员
#define DEMO_OPERATOR					4		//学习操作员
#define SYSTEM_OPERATOR_ID				200		//系统管理员ID
#define DIRECTOR_OPERATOR_ID			0		//主管操作员ID
#define DEMO_OPERATOR_ID				99		//学习操作员ID

#define PRINT_TEMPLATE_NUMBER			12		//打印模板个数
#define CLEAN_VOL_THRESHOLD             600

#define TRADE_NO_LEN					6		/**< 纳税人行业代码 */
#define TRADE_NAME_LEN					40		/**< 纳税人行业名称*/
//#define BANK_NO_LEN						50		/**< 开户银行帐号 */
//#define BANK_NAME_LEN					80		/**< 开户银行名称 */
//#define INDUS_REG_NO_LEN				60		/**< 工商登记号 */
#define INDUS_REG_ADDR_LEN				100		/**< 登记注册地址 */

//票样打印
#define SALE_REMARKS_LEN                     80  //备注长度
//#define SELF_DEF_TAB_LEN                   30  //自定义标签长度
//#define SELF_DEF_CONT_LEN                  30  //自定义内容长度

//款机升级版 付款方识别号默认值
#define   PAYER_NAME_DEF                         "000000000000000"     //付款方默认识别号

/*! 款机启动检测结果 */
#define BOOT_NO_CARD					1
#define BOOT_CARD_NOT_MATCH				2
#define BOOT_FISCAL_PIN_ERR				3
#define BOOT_DAY_SUM_ERR				4
#define BOOT_NO_TEMPLATE				5
#define BOOT_INVALID_TEMPLATE			6
#define BOOT_CARD_ERR					7

/* printer .c .h 所需补充 (ZL) */
#define FORWARD_LINES			8			//前向走纸行数
#define REPORT_LINE_SPACE		12			//行间距1
#define REPORT_LINE_SPACE2		5			//行间距2

//#define GOODS_CODE_LEN					16		//商品编码长度
#define MACHINE_NO_LEN					16		//机器编号(ASC)长度
#define FKDW_NAME_LEN					PAYER_NAME_LEN		//付款单位名称长度
#define INV_GOODS_MAX_COUNT				6        //MAX_NUM_LIST_SINGLE 发票商品行最大行数
#define TAX_ITEM_MAX_COUNT				6		//当前使用的税种税目最大个数
#define TAX_ITME_CODE_LEN				4		//税种税目编码长度



//SYSLOG 事件类型
#define EVENT_DOWNLOAD_MACHINE_NO		0x00    /**< 下载机器编号*/
#define EVENT_LOGIN_SYSTEM				0x01	/**< 登录系统*/
#define EVENT_UPDATE_SYSTEM				0x02	/**< 更新系统*/
#define EVENT_UPDATE_TEMPLATE			0x03	/**< 更新模板*/
#define EVENT_POWER_OFF					0x04	/**< 掉电*/
#define EVENT_PULL_CARD					0x05	/**< 拔卡*/
#define EVENT_ADD_OPERATOR				0x06	/**< 增加操作员*/
#define EVENT_DEL_OPERATOR				0x07	/**< 删除操作员*/
#define EVENT_EDIT_OPERATOR				0x08	/**< 编辑操作员*/
#define EVENT_INVOICE_ROLL				0x09	/**< 发票信息滚动*/
#define EVENT_VOLUME_ROLL				0x0A	/**< 发票卷信息滚动*/
#define EVENT_SYSLOG_ROLL				0x0B	/**< 系统日志滚动*/
#define EVENT_UPDATE_MACHINE_INFO		0x0C	/**< 更新机器信息*/
#define EVENT_ILLEGAL_USER_CARD			0x0D	/**< 非法用户卡*/
#define EVENT_ILLEGAL_FISCAL_CARD		0x0E	/**< 非法税控卡*/
#define EVENT_ILLEGAL_MANAGE_CARD		0x0F	/**< 非法管理卡*/
#define EVENT_ILLEGAL_PIN				0x10	/**< 非法PIN码*/
#define EVENT_INITIALIZATION			0x11	/**< 初始化*/
#define EVENT_DISTRIBUTE				0x12	/**< 发票分发*/
#define EVENT_MAKE_INVOICE				0x13	/**< 开正票*/
#define EVENT_RTINV_ROLL				0x14	/**< 红票信息表滚动*/
#define EVENT_SET_TIME					0x15	/**< 设置时间*/
#define EVENT_DECLARE_TAX				0x16	/**< 保税*/
#define EVENT_UPDATE_CORP				0x17	/**< 完税*/
#define EVENT_CHECK_TAX					0x18	/**< 稽查*/
#define EVENT_UPDATE_USER_INFO			0x19	/**< 更新用户信息*/
#define EVENT_IMPORT_INVOICE			0x1A	/**< 发票导入*/
#define EVENT_POWEROFF_INITMACH			0x1B	/**< 掉电保护：初始化机器*/
#define EVENT_POWEROFF_FPFF				0x1C	/**< 掉电保护：发票分发*/
#define EVENT_POWEROFF_FPDR				0x1D	/**< 掉电保护：发票导入*/
#define EVENT_POWEROFF_INV				0x1E	/**< 掉电保护：开票*/
#define EVENT_POWEROFF_ROLL				0x1F	/**< 掉电保护：滚动*/
#define EVENT_POWEROFF_DECLARE			0x20	/**< 掉电保护：保税*/
#define EVENT_POWEROFF_PAY_TAX			0x21	/**< 掉电保护：完税*/
#define EVENT_MAKE_RET_INVOICE			0x22	/**< 开红票*/
#define EVENT_MAKE_WAS_INVOICE			0x23	/**< 开废票*/

#define EVENT_DATABASE_LOCKED			0x30	/**< 数据库已锁*/
#define EVENT_DATABASE_PREPARE_ER		0x31	/**< 调用sqlite3_prepare()返错*/
#define EVENT_DATABASE_FILEDTYPE_ER		0x32	/**< sqlite3_bind时有错误*/
#define EVENT_DATABASE_STEP_ER			0x33	/**< 调用sqlite3_step()返错*/
#define EVENT_DATABASE_FINALIZE_ER		0x34	/**< 调用sqlite3_finalize()返错*/
#define EVENT_DATABASE_EXEC_ER			0x35	/**< sqlite3_exec()返错*/


//机器编码下载 相关
#define PRODUCT_TYPE		0		/**< 1：机器类型只有1位  0：机器类型为2位*/

#if PRODUCT_TYPE == 1
#define PRODUCT_TYPE_CODE                0x20       /**< 机器类型*/
#else 

#if (POS_TYPE == POS_APE3000R)
#define PRODUCT_TYPE_CODE_BYTE1          0x00       /**< 机器类型 字节1*/
#define PRODUCT_TYPE_CODE_BYTE2          0x07       /**< 机器类型 字节2*/

#elif (POS_TYPE == POS_APE4000R)
#define PRODUCT_TYPE_CODE_BYTE1          0x00       /**< 机器类型 字节1*/
#define PRODUCT_TYPE_CODE_BYTE2          0x09       /**< 机器类型 字节2*/

#elif (POS_TYPE == POS_APE4020R)
#define PRODUCT_TYPE_CODE_BYTE1           0x02       /**< 机器类型 字节1*/
#define PRODUCT_TYPE_CODE_BYTE2           0x00       /**< 机器类型 字节2*/

#elif (POS_TYPE == POS_APE5020R)
#define PRODUCT_TYPE_CODE_BYTE1          0x02       /**< 机器类型 字节1*/
#define PRODUCT_TYPE_CODE_BYTE2          0x01       /**< 机器类型 字节2*/

#endif
#endif

#define MACHINE_S_VER                    "0.0"       /**< 机器类型*/

//MAC地址下载 相关 by yy 20120524
#define MAC_LEN	                 18	 /**< 存储MAC地址的缓存长度*/

//公共配置区域
#if (RELEASE_VER == 1)
#define AUTOISSUE 				0				/**< 1 = 容量测试开启， 0 = 容量测试关闭*/
#else
#define AUTOISSUE 				1		
#endif

#define DAYSUM_LIMIT_DAYS       2000			/**< 机器允许开票的天数*/
#define DEC_LIMIT_DAYS          100				/**< 机器允许申报的最长天数*/
#define SG_MONEY_LMT            0XFFFFFFFFU     /**< 单张开票限额默认值*/
#define ISSUE_END_DATE          20990101		/**< 开票截止日期默认值*/
//#define MAX_MONEY               0XFFFFFFFFU		/**< 金额最大限额*/
#define MAX_MONEY_A              1000000000	    	/**< 金额限额*/
#define MAX_MONEY               MAX_MONEY_A*100		/**< 金额最大限额*/
#define DEFAULTE_VOL_NUM        100           /**< 每卷默认发票张数（字符串形式，便于控件显示）*/



//ProgressBar定义
#define	BAR_DEF()			CaProgressBar info("")
#define	BAR_SHOW(x)			{info.SetText(x);info.Show();}



#define ISSUE_TYPE  0 /**< 0 = 简单版的自动开票，1 = 15所容量测试的自动开票*/
 
//#endif

//记录滚动 相关
#define  INV_HEAD_DEL_MAX   3000//5000  /**< INV_HEAD滚动时删除的最大记录数 */
#define  INV_HEAD_MAX		20000//35000  /**< INV_HEAD滚动表容纳最大记录数	 */
#define  INV_SUM_DEL_MAX   500  /**< INV_SUM滚动时删除的最大记录数 */
#define  INV_SUM_MAX		1000  /**< INV_SUM滚动表容纳最大记录数	 */
#define  RT_INV_DEL_MAX   500  /**< RT_INV滚动时删除的最大记录数 */
#define  RT_INV_MAX		1000  /**< RT_INV滚动表容纳最大记录数	 */
#define  SYSLOG_DEL_MAX   2000  /**< RT_INV滚动时删除的最大记录数 */
#define  SYSLOG_MAX		20000  /**< RT_INV滚动表容纳最大记录数	 */

#define PLU_MAX        5000		/**< RT_INV滚动表容纳最大记录数	 */
#define CLIENT_MAX     50		/**< RT_INV滚动表容纳最大记录数	 */
#define OPERATOR_MAX   100		/**< RT_INV滚动表容纳最大记录数	 */

//返回移动平台的错误码
#define M2M_NO_INV        0x11 //无发票
#define M2M_EXCD_SV_TIME  0x13 //超过缓存时间
#define M2M_EXCD_SV_SUM   0x14 //超过缓存金额
#define M2M_IMEI_LEN	16

//-----3G相关 by yy 20120529
//3G网卡类型
#define EVDO_CARD			1		//电信
#define UNICOM_CARD			2		//联通

#endif
