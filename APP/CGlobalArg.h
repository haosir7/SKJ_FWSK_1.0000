#ifndef __CGLOBAL_ARG_H
#define __CGLOBAL_ARG_H

#include "IncludeMe.h"
#include "CMachine.h"
#include "COperator.h"
#include "CInvVol.h"
#include "CUserInfo.h"
#include "SysMacDef.h"
#include "TemplateDataDesign.h"
#include "TDateTime.h"
#include "CUserMore.h"
#include "CInvKind.h"
#include "CPrnInfo.h"

class CGlobalArg;

/*! 系统常用参数变量类对象，整个应用程序共有一个 */
extern CGlobalArg *g_globalArg;


/**
 *@class CGlobalArg 
 *@brief 常用全局变量与系统参数
 *@brief 该类在程序启动以后，只会产生一个全局对象。
 *@brief 对象的成员变量包含应用程序常用的全局变量和系统参数。
 */
class CGlobalArg
{
public:

	/**
	 * brief 初始化公共参数值
	 * @param void
	 * @return UINT8 
	 */
	UINT8 InitGlobalArg(void);
	/**
	 * brief 计算申报期内的正票、红票累计金额；发票卷正红票累计金额
	 * @param void
	 * @return UINT8 
	 */
	UINT8 CalculateSum(void);

	/**
	 * brief 读取打印模板
	 * @param UINT8 *TemplateBuf 模板缓存区
	 * @param UINT8 TemplateNo 模板号
	 * @param UINT32 len 模板长度
	 * @return static UINT8 
	 */
	static UINT8 ReadPrintTemplate(UINT8 *TemplateBuf, UINT8 TemplateNo, UINT32 len);

	/**
	 * brief 检查是否需要进行日交易汇总
	 * @param UINT32 date
	 * @return UINT8 
	 */
//	UINT8 CheckDaySum(UINT32 date);

	/**
	 * brief 日交易汇总函数
	 * @param UINT32 uDate
	 * @return static UINT8 
	 */
//	static UINT8 DaySum(UINT32 uDate);

	/**
	 * brief 得到CGlobalArg的唯一对象
	 * @param void
	 * @return static CGlobalArg* 
	 */
	static CGlobalArg* GetInstance(void);

	/**
	 * brief 初始化各个系统值
	 * @param void
	 * @return UINT8 
	 */
	UINT8 InitSysArg(void);

	/**
	 * brief 显示消息框
	 * @param[in] strInfo  显示的消息信息
	 */
	void ShowMsg(string strInfo);

	/**
	 * brief 添加系统日志记录
	 * @param[in]  Eventtype 事件类型号
	 * @param[in]  EventResult  事件结果
	 * @param[in]  Backup 备注信息
	 * @return 1: SUCCESS, 0: FAILURE
	 */
	static UINT8 SysLog(UINT8 Eventtype, UINT32 EventResult, string Backup = "");

	/**
	 * brief 系统日志滚动
	 * @return 1: SUCCESS, 0: FAILURE
	 */
	UINT8 SysLogRoll();
	
	/**
	 * brief 判断是否超过缓存时间限制
	 * @param[in]  pEndDate  离线开票截止日期
	 * @return 1: SUCCESS（是）, 0: FAILURE（否）
	 */
	UINT8 IfExceedSvTime(TDateTime *pEndDate = NULL);

	/**
	 * brief 款机与转换器联机
	 */
	INT32 OnLineTest(string &strErr);

	


	//-----------------------------------
	// 系统参数区
	//-----------------------------------
	UINT8  m_initFlag;			/**< 初始化标志 */
	UINT32 m_forbidDaySum;		/**< 日交易封死标志 */
	UINT32 m_initDate;			/**< 机器初始化日期 */
	UINT32 m_updateDate;			/**< 更新权限日期 */
//	UINT8  m_invFormat;			/**< 发票版式 */
	UINT8  m_invDeliverFlag;	/**< 抄报标志 */
	UINT8  m_decFlag;			/**< 申报标志 */
//	UINT8  m_allDecFlag;		/**< 变更申报标志 */
	UINT32 m_startDecDate;		/**< 申报起始日期 */
	UINT32 m_decDate;			/**< 申报截止日期 */
//	UINT8  m_taxID[TAX_ID_NUM];	/**< 税种税目索引号 */

	UINT8  m_decMedium;			/**< 申报介质 */
	UINT8  m_bidirection;		/**< 双向打印参数 */
	UINT8  m_saleMode;			/**< 默认销售模式 */
	UINT8  m_decAlarm;			/**< 申报报警天数 */
	UINT8  m_curTemplateNo;		/**< 当前打印模板号 */

	
	UINT32  m_perReccount;    /**< 每卷张数*/
	
	string m_defaultPayerName;	/**< 默认付款人名称 */
	UINT32 m_com1BaudRate;		/**< 串口1波特率 */
	UINT32 m_com2BaudRate;		/**< 串口2波特率 */
	UINT32 m_zhqcomBaudRate;		/**< 转换器串口波特率 */
	UINT8  m_printer;			/**< 报表打印机 */
//	UINT32 m_dataTransMedium;	/**< 数据传输介质 */
	UINT8  m_itemNameLen;		/**< 商品名称最大长度 */
	UINT8  m_payerCorpLen;		/**< 付款人/收款人名称最大长度 */
	UINT8  m_netOn;            /**< 登录验证是否成功 1：成功，0：失败 */

	//-----------------------------------
	// 销售限额参数区
	//-----------------------------------
	INT64  m_normalSum;			/**< 申报期正票累计金额 */
	INT64  m_returnSum;			/**< 申报期红票累计金额 */
	INT64  m_normalVolSum;		/**< 发票卷正票累计金额 */
	INT64  m_returnVolSum;		/**< 发票卷红票累计金额 */
	
	//-----------------------------------
	// 公共参数区
	//-----------------------------------
	UINT8		m_bootStatus;	/**< 款机启动检测状态 */
	CMachine	*m_machine;		/**< 机器信息 */
	COperator	*m_operator;	/**< 操作员信息 */
	CInvVol		*m_curInvVol;	/**< 当前卷信息 */
	CUserInfo	*m_corpInfo;	/**< 纳税户信息 */
	CUserMore   *m_usermore;    /**< 纳税户补充信息*/
	CInvKind    *m_invKind;     /**< 票种信息*/
	CPrnInfo    *m_prnInfo;     /**< 打印票样信息*/
	UINT8        m_IngSegPoint;  /**< 决定做哪个号段操作，0：发票缴销，1：发票退回*/
	UINT8		m_sendInvFlag;	/**< 上传发票标志 */
	
	UINT8		m_declareLimitFlag;  /**< 申报期是否超过100天， 1：是；0：否 */
	string      m_pydm;        /**< 票样代码 */
	string      m_kindFlag;    /**< 同一类票源标示 ，是票样代码的中间2位 */
	string      m_sjbsfs;      /**< 数据报送方式 */
	UINT8       m_allowIssue;  /**< 是否有对应打印模板开票权限，1：是，0：否*/

	UINT8       m_threadIn;		/**< 允许进入次线程传送部分 0：不允许，1：允许*/
	string      m_strMsg;       /**< 中间件错误消息*/
	UINT8       m_ifMsg;        /**< 次线程是否有错误消息,0=无，1=有*/
	UINT32      m_InvServNum;        /**< 当前缓存发票数量 */
	INT64       m_InvServSum;		/**< 当前缓存发票金额 */


	// 模板方面
	TSaveTemplateInfo *pSaveTemplateInfo;   /**< 需保存的模板属性 */
	UINT8 *m_templateBuffer;	/**< 模板缓冲区 */
	
	// 请求码
//	INT8		*m_sq_wm;  /**< 请求码指针 */

	//金税盘访问
//	CJSKInfoFunc *m_jskInfoFunc;

	//-----------------------------------
	// 滚动参数区
	//-----------------------------------
	INT32 m_invCount;			/**< INV_HEAD表的当前记录条数 */
	INT32 m_rtInvCount;			/**< RT_INV表的当前记录条数 */
	INT32 m_invSumCount;		/**< INV_SUM表的当前记录条数 */

	INT32 m_clientCount;		/**< CLIENT表的当前记录条数 */
	INT32 m_operatorCount;		/**< OPERATOR表的当前记录条数 */
	INT32 m_pluCount;			/**< PLU表的当前记录条数 */

	//-----------------------------------
	// 税控设备参数区
	//-----------------------------------
	
	string  m_strZskl;          /**< 证书口令 */
	string  m_strSksbkl;        /**< 税控设备(金税盘)口令 */


	UINT8  m_pthreadFlag;		/**< 次线程运行标志 1：运行  0停止 */
	string m_pthreadErr;		/**< 次线程返回错误 */
	string m_pthreadNsrsbh;
	string m_pthreadJqbh;
	string m_pthreadSksbbh;
	string m_pthreadSksbkl;
	string m_pthreadFplxdm;
	string m_pthreadKpjh;
	string m_pthreadZskl;


private:
	/**
	 * CGlobalArg
	 * @return 
	 */
	CGlobalArg();

	/**
	 * ~CGlobalArg
	 * @return 
	 */
	~CGlobalArg();
	
	static CGlobalArg *m_globalArg;	/**< CGlobalArg对象，静态 */
	
};



/*! 系统常用参数变量类对象，整个应用程序共有一个 */
extern CGlobalArg *g_globalArg;


#endif
