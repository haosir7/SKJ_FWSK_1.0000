#ifndef PRN_PRINTER_H
#define PRN_PRINTER_H


#include "SysMacDef.h"
#include "comdatatype.h"
#include "TemplateDataDesign.h"
#include "PowerOffData.h"
#include "TemplatePrintInv.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define SHIDA 		1
#define PRINT_INV_DEFAULT 	0

#define MAX_CHAR_NUM		40


extern INT8 *pGeneralPrnBuff;

/*-----------------------------------------------------------------------------
发票打印数据  
打印内容：
1-发票代码
2-发票号（打印）
3-机器编号
4-收款单位
5-税号
6-开票日期
7-收款人名称
8-付款单位（个人）
9-商品名称（循环项）
10-单价  （循环项）
11-数量(规格) （循环项）
12-金额 （循环项）
13-金额合计（小写）
14-金额合计（大写）
15-税控码 
注：退票要打“退”字
              
------------------------------------------------------------------------------*/
PACK_PREFIX struct TInvDetailPrnData
{
	UINT8   Property;                    /*明细行性质 HEX              1   */
	                                     /*1-一般商品行，2-折扣行          */
	                                     /*3-带折扣的商品行,4-服务费行     */
	UINT8   GoodsCode[GOODS_CODE_LEN+1]; /*商品编码   14 ASC               */
	UINT8   GoodsName[GOODS_NAME_LEN+1]; /*商品名称   20 ASC               */
	UINT8   TaxItem;                     /*税种税目索引号              1   */
	UINT32  Amount;                      /*商品数量                    4   */
	UINT32  Price;                       /*单价                        4   */
	UINT32  Sum;                         /*金额                        4   */
};

PACK_PREFIX struct TInvPrnData
{
	UINT8         PrintType;					/*打印类型             	
												NORMAL_PRINT 0  正常打印
												STUB_PRINT 1	存根打印*/
		
	UINT8         TypeCode[INV_TYPE_CODE_LEN]; /*发票代码由 10 BCD             */
	UINT32        InvNo;                       /*发票号码                      */
	UINT8         MachineCode[MACHINE_NO_LEN+1];/*开票机编号 16 ASC             */
	UINT8         Type;                        /*开票类型                      */
	                                           /* 01：正票  02：退票   03：废票*/
	UINT8         CorpName[CORP_NAME_LEN+1];   /*收款单位名称 40 ASC           */
	UINT8         TaxCode[TAX_CODE_LEN+1];     /* 税号 20 ASC                  */ 
	UINT32        Date;                        /*开票日期 CCYYMMDD             */
	UINT32        TotalSum;                    /*开票总金额 单位：分           */
	UINT32        PosInvNo;                    /*原发票号码                    */
	UINT8         ClientName[FKDW_NAME_LEN+1]; /*付款人 40 ASC                 */
	UINT8         OperatorID;                  /*开票员编号HEX                 */
	UINT8         OperatorName[OPERATOR_NAME_LEN+1];/*开票员姓名，10 ASC       */
	UINT8         TaxCtrCode[INV_TAX_CTRL_CODE_LEN];/*税控码 由8位HEX          */ 
	UINT8         InvDetailCount;              /* 商品行数量                   */
	struct TInvDetailPrnData InvDetailPrnData[INV_GOODS_MAX_COUNT]; /*6个商品行,结构数据 */        	
	
};



/*--------------------------------------------------------------------------
单卷汇总信息：
1－单位名称
2－税号
3－机器号（16位的机器编号）
4－开票起始时间
5－开票中止时间
6－发票代码
7－发票起始号　
8－发票终止号  
9－正常发票份数
10－正常发票开具金额
11－废票份数
12－退票份数
13－退票金额
----------------------------------------------------------------------------*/
PACK_PREFIX struct TVolSumPrnData
{
	
	UINT8        CorpName[CORP_NAME_LEN+1];     /*单位名称 40 ACS 最多20个汉字  */
	UINT8        TaxCode[TAX_CODE_LEN+1];       /* 税号 20 ASC                  */ 
	UINT8        MachineCode[MACHINE_NO_LEN+1]; /*开票机编号 16 ASC             */
	UINT32       StartDate;                     /*开票起始日期   YYYYMMDD       */
	UINT32       EndDate;                       /*开票截至日期   YYYYMMDD       */
	UINT8        TypeCode[INV_TYPE_CODE_LEN*2+1];   /*发票代码   10 BCD             */
	UINT32       StartInvNo;                    /*发票起始号码                  */
	UINT32       EndInvNo;                      /*发票中止号码                  */
	UINT32       PosInvCount;                   /*正常发票份数                  */
	UINT32       PosInvSum;                     /*正常发票金额                  */
	UINT32       WasteInvCount;                 /*废票份数                      */
	UINT32       ReInvCount;                    /*退票份数                      */
	UINT32       ReInvSum;                      /*退票金额                      */
};
/*----------------------------------------------------------------------------
时间段汇总：
1－单位名称
2－税号
3－机器号（16位的机器编号）
4－开票开始时间
5－开票中止时间
6－正常发票份数
7－正常发票开具金额
8－废票份数
9－退票份数
10－退票金额
----------------------------------------------------------------------------*/
PACK_PREFIX struct TDateSegSumPrnData
{
	UINT8        CorpName[CORP_NAME_LEN+1];     /*单位名称 40 ACS 最多20个汉字  */
	UINT8        TaxCode[TAX_CODE_LEN+1];       /* 税号 20 ASC                  */ 
	UINT8        MachineCode[MACHINE_NO_LEN+1]; /*开票机编号 16 ASC             */
	UINT32       StartDate;                     /*开票起始日期   YYYYMMDD       */
	UINT32       EndDate;                       /*开票截至日期   YYYYMMDD       */
	UINT32       PosInvCount;                   /*正常发票份数                  */
	UINT32       PosInvSum;                     /*正常发票金额                  */
	UINT32       WasteInvCount;                 /*废票份数                      */
	UINT32       ReInvCount;                    /*退票份数                      */
	UINT32       ReInvSum;                      /*退票金额                      */	
//	UINT8        TaxItem[TAX_ITEM_MAX_COUNT];   /*税种税目序列hex               */
//	UINT32       PosInvSumByTax[TAX_ITEM_MAX_COUNT]; /*正常发票金额                  */
//	UINT32       ReInvSumByTax[TAX_ITEM_MAX_COUNT];  /*退票金额                      */	
};


/*--------------------------------------------------------------------------- 
日交易汇总报表
1－单位名称
2－税号
3－机器编号
4－开票时间
5－正常发票份数
6－废票份数
7－退票份数
8－税种税目（循环 总共6种）
9－正常发票开具金额 （循环）
10－退票金额 （循环）
--------------------------------------------------------------------------*/
PACK_PREFIX struct TDaySumInfoPrnData
{

	UINT8        CorpName[CORP_NAME_LEN+1];     /*单位名称 40 ACS 最多20个汉字  */
	UINT8        TaxCode[TAX_CODE_LEN+1];       /* 税号 20 ASC                  */ 
	UINT8        MachineCode[MACHINE_NO_LEN+1]; /*开票机编号 16 ASC             */
	UINT32       Date;                          /*开票日期   YYYYMMDD           */
	UINT32       PosInvCount;                   /*正常发票份数                  */
	UINT32       WasteInvCount;                 /*废票份数                      */
	UINT32       ReInvCount;                    /*退票份数                      */
	UINT32		 PosInvSum;						/*正常发票金额                  */
	UINT32		 ReInvSum;						/*退票金额                      */	
//	UINT8        TaxItem[TAX_ITEM_MAX_COUNT];   /*税种税目序列hex               */
//	UINT32       PosInvSum[TAX_ITEM_MAX_COUNT]; /*正常发票金额                  */
//	UINT32       ReInvSum[TAX_ITEM_MAX_COUNT];  /*退票金额                      */	
};


/*---------------------------------------------------------------------------
申报信息报表：
1－单位名称
2－税号
3－机器号
4－申报起始日期
5－申报截至日期
6－正票份数
7－退票份数
8－废票份数
9－税种税目（循环 总共6种）
10－正常发票开具金额 （循环）
11－退票金额 （循环）
----------------------------------------------------------------------------*/
PACK_PREFIX struct TDeclareSumPrnData
{
	UINT8        CorpName[CORP_NAME_LEN+1];     /*单位名称 40 ACS 最多20个汉字  */
	UINT8        TaxCode[TAX_CODE_LEN+1];       /* 税号 20 ASC                  */ 
	UINT8        MachineCode[MACHINE_NO_LEN+1]; /*开票机编号 16 ASC             */
	UINT32       StartDate;                     /*申报起始日期   YYYYMMDD       */
	UINT32       EndDate;                       /*申报截至日期   YYYYMMDD       */
	UINT32       PosInvCount;                   /*正常发票份数                  */
	UINT32       WasteInvCount;                 /*废票份数                      */
	UINT32       ReInvCount;                    /*退票份数                      */
	UINT8        TaxItem[TAX_ITEM_MAX_COUNT];   /*税种税目序列hex               */
	UINT32       PosInvSum[TAX_ITEM_MAX_COUNT]; /*正常发票金额                  */
	UINT32       ReInvSum[TAX_ITEM_MAX_COUNT];  /*退票金额                      */	
};


/*---------------------------------------------------------------------------
 机器信息：
1－机器编号
2－生产日期
3－硬件版本号
4－软件版本号
 ----------------------------------------------------------------------------*/
PACK_PREFIX struct TMachinePrnData 
{
	UINT8    MachineNo[MACHINE_NO_LEN+1];      	/* 机器编号ASC         16       */  
	UINT32   MakeDate;		                    /* 生产日期             4       */  
	UINT8    SoftVer[SOFT_VER_LEN +1];	        /* 软件版本ASC         18       */
	UINT8    HardVer[HARD_VER_LEN +1];  	    /* 硬件版本ASC         18       */
	UINT8    IMEI[M2M_IMEI_LEN+1];					/*无线IMEI号 */
	UINT8    IMEIFlag;
};

/*---------------------------------------------------------------------------
发票限额:
1-单位名称
2-税号
3-开票机号编号
4-单张发票开票金额限额
5-发票累计金额限额
6-退票累计金额限额
-----------------------------------------------------------------------------*/
PACK_PREFIX struct TInvLimitPrnData
{
	UINT8    CorpName[CORP_NAME_LEN+1];         /*单位名称 40 ACS 最多20个汉字  */
	UINT8    TaxCode[TAX_CODE_LEN+1];           /* 税号 20 ASC                  */
	UINT8    MachineNo[MACHINE_NO_LEN+1];      	/* 机器编号 ASC        16       */  
	UINT32   SigleInvLimit;                     /*单张发票限额                  */
	UINT32   PosInvTotalLimit;                  /*正常发票累计限额              */  
	UINT32   ReInvTotalLimit;                   /*退票累计限额                  */
};

/*----------------------------------------------------------------------------
 企业信息
1-税控卡编号
2-税控卡使用口令
3-应用启用日期
4-应用有效日期
5-纳税人名称
6-纳税人编码
7-税号
8-主管分局代码
9-申报方式
10-开票截止日期
11-单张发票开票金额限额
12-发票累计金额限额
13-退票累计金额限额
---------------------------------------------------------------------------- */
PACK_PREFIX struct TCorpInfoPrnData
{
	UINT8  Nsrsbh[CORP_SBH_LEN+1];				/**< 纳税人识别号 */
	UINT8  Nsrmc[CORP_NAME_LEN+1];				/**< 纳税户名称 */
	UINT8  Nsrswjgdm[CORP_SWJGDM_LEN+1];		/**< 主管税务机关代码 */
	UINT8  Nsrswjgmc[CORP_JYXMZWMC_LEN+1];		/**< 主管税务机关名称 */
	UINT8  Jspsbh[CORP_JSPBH_LEN + 1];			/**< 金税盘设备号 */		
	UINT8  Zfbz[CORP_ZFBZ_LEN];					/**< 主分机标志： 0代表主机 1代表分机 */
	UINT8  Qylx[CORP_QYLX_LEN + 1];				/**< 企业类型 */
	UINT8  Qysj[CORP_QYSJ_LEN + 1];				/**< 启用时间 */
};
/*----------------------------------------------------------------------------
票种信息
1-发票种类代码
2-发票种类名称
3-发票最小开具金额
4-发票最大开具金额
---------------------------------------------------------------------------- */
PACK_PREFIX struct TInvKindInfoPrnData
{
	UINT8  InvKindCode[INV_KIND_CODE_LEN+1];	/**< 发票种类代码 */
	INT64 MaxSign;		/**< 单张发票限额 */	
	UINT32 Lxssr;			/**< 离线锁死日（每月的第几天） */
	UINT32 Lxkjsj;		/**< 离线开具时间（单位小时） */
	INT64  InvMaxMoney;		/**< 发票最大开具金额 */
};

/*------------------------------------------------------------------------
 财务报表：
1-起始日期
2-中止日期
3-折扣
4-服务费
5-退货
6-现金入账
7-现金出账
8-现金支付
9-信用卡支付
10-外汇支付
11-签单支付
12-营业额
 --------------------------------------------------------------------------*/
PACK_PREFIX struct TFinancePrnData
{
	UINT32 StartDate;              /*起始日期                                 */          
	UINT32 EndDate;                /*中止日期                                 */
	UINT32 DiscountSum;            /*折扣                                     */
	UINT32 ServiceFeeSum;          /*服务费                                   */
	UINT32 ReInvSum;               /*退货                                     */
	UINT32 CashEnterSum;           /*现金入帐                                 */
	UINT32 CanshOutSum;            /*现金出帐                                 */
	UINT32 CashPaySum;             /*现金                                     */ 
	UINT32 CreditPaySum;           /*信用卡                                   */ 
	UINT32 ExchangePaySum;         /*外汇                                     */
	UINT32 SignPaySum;             /*签单                                     */
	UINT32 Turnover;               /*营业额                                   */
};

//--------------------------------------------------------------------------
//税种税目信息                                                   51
//--------------------------------------------------------------------------
PACK_PREFIX struct TTaxItem                       
{
	UINT8   ID;                           		//税种税目索引号HEX   唯一  1 
	UINT8   Code[TAX_ITME_CODE_LEN*2+1];      		//税种项目代码 BCD          4
	UINT32  TaxRate;                      		//税率扩大1000倍, 如130表示税率0.13 4
//	UINT8   ChName[TAX_ITEM_NAME_LEN+1];  		//经营项目中文名称         21
	//UINT8   EnName[TAX_ITEM_NAME_LEN+1];  		//经营项目英文名称         21
	//Key:ID                                                 
	//Query: ID                                               
};

/*------------------------------------------------------------------------
税种税目信息：
1-项目数
2-税种税目索引号
3-税种项目代码
4-税率
5-经营项目中文名称
6-经营项目英文名称
 --------------------------------------------------------------------------*/
PACK_PREFIX struct TTaxItemPrnData
{
	UINT8   Count;                               /*税种税目实际使用个数     */     
	struct TTaxItem TaxItems[TAX_ITEM_MAX_COUNT];/*税种税目列表             */
};

/*----------------------------------------------------------------------------
 获取离线相关数据：
		
1-未上传发票张数
2-未上传发票时间
3-未上传发票累计金额
4-上传张数
5-上传时间间隔
---------------------------------------------------------------------------- */
PACK_PREFIX struct TGetOffLineData
{
	UINT32 Wscfpzs;
	UINT32 Wscfpsj;
	INT64  Wscfpljje;
	UINT32 Sczs;
	UINT32 Scsjjg;
};

/**************************************************************************
功能     ：打印税控机基本信息
格式     ：int print_macinfo(struct TMachinePrnData *machineData, 
             UINT8 bFindBlackMark )
输入参数 ：struct TMachinePrnData *machineData 税控机基本信息
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
int print_macinfo(struct TMachinePrnData *machineData,  UINT8 bFindBlackMark );

/**************************************************************************
功能     ：打印企业用户信息
格式     ：int print_corpinfo(struct TCorpInfoPrnData *corpData, 
             UINT8 bFindBlackMark )
输入参数 ：struct TCorpInfoPrnData *corpData 企业用户信息
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
int print_corpinfo(struct TCorpInfoPrnData *corpData,  UINT8 bFindBlackMark );


/*!
@brief 票种信息打印函数
@param[in] invkindData 票种信息指针
@param[in] bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标后开始打印
\author   zcy
\version  1.0
\date     2011-7-5

*/
int print_invKindinfo(struct TInvKindInfoPrnData *invkindData,  UINT8 bFindBlackMark );


/**************************************************************************
功能     ：打印单卷发票使用汇总数据报表
格式     ：INT8 print_volumeinfo(struct TVolSumPrnData *volsumdata,
                     UINT8 bFindBlackMark)
输入参数 ：struct TVolSumPrnData *volsumdata  发票卷汇总数据
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
INT8 print_volumeinfo(struct TVolSumPrnData *volsumdata,UINT8 bFindBlackMark);


/**************************************************************************
功能     ：打印时间段发票使用汇总数据报表
格式     ：int print_dateseginvsuminfo( struct TDateSegSumPrnData * dateSegSumData
                             UINT8 bFindBlackMark );
输入参数 ：struct TDateSegSumPrnData * dateSegSumData  时间段发票汇总数据
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
int print_dateseginvsuminfo( struct TDateSegSumPrnData * dateSegSumData,
                             UINT8 bFindBlackMark, UINT32 startTime, UINT32 endTime);

/**************************************************************************
功能     ：打印日交易汇总数据报表
格式     ：int print_daystatinfo( struct TDaySumInfoPrnData *daySumData,
         ： UINT8 bFindBlackMark );
输入参数 ：struct TDaySumInfoPrnData *daySumData 日交易汇总数据
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
int print_daystatinfo( struct TDaySumInfoPrnData *daySumData, UINT8 bFindBlackMark );

/**************************************************************************
功能     ：打印申报数据汇总报表查询
格式     ：int print_sbstatinfo( struct TDeclareSumPrnData *declareData,
                       bool bFindBlackMark )
输入参数 ：struct TDeclareSumPrnData *declareData 申报数据汇总数据
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
int print_sbstatinfo(  struct TDeclareSumPrnData *DeclareSumPrnData  );

/**************************************************************************
功能     ：打印57mm发票
格式     ：UINT8 print_inv57( struct TInvPrnData *invData,
                 UINT8 bFindBlackMark )
输入参数 ：struct TInvPrnData *invData  发票数据
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/

INT8 print_inv57(TPrnInvoiceInfo *invData);
                 
/**************************************************************************
功能     ：打印税种税目信息
格式     ：UINT8 print_taxiteminfo( struct TTaxItemPrnData *taxitem,
                 UINT8 bFindBlackMark )
输入参数 ：struct TTaxItemPrnData *taxitem  税种税目信息
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/

//INT8 print_taxiteminfo(UINT8 bFindBlackMark );

/**************************************************************************
功能     ：打初始化回单信息
格式     ：int print_initinfo()
输入参数 ：struct TCorpInfoPrnData *corpData 企业用户信息
         ：
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void  print_initinfo(UINT8 Init_Update);

/**************************************************************************
功能     ：打印发票限额查询
格式     ：INT8 print_invoiceLimits( struct TInvLimitPrnData *invLimits,UINT8 bFindBlackMark )
输入参数 ：struct TInvLimitPrnData *invLimits  发票限额信息
         ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/

INT8 print_invoiceLimits( struct TInvLimitPrnData *invLimits, UINT8 bFindBlackMark );

/**************************************************************************
功能     ：判断打印机当前是否为忙
格式     ：UINT8  IsBusy(void);
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
UINT8  IsBusy(void);


PACK_PREFIX struct TBegin_End_Date
{
	UINT32       StartDate;                     /*统计起始日期   YYYYMMDD       */
	UINT32       EndDate;                       /*统计截至日期   YYYYMMDD       */
};

//----------------------------------【行】--------------------------------
PACK_PREFIX struct TInvDetail_Line
{
	UINT32      Date;                	  /*日期   YYYYMMDD       */
	UINT32      InvNo;                    /*发票号码              */
	INT64  	Sum;                      /*金额              4   */
};

//----------------------------------【尾】--------------------------------
PACK_PREFIX struct TInvDetail_End
{
	UINT32      PosInvSum;              	 /* 正票金额  */
	UINT32      ReInvSum;                    /* 退票金额  */
	INT64  	NetInvSum;                   /* 净金额    */
};

/*--------------------------------------------------------------------------
赵健青 2005-04-06

加油站 发票日统计报表 数据结构
------------------------------------【行】----------------------------------*/
PACK_PREFIX struct TDay
{
	UINT32      Date;                	  /*日期   YYYYMMDD       */
	UINT32  	Amount;                   /*商品数量          4   */
	INT64  	Sum;                      /*金额              4   */
};

/*--------------------------------------------------------------------------
赵健青 2005-04-06

加油站 发票月统计报表 数据结构
------------------------------------【行】----------------------------------*/
PACK_PREFIX struct TMonth
{
	UINT32      month;                	  /*日期   YYYYMM       */
	UINT32  	Amount;                   /*商品数量          4   */
	INT64  	Sum;                      /*金额              4   */
};

/*--------------------------------------------------------------------------
赵健青 2005-04-07

加油站 销售报表 数据结构
------------------------------------【行】----------------------------------*/
PACK_PREFIX struct TSalesman
{
	char       Name[OPERATOR_NAME_LEN+1];/*开票员姓名，10 ASC       */
	UINT32  	Amount;                   /*开票数量          4   */
	INT64  	Sum;                      /*金额              4   */
};

/*--------------------------------------------------------------------------
赵健青 2005-04-07

加油站 商品 数据结构
------------------------------------【行】----------------------------------*/
PACK_PREFIX struct TGoods
{
	UINT32 GoodsNum;					/*油品数量，打印时保留小数点2位	*/	
	UINT32  Price;                       /*单价                  4   */
	INT64  SalesSum;                    /*销售金额              4   */

	char   	GoodsName[GOODS_NAME_LEN+1]; /*商品名称   20 ASC         */
};

/*-------------------------------------------------------------------------
		加油站要求 《统计 报表》
		
作者     ：赵健青                       
日期     ：2005-04-06
---------------------------------【头】----------------------------------*/


/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_Head( struct TBegin_End_Date *Begin_End_Date, \
						   int x0,int y0,char *Title, \
						   int x1,int y1,char *FieldName_1, \
						   int x2,int y2,char *FieldName_2, \
						   int x3,int y3,char *FieldName_3 );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_invDetail_Line( struct TInvDetail_Line *invDetail_Line );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_invDetail_End( struct TInvDetail_End *invDetail_End );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_Day_Line( struct TDay *day );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_NetSum_End( INT64 NetSum , INT64 LiterSum);

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_Month_Line( struct TMonth *Month );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_Goods_Line( struct TGoods * Goods );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
void print_Sales_Line( struct TSalesman *Salesman );

/**************************************************************************
功能     ：
格式     ：
输入参数 ：无
输出参数 ：无
返回值   ：-1——失败，0——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
INT8 PrintRptTitle(INT8 *title);

#ifdef __cplusplus
}
#endif

#endif
