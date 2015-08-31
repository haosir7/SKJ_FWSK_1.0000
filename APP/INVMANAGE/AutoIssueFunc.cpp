/*! \file    AutoIssueFunc.cpp
   \brief    awe4000r发票管理的自动开票函数
   \author   Yu Yan 
   \version  1.0
   \date     2008-5-21
 */

#include "AutoIssueFunc.h"
#include "CGlobalArg.h"
#include "PowerOffData.h"
#include "TDateTime.h"
#include "InvManageFunc.h"
#include "keypad.h"
#include "CaProgressBar.h"
//#include "CInvImportFun.h" 
#include "CGlobalArg.h"
#include "CSysLog.h"
#include "CInvSum.h"
//#include "JSKInvDownProc.h"

#include "SaleData.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

//#if AUTOISSUE

#define PRINT  0
#define SMALL_INV_NUM 		3			/**< 正常天每日的开票的数量 */
#define BIG_INV_NUM 		200			/**< 5719张后正常天每日的开票的数量 */
#define THREE_MONTH 			3
#define TAX_DIVISOR             4		/**< 允许开票的数目数量加1 */
#define MAX_INV_NO 				5719    /**< 自动连续开票最大的发票数 */
#define STOP_INV_NO				5700	/**< 自动连续开票暂停的发票数 */

#if AUTOISSUE
UINT8 AutoIssue(UINT8 &nIfInvSum)
{
	DBG_PRINT(("AutoIssue()"));
	switch(ISSUE_TYPE) 
	{
		case 0:
			DBG_PRINT(("SimpleAutoIssue()"));
			SimpleAutoIssue(nIfInvSum);
			break;
// 		case 1:
// 			ComplexAutoIssue();
// 			break;
// 		case 2:
// 			NetAutoIssue(nIfInvSum);
		default: break;
	}
	
	return SUCCESS;
}

UINT8 SimpleAutoIssue(UINT8 &nIfInvSum)
{
//	INT32 nKey = 0x0;
	UINT8 nKeyTime = 0;
	UINT32 nDate;
    UINT8 nVolNum = 0;//发票卷数
	UINT32 nInvNo = 0;
	UINT32 nCurrentInvNo = 0;
	INT8 chValue[256];
	string strInfo;
	memset((void*)chValue, 0, sizeof(chValue));
	SaleData saleInfo;
	CaProgressBar proBar("");
	UINT8 ret;
	
	for(;;)
	{
		//超过开票截止日期，停止开票
		nDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
		DBG_PRINT(("for后 nDate = %u", nDate));
		DBG_PRINT(("for后 g_globalArg->m_curInvVol->m_remain = %u", g_globalArg->m_curInvVol->m_remain));

//		if (nDate>g_globalArg->m_corpInfo->m_ieDate) 
//		{
//			CaMsgBox::ShowMsg("超过开票截止日期\n请立即申报!");
//			return FAILURE;		
//		}
// 		if (1 == g_globalArg->m_declareLimitFlag)
// 		{
// 			CaMsgBox::ShowMsg("申报期超限,请立即申报");
// 			return FAILURE;
// 		}
		
		//连续按两次确认键，则退出开票程序
		if(CheckKey2(nKeyTime)==SUCCESS)
		{
			return SUCCESS;
		}

		DBG_PRINT(("开始自动开票"));

//		if(g_globalArg->m_curInvVol->m_remain != 0)
//		{
//			nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - 
//					g_globalArg->m_curInvVol->m_remain;
			//判断是否滚动，若要滚动，显示消息框
// 			if(IfInvHeadRoll()!=SUCCESS)
// 			{
// 				return FAILURE; 			
// 			}
// 			if(IfInvSumRoll()!=SUCCESS) 
// 			{
// 				return FAILURE; 			
// 			}
// 			if(IfRtInvRoll()!=SUCCESS) 
// 			{
// 				return FAILURE; 			
// 			}

// 			if (g_globalArg->m_curInvVol->m_remain == 2)
// 			{
// 				DBG_PRINT(("自动开红票"));
// 				if ((ret = SimpleMakeReturnInv(saleInfo)) != SUCCESS)
// 				{
// 					if (ret==DB_FULL) 
// 					{
// 						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// 					}
// 					else if (HAVE_ROLLED==ret)
// 					{
// 						CaMsgBox::ShowMsg("滚动完毕!");			
// 					}
// 					else
// 					{
// 						CaMsgBox::ShowMsg("开红票失败!");
// 					}					
// 					return FAILURE;
// 				}
// 			}
// 			else if (g_globalArg->m_curInvVol->m_remain == 1)
// 			{
// 				DBG_PRINT(("自动开废票"));
// 				if ((ret = SimpleMakeWasteInv(saleInfo)) != SUCCESS)
// 				{
// 					if (ret==DB_FULL) 
// 					{
// 						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// 					}
// 					else if (HAVE_ROLLED==ret)
// 					{
// 						CaMsgBox::ShowMsg("滚动完毕!");			
// 					}
// 					else
// 					{
// 						CaMsgBox::ShowMsg("开废票失败!");
// 					}	
// 					return FAILURE;
// 				}
// 			}
// 			else
// 			{
// 			while (g_globalArg->m_InvServNum > 0)
// 			{
// 				CommonSleep(1000);
// 			}
			CommonSleep(1000);
				DBG_PRINT(("自动开正票"));
				string strErr("");
				if ((ret = SimpleMakeNormalInv(saleInfo,strErr))!= SUCCESS)
				{
					DBG_PRINT(("ret= %u",ret));
					if (ret==DB_FULL) 
					{
						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
					}
					else if (HAVE_ROLLED==ret)
					{
						CaMsgBox::ShowMsg("滚动完毕!");			
					}
					else if(INV_LIB_ERR == ret)
					{
						CaMsgBox::ShowMsg(g_globalArg->m_strMsg);
					}
					else
					{
						CaMsgBox::ShowMsg(strErr);
					}	
					return FAILURE;
				}
//			}
			//----销售相关变量复位
			saleInfo.InitSaleData(1);	//销售信息初始化
			saleInfo.InitInvHead();		//单张发票信息初始化
			
			//memset((void*)chValue, 0, sizeof(chValue));
			nCurrentInvNo++;
			sprintf(chValue, "自动开票: %u号",nCurrentInvNo);
			proBar.SetText(chValue);
			proBar.ReFresh();

			
			/*
		}
		else
		{
			//卷汇总，查询发票卷，并导入。若无发票卷，则报错并返回
			if (GetInvNo(saleInfo, nInvNo) != SUCCESS)
			{
				return FAILURE;
			}

			if(nVolNum < 1)
			{
				nVolNum++;
				continue;
			}
			nVolNum = 0;
			
			//修改系统时间到第二天	
// 			TDateTime dateTime(nDate, 80000);
// 			dateTime++;
// 			if (!TDateTime::SetCurrentDateTime(dateTime))
// 			{
// 				CaMsgBox::ShowMsg("修改日期时间错误!");
// 				return FAILURE;			
// 			}
// 			UINT32 nTestDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 			DBG_PRINT(("延迟后 nTestDate = %u", nTestDate));
		
			//日汇总
// 			INT32 errorcode = 0;
// 			INT8 charValue[128];
// 			memset((void*)charValue, 0, sizeof(charValue));
// 			if ((errorcode = g_globalArg->CheckDaySum(0)) != SUCCESS)
// 			{
// 				sprintf(charValue, "日交易汇总错误：%02x", errorcode);
// 				CaMsgBox::ShowMsg(charValue);
// 				return FAILURE;	
// 			}

			
		}*/
	}
}


UINT8 SimpleMakeNormalInv(SaleData &tmpSaleData,string &strErr)
{
	UINT8 ret;
	CInvDet *invDet = NULL;
	CInvHead invHead;
	tmpSaleData.m_smallInvInfo = &invHead;
    UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	INT64 tmpDJ=0;
	INT64 moneyTaxSum=0;
	//组装正票
	//组装发票头
	//tmpSaleData.m_smallInvInfo->m_fphm = g_globalArg->m_curInvVol->m_ieno + 1 - 
	//	g_globalArg->m_curInvVol->m_remain;  /**< 发票号码 */

	ret = SALE_GetCurInv(g_globalArg->m_curInvVol,strErr);
	DBG_PRINT(("strErr= %s",strErr.c_str()));

	if (ret ==FAILURE)
	{
		DBG_PRINT(("获取当前发票号码错误：%s",strErr.c_str()));
		return ret;
	}


	tmpSaleData.m_smallInvInfo->m_fphm = g_globalArg->m_curInvVol->m_curInvNo;
    tmpSaleData.m_smallInvInfo->m_fpdm = g_globalArg->m_curInvVol->m_code;  /**< 发票代码简称 */  	
	tmpSaleData.m_smallInvInfo->m_kplx = NORMAL_INV;		//开票类型	

	tmpSaleData.m_smallInvInfo->m_yfphm = 0;		//原发票号码
	tmpSaleData.m_smallInvInfo->m_yfpdm = "";		//原发票号码	
    tmpSaleData.m_smallInvInfo->m_fplb = atoi(g_globalArg->m_invKind->m_pzCode.c_str()); //发票种类
	
	tmpSaleData.m_smallInvInfo->m_fkdw = "个人";			//付款人名称
	tmpSaleData.m_smallInvInfo->m_fkdwsh = "012345678901234";
	tmpSaleData.m_smallInvInfo->m_sky = g_globalArg->m_operator->m_name;		//操作员名称
//	tmpSaleData.m_smallInvInfo->m_opId = g_globalArg->m_operator->m_opId;		//操作员id	
//	tmpSaleData.m_smallInvInfo->m_uID = 0;          //纳税户名称ID号
//	tmpSaleData.m_smallInvInfo->m_ptype = 0;		//付款方式
	tmpSaleData.m_smallInvInfo->m_sphsl = 3;//当前发票总商品行加1

	tmpSaleData.m_smallInvInfo->m_kphjje = (INT64)(3.00 * SUM_EXTENSION);//当前发票总金额
 	tmpSaleData.m_smallInvInfo->m_backup1 = "";		//备用字段1

//	tmpSaleData.m_smallInvInfo->m_backup2 = "";		//备用字段2 
//	tmpSaleData.m_smallInvInfo->m_backup3 = "";		//备用字段3

	//发票明细
	for(UINT8 i = 1; i<4; i++)
	{
//		invDet = new CInvDet;
		invDet = invDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
		DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
		invDet->m_fpdm = tmpSaleData.m_smallInvInfo->m_fpdm; 
		invDet->m_fphm = tmpSaleData.m_smallInvInfo->m_fphm;
	//	invDet->m_sphxh = i; 
		invDet->m_kprq = nCurDate;
		invDet->m_kpsj = nCurTime;
		invDet->m_kplx = NORMAL_INV;
		invDet->m_sky = g_globalArg->m_operator->m_name;


		invDet->m_spbm = "0000000000001";
		invDet->m_sphxh = i;
		invDet->m_spmc = "测试商品1";
	//	invDet->m_taxId = 1; //非税控只允许税目1
		invDet->m_spsl = 1.0; //保存四舍五入后的值
		invDet->m_dotNum = 0;
		invDet->m_spdj = (double)1.00;
		invDet->m_spje = (INT64)(1.00*SUM_EXTENSION);
		invDet->m_sl = 0.03;
		invDet->m_spse=tmpSaleData.CountTax(invDet->m_spje, (UINT32)(invDet->m_sl*SUM_EXTENSION)); //商品税额
	    invDet->m_je= invDet->m_spje - invDet->m_spse; //商品金额(不含税)
	
		invDet->m_spdw = "个";	//商品单位
		
		tmpDJ = double2int(invDet->m_spdj*SUM_EXTENSION);
		DBG_PRINT(("tmpDJ==%lld ", tmpDJ));
		tmpDJ -=tmpSaleData.CountTax(double2int(invDet->m_spdj*SUM_EXTENSION), (UINT32)(invDet->m_sl*SUM_EXTENSION));
		DBG_PRINT(("tmpDJ==%lld ", tmpDJ));
		
		invDet->m_dj= (double)(tmpDJ*1.0)/SUM_EXTENSION; //商品单价(不含税)
		
		invDet->m_je= invDet->m_spje-invDet->m_spse; //商品金额(不含税)
		moneyTaxSum += invDet->m_spse;
		invDet->m_property = DETAIL_GENERAL_GOODS;
		invDet->m_backup = "";
		tmpSaleData.m_smallInvInfo->InsertNode(invDet);	//插入节点
		
	}

	//当前发票累计税额增加
	tmpSaleData.m_smallInvInfo->m_kpse = moneyTaxSum;
	DBG_PRINT(("当前发票累计税额 tmpSaleData.m_smallInvInfo->m_kpse = %lld", tmpSaleData.m_smallInvInfo->m_kpse));
	moneyTaxSum = 0;

	ret = tmpSaleData.MakeInvoiceHandle(PRINT, INV_ROLL_PROC);
	tmpSaleData.m_smallInvInfo->DelList();
	tmpSaleData.m_smallInvInfo = NULL;
	if (ret != SUCCESS)
	{
		if (ret==DB_FULL) 
		{
			DBG_RETURN(DB_FULL);
		}
		else if (ret==HAVE_ROLLED) 
		{
			DBG_RETURN(HAVE_ROLLED);
		}
		else
		{
			DBG_RETURN(ret);
		}	
	}	
	DBG_RETURN(SUCCESS);	
}


// UINT8 SimpleMakeReturnInv(SaleData &tmpSaleData)
// {
// 	UINT8 ret;
// 	CInvDet*invDet = NULL;
// 	CInvHead invHead;
// 	tmpSaleData.m_smallInvInfo = &invHead;
//     UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
// 
// 	//组装红票
// 	//组装发票头
// 	tmpSaleData.m_smallInvInfo->m_InvNo = g_globalArg->m_curInvVol->m_InvEndNo + 1 - 
// 		g_globalArg->m_curInvVol->m_remain;  /**< 发票号码 */
//     tmpSaleData.m_smallInvInfo->m_codeNo = g_globalArg->m_curInvVol->m_codeNo;  /**< 发票代码简称 */  	
// 	DBG_PRINT(("m_smallInvInfo->m_InvNo =  %u", tmpSaleData.m_smallInvInfo->m_InvNo));
//     DBG_PRINT(("m_smallInvInfo->m_codeNo =  %u", tmpSaleData.m_smallInvInfo->m_codeNo));
// 
// 	tmpSaleData.m_smallInvInfo->m_issuetype = RETURN_INV;		//开票类型	
// 	tmpSaleData.m_smallInvInfo->m_oldInv = tmpSaleData.m_smallInvInfo->m_InvNo - 1;		//原发票号码	
//     DBG_PRINT(("m_smallInvInfo->m_issuetype =  %u", tmpSaleData.m_smallInvInfo->m_issuetype));
//    
// 	tmpSaleData.m_smallInvInfo->m_payer = "个人";			//付款人名称
// 	tmpSaleData.m_smallInvInfo->m_operator = g_globalArg->m_operator->m_name;		//操作员名称
// 	tmpSaleData.m_smallInvInfo->m_opId = g_globalArg->m_operator->m_opId;		//操作员id	
// 	tmpSaleData.m_smallInvInfo->m_uID = 0;          //纳税户名称ID号
// 	tmpSaleData.m_smallInvInfo->m_payType = 0;		//付款方式
// 	tmpSaleData.m_smallInvInfo->m_goodsCount = 3;//当前发票总商品行加1
// 	tmpSaleData.m_smallInvInfo->m_moneySum = (INT64)(0.3 * SUM_EXTENSION);//当前发票总金额
// 
//  	tmpSaleData.m_smallInvInfo->m_backup1 = "";		//备用字段1
// 	tmpSaleData.m_smallInvInfo->m_backup2 = "";		//备用字段2 
// 	tmpSaleData.m_smallInvInfo->m_backup3 = "";		//备用字段3
// 
// 	
// 	//发票明细
// 	for(UINT8 i = 1; i<4; i++)
// 	{
// //		invDet = new CInvDet;
// 		invDet = invDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
// 		DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
// 		invDet->m_codeNo = tmpSaleData.m_smallInvInfo->m_codeNo; 
// 		invDet->m_InvNo = tmpSaleData.m_smallInvInfo->m_InvNo;
// 		invDet->m_destriptionNo = 0; 
// 		invDet->m_issueDate = nCurDate;
// 		invDet->m_issueTime = nCurTime;
// 		invDet->m_issuetype = RETURN_INV;
// 
// 		invDet->m_code = "1000000000001";
// 		invDet->m_dpNo = i;
// 		invDet->m_name = "测试商品1";
// 		invDet->m_taxId = 1;
// 		invDet->m_quantity = 1.0; //保存四舍五入后的值
// 		invDet->m_dotNum = 0;
// 		invDet->m_price = (double)0.1;
// 
// 		invDet->m_moneySum = (INT64)(0.1*SUM_EXTENSION);
// 		invDet->m_property = DETAIL_GENERAL_GOODS;
// 		invDet->m_backup = "";
// 		tmpSaleData.m_smallInvInfo->InsertNode(invDet);	//插入节点		
// 	}
// 
// 	ret = tmpSaleData.MakeInvoiceHandle(PRINT, INV_ROLL_PROC);
// 	tmpSaleData.m_smallInvInfo->DelList();
//     tmpSaleData.m_smallInvInfo = NULL;
// 	if (ret != SUCCESS)
// 	{
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}	
// 	}	
// 	DBG_RETURN(SUCCESS);
// 
// }
// 
// 
// UINT8 SimpleMakeWasteInv(SaleData &tmpSaleData)
// {
// 	UINT8 ret;
// 	//发票作废
// 	ret = InvWasteHandle(&tmpSaleData);
// 	if(ret != SUCCESS)
// 	{
// 		DBG_PRINT(("InvWaste error !"));
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}	
// 	}
// 		DBG_RETURN(SUCCESS);
// }




UINT8 GetInvNo(SaleData &tmpSaleData,UINT32 &invNo)
{
//	UINT8 status;
	UINT8 errorcode;
	CInvVol invVol;
	UINT8 nIfInvSum;

	CaProgressBar proBar("");

	if (g_globalArg->m_curInvVol->m_remain == 0)
	{
		DBG_PRINT(("单卷汇总"));
		proBar.SetText("单卷汇总");
		proBar.ReFresh();

		POWOFF_DISABLE(); //屏蔽掉电中断
// 		if(!tmpSaleData.InvSum(nIfInvSum))
// 		{
// 			DBG_PRINT(("Get volume Sum error !"));
// 			CaMsgBox::ShowMsg("单卷汇总失败!");
// 			POWOFF_ENABLE(); //开掉电中断
// 			return FAILURE;
// 		}
		POWOFF_ENABLE(); //开掉电中断
		//判断卷汇总是否要滚动，若滚动，显示消息框
// 		if(IfInvSumRoll()!=SUCCESS) 
// 		{
// 			return FAILURE; 			
// 		}

		proBar.SetText("导入新发票卷");
		proBar.ReFresh();

		//查询可导入的发票卷		
		invVol.m_filter = "where USED_FLAG = 1 and OVER_FLAG = 1 order by NO limit 1";
		invVol.Requery();  //查询
		errorcode = invVol.LoadOneRecord();
		if (errorcode != SQLITE_OK)
		{
			CaMsgBox::ShowMsg("查询可导入发票卷失败!");
			return FAILURE;
		}

		//进行真正的导入操作
		
// 		InvoiceInfoPer_u invoiceInfoPerData; /**< 发票分发临时文件结构体*/			  
// 		memset((void *)&invoiceInfoPerData, 0, sizeof(InvoiceInfoPer_u));
// 	
// 		//查询发票代码
// 		CInvcodeLink invcodeLink;
// 		string strCode; 
// 		errorcode = invcodeLink.CodeNo2Code(strCode, invVol.m_codeNo);
// 		DBG_PRINT(("********errocode = %u\n", errorcode));
// 		if (errorcode!=SUCCESS)
// 		{
// 			CaMsgBox::ShowMsg("查询发票代码失败!");
// 			return FAILURE;
// 		}
// 		char2bin((UINT8 *)(invoiceInfoPerData.invoice_code), (INT8 *)(strCode.c_str()), INV_TYPE_CODE_LEN);

// 
// 		invoiceInfoPerData.InvStart_no = invVol.m_InvStartNo;
// 		invoiceInfoPerData.InvEnd_no = invVol.m_InvEndNo;


		//导入发票卷
		string strErr;
// 		CInvoiceImport invImport;
// 		POWOFF_DISABLE(); //屏蔽掉电中断
// //		if ((invImport.ImportInvVolHandle(&invoiceInfoPerData, strErr)) != SUCCESS)
// 		if ((invImport.ImportInvoice((UINT8 *)(invVol.m_code.c_str()), invVol.m_isno, strErr, 1)) != SUCCESS)
// 		{
// 			CaMsgBox::ShowMsg("发票导入失败!");
// 			return FAILURE;
// 		}

// 		//更新当前卷信息(将发票领用卷信息更新)
// 		char sqlbuf[512];
// 		string sqlstr;	
// 		memset((void*)sqlbuf,0,sizeof(sqlbuf));
// 
// 		sprintf(sqlbuf,"update INV_VOL set USED_FLAG = %u where IS_NO = %u and CODE = %u",0, invVol.m_InvStartNo, invVol.m_codeNo);
// 		sqlstr=sqlbuf;
// 		invVol.SetSQL(sqlstr);
// 		if (invVol.ExecuteSQL() != SQLITE_OK)
// 		{
// 			CaMsgBox::ShowMsg("更新当前卷信息错误!");
// 			return FAILURE;
// 		}
		POWOFF_ENABLE();//开掉电中断

		//当前卷信息赋值
// 		INT8 value[256];
// 		memset((void *)value, 0, sizeof(value));
// 		sprintf(value," where USED_FLAG = %u and  OVER_FLAG = %u", 0 , 1);
// 		g_globalArg->m_curInvVol->m_filter = value;
// 		g_globalArg->m_curInvVol->Requery(); 
// 		errorcode = g_globalArg->m_curInvVol->LoadOneRecord(); 
// 		switch(errorcode)
// 		{
// 		case SQLITE_OK:
// 			break;
// 		default:
// 			CaMsgBox::ShowMsg("读当前卷信息错误!");
// 			return FAILURE;
// 		}

	}
	//计算当前发票号
	invNo = g_globalArg->m_curInvVol->m_ieno + 1 - 
		g_globalArg->m_curInvVol->m_remain;

	return SUCCESS;
}


UINT8 CheckKey2(UINT8 &nKeyTime)
{
	INT32 nKey = 0x0;
	if((nKey = ReadKeypad())&&(RETURN_MAIN_MENU == nKey) )
   	{
		nKeyTime++;
		if (nKeyTime==2)
		{
			nKeyTime = 0;
			return SUCCESS;
		}		
   	}
	return FAILURE;
}


//----------15所 容量测试------------------//

// UINT8 ComplexAutoIssue()
// {
// 	UINT32 i;
// //	UINT32 date;
// 	UINT8 status;
// 	UINT32 invNo;
// 	UINT8 nKeyTime = 0;
// 	SaleData saleDataInfo, *pSaleData;
// 	pSaleData = &saleDataInfo;
// 	CInvHead invHead;
// 	INT8 charValue[256];
// 	UINT8 ret = 0;
// 	INT32 nIssuedNum = 0; //当天已开发票数
// 	UINT32 nNormalInvNum = SMALL_INV_NUM; //正常天每天开票数量
// 	
// 	UINT32 nDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 	UINT32 DeclareDay = ThreeMonthsEndDay(g_globalArg->m_startDecDate, THREE_MONTH);
// 	
// 	memset((void*)charValue, 0, sizeof(charValue));
// 	sprintf(charValue,"where I_DATE = %u",nDate);
// 	invHead.m_filter = charValue;
// 	nIssuedNum = invHead.GetRecordNum();//当天已开发票数量统计
// 	if (nIssuedNum<0) 
// 	{
// 		CaMsgBox::ShowMsg("统计当天已开发票数失败");
// 		return FAILURE;
// 	}
// 		
// 	while(1)//g_globalArg->m_invCount<MAX_INV_NO
// 	{
// 		//开票
// 		nDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 		if (IsLastDay(nDate))//本月最后一天
// 		{
// 			ret = LastDayIssue(saleDataInfo, nNormalInvNum, nIssuedNum, nKeyTime);
// 		}
// 		else
// 		{
// 			ret = NormalDayIssue(saleDataInfo, nNormalInvNum, nIssuedNum,nKeyTime);
// 		}
// 		if (ret!=SUCCESS) 
// 		{
// 			return FAILURE;	
// 		}
// 		nIssuedNum = 0; //当天已开票数清0;
// 		
// 
// 		if (nDate>=DeclareDay) 
// 		{
// 			CaMsgBox::ShowMsg("3月已到，立刻申报");
// 		 	return SUCCESS;			
// 		}
// 
// 		//修改系统时间到第二天	
// 		TDateTime dateTime(nDate, 80000);
// 		dateTime++;
// 		if (!TDateTime::SetCurrentDateTime(dateTime))
// 		{
// 			CaMsgBox::ShowMsg("修改日期时间错误!");
// 			return FAILURE;			
// 		}
// 		UINT32 nTestDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 		DBG_PRINT(("修改系统时间到 nTestDate = %u", nTestDate));
// 	
// 		//日汇总
// 		INT32 errorcode = 0;
// 		memset((void*)charValue, 0, sizeof(charValue));
// 		if ((errorcode = g_globalArg->CheckDaySum(0)) != SUCCESS)
// 		{
// 			sprintf(charValue, "日交易汇总错误：%02x", errorcode);
// 			CaMsgBox::ShowMsg(charValue);
// 			return FAILURE;	
// 		}
// 
// 		//STOP_INV_NO号以后的票开完一张就退出开票程序
// 		if (g_globalArg->m_invCount>=STOP_INV_NO) 
// 		{
// 			return SUCCESS;			
// 		}
// 	}
// 	return SUCCESS;
// }
// 
// UINT32 ComputerMoney(UINT32 date, UINT8 No)
// {
// 	UINT32 y;
// 	UINT32 mm;
// 	UINT32 dd;
// 
// 	y = (date / 10000) % 10;
// 	mm = (date / 100) % 100;
// 	dd = date % 100;
// 	return (y * 100000 + mm * 1000 + dd * 10 + No);
// }
// 
// 
// 
// UINT32 ThreeMonthsEndDay(UINT32 nBeginDate, UINT8 nMonths)
// {
// 	TDateTime sDate(nBeginDate, 0);//申报起始日
// 	TDateTime eDate;
// 	UINT32 nEndDay = 0;
// 	
// 	eDate = sDate.MonthAdd(sDate, nMonths); //nMonths个月后的日期
// 	eDate--;  //减一天
// 	nEndDay = eDate.FormatInt(YYYYMMDD); //申报截止日
// 	DBG_PRINT(("申报截止日 nEndDay=  %u", nEndDay));
// 
// 	return nEndDay;	
// }
// 
// 
// UINT8 IsLastDay(UINT32 nDate)
// {
// 	UINT32 nNextDate = 0;
// 	TDateTime curDate(nDate, 0);
// 
// 	curDate++;	//下一天
// 	nNextDate = curDate.FormatInt(YYYYMMDD);
// 	if ((nNextDate % 100) == 1)
// 	{
// 		return SUCCESS; //当天是本月最后一天
// 	}
// 	else
// 	{
// 		return FAILURE; //当天不是本月最后一天
// 	}
// }
// 
// 
// UINT8 MakeNormalInv(SaleData &tmpSaleData, UINT8 nTaxId, UINT32 nInvNo)
// {
// 	UINT8 ret;
// 	CInvDet *invDet = NULL;
// 	CInvHead invHead;
// 	tmpSaleData.m_smallInvInfo = &invHead;
//     UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
// 
// 	//组装正票
// 	//组装发票头
// 	tmpSaleData.m_smallInvInfo->m_InvNo = nInvNo;  /**< 发票号码 */
//     tmpSaleData.m_smallInvInfo->m_codeNo = g_globalArg->m_curInvVol->m_codeNo;  /**< 发票代码简称 */  	
// 
// 	tmpSaleData.m_smallInvInfo->m_issuetype = NORMAL_INV;		//开票类型	
// 	tmpSaleData.m_smallInvInfo->m_oldInv = 0;		//原发票号码	
//   
// 	tmpSaleData.m_smallInvInfo->m_payer = "个人";			//付款人名称
// 	tmpSaleData.m_smallInvInfo->m_operator = g_globalArg->m_operator->m_name;		//操作员名称
// 	tmpSaleData.m_smallInvInfo->m_opId = g_globalArg->m_operator->m_opId;		//操作员id	
// 	tmpSaleData.m_smallInvInfo->m_uID = 0;          //纳税户名称ID号
// 	tmpSaleData.m_smallInvInfo->m_payType = 0;		//付款方式
// 	tmpSaleData.m_smallInvInfo->m_goodsCount = 1;//当前发票总商品行加1
// 	tmpSaleData.m_smallInvInfo->m_moneySum = ComputerMoney(nCurDate, nTaxId);//当前发票总金额
// 
//  	tmpSaleData.m_smallInvInfo->m_backup1 = "";		//备用字段1
// 	tmpSaleData.m_smallInvInfo->m_backup2 = "";		//备用字段2 
// 	tmpSaleData.m_smallInvInfo->m_backup3 = "";		//备用字段3
// 
// 	//发票明细	
// //	invDet = new CInvDet;
// 	invDet = invDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
// 	DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
// 	invDet->m_codeNo = tmpSaleData.m_smallInvInfo->m_codeNo; 
// 	invDet->m_InvNo = tmpSaleData.m_smallInvInfo->m_InvNo;
// 	invDet->m_destriptionNo = 0; 
// 	invDet->m_issueDate = nCurDate;
// 	invDet->m_issueTime = nCurTime;
// 	invDet->m_issuetype = NORMAL_INV;
// 
// 	invDet->m_code = "1000000000001";
// 	invDet->m_dpNo = nTaxId;
// 	invDet->m_name = "测试商品1";
// 	invDet->m_taxId = nTaxId;
// 	invDet->m_quantity = 1.0; //保存四舍五入后的值
// 	invDet->m_dotNum = 0;
// 	invDet->m_price = ((double)(tmpSaleData.m_smallInvInfo->m_moneySum))/SUM_EXTENSION;
// 
// 	invDet->m_moneySum = tmpSaleData.m_smallInvInfo->m_moneySum;
// 	invDet->m_property = DETAIL_GENERAL_GOODS;
// 	invDet->m_backup = "";
// 	tmpSaleData.m_smallInvInfo->InsertNode(invDet);	//插入节点
// 		
// 
// 	ret = tmpSaleData.MakeInvoiceHandle(0, INV_ROLL_PROC);
// 	tmpSaleData.m_smallInvInfo->DelList();
// 	tmpSaleData.m_smallInvInfo = NULL;
// 	if (ret != SUCCESS)
// 	{
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}		
// 	}	
// 	DBG_RETURN(SUCCESS);	
// }
// 
// UINT8 MakeReturnInv(SaleData &tmpSaleData, UINT8 nTaxId, UINT32 nInvNo)
// {
// 	UINT8 ret;
// 	CInvDet*invDet = NULL;
// 	CInvHead invHead;
// 	tmpSaleData.m_smallInvInfo = &invHead;
//     UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
// 
// 	//组装红票
// 	//组装发票头
// 	tmpSaleData.m_smallInvInfo->m_InvNo = nInvNo;  /**< 发票号码 */
//     tmpSaleData.m_smallInvInfo->m_codeNo = g_globalArg->m_curInvVol->m_codeNo;  /**< 发票代码简称 */  	
// 	DBG_PRINT(("m_smallInvInfo->m_InvNo =  %u", tmpSaleData.m_smallInvInfo->m_InvNo));
//     DBG_PRINT(("m_smallInvInfo->m_codeNo =  %u", tmpSaleData.m_smallInvInfo->m_codeNo));
// 
// 	tmpSaleData.m_smallInvInfo->m_issuetype = RETURN_INV;		//开票类型	
// 	tmpSaleData.m_smallInvInfo->m_oldInv = tmpSaleData.m_smallInvInfo->m_InvNo - 3;		//原发票号码	
//     DBG_PRINT(("m_smallInvInfo->m_issuetype =  %u", tmpSaleData.m_smallInvInfo->m_issuetype));
//    
// 	tmpSaleData.m_smallInvInfo->m_payer = "个人";			//付款人名称
// 	tmpSaleData.m_smallInvInfo->m_operator = g_globalArg->m_operator->m_name;		//操作员名称
// 	tmpSaleData.m_smallInvInfo->m_opId = g_globalArg->m_operator->m_opId;		//操作员id	
// 	tmpSaleData.m_smallInvInfo->m_uID = 0;          //纳税户名称ID号
// 	tmpSaleData.m_smallInvInfo->m_payType = 0;		//付款方式
// 	tmpSaleData.m_smallInvInfo->m_goodsCount = 1;//当前发票总商品行加1
// 	tmpSaleData.m_smallInvInfo->m_moneySum = ComputerMoney(nCurDate, nTaxId);//当前发票总金额
// 
//  	tmpSaleData.m_smallInvInfo->m_backup1 = "";		//备用字段1
// 	tmpSaleData.m_smallInvInfo->m_backup2 = "";		//备用字段2 
// 	tmpSaleData.m_smallInvInfo->m_backup3 = "";		//备用字段3
// 
// 	
// 	//发票明细	
// 	// invDet = new CInvDet;
// 	invDet = invDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
// 	DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
// 	invDet->m_codeNo = tmpSaleData.m_smallInvInfo->m_codeNo; 
// 	invDet->m_InvNo = tmpSaleData.m_smallInvInfo->m_InvNo;
// 	invDet->m_destriptionNo = 0; 
// 	invDet->m_issueDate = nCurDate;
// 	invDet->m_issueTime = nCurTime;
// 	invDet->m_issuetype = RETURN_INV;
// 
// 	invDet->m_code = "1000000000001";
// 	invDet->m_dpNo = nTaxId;
// 	invDet->m_name = "测试商品1";
// 	invDet->m_taxId = nTaxId;
// 	invDet->m_quantity = 1.0; //保存四舍五入后的值
// 	invDet->m_dotNum = 0;
// 	invDet->m_price = ((double)(tmpSaleData.m_smallInvInfo->m_moneySum))/SUM_EXTENSION;
// 
// 	invDet->m_moneySum = tmpSaleData.m_smallInvInfo->m_moneySum;
// 	invDet->m_property = DETAIL_GENERAL_GOODS;
// 	invDet->m_backup = "";
// 	tmpSaleData.m_smallInvInfo->InsertNode(invDet);	//插入节点		
// 	
// 
// 	ret = tmpSaleData.MakeInvoiceHandle(0, INV_ROLL_PROC);
// 	tmpSaleData.m_smallInvInfo->DelList();
//     tmpSaleData.m_smallInvInfo = NULL;
// 	if (ret != SUCCESS)
// 	{
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}	
// 	}	
// 	DBG_RETURN(SUCCESS);
// 
// }
// 
// UINT8 MakeWasteInv(SaleData &tmpSaleData)
// {
// 	UINT8 ret;
// 	//发票作废
// 	ret = InvWasteHandle(&tmpSaleData);
// 	if(ret != SUCCESS)
// 	{
// 		DBG_PRINT(("InvWaste error !"));
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}		
// 	}
// 	DBG_RETURN(SUCCESS);
// }
// 
// 
// UINT8 NormalDayIssue(SaleData &tmpSaleData, UINT32 nInvNum, UINT32 IssuedNum, UINT8 &nKeyTime)
// {
// 	UINT32 i;
// 	UINT8 ret;
// 	UINT32 nInvNo = 0;
// 	INT8 chValue[128];
// 	memset((void*)chValue, 0, sizeof(chValue));
// 	CaProgressBar proBar("");
// 	
// 	//三张正票
// 	for(i=IssuedNum+1; i<=nInvNum; i++)
// 	{
// 		//卷汇总，查询发票卷，并导入。若无发票卷，则报错并返回
// 		if (GetInvNo(tmpSaleData, nInvNo) != SUCCESS)
// 		{
// 			return FAILURE;
// 		}
// 		//两次按“退出键”,则退出程序
// 		if(CheckKey(nKeyTime)==SUCCESS)
// 		{
// 			return FAILURE; 
// 		}
// 		//判断是否滚动，若要滚动，显示消息框
// 		if(IfInvHeadRoll()!=SUCCESS)
// 		{
// 			return FAILURE; 			
// 		}
// 
// 		DBG_PRINT(("自动开正票"));
// 		if ((ret = MakeNormalInv(tmpSaleData, i%TAX_DIVISOR, nInvNo))!= SUCCESS)
// 		{
// 			if (ret==DB_FULL) 
// 			{
// 				CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// 			}
// 			else if (HAVE_ROLLED==ret)
// 			{
// 				CaMsgBox::ShowMsg("滚动完毕!");			
// 			}
// 			else
// 			{
// 				CaMsgBox::ShowMsg("开票失败!");
// 			}	
// 			return FAILURE;
// 		}
// 		//----销售相关变量复位
// 		tmpSaleData.InitSaleData(1);	//销售信息初始化
// 		tmpSaleData.InitInvHead();		//单张发票信息初始化
// 		
// 		//memset((void*)chValue, 0, sizeof(chValue));
// 		sprintf(chValue, "自动开票: %u号",nInvNo);
// 		proBar.SetText(chValue);
// 		proBar.ReFresh();
// 	}
// 	
// 	return SUCCESS;
// 
// }
// 
// 
// UINT8 LastDayIssue(SaleData &tmpSaleData, UINT32 nInvNum, UINT32 IssuedNum, UINT8 &nKeyTime)
// {
// 	UINT32 i;
// 	UINT8 ret;
// 	UINT32 nInvNo = 0;
// 	INT8 chValue[256];
// 	UINT32 retIssuedNum = 0;
// 	memset((void*)chValue, 0, sizeof(chValue));
// 	CaProgressBar proBar("");
// 	
// 	if (IssuedNum<=nInvNum)
// 	{
// 		//三张正票
// 		if (NormalDayIssue(tmpSaleData, nInvNum, IssuedNum, nKeyTime)!=SUCCESS) 
// 		{
// 			return FAILURE;		
// 		}
// 		IssuedNum = 0;
// 	}
// 	else
// 	{
// 		retIssuedNum = IssuedNum - 3;//红票已开数量		
// 	}
// 
// 	//三张红票和一张废票
// 	for(i=retIssuedNum+1; i<=nInvNum+1; i++)
// 	{
// 		//卷汇总，查询发票卷，并导入。若无发票卷，则报错并返回
// 		if (GetInvNo(tmpSaleData, nInvNo) != SUCCESS)
// 		{
// 			return FAILURE;
// 		}
// 		//两次按“退出键”,则退出程序
// 		if(CheckKey(nKeyTime)==SUCCESS)
// 		{
// 			return FAILURE; 
// 		}
// 		if(IfInvHeadRoll()!=SUCCESS)
// 		{
// 			return FAILURE; 			
// 		}
// 
// 		if (4 == i)
// 		{
// 			DBG_PRINT(("自动开废票"));
// 			ret = SimpleMakeWasteInv(tmpSaleData);			
// 		}
// 		else
// 		{
// 			DBG_PRINT(("自动开红票"));
// 			ret = MakeReturnInv(tmpSaleData, i%TAX_DIVISOR, nInvNo);
// 		}
// 
// 		if (ret!= SUCCESS)
// 		{
// 			if (ret==DB_FULL) 
// 			{
// 				CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// 			}
// 			else if (HAVE_ROLLED==ret)
// 			{
// 				CaMsgBox::ShowMsg("滚动完毕!");			
// 			}
// 			else
// 			{
// 				CaMsgBox::ShowMsg("开票失败!");
// 			}	
// 			return FAILURE;
// 		}
// 		//----销售相关变量复位
// 		tmpSaleData.InitSaleData(1);	//销售信息初始化
// 		tmpSaleData.InitInvHead();		//单张发票信息初始化
// 		
// 		//memset((void*)chValue, 0, sizeof(chValue));
// 		sprintf(chValue, "自动开票: %u号",nInvNo);
// 		proBar.SetText(chValue);
// 		proBar.ReFresh();
// 	}	
// 	DBG_RETURN(SUCCESS);
// 
// }

UINT8 IfInvHeadRoll()
{
	CInvHead curInvHead;
	UINT8 ret = SUCCESS;
	UINT32 nInvDelDate = 0;
	
//	ret = CheckIfFull(g_globalArg->m_invCount, g_globalArg->m_invSumCount);
//	if (ret==SUCCESS) 
//	{
//		DBG_RETURN(DB_FULL);		
//	}

	TDateTime lastDate(g_globalArg->m_startDecDate, 000001);
	lastDate--;
	UINT32 nRefDate = lastDate.FormatInt(YYYYMMDD);//上次申报的截止日期

	//判断是否需要滚动，并计算需滚动的记录数量
	ret = curInvHead.CheckRoll(g_globalArg->m_invCount, nRefDate,nInvDelDate);
	if (ret!=SUCCESS)
	{
		CaMsgBox::ShowMsg("发票张数滚动判断失败!");
		DBG_RETURN(FAILURE);
	}

	//滚动
	if (nInvDelDate>0)
	{
		DBG_PRINT(("nInvDelDate = %u", nInvDelDate));
		CaMsgBox::ShowMsg("发票张数要滚动!");
	}
	DBG_RETURN(SUCCESS);
}

UINT8 IfInvSumRoll()
{
	CInvSum curInvSum;
	UINT8 ret = SUCCESS;
	UINT32 nSumDelDate = 0;

	TDateTime lastDate(g_globalArg->m_startDecDate, 000001);
	lastDate--;
	UINT32 nRefDate = lastDate.FormatInt(YYYYMMDD);//上次申报的截止日期

	ret = curInvSum.CheckRoll(g_globalArg->m_invSumCount, nRefDate,nSumDelDate);
	if (ret!=SUCCESS)
	{
		CaMsgBox::ShowMsg("单卷汇总滚动判断失败!");
		DBG_RETURN(FAILURE);
	}

	if (nSumDelDate>0)
	{
		DBG_PRINT(("nSumDelDate = %u", nSumDelDate));
		CaMsgBox::ShowMsg("单卷汇总要滚动!");		
	}
	
	DBG_RETURN(SUCCESS);

}

UINT8 IfRtInvRoll()
{
	CRtInv curRtInv;
	UINT8 ret = SUCCESS;
// 	UINT32 nSumDelDate = 0;
	UINT32 nRtDetNo = 0;

//	TDateTime lastDate(g_globalArg->m_startDecDate, 000001);
//	lastDate--;
//	UINT32 nRefDate = lastDate.FormatInt(YYYYMMDD);//上次申报的截止日期

	ret = curRtInv.CheckRoll(g_globalArg->m_rtInvCount, nRtDetNo);
	if (ret!=SUCCESS)
	{
		CaMsgBox::ShowMsg("红票信息滚动判断失败!");
		DBG_RETURN(FAILURE);
	}

	if (nRtDetNo>0)
	{
		DBG_PRINT(("nRtDetNo = %u", nRtDetNo));
		CaMsgBox::ShowMsg("红票信息要滚动!");		
	}
	
	DBG_RETURN(SUCCESS);

}

//-----------15所 容量测试----------------------//

/*
UINT8 WriteSysLog(void)
{
	CSysLog syslog;
	INT32 nCount = 0;
	INT32 nLeft = 0;
	UINT32 i;
	INT8 chMsg[128];
	CaProgressBar proBar("");
	UINT8 nKeyTime = 0;
	UINT8 ret;

	nCount = syslog.GetSysLogNum();
	DBG_PRINT(("nCount = %d", nCount));
	
	if (nCount<0)
	{
		nCount = 0;
		CaMsgBox::ShowMsg("统计系统日志表失败");
		DBG_RETURN(FAILURE);
	}

	nLeft =  SYSLOG_MAX - nCount;
	if (nLeft>0) 
	{
		for(i = 0; i<nLeft; i++)
		{
			//两次按“退出键”,则退出程序
			if(CheckKey(nKeyTime)==SUCCESS)
			{
				return FAILURE; 
			}
			ret = CGlobalArg::SysLog(EVENT_IMPORT_INVOICE, SUCCESS);//写系统日志
			if (ret!=SUCCESS)
			{
				CaMsgBox::ShowMsg("写系统日志表失败");
				DBG_RETURN(FAILURE);
			}
			nCount++;
			sprintf(chMsg, "插入SysLog记录: %u号",nCount);
			proBar.SetText(chMsg);
			proBar.ReFresh();

//			for (UINT32 j = 0; j<0xAFFFFF; j++);
			FOR_DELAY(0xAFFFFF);
		}
	}
	DBG_RETURN(SUCCESS);
}*/



/*
UINT8 WritePlu(void)
{
//	CPlu plu;
	CDept plu;
	INT32 nCount = 0;
//	INT32 nLeft = 0;
	UINT32 i;
	CaProgressBar proBar("");
	INT8 chValue[128];
	INT8 chMsg[128];
	memset((void*)chValue, 0, sizeof(chValue));
	memset((void*)chMsg, 0, sizeof(chMsg));
	UINT8 nKeyTime = 0;
	UINT8 ret;

		for(i = g_globalArg->m_pluCount+1; i<=PLU_MAX; i++)
		{
			//两次按“退出键”,则退出程序
			if(CheckKey(nKeyTime)==SUCCESS)
			{
				return FAILURE; 
			}
	
			sprintf(chValue, "xx%u", i);
			plu.m_code = chValue;
//			plu.m_nameAbbr = "2";
			plu.m_name = "蒙牛冰淇淋";
			plu.m_dept = 2;
			plu.m_taxId = 2;
			plu.m_price = 50.0;
			plu.m_rate = 100.0;
			plu.m_unit = "piece";
			ret = plu.AddNew();
			if (ret != SQLITE_OK)
			{
				if (SQLITE_ERROR==ret) {
				}
				DBG_PRINT(("写PLU表失败 ret = %u", ret));
				CaMsgBox::ShowMsg("写PLU表失败");
				DBG_RETURN(FAILURE);
			}

			g_globalArg->m_pluCount++;
			sprintf(chMsg, "插入PLU记录: %u号",g_globalArg->m_pluCount);
			proBar.SetText(chMsg);
			proBar.ReFresh();

//			for (UINT32 j = 0; j<0xAFFFFF; j++);
			FOR_DELAY(0xAFFFFF);

		}

	DBG_RETURN(SUCCESS);
}*/


// UINT8 NetAutoIssue(UINT8 &nIfInvSum)
// {
// //	INT32 nKey = 0x0;
// 	UINT8 nKeyTime = 0;
// 	UINT32 nDate;
//     UINT8 nVolNum = 0;//发票卷数
// 	UINT32 nInvNo = 0;
// 	UINT32 nCurrentInvNo = 0;
// 	INT8 chValue[256];
// 	string strInfo;
// 	memset((void*)chValue, 0, sizeof(chValue));
// 	SaleData saleInfo;
// 	CaProgressBar proBar("");
// 	UINT8 ret;
// 
// 	//----查询商品信息
// 	CDept curDept;
// 	//组装查询条件
// 	sprintf(chValue,"WHERE DP_NO = %d", 1);
// 	DBG_PRINT(("--------chValue = %s--------", chValue));
// 	curDept.m_filter = chValue;
// 	curDept.Requery();
// 	INT32 errorcode = curDept.LoadOneRecord();
// 
// 	if (errorcode != SQLITE_OK)
// 	{
// 		CaMsgBox::ShowMsg("c查询部类信息错");
// 		DBG_PRINT((" Warning: No matched goods!  errorcode = %d", errorcode));
// 		ret = FAILURE;
// 		DBG_RETURN(ret);	
// 	}
// 	
//     DBG_PRINT(("m_dpNo : %d ", curDept.m_dept));
// 	DBG_PRINT(("m_name : %s ", curDept.m_name.c_str()));
// 	DBG_PRINT(("m_taxId : %d ", curDept.m_taxId));
// 	
// 	for(;;)
// 	{
// 		//超过开票截止日期，停止开票
// 		nDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 		DBG_PRINT(("for后 nDate = %u", nDate));
// 		DBG_PRINT(("for后 g_globalArg->m_curInvVol->m_remain = %u", g_globalArg->m_curInvVol->m_remain));
// 
// //		if (nDate>g_globalArg->m_corpInfo->m_ieDate) 
// //		{
// //			CaMsgBox::ShowMsg("超过开票截止日期\n请立即申报!");
// //			return FAILURE;		
// //		}
// //		if (1 == g_globalArg->m_declareLimitFlag)
// //		{
// //			CaMsgBox::ShowMsg("申报期超限,请立即申报");
// //			return FAILURE;
// //		}
// 		
// 		//连续按两次确认键，则退出开票程序
// 		if(CheckKey(nKeyTime)==SUCCESS)
// 		{
// 			return SUCCESS;
// 		}
// 
// 		DBG_PRINT(("开始自动开票"));
// 
// 		if(g_globalArg->m_curInvVol->m_remain != 0)
// 		{
// 			nCurrentInvNo = g_globalArg->m_curInvVol->m_InvEndNo + 1 - 
// 					g_globalArg->m_curInvVol->m_remain;
// 			//判断是否滚动，若要滚动，显示消息框
// 			if(IfInvHeadRoll()!=SUCCESS)
// 			{
// 				return FAILURE; 			
// 			}
// 			if(IfInvSumRoll()!=SUCCESS) 
// 			{
// 				return FAILURE; 			
// 			}
// 			if(IfRtInvRoll()!=SUCCESS) 
// 			{
// 				return FAILURE; 			
// 			}
// 
// //			if (g_globalArg->m_curInvVol->m_remain == 2)
// //			{
// //				DBG_PRINT(("自动开红票"));
// //				if ((ret = SimpleMakeReturnInv(saleInfo)) != SUCCESS)
// //				{
// //					if (ret==DB_FULL) 
// //					{
// //						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// //					}
// //					else if (HAVE_ROLLED==ret)
// //					{
// //						CaMsgBox::ShowMsg("滚动完毕!");			
// //					}
// //					else
// //					{
// //						CaMsgBox::ShowMsg("开红票失败!");
// //					}					
// //					return FAILURE;
// //				}
// //			}
// //			else if (g_globalArg->m_curInvVol->m_remain == 1)
// //			{
// //				DBG_PRINT(("自动开废票"));
// //				if ((ret = SimpleMakeWasteInv(saleInfo)) != SUCCESS)
// //				{
// //					if (ret==DB_FULL) 
// //					{
// //						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// //					}
// //					else if (HAVE_ROLLED==ret)
// //					{
// //						CaMsgBox::ShowMsg("滚动完毕!");			
// //					}
// //					else
// //					{
// //						CaMsgBox::ShowMsg("开废票失败!");
// //					}	
// //					return FAILURE;
// //				}
// //			}
// //			else
// //			{
// 				DBG_PRINT(("自动开正票"));
// 				if ((ret = NetMakeNormalInv(saleInfo, &curDept))!= SUCCESS)
// 				{
// 					if (ret==DB_FULL) 
// 					{
// 						CaMsgBox::ShowMsg("存储空间已满，无法开票 请尽快申报!");
// 					}
// 					else if (HAVE_ROLLED==ret)
// 					{
// 						CaMsgBox::ShowMsg("滚动完毕!");			
// 					}
// 					else
// 					{
// 						CaMsgBox::ShowMsg("开正票失败!");
// 					}	
// 					return FAILURE;
// 				}
// //			}
// 			//----销售相关变量复位
// 			saleInfo.InitSaleData(1);	//销售信息初始化
// 			saleInfo.InitInvHead();		//单张发票信息初始化
// 			
// 			//memset((void*)chValue, 0, sizeof(chValue));
// 			sprintf(chValue, "自动开票: %u号",nCurrentInvNo);
// 			proBar.SetText(chValue);
// 			proBar.ReFresh();
// 		}
// 		else
// 		{
// 			//卷汇总，查询发票卷，并导入。若无发票卷，则报错并返回
// 			if (GetInvNo(saleInfo, nInvNo) != SUCCESS)
// 			{
// 				return FAILURE;
// 			}
// 
// 			if(nVolNum < 1)
// 			{
// 				nVolNum++;
// 				continue;
// 			}
// 			nVolNum = 0;
// 			
// 			//修改系统时间到第二天	
// 			TDateTime dateTime(nDate, 80000);
// 			dateTime++;
// 			if (!TDateTime::SetCurrentDateTime(dateTime))
// 			{
// 				CaMsgBox::ShowMsg("修改日期时间错误!");
// 				return FAILURE;			
// 			}
// 			UINT32 nTestDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 			DBG_PRINT(("延迟后 nTestDate = %u", nTestDate));
// 		
// 			//日汇总
// 			INT32 errorcode = 0;
// 			INT8 charValue[128];
// 			memset((void*)charValue, 0, sizeof(charValue));
// 			if ((errorcode = g_globalArg->CheckDaySum(0)) != SUCCESS)
// 			{
// 				sprintf(charValue, "日交易汇总错误：%02x", errorcode);
// 				CaMsgBox::ShowMsg(charValue);
// 				return FAILURE;	
// 			}
// //			UINT32 nIndex = 0;
// //			while (nIndex<0xFFFFFF)
// //			{
// //				nIndex = nIndex + 1;
// //			}
// 			
// 		}
// 	}
// }
// 
// 
// UINT8 NetMakeNormalInv(SaleData &tmpSaleData, CDept *pDept)
// {
// 	UINT8 ret;
// 	CInvDet *invDet = NULL;
// 	CInvHead invHead;
// 	tmpSaleData.m_smallInvInfo = &invHead;
//     UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
// 	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
// 
// 	//组装正票
// 	//组装发票头
// 	tmpSaleData.m_smallInvInfo->m_InvNo = g_globalArg->m_curInvVol->m_InvEndNo + 1 - 
// 		g_globalArg->m_curInvVol->m_remain;  /**< 发票号码 */
//     tmpSaleData.m_smallInvInfo->m_codeNo = g_globalArg->m_curInvVol->m_codeNo;  /**< 发票代码简称 */  	
// 
// 	tmpSaleData.m_smallInvInfo->m_issuetype = NORMAL_INV;		//开票类型	
// 	tmpSaleData.m_smallInvInfo->m_oldInv = 0;		//原发票号码	
//   
// 	tmpSaleData.m_smallInvInfo->m_payer = "个人";			//付款人名称
// 	tmpSaleData.m_smallInvInfo->m_operator = g_globalArg->m_operator->m_name;		//操作员名称
// 	tmpSaleData.m_smallInvInfo->m_opId = g_globalArg->m_operator->m_opId;		//操作员id	
// 	tmpSaleData.m_smallInvInfo->m_uID = 0;          //纳税户名称ID号
// 	tmpSaleData.m_smallInvInfo->m_payType = 0;		//付款方式
// 	tmpSaleData.m_smallInvInfo->m_goodsCount = 3;//当前发票总商品行加1
// 	tmpSaleData.m_smallInvInfo->m_moneySum = (INT64)(0.3 * SUM_EXTENSION);//当前发票总金额
// 
//  	tmpSaleData.m_smallInvInfo->m_backup1 = "";		//备用字段1
// 	tmpSaleData.m_smallInvInfo->m_backup2 = "";		//备用字段2 
// 	tmpSaleData.m_smallInvInfo->m_backup3 = "";		//备用字段3
// 
// 
// 	//发票明细
// 	for(UINT8 i = 1; i<4; i++)
// 	{
// //		invDet = new CInvDet;
// 		invDet = invDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
// 		DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
// 		invDet->m_codeNo = tmpSaleData.m_smallInvInfo->m_codeNo; 
// 		invDet->m_InvNo = tmpSaleData.m_smallInvInfo->m_InvNo;
// 		invDet->m_destriptionNo = 0; 
// 		invDet->m_issueDate = nCurDate;
// 		invDet->m_issueTime = nCurTime;
// 		invDet->m_issuetype = NORMAL_INV;
// 
// 		invDet->m_code = pDept->m_code;
// 		invDet->m_dpNo = pDept->m_dept;
// 		invDet->m_name = pDept->m_name;
// 		invDet->m_taxId = pDept->m_taxId; //非税控只允许税目1
// 		invDet->m_quantity = 1.0; //保存四舍五入后的值
// 		invDet->m_dotNum = 0;
// 		invDet->m_price = (double)0.1;
// 
// 		invDet->m_moneySum = (INT64)(0.1*SUM_EXTENSION);
// 		invDet->m_property = DETAIL_GENERAL_GOODS;
// 		invDet->m_backup = "";
// 		invDet->m_tradeName = pDept->m_tradeName;
// 		invDet->m_indusCode = pDept->m_indusCode;
// 		tmpSaleData.m_smallInvInfo->InsertNode(invDet);	//插入节点
// 		
// 	}
// 	
// 	tmpSaleData.m_smallInvInfo->m_tradeName = pDept->m_tradeName;
// 
// 	ret = tmpSaleData.MakeInvoiceHandle(PRINT, INV_ROLL_PROC);
// 	tmpSaleData.m_smallInvInfo->DelList();
// 	tmpSaleData.m_smallInvInfo = NULL;
// 	if (ret != SUCCESS)
// 	{
// 		if (ret==DB_FULL) 
// 		{
// 			DBG_RETURN(DB_FULL);
// 		}
// 		else if (ret==HAVE_ROLLED) 
// 		{
// 			DBG_RETURN(HAVE_ROLLED);
// 		}
// 		else
// 		{
// 			DBG_RETURN(FAILURE);
// 		}	
// 	}	
// 	DBG_RETURN(SUCCESS);	
// }




#endif


