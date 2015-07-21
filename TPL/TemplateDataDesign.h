/*! \file    TemplateDataDesign.h
\brief    打印模板数据结构。
\author   Xiao Pengkun   2005-07-20
 */

#ifndef  PRN_DATA_DESIGN_H
#define  PRN_DATA_DESIGN_H

#include "TemplateMacro.h"
#include "VersionConfig.h"

#define PT_NAME_LEN          20          //模板名称长度
#define PT_VER_LEN           8          //模板版本号长度
#define PT_DIS_LEN           20         //地区名长度
#define PT_DATE_LEN          16         //日期长度 
#define PT_MAX_ITEM_COUNT    12         //模板行打印项最大个数
#define PT_FIELD_NAME_LEN    16         //关键字名称长度
#define PT_FIELD_VALUE_COUNT 6          //一个字段值的最大个数
#define PT_LINE_TMP_BUF_LEN  32         //模板行零时缓存长度

#define PRINT_DATA_BUF_LEN  4096 //打印数据项缓存，主要是发票和报表的数据项

#define PRINT_LINE_WORD_LEN 64 //打印行最大打印字符数

#define PRINT_LINE_BUF_LEN 128 //打印行缓存,最终输出打印行数据

#define PRINT_TEMPLATE_BUF_LEN  4096//打印模板缓存,存储模板文件

#define TEMPLATE_LINE_BUF_LEN   256 //模板行缓存，每次提取模板文件一行

#define PRINT_TEMPLATE_LINE_FIELD_BUF_LEN  256//模板行中各个属性缓存

#define MATCH_FIELD_LABEL_BUF_LEN   64       //匹配行中各属性标签

#define BEGIN_END_CHARACTER0  '/' //模板文件的注释标志

#define BEGIN_END_CHARACTER1  '*'

#define ATTIBUTE_SEPERATE_FIELD '#'//模板行属性标签与数据的分隔符

#define TEMPLATE_LINE_END_CHARACTER '\n'//模板行结束标志

#define TEMPLATE_ATTRIBUTE_SEPERATE_CHARACTER '~'//模板行属性分隔符

#define PRINT_TEMPLATE_FILE_NAME  "../template"//打印模板的存储文件路径与文件名称前8字节

/*模板有关数据结构*/

//#define CPLUSPLUS_A
#define SIG_LINE_LEN   50   //一行打印长度


//打印模板行0
typedef struct
{
	//INT8 rptName[PT_NAME_LEN+1];     //报表名称ASC－20
	UINT8    TempType;                //模板类型：发票或报表
	INT8     chVer[PT_VER_LEN+1];      //版本
	INT8     chDis[PT_DIS_LEN+1];      //应用地区
	INT8    chDate[PT_DATE_LEN+1];     //应用日期
}TPrnTempLine0;

//打印模板行1
typedef struct 
{
	UINT16 PaperW;                  //打印纸宽度  
	UINT16 PaperL;                  //打印纸长度 
	INT16  FontH;                   //字体高度
	INT16  FontW;                   //字体宽度
	UINT8  MarkFlag;                //黑标标志 1－无黑标，2－左，3－右，4－两边 
	//UINT8  SetFlag;                 //套打标志 1－全打，2－套打
/*-----------------added xiaopk 20060207--------------------*/
	UINT8  FixedLengthFlag;             //定长打印标志
/*-------------------added end------------------------------*/
	UINT8  MaxInvLineCount;         //发票最大明细行数
	//UINT8  PrintType;               //发票类型：1---电子存根，0---正常票 
	INT32   RepairPoint;                  //发票打印补偿量
	UINT8  MaxFKDWCharacter;        //发票打印行付款单位每行最大字符数
	UINT8  MaxSKDWCharacter;        //发票打印行收款单位每行最大字符数
	UINT8  MaxSKDWDZCharacter;        //发票打印行收款单位地址每行最大字符数
	UINT8  MaxSKYCharacter;         //发票打印行收款员每行最大字符数
	UINT8  MaxSPMCCharacter;        //发票打印行商品名称每行最大字符数
	UINT8  MaxJEDXCharacter;        //发票打印行金额大写每行最大字符数
	UINT8  EndBackwardFlag;		//打印结束后退纸标志  0：进纸，1：退纸，距离：EndForwardPoint
    INT32  EndForwardPoint;         //打印结束后走纸距离
    INT32  BeginForwardPoint;       //打印开始走纸距离
	UINT8  MaxBZCharacter;        //发票打印行备注每行最大字符数
    
    /*---------added xiaopk 20051123------------------------*/
    INT16  BackwardPoint;           // 2010打印结束后退纸距离
    /*----------added end------------------------------------*/
    UINT8  MaxGoodsCount;           //对于定长版的最大商品行数
    UINT16 UnfixedForwardPoint;    //不定长打印结束后走纸的距离
/**********for 57mm***********/
	UINT8  InvFlag76;              //判断是否打76/57mm	
/**********for 57mm***********/
	

	UINT16  QRCodeLeftMargin;		//二维码左边距

}TPrnTempLine1;

//打印项
typedef struct
{
	UINT8 Type;                   //0-未使用，1－标签，2－打印内容
	INT16 Left;                   //左起位置
	UINT8 DataLen;
	INT8  *dataPtr;
	
}TPrnTempItem;

//打印行内容
typedef struct 
{
	UINT8 Index;                            //行号
	UINT8 Type;                             //行类型：0--普通行 1-空行，2－无效行
	UINT8 High;                             //行高
	UINT8 BhighOne;					       //行间距1
    UINT8 BhighTwo;                        //行间距2 
    INT32 EmptyForwardHigh;                //空行走纸的距离 
	INT16 FontW;						    //字体宽度
  	INT16 FontH;                            //字体高度
	INT8  chDatabuf[PRINT_LINE_BUF_LEN+1];  //行缓存
    TPrnTempItem Items[PT_MAX_ITEM_COUNT];  //打印项最多12项
	UINT8 TotalCount;                       //标签和内容个数的和
	UINT8 GoodsNo;                          //商品明细行序号
	
}TPrnTempRow;


//打印行数据结构
typedef struct
{
  INT32 Property;					   //0-普通行，1－空行, 2--无效行，3--结束行
  INT32 High;						   //行高
  UINT8 Bhigh; 
 
  INT16 EmptyForwardHigh;              //空行走纸的距离 
  INT16 BeginForwardPoint;             //打印开始走纸距离
  INT16 EndForwardPoint;               //打印结束走纸距离
  INT16 BackwardPoint;                 // 2010打印结束后退纸距离
 
  INT8  MarkFlag;                      //是否找黑标
  INT16 FontW;						   //字体宽度
  INT16 FontH;                         //字体高度
  INT8	chContent[PRINT_LINE_BUF_LEN+1]; //打印内容  
} TPrnLineInfo;



//发票商品行数据结构
typedef struct
{

	UINT8    Type;                                  //商品行
	                                                //性质：1-普通商品行 2-折扣行 3-带折扣的商品行 4-配件
	                                                //（数量，单价,金额为零 ）,只打印名称* 7--商品名称超长站桩的商品行
	INT8     chProductPLUCode[PLU_CODE_PRN_LEN+1];  //商品条码
	INT8     chCorpDefCode[PLU_CODE_PRN_LEN+1];     //商家自定义编码(唯一) 
	INT8     chProductName[PLU_NAME_PRN_LEN+1];		//商品名称

#ifdef CPLUSPLUS_A //C++接口所需数据
	double   Amount;                                //商品数量
	double   Price;                                 //单价 
	double   DiscountPrice;                         //折后价
	double   Sum;                                   //金额
	double   DiscountSum;                           //折后金额  
#else              //C接口所需数据
	INT64   Amount;                                //商品数量
//	UINT32   Price;                                 //单价 
	INT64   Price;                                 //单价 
	UINT32   DiscountPrice;                         //折后价
//	UINT32   Sum;                                   //金额
	INT64   Sum;                                   //金额
	INT64   TaxSum;                                 //税额
	UINT32   DiscountSum;                           //折后金额  
#endif

	float    TaxItemIndex;                          //税率
	float    Zsl;									//征收率
	UINT32   DeptIndex;                             //商品所属部类索引号
	UINT8    ProductLineNo;                         //商品行序号

}TPrnInvGoodsLine;

//
typedef struct 
{
	UINT8   PrintType;				//套打或是全打,01:套打 02：全打 03:电子存根
	UINT8   InvType;                //开票类型, 01：正票 02：退票 03：废票
	UINT8   PayMode;                //付款方式    
	UINT32  PosInvNo;               //原发票号码,作为退票用
	UINT8   GoodsLineCount;         //商品明细行数；
	UINT8   PrintKind;               //发票打印方式 

#ifdef CPLUSPLUS_A		//C++接口所需数据
	INT8    DateStr[DATE_PRN_LEN+1];					//开票日期 
#else					//C接口所需数据
	UINT32	m_Date;                                     //开票日期 CCYYMMDD
	UINT32  m_Time;	                                    //开票时间
#endif

	INT8    chTypeCode[INV_TYPE_CODE_PRN_LEN+1];		//发票代码 
	UINT32  InvNo;										//发票号码
	INT8    chCorpName[CORP_NAME_PRN_LEN+1];			//纳税人名称
	INT8    chCorpNo[CORP_NO_PRN_LEN+1];				//纳税人编号
	INT8    chCorpCode[CORP_TAX_CODE_PRN_LEN+1];		//纳税人税号20ASC
	INT8    chClientName[CORP_NAME_PRN_LEN+1];			//付款人
	INT8    chClientCode[CORP_TAX_CODE_PRN_LEN+1];							//付款人代码
	INT8    chMachineNo[MACHINE_NO_PRN_LEN+1];			//机器编码   

	INT8    chPosCode[INV_TYPE_CODE_PRN_LEN+1];		//原发票代码 


	UINT32  OperatorID;									//收款员编号
	INT8    chOperatorName[OPERATOR_NAME_PRN_LEN+1];    //收款员名称
	INT8    chShopNo[SHOP_CODE_PRN_LEN+1];              //店号   
	INT8    chMachineIndex[CORP_DEF_MACHINE_NO_PRN_LEN+1];//企业自定义机器编号
	INT8    chKPLSH[KPLSH_PRN_LEN+1];                   //开票流水号  
	INT64  InvSumLowcase;										//单张发票价税合计小写
	INT8    chInvSumUppercase[INV_SUM_CH_PRN_LEN+1];			//单张发票价税合计大写

	INT64  InvTaxLowcase;										//单张发票合计税额小写
	INT64  InvMoneyLowcase;									//单张发票合计金额小写

	INT8    chTaxCtrCode[INV_TAX_CTRL_CODE_PRN_LEN+1];			//税控码 8HEX->20ASC
	INT8    chJzlshCode[INV_TAX_CTRL_CODE_PRN_LEN+1];			//税控码 8HEX->20ASC
	INT8    chClientTelephoneNo[CLIENT_PHONE_PRN_LEN +1];          //付款方手机号

	INT8    chCorpAddr[CORP_ADDR_PRN_LEN+1];					//纳税人地址
	INT8    chPhoneNo[CORP_PHONE_PRN_LEN+1];					//纳税人电话

	INT8    chRemarks[REMARKS_LEN+1];                 //备用字段
	INT8    chSelfDefTab1[SELF_DEF_TAB_LEN+SELF_DEF_TAB_LEN+4];        //自定义1,因为要加“：”
	INT8    chSelfDefCont1[SELF_DEF_TAB_LEN+1];       //自定义内容1
	INT8    chSelfDefTab2[SELF_DEF_TAB_LEN+SELF_DEF_TAB_LEN+4];        //自定义2
	INT8    chSelfDefCont2[SELF_DEF_TAB_LEN+1];       //自定义内容2
	INT8	chHYFL[SZSM_CH_NAME_LEN+1];		//行业分类

	INT8    chKPDZJC[KPDZJC_CH_LEN+1];            //开票地址简称

	INT8    chYWTM[YWTM_CH_LEN+1];            //一维条码
/*
#if YILIAO_VERSION
	UINT8	chZYH[ZYH_LEN+1];									//住院号
	UINT8   chZPH[ZPH_LEN+1];
#endif*/
	
	
/*
#if PETROL_STATION_VER
    UINT8   OilGunID;											//油枪号
   
#endif*/


	TPrnInvGoodsLine GoodsLines[GOODS_LINE_MAX_PRN_COUNT];		//商品行

}TPrnInvoiceInfo;

//需要保存的模版属性
typedef struct 
{
	UINT8	markFlag;                 //1 黑标 ， 0 定长
	INT8	TempVersion[32];           
	UINT8   MaxSPMCCharacter;        //发票打印行商品名称每行最大字符数
	UINT8   EndBackwardFlag;		//打印结束后退纸标志  0：进纸，1：退纸，距离：EndForwardPoint
	UINT16  EndForwardPoint;         //走纸的距离
	UINT8   MaxGoodsCount;            //单张发票最大商品行数
	UINT8   workMode;                   /*开票机工作模式 WORK_MODE-正常销售 */
										/*           TRAINING_MODE-训练模式	*/
	UINT8   FixedModeFlag;              //定长销售模式标志，1为定长，0为不定长
	UINT16  BackwardPoint;           // 2010打印结束后退纸距离
	INT32   RepairPoint;			 //zongcan test
/**********for 57mm***********/
	UINT8   InvFlag76;                 //判断是否打76/57
/**********for 57mm***********/
	UINT16	QRCodeLeftMargin;			//二维码左边距
/*	UINT16  FontH;
	UINT16  FontW;
	UINT8   MaxFKDWCharacter;        //发票打印行付款单位每行最大字符数
	UINT8   MaxSKDWCharacter;        //发票打印行收款单位每行最大字符数
	UINT8   MaxSKYCharacter;         //发票打印行收款员每行最大字符数
	UINT8   MaxJEDXCharacter;        //发票打印行金额大写每行最大字符数
    INT32   BeginForwardPoint;       //打印开始走纸距离
    UINT16  BackwardPoint;           // 2010打印结束后退纸距离
    UINT16  UnfixedForwardPoint;
 */
}TSaveTemplateInfo;

#endif

