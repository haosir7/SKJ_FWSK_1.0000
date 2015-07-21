/*! \file    SaleData.h
   \brief    awe4000r销售程序中的销售数据类
   \author   Yu Yan
   \version  1.0
   \date     2007-11-28
 */


#ifndef SALE_DATA_H
#define SALE_DATA_H

#include "IncludeMe.h"
#include "SysMacDef.h"
 
#include "CMachine.h"
//#include "CPlu.h"
#include "CDept.h"
//#include "CInvSum.h"
#include "CInvHead.h"
#include "CInvDet.h"
#include "CInvVol.h"
#include "CUserInfo.h"
//#include "CInvcodeLink.h"
#include "CDB.h"
//#include "TemplateDataDesign.h"
//#include "TemplateGeneralFuc.h"
//#include "TemplateIndep.h"
//#include "TemplateInterface.h"
// #include "TemplateMacro.h"
//#include "printer.h"
#include "arithmetic.h"
#include "commonFunc.h"

#include "PrintDriver.h"
#include "CRtInv.h"
#include "SaleFunc.h"
#include "CGlobalArg.h"
#include "PowerOffData.h"
#include "PrintDriver.h"

class SaleData;

extern SaleData *pSaleData; /**< 销售数据类指针*/


/**
 *@enum err 
 *@brief 错误代码
 */
enum retType
{    //销售
	 SUCCEED = 1,				/**< 成功*/
	 NO_SUCH_GOODS = 2,			/**< 发票明细中未找到该商品*/
     EXCEED_PRICE,				/**< 单价超限*/
	 SHORT_OF_INV,				/**< 超过剩余发票份数*/
	 TAX_ID_ILLEGAL,			/**< 税种税目错误*/
	 NM_EXCEED,					/**< 单张开票金额超限*/
     NM_SUM_EXCEED,				/**< 正票累计金额超限*/
	 RM_SUM_EXCEED,				/**< 红票累计金额超限*/
	 NVM_SUM_EXCEED,			/**< 单卷正票累计金额超限*/
	 RVM_SUM_EXCEED,			/**< 单卷红票累计金额超限*/
	 MONEY_NOT_ENOUGH,			/**< 现金金额不足*/
     MONEY_EXCEED,				/**< 现金金额超限*/
	 MONEY_ZERO,				/**< 现金金额为零*/
	 DISCOUNT_MONEY_ZERO,        /**< 折扣金额为零*/
	 REDUCT_MONEY_SMALL,        /**< 折让金额过小*/
	 NO_GOODS,					/**< 发票明细中没有商品*/
	 NO_PAPER,					/**< 没有纸质发票*/
	 INV_ZERO,					/**< 没有电子发票*/
	 PRN_INV_ERROR,				/**< 开票失败*/
	 UPDATE_INV_VOL_ERROR,		/**< 更新INV_VOL表失败*/
	 REBATE_ERROR,				/**< 折扣率超限*/
	 GET_FISCAL_CODE_ERROR,		/**< 取税控码失败*/
	 DB_SAVE_ERROR,				/**< 存数据库失败*/
	 DB_FULL,					/**< 数据库已满*/
	 MALLOC_MEM_ERROR,          /**<申请动态内存失败*/
	 INV_SUM_ERROR,				/**<卷汇总失败*/
	 PRICE_NOT_ENOUGH,			/**<价格过小*/
	 EXCEED_NAME_LEN,			/**<付款人名称过长*/
	 EXCEED_AMOUNT,             /**<商品数量超限*/
	 ILLEGAL_AMOUNT,             /**<商品数量非法*/
	 NO_NAME,                   /**<未输入付款人名称*/
	 MUST_INPUT_SUM,            /**<必须输入总价*/
	 MUST_RE_LOGIN,             /**<必须重登录*/
	 NO_GOODS_CANCEL,           /**<无商品可取消*/
	 RETURN_GOODS_EXCEED,		/**<退货数量超限*/
	 HAVE_ROLLED,				/**<数据滚动过*/
	 REDUCT_EXCEED,				/**<折让金额超过商品行金额*/
	 NO_REDUCT,					/**<非普通商品行，无法折让*/
	 ONE_INV,                   /**<一张发票只允许4个商品行*/
	 NM_EXCEED_MIN,              /**<发票金额超下限*/
	 BX_RET_MONTH_ERR,           /**< 冲红票非本月以前*/
	 TAX_UNAUTHORIZED,				/**< 税率未授权*/


	 //发票管理
     WASTE_NUM_EXCEED,          /**< 超过剩余发票份数*/
	 WASTE_NUM_ERROR,           /**< 作废份数不合理*/
	 RETURN_NUM_ERROR,          /**< 红票号码不合理*/
	 QUERY_ERROR,               /**< 查询该张发票失败*/
	 HAVE_RT_INV,				/**< 已开过红票*/
	 HAVE_FU_INV,				/**< 该票已充红*/
	 NO_INV,                    /**< 非本机已开发票*/
	 RT_INV,                    /**< 红票*/
	 WT_INV,                    /**< 废票*/
	 CODE_NO_ERROR,             /**< 发票代码非法*/
	 PRE_INV_ERROR,             /**< 查询上笔交易号码失败*/
	 NO_DIST_INV ,              /**< 当前0卷发票未导入*/
	 NO_MATCHED_INV,            /**< 无匹配的发票存根*/
	 NO_START_INV,				/**< 未输入发票起始号码*/
	 NO_END_INV,				/**< 未输入发票截止号码*/
	 NO_ISSUED_INV,				/**< 退本卷未开的发票*/
	 NO_ISSUED_VOL,				/**< 退非本卷未开发票 */
	 CODE_INV_NO,                  /**< 发票代码  */
	 NO_INV_NO,                 /**< 发票号码  */
	 DIF_PYCODE,                /**< 票样代码不同 */
	 NO_INV_ERROR,              /**< 发票起始号大于发票截止号 */

	 WASTE_INV_STUB,			/**< 作废发票不能打印电子存根*/
	 NO_NET,					/**< 未建立网络连接 */
	 INV_LIB_ERR,			/**< 中间库返回错误信息	*/

	 //广西网票
	 GX_SV_NUM_EXCEED,			/**< 超过缓存张数*/
	 GX_SV_SUM_EXCEED,			/**< 超过缓存金额*/
	 GX_SV_TIME_EXCEED			/**< 超过缓存时间*/
	 
};

//销售属性
#define REBATE  1				/**< 折扣*/
#define REDUCTION  2			/**< 折让*/
#define SERVICE  3				/**< 加成，服务费*/

//销售状态
#define WORK_INCOMPLETE	 0		/**< 销售进行中*/
#define WORK_COMPLETE	 1		/**< 销售完成  */
#define WORK_ERROR	 	 2		/**< 销售错误  */

#define MINIMUM_PRECISION 0.0000000001  /**< 比较浮点数是否为0的基准*/
#define BASE_PRICE  0.01				/**< 判断零时价格是否有值的基准*/

#define CANCEL_PRODUCT 1				/**< 取消商品*/
#define MAX_GOODS_LINE 56              /**< 一次性允许开具的商品行数上限*/

/**
 *@class SaleData 
 *@brief 销售数据
 */
class SaleData
{
public:
				
	UINT8 m_workState;						  /**< 销售状态			*/

	UINT8  m_firstGoodFlag;                   /**< 第一个商品标志，不定长模式下   */
	UINT8  m_inputClientNameFlag;             /**< 输入付款人标志  不定长模式下*/
	UINT8  m_reductFlag;					  /**< 加成标志	??考虑删除		*/
	UINT8  m_serviceFeeFlag;				  /**< 服务费标志  ??考虑删除	*/

	double m_tmpPrice;						  /**< 临时价格			*/
	INT64  m_tmpSum;						  /**< 临时金额			*/
	double m_tmpAmount;						  /**< 临时商品数量		*/
	UINT8  m_tmpDept;						  /**< 临时部类			*/
	double m_tmpRate;						  /**< 临时百分率		*/
    UINT8  m_tmpGoodsNum;                     /**< 临时商品行个数   */
	string m_tmpClient;                       /**< 临时付款人名称   */
	INT64  m_tmpMoneySum;					  /**< 临时单张票累计金额*/
	
	UINT8  m_tmptypeCode[INV_TYPE_CODE_LEN];  /**< 发票代码（BCD）*/
	UINT32 m_tmpInvNo;

	UINT8 m_saleMode;						  /**< 销售模式，总价、临时价，数量	*/
	UINT8 m_invtype;                          /**< 开票类型，正、废、红票  */

//	class CMachine *m_macheineInfo;			  /**< 机器信息				*/
	UINT32 m_maxAllowCount;					  /**< 每张发票最大允许商品行数量	*/
	UINT32 m_maxAllowSum;					  /**< 最大允许金额			*/

//	class CPlu  *m_pluInfo;                   /**< PLU信息	*/
	class CDept *m_deptInfo;                  /**< 部类信息 */
//	class CInvSum *m_volSumInfo;              /**< 单卷发票汇总信息*/
				
   // IssuedInvoice *m_issuedInvoiceData;       /**< 执行开票命令发送的数据结构*/
	TPrnInvoiceInfo *m_pInvPrnData;			  /**< 发票打印数据				*/

	UINT8 m_fiscalCode[INV_TAX_CTRL_CODE_LEN+1];/**< 发票税控码				*/ 

	class CInvHead *m_singleInvInfo;	      /**< 单张发票信息	*/
	class CInvHead *m_smallInvInfo;	          /**< 单张发票信息,定长发票拆分出的发票	*/
				
	UINT8 m_detailCount;					  /**< 商品行总数量（多张发票）	*/
											  /**< 不定长此值和单张发票信息中的数量一致*/
											  /**< 定长涉及到多张发票时，此值为总数量 */
													
	UINT32 m_goodsSum;						  /**< 商品行累计金额			*/

    UINT8 m_cancel;                           /**< 取消标志         */
	double m_realCancelNum;                   /**< 实际取消商品数量    */
	UINT8 m_property;                         /**< 商品是否有折扣，加成等 */
	class CInvDet *m_invDet;                  /**< 商品明细*/
	class CInvDet *m_pHead;                   /**< 保存发票中链表的头指针 */
	class CInvDet m_cancelDet;                /**< 取消的商品明细 */
	
	double m_saveAmount;					  /**< 四舍五入后的商品数量 */
	INT32  m_dotNum;						  /**< 四舍五入后数量小数位数 */

	UINT32 m_oldInv;                          /**< 被退的发票号码 */
//	UINT32 m_oldCodeNo;                       /**< 被退的发票代码关联号 */
	string m_oldCodeNo;						  /**< 被退的发票代码 */
	UINT8  m_returnOther;                     /**< 是否退非本机发票 1=是，0=否 */

	UINT32  m_nInvCount;                      /**< 发票打印需要的张数 */ 
public:
	/*!
	@brief 构造函数	
	*/
	SaleData();

	/*!
	@brief 析构函数	
	*/
	~SaleData();

    /*!
	@brief 释放部类销售中所开辟的内存	
	@param[in] nPointerNum     指针个数
	@return  1: SUCCESS 
	*/
 //    UINT8 DelDeptSalePointer(UINT8 nPointerNum);

	/*!
	@brief 初始化发票头中的成员变量	
	@return  1: SUCCESS 
	*/
	UINT8 InitInvHead();
    
	/*!
	@brief 初始化销售数据中的成员变量
	@param[in] set 是否设置部分变量 1：设置， 0：不设置
	@return  1: SUCCESS 
	*/
	UINT8 InitSaleData(UINT8 set);

//------------------------------
	
	/*!
	@brief 销售主流程	
	@param[in] pluInfo      保存PLU信息的对象指针 
	@return  1 成功； 0 失败
	*/
	UINT8 Sale(CDept *deptInfo);

    /*!
	@brief 根据部类号搜索商品信息	
	@param[in] nDeptNo        部类号 
	@return  1 成功； 0 失败
	*/
  	UINT8 SearchGoodsByDept(INT32 nDeptNo);

	/*!
	@brief 根据PLU搜索商品信息	
	@param[in] PLUCode      PLU码
	@param[out] pluInfo      保存PLU信息的对象指针
	@return  1 成功； 0 失败
	@attention  pluInfo不等于NULL。
	*/
//	UINT8 SearchGoodsByPLU(string PLUCode, CPlu *pluInfo);
    
	/*!
	@brief 扫描商品行	
	@param[in] flagMakeInv  标记是否开票, 1：开票，0：不开票
	@return  1： 成功； 其它：失败
	*/
    UINT8 ScanGoodsLine(UINT8 flagMakeInv);	

	/*!
	@brief 开票句柄	
	@param[in] nIfPrn 是否打印发票，1=打印，0=不打印
	@param[in] nId 子过程ID号
	@return  1： 成功； 其它：失败
	*/
	UINT8 MakeInvoiceHandle(UINT8 nIfPrn, UINT8 nId);
    
    /*!
	@brief 比较两个浮点数是否相等
	@return  1： 相等； 0：不相等
	*/
    UINT8 doublecmp(double d1, double d2);
    
	 /*!
	@brief 计算税额
	 含税金额  nJe= 10*100
	   税率    nSl= 0.17*100
	@return  税额 1.45*100
	*/
//	INT64 CountTax(double fJe, double fSl);
	INT64 CountTax(INT64 nJe, INT32 nSl);
	/*!
	@brief 组装打印的发票头
	@return  1： 成功； 0：失败
	*/
	UINT8 ComposePrnInv();

	/*!
	@brief 更新发票领用信息表中剩余发票份数
	@return  1： 成功； 0：失败
	*/
	UINT8 UpdateVolRemain(UINT32 remain);
    
	/*!
	@brief 计算当前商品行所需发票数目
	@return  所需发票数目
	*/
	UINT32 CalculateInvNum();

    /*!
	@brief 组装为获得税控码所发送的数据结构
	@param[in] smallInv 当前发票信息
	@param[out] pIssuedInvData  为获得税控码所发送的数据结构
	@return  1: SUCCESS; 0: FAILURE
	*/
//	UINT8 ComposeIssuedInvData(CInvHead *smallInv, IssuedInvoice *pIssuedInvData);
    
	/*!
	@brief 统计一张发票某税种税目的开票总金额
	@param[in] CodeNo 发票代码关联号
    @param[in] InvNo  发票号
	@param[out] TaxID  税种税目号
	@return  一张发票对应TaxID的开票总金额
	*/
//	INT64 GetTaxIDMoneySUM(CInvDet * invDet, UINT32 CodeNo, UINT32 InvNo, UINT8 TaxID);
	
    /*!
	@brief 获得防伪税控码
	@param[in] pIssuedInvData  为获得税控码所发送的数据结构
	@param[out] errorcode 错误代码
    @param[out] fiscalCode 税控码指针
	@return  1: SUCCESS; 0: FAILURE
	*/
//	UINT8 GetFiscalCode(IssuedInvoice *pIssuedInvData, string fpdm, UINT8 *fiscal_code);
	/*!
	@brief 获得防伪税控码
	@param[in] pInv  为获得税控码所发送的发票数据结构
	@param[out] fiscalCode  税控码
	@param[out] strErr 错误信息
    @param[out] nRetCode 读盘的错误号
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 GetFiscalCode(CInvHead *pInv,UINT8 *fiscalCode, UINT32 &nRetCode, string &strErr);
    
   	/*!
	@brief 卷汇总
	@param[out] 是否执行了卷汇总操作 1：是； 0：否
	@return  1: SUCCESS; 0: FAILURE
	*/
	//UINT8 InvSum(UINT8 &nIfSum);

	/*!
	@brief 根据PLU码删除商品
	@param[in] strCode        PLU码 
	@param[in] nCancelNum     删除商品数量 
	@param[out] dPrice        删除商品价格
	@return  1: SUCCESS; 非1  错误代码
	*/
	UINT8 CancelGoodsByPLU(string &strCode, double nCancelNum, double &dPrice);

	/*!
	@brief 根据部类号码删除商品
	@param[in] dpNo        部类号码 
	@param[in] nCancelNum     删除商品数量 
	@param[out] dPrice        删除商品价格
	@return  1: SUCCESS; 非1  错误代码
	*/
	UINT8 CancelGoodsByDept(string dpNo, double nCancelNum, double &dPrice);

	/*!
	@brief 填充打印发票结构体的发票头
	@param[out] invFullInfo 发票头信息结构体指针
	@param[in] smallInvInfo 发票整体信息指针
	@return  1： 成功； 其它：失败
	*/
	UINT8 InvClassToStuct(struct TInvFullInfo *invFullInfo, CInvHead *smallInvInfo);

	/*!
	@brief 部分数据库内容滚动
	@param[out] nIfRolled 是否实施了滚动操作 1：是，0：否
	@return  1： 成功； 其它：失败
	*/
	UINT8 RollData(UINT8 &nIfRolled);


//---------------------------按键响应函数
    /*!
	@brief 部类销售	
	@param[in] nDeptNo        部类号 
	@param[in] strCode        商品编码
	@return  1 成功；非1  错误代码
	*/
//	UINT8 DeptSale(string nDeptNo);
	UINT8 DeptSale(UINT32 nDeptNo=0, string strCode="");

	/*!
	@brief PLU销售	
	@param[in] strName       临时商品名称 
	@param[in] strErr       错误信息
	@return  1 SUCCESS；0：FAILURE（错误信息见strErr）；其它：错误代码
	*/
	UINT8 PLUSale(string strName, string &strErr);
    
	/*!
	@brief 现金结账	
	@param[out] IfInvSum   是否进行了卷汇总 1：是；0：否 
	@return  1 成功；非1  错误代码
	*/
	UINT8 PayByCash(UINT8 &IfInvSum);

    /*!
	@brief 打折扣	
	@param[in] fRate      折扣率 
	@return  1 成功；非1  错误代码
	*/
	UINT8 Discount(double fRate);

	/*!
	@brief 临时价输入	
	@param[in] dPrice      临时价 
	@return  1 成功；非1  错误代码
	*/
	UINT8 PriceInput(double dPrice);

	/*!
	@brief 总价输入	
	@param[in] dSum     总价 
	@return  1 成功；非1  错误代码
	*/
	UINT8 SumInput(double dSum);

	/*!
	@brief 付款人输入	
	@param[in] chfkdw     付款人字符串指针 
	@return  1 成功；非1  错误代码
	*/
	UINT8 ClientNameInput(const char *chfkdw);
	/*!
	@brief 付款人及付款人税号同时输入	
	@param[in] chfkdw     付款人字符串指针 
	@param[in] chfkdwsh     付款人税号字符串指针 
	@return  1 成功；非1  错误代码
	*/
	UINT8 ClientNameInput(const char *chfkdw, const char *chfkdwsh);

	/*!
	@brief 付款人输入	
	@param[in] dSum     付款人字符串指针 
	@return  1 成功；非1  错误代码
	*/
	UINT8 ClientCodeInput(const char *content, string &strErr);


	/*!
	@brief 备注输入	
	@param[in] content     字符串指针 
	@return  1 成功；非1  错误代码
	*/
	UINT8 RemarksInput(const char *content, string &strErr);

	/*!
	@brief 商品取消	
	@return  1 成功；非1  错误代码
	*/
	UINT8 CancelGoods(void);

	/*!
	@brief 撤销商品取消	
	@return  1 成功；非1  错误代码
	*/
	UINT8 NoCancelGoods(void);

	/*!
	@brief 乘法销售	
	@return  1 成功；非1  错误代码
	*/
	UINT8 Plus(double tmpAmount);

	/*!
	@brief 整单取消	
	@return  1 成功；非1  错误代码
	*/
	UINT8 TotalCancel(void);

	/*!
	@brief 折让
	@param[in] money 折让掉的金额
	@return  1 成功；非1  错误代码
	*/
	UINT8 GoodsReduct(double money);


	/*!
	@brief 离线开票3个条件是否满足
	@param[in] nSum 开票金额（正数）
	@param[out] strErr 错误信息
	@return  1 SUCCESS；0  FAILURE
	*/
	UINT8 IsOffLineOk(INT64 nSum, string& strErr);

	
};

#endif
