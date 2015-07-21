/*! \file   TemplateMacro.h
\brief    打印模板宏定义
\author   Xiao Pengkun
 */

#ifndef PRN_MACRO_H
#define PRN_MACRO_H

#include "comdatatype.h"

#ifndef RETURN_INV
#define RETURN_INV    2
#endif

#ifndef RET_MANUAL_INV
#define RET_MANUAL_INV   6
#endif

#ifndef RET_SPECIAL_INV
#define RET_SPECIAL_INV		7
#endif

#ifndef NORMAL_INV
#define NORMAL_INV    1
#endif

#ifndef STUB_PRINT	
#define STUB_PRINT	1
#endif

#define NORMAL_TEMPLATE_LINE              0     //普通模板行
#define EMPTY_TEMPLATE_LINE               1     //空模板行
#define INVALID_TEMPLATE_LINE             2     //无效模板行
#define NEED_SAVE_TEMPLATE_LINE           3     //需要存储数据的模板行
#define END_INVOICE_HEAD_LINE             4     //发票头部模板结束行 
#define END_INVOICE_DATA_LINE             5     //发票数据模板结束行

#define MAX_DETAIL_COUNT                 64
/*商品明细行性质*/
#define  DETAIL_GENERAL_GOODS			0	/* 一般商品行					*/
#define  DETAIL_DISCOUNT				1	/* 折扣行						*/
#define  DETAIL_GOODS_DISCOUNT			2	/* 带折扣的商品行				*/
#define  DETAIL_SERVICE_FEE				3	/* 服务费行						*/
#define  DETAIL_EMPTY					4	/* 空行							*/
#define  DETAIL_GOODS_REDUCT			6	/* 带折让的商品行	            */

#define WORK_MODE '\1'                      /* 正常工作模式					*/
#define TRAINING_MODE '\2'                  /* 训练模式						*/

#define  PRICE_EXTENSION_TEMPLATE		   100			/* 价格放大倍数					*/
#define  NUMBER_EXTENSION_TEMPLATE	       10000			/* 数量放大倍数					*/
#define  SUM_EXTENSION_TEMPLATE		       100				/* 金额放大倍数					*/
#define  RATE_EXTENSION_TEMPLATE		   100				/* 折扣服务率放大倍数			*/

#define  NORMAL_PRINT_TEMPLATE 		    0				/* 正常打印发票				 	*/
#define  STUB_PRINT_TEMPLATE 			1				/* 电子存根打印 				*/

#define  NORMAL_INV_TEMPLATE             1               /*正常发票 */
#define  RETURN_INV_TEMPLATE             2               /*已退发票 */
#define  WASTE_INV_TEMPLATE              3               /*作废发票 */ 
#define  RETURN_INV_NOTEXIT_TEMPLATE     6               /*退本机不存在发票*/  

#define  DETAIL_DISCOUNT_TEMPLATE		 1	     /* 折扣行			*/
/*---------added zs 20070424----------------------------------------*/
#define  DETAIL_REDUCTION_TEMPLATE		 5		/* 折让行			*/
#define  DETAIL_SERVICE_TEMPLATE		 4		/* 加成行			*/
/*----------added end---------------------------------------------------*/
/*---------added xiaopk 20051204----------------------------------------*/
#define DETAIL_SPMCMAX_DISCOUNT_TEMPLATE  8      /* 超长商品行的折扣行*/
/*----------added end---------------------------------------------------*/
#define PRICE_DISCOUNT_PRN_DEC            1    //单价折扣精度 
#define PRICE_PRN_DEC                     2    //单价精度 
#define AMOUNT_PRN_DEC                    2    //数量精度 
#define SUM_PRN_DEC                       2    //金额精度

#define DATE_PRN_LEN                      20   //日期打印长度
#define MACHINE_NO_PRN_LEN                16   //机器编号(ASC)长度

#define SHOP_CODE_PRN_LEN                 10   //商店编号
#define CORP_DEF_MACHINE_NO_PRN_LEN       10   //企业自定义机器编号长度

#define CORP_NAME_PRN_LEN                 200   //纳税人名称长度ASC
#define CORP_NO_PRN_LEN                   20//16   //企业编号长度(8BCD)->16ASC
#define CORP_TAX_CODE_PRN_LEN             20   //企业税号长度       
#define INV_TYPE_CODE_PRN_LEN             20   //发票代码长度ASC
#define INV_TAX_CTRL_CODE_PRN_LEN         20   //发票税控码长度8HEX->20ASC
#define INV_SUM_CH_PRN_LEN                64   //发票总金额的中文大写的长度
#define KPLSH_PRN_LEN                     12   //开票流水号
#define PLU_CODE_PRN_LEN                  16   //商品编码长度
#define PLU_NAME_PRN_LEN                  40   //商品名称长度
#define CORP_ADDR_PRN_LEN				  100		//企业名称长度
#define CORP_PHONE_PRN_LEN				   24		//企业电话长度
#define CLIENT_PHONE_PRN_LEN               11      //付款方手机号
#define SZSM_CH_NAME_LEN					40		//税种税目名称（行业分类）
#define KPDZJC_CH_LEN                      100      //开票地址简称
#define  YWTM_CH_LEN                        20       //一维条码        

#define GOODS_LINE_MAX_PRN_COUNT          16//6    //发票商品行最大个数    

#define OPERATOR_NAME_PRN_LEN             10   //操作员名称长度

#define REMARKS_LEN                        80  //备注长度
#define SELF_DEF_TAB_LEN                   18  //自定义标签长度
#define SELF_DEF_CONT_LEN                  20  //自定义内容长度

#endif























