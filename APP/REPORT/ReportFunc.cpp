/*! \file    ReportFunc.cpp
   \brief    awe4000r报表打印的应用函数
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */


#include "ReportFunc.h"
#include "CaProgressBar.h"
#include "keypad.h"

#include "TDateTime.h"
#include "PrintDriver.h"
#include "CGlobalArg.h"
#include "CInvDet.h"
#include "YWXMLGY.h"
#include "APIBase.h"

#ifndef WIN32
#include <sys/timeb.h>
#include "lcd_tiny.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#define MAX_LINE_PRINT_NUM		255		//打印行可打印的最大数
//事件名称列表
INT8 *g_EventTip[] = 
{
    "下载机器编号",
	"登录系统",
	"更新系统",
	"更新模板",
	"掉电",
	"拔卡",
	"增加操作员",
	"删除操作员",
	"编辑操作员",
	"发票信息滚动",
	"发票卷信息滚动",
	"系统日志滚动",
	"更新机器信息",
	"非法用户卡",
	"非法税控卡",
	"非法管理卡",
	"非法PIN码",
	"初始化",
	"发票分发",
	"开正票",
	"红票信息表滚动",
	"设置时间",
	"报税",
	"完税",
	"稽查",
	"更新用户信息",
	"发票导入",
	"掉电保护：初始化机器",
	"掉电保护：发票分发",
	"掉电保护：发票导入",
	"掉电保护：开票",
	"掉电保护：滚动",
	"掉电保护：报税",
	"掉电保护：完税",
	"开红票",
	"开废票"
	
};

UINT8 YesNoMsgBox(string strInfo)
{
	DBG_ENTER("YesNoMsgBox(string strInfo)");
	
	CaMsgBox msgBox(strInfo.c_str(),CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		DBG_PRINT((" msgBox.m_iStatus == NO_PRESSED!"));
		DBG_RETURN(FAILURE); 
	}
	
	if(msgBox.m_iStatus == OK_PRESSED)
	{
//		ReFresh();
		DBG_PRINT((" msgBox.m_iStatus == OK_PRESSED!"));
		DBG_RETURN(SUCCESS);		
	}
}

//月统计查询
UINT8 GetMonthSumCount(CTjxxhz *pTjxxhz, string &strErr)
{
	INT32 retCode = 0;
	
	DBG_PRINT(("g_YwXmlArg->m_fplxdm : %s", g_YwXmlArg->m_fplxdm.c_str()));
	
	retCode = g_pAPIBase->TJXXCXPro_API(*g_YwXmlArg, pTjxxhz, strErr);
	DBG_PRINT(("GetMonthSumCount_retCode : %d", retCode));

	if (retCode != SUCCESS)
	{
		return FAILURE;
	}
	return SUCCESS;
}

/*
//----日汇总查询
UINT8 GetDaySumCount(UINT32 nStartDate, UINT32 nEndDate, UINT32 &nNum)
{
	DBG_ENTER("GetDaySumCount");

	nNum = 0;
	INT32 errorcode;
	INT32 nTmpNum = 0;
	CDaySum curDaySum;
	CDaySum *pDaySum = &curDaySum;
	DBG_ASSERT_EXIT((pDaySum != NULL), (" pDaySum == NULL!"));

	//查总记录数

	INT8 value[256];
	memset((void*)value, 0, sizeof(value));
	pDaySum->m_filter = "";
	sprintf(value,"where I_DATE >= %u and I_DATE <= %u",nStartDate,nEndDate);
	pDaySum->m_filter.append(value);
	DBG_PRINT((" pDaySum->m_filter = %s!", pDaySum->m_filter.c_str()));
	nTmpNum = pDaySum->GetRecordNum();

// 	delete pDaySum;
	pDaySum = NULL;

	if (nTmpNum<0)
	{
		DBG_RETURN(FAILURE); 		
	}
	nNum = nTmpNum;
	DBG_RETURN(SUCCESS); 
}

//----时间段汇总
*/
UINT8 IsTimeValid(UINT32 hhmmss)
{
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	
	hour   = (UINT8)((hhmmss/10000));
	minute = (UINT8)((hhmmss/100)-(hour)*100);
	second = (UINT8)(hhmmss-(hhmmss/100)*100);

	if (hour>23 || minute >59 || second >59) 
	{
		return FAILURE;
	}

	return SUCCESS;
}
//---打印系统日志

UINT8 PrnSysLogHead(UINT32 nStartDate, UINT32 nEndDate)
{
	DBG_PRINT((" PrnSysLogHead"));

	UINT32 nCurDate, nCurTime;
	
	SetLineSpace(REPORT_LINE_SPACE);

	PrintReportLine("         << 安 全 日 志 报 表 >>", sizeof("         << 安 全 日 志 报 表 >>"));
	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100, nCurTime % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "打印区间：%04lu-%02lu-%02lu 至 %04lu-%02lu-%02lu", 
			 nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100, 
			 nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));			

	PrintReportLine("========================================", sizeof("========================================"));
	
	return SUCCESS;
}

UINT8 PrnSysLogDetail(const CSysLog *pSysLog)
{
	INT8 chValue[256];
	DBG_ASSERT_EXIT((pSysLog != NULL), (" pSysLog == NULL "));

	SetLineSpace(REPORT_LINE_SPACE);
	//事件发生日期
	sprintf(pGeneralPrnBuff, "事件发生日期：%04lu-%02lu-%02lu  时间：%02lu:%02lu:%02lu", pSysLog->m_idate / 10000,
					(pSysLog->m_idate % 10000) / 100, pSysLog->m_idate % 100, pSysLog->m_itime / 10000, (pSysLog->m_itime % 10000) / 100, pSysLog->m_itime % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));		
	
	//事件类型
	sprintf(pGeneralPrnBuff, "事件类型：%s", g_EventTip[pSysLog->m_type]);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));	
		
	//事件操作员
//	memset((void*)chValue, 0, sizeof(chValue));
//	sprintf(chValue, "where OP_ID = %u", pSysLog->m_operator);
//	COperator eventOperator;
//	eventOperator.m_filter = chValue;
//	eventOperator.Requery();
//	if(eventOperator.LoadOneRecord()!=SQLITE_OK)
//	{
//		CaMsgBox::ShowMsg("读操作员信息失败");
//		eventOperator.m_name = "";
//	}
	sprintf(pGeneralPrnBuff, "操作员：  %s", pSysLog->m_operator.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	//事件结果
	switch(pSysLog->m_result) 
	{
		case FAILURE:
			sprintf(pGeneralPrnBuff, "事件结果：失败");
			break;
		case SUCCESS:
			sprintf(pGeneralPrnBuff, "事件结果：成功");
			break;
		case OVERFLOW:
			sprintf(pGeneralPrnBuff, "事件结果：越界");
			break;
		default:
			sprintf(pGeneralPrnBuff, "事件结果：失败  错误类型代码：%u", pSysLog->m_result);
			break;
	}
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	//事件备注
	if ("" != pSysLog->m_backup) 
	{
		sprintf(pGeneralPrnBuff, "事件备注：%s", pSysLog->m_backup.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	}
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));

	DBG_PRINT((" PrnSysLogDetail SUCCESS"));
	return SUCCESS;
}

/*
//----打印PLU
UINT8 PrnPLUHead()
{
	DBG_PRINT((" PrnPLUHead"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("             PLU信息列表", sizeof("             PLU项目列表"));	
	PrintReportLine("", sizeof(""));	
	PrintReportLine("代码            名称               价格 ", sizeof("代码            名称               价格 "));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnPLUDetail(const CPlu *pPlu)
{
	DBG_PRINT((" PrnPLUDetail"));
	DBG_ASSERT_EXIT((pPlu != NULL), (" pPlu == NULL "));

	SetLineSpace(REPORT_LINE_SPACE);
	sprintf(pGeneralPrnBuff, "%-16s%-18s%6.2f", pPlu->m_code.c_str(), pPlu->m_name.c_str(), pPlu->m_price);				
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

//	ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}
*/
//----打印客户名称
UINT8 PrnClientHead()
{
	DBG_PRINT((" PrnPLUHead"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("             客户管理报表", sizeof("             客户管理报表"));	
	PrintReportLine("", sizeof(""));	
	PrintReportLine("编号               名称", sizeof("编号              名称"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnClientDetail(const CClient *pClient)
{
	DBG_PRINT((" PrnClientDetail"));
	DBG_ASSERT_EXIT((pClient != NULL), (" pClient == NULL "));

	SetLineSpace(REPORT_LINE_SPACE);
	sprintf(pGeneralPrnBuff, "%-5s%-18s", pClient->m_khbh.c_str(), pClient->m_khmc.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
//	ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}

//----部类报表
UINT8 PrnDeptHead()
{
	DBG_PRINT((" PrnDeptHead"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("             商品信息列表", sizeof("             商品信息列表"));	
	PrintReportLine("", sizeof(""));	
	PrintReportLine("商品编码        部类号      名称", sizeof("商品编码        部类号      名称"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnDeptDetail(const CDept *pDept)
{
	DBG_PRINT((" PrnDeptDetail"));
	DBG_ASSERT_EXIT((pDept != NULL), (" pDept == NULL "));

	SetLineSpace(REPORT_LINE_SPACE);
	sprintf(pGeneralPrnBuff, "%-13s   %-4u   %-20s",  pDept->m_spbm.c_str(), pDept->m_blh, pDept->m_spmc.c_str());				
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

//	ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}

//----操作员报表
UINT8 PrnOperatorHead()
{
	DBG_PRINT((" PrnOperatorHead"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("             操作员信息列表", sizeof("             操作员信息列表"));	
	PrintReportLine("", sizeof(""));	
	PrintReportLine("序号                      名称", sizeof("序号                      名称"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnOperatorDetail(const COperator *pOperator)
{
	DBG_PRINT((" PrnOperatorDetail"));
	DBG_ASSERT_EXIT((pOperator != NULL), (" pOperator == NULL "));

	SetLineSpace(REPORT_LINE_SPACE);
	sprintf(pGeneralPrnBuff, "%03d      %s", pOperator->m_opid, pOperator->m_name.c_str());				
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
    
	//ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}
/*
//----销售员统计

UINT8 PrnSalerSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime)
{
	DBG_PRINT((" PrnSalerSumHead()"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("    << 销 售 员  销 售 统 计 报 表 >>", sizeof("    << 销 售 员  销 售 统 计 报 表 >>"));
//	PrintReportHead(startDate, endDate, &startTime, &endTime);		//打印制表信息
	UINT32 nCurDate, nCurTime;

	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	

	sprintf(pGeneralPrnBuff, "统计区间：   %04lu-%02lu-%02lu   %02lu:%02lu:%02lu",
									nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100, 
									nStartTime / 10000, (nStartTime % 10000) / 100,(nStartTime % 10000) % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "          至 %04lu-%02lu-%02lu   %02lu:%02lu:%02lu", 
									nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100, 
									nEndTime / 10000, (nEndTime % 10000) / 100, (nEndTime % 10000) % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	PrintReportLine("========================================", sizeof("========================================"));

	return SUCCESS;
}

UINT8 PrnSaler(const string *pSaler)
{
	DBG_PRINT((" PrnSaler()"));
	sprintf(pGeneralPrnBuff, "销售员：%s", pSaler->c_str());
	DBG_PRINT((" 销售员：%s", pSaler->c_str()));
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("商 品                    销售金额", sizeof("商 品                    销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnSalerDeptSum(const string *pDeptName, INT64 nSum)
{
	DBG_PRINT((" PrnSalerDeptSum()"));
	sprintf(pGeneralPrnBuff, "%-20s %.2f", pDeptName->c_str(), (double)nSum/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	return SUCCESS;
}

UINT8 PrnSalerSum(UINT32 nInvCount, INT64 nSum)
{
	DBG_PRINT((" PrnSalerSum()"));
	DBG_PRINT((" nInvCount = %u, nSum = %d", nInvCount, nSum));

	sprintf(pGeneralPrnBuff, "开票数量：%u", nInvCount);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "销售金额：%.2f", (double)nSum/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));		
	PrintReportLine("========================================", sizeof("========================================"));

	return SUCCESS;
}
*/

//----日销售统计
UINT8 PrnSaleSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime, UINT8 nIfDay)
{
	DBG_PRINT((" PrnDaySaleSumHead()"));
	SetLineSpace(REPORT_LINE_SPACE);

	if (1==nIfDay)
	{
		PrintReportLine("       << 发 票 日 统 计 报 表 >>", sizeof("        << 发 票 日 统 计 报 表 >>"));
	}
	else if (0==nIfDay) 
	{
		PrintReportLine("       << 发 票 月 统 计 报 表 >>", sizeof("        << 发 票 月 统 计 报 表 >>"));

	}
	

	UINT32 nCurDate, nCurTime;

	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	if (1==nIfDay) 
	{
		sprintf(pGeneralPrnBuff, "统计区间：   %04lu-%02lu-%02lu   %02lu:%02lu:%02lu",
									nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100, 
									nStartTime / 10000, (nStartTime % 10000) / 100,(nStartTime % 10000) % 100);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		
		sprintf(pGeneralPrnBuff, "          至 %04lu-%02lu-%02lu   %02lu:%02lu:%02lu", 
										nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100, 
										nEndTime / 10000, (nEndTime % 10000) / 100, (nEndTime % 10000) % 100);
	}
	else if (0==nIfDay)
	{
		sprintf(pGeneralPrnBuff, "统计区间：   %04lu-%02lu-%02lu  至 %04lu-%02lu-%02lu ",
									nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100, 
									nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100);
	}
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	PrintReportLine("========================================", sizeof("========================================"));

	return SUCCESS;
}

UINT8 PrnDate(const string *strIssueDate)
{
	DBG_PRINT((" PrnSaler()"));
	sprintf(pGeneralPrnBuff, "开票日期：%s", strIssueDate->c_str());
	DBG_PRINT((" 开票日期：%s", strIssueDate->c_str()));
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("商 品                 销售金额", sizeof("商 品                 销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnDayDeptSum(const string *pDeptName, INT64 nSum)
{
	DBG_PRINT((" PrnSalerDeptSum()"));


	sprintf(pGeneralPrnBuff, "%-20s %.2f", pDeptName->c_str(), (double)nSum/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("========================================", sizeof("========================================"));
	return SUCCESS;
}

UINT8 PrnDaySaleSum(UINT32 nInvCount, INT64 nSum)
{
	DBG_PRINT((" PrnSalerSum()"));
	DBG_PRINT((" nInvCount = %u, nSum = %d", nInvCount, nSum));

	sprintf(pGeneralPrnBuff, "日开票数量：%u", nInvCount);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "日销售总额：%.2f", (double)nSum/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));		
	PrintReportLine("========================================", sizeof("========================================"));

	return SUCCESS;
}

UINT8 PrnAllDaySaleSum(UINT32 nInvCount, INT64 nSum)
{
	DBG_PRINT((" PrnAllDaySaleSum()"));
	DBG_PRINT((" nInvCount = %u, nSum = %d", nInvCount, nSum));

	PrintReportLine("           总 计", sizeof("           总 计"));
	sprintf(pGeneralPrnBuff, "总开票 数量： %u", nInvCount);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "累 计 金 额： %.2f", (double)nSum/100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	PrintReportLine("商 品                    销售金额", sizeof("商 品                    销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));

	return SUCCESS;
}

UINT8 PrnDeptMoney(INT64 &sumMoney, const string *strFilter)
{
	DBG_ASSERT_EXIT((strFilter != NULL), (" strFilter == NULL!"));

	INT32 retCode;
	INT64 norMoney = 0;//某部类正票金额
	INT64 retMoney = 0;//某部类红票金额
	
	INT8 chDeptValue[128];
	memset((void*)chDeptValue, 0, sizeof(chDeptValue));
	INT8 chValue[512];
	memset((void*)chValue, 0, sizeof(chValue));

	CInvDet invDet;
	CDept dept;

	//按部类进行统计,打印部类金额
	dept.Requery();
	retCode = dept.MoveFirst();
	while((retCode == SQLITE_ROW)||(retCode == SQLITE_DONE))
	{
		if(retCode == SQLITE_DONE)
		{
			dept.m_spbm = "000";
			dept.m_blh = 0;
			dept.m_spmc = "其它";
		}
		sprintf(chDeptValue, " and SPBM = '%s' ", dept.m_spbm.c_str());//部类号
	
		DBG_PRINT(("chDeptValue %s", chDeptValue));
		sprintf(chValue, " and KPLX = %u ", NORMAL_INV);//发票类型
		invDet.m_filter = *strFilter;
		invDet.m_filter += chDeptValue;
		invDet.m_filter += chValue;
		norMoney = invDet.PartMoneySum();//某部类的正票销售金额
		DBG_PRINT(("整个时段某部类的销售金额 %d", norMoney));

		sprintf(chValue, " and (KPLX = %u or KPLX = %u or KPLX = %u)", 
			RETURN_INV, RET_MANUAL_INV, RET_SPECIAL_INV);//发票类型
		invDet.m_filter = *strFilter;
		invDet.m_filter += chDeptValue;
		invDet.m_filter += chValue;
		retMoney = invDet.PartMoneySum();//某部类的红票销售金额
		DBG_PRINT(("整个时段某部类的销售红金额 %d", retMoney));

		//正-红
		norMoney = norMoney - retMoney;//某部类的销售金额
		sumMoney += norMoney;
		if (0 != norMoney)
		{
			DBG_PRINT(("整个时段某部类的销售金额 %d", norMoney));
			PrnDayDeptSum(&(dept.m_spmc), norMoney);//打印整个时段部类销售金额
		}
					
		if(retCode == SQLITE_DONE)
		{
			break;
		}
		retCode = dept.MoveNext();		
	}
		if (retCode != SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("查询部类信息失败");
		DBG_PRINT(("查询部类信息失败"));
		dept.MoveEnd();	
		return FAILURE;
	}
	dept.MoveEnd(); 
	return SUCCESS;
}

UINT8 PrnPeriodDeptMoney(INT64 &sumMoney, UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime)
{
	INT32 retCode;
	INT64 norMoney = 0;//某部类正票金额
	INT64 retMoney = 0;//某部类红票金额
	
	INT8 chDeptValue[128];
	memset((void*)chDeptValue, 0, sizeof(chDeptValue));
	INT8 chValue[512];
	memset((void*)chValue, 0, sizeof(chValue));

	CInvDet invDet;
	CDept dept;

	UINT8 nCount = 0;
	UINT8 i = 0;
	string filterArr[3];

	if (nStartDate == nEndDate) 
	{
		DBG_PRINT(("是同一天"));
		nCount = 1;
		sprintf(chValue, "where KPRQ = %u and KPSJ >= %u and KPSJ <= %u ", nStartDate, nStartTime, nEndTime);		
		filterArr[0] = chValue;
		DBG_PRINT(("filterArr[0] = %s", filterArr[0].c_str()));
	}
	else
	{
		DBG_PRINT(("不是同一天"));
		nCount = 3;
		//起始日
		sprintf(chValue, "where KPRQ = %u and KPSJ >= %u ", nStartDate, nStartTime);
		filterArr[0] = chValue;	
		DBG_PRINT(("filterArr[0] = %s", filterArr[0].c_str()));
		//截止日
		sprintf(chValue, "where KPRQ = %u and KPSJ <= %u ", nEndDate, nEndTime);
		filterArr[1] = chValue;	
		DBG_PRINT(("filterArr[1] = %s", filterArr[0].c_str()));
		//中间时段
		sprintf(chValue, " where KPRQ > %u and KPRQ < %u",nStartDate,nEndDate);
		filterArr[2] = chValue;
		DBG_PRINT(("filterArr[2] = %s", filterArr[0].c_str()));
	}

	//按部类进行统计,打印部类金额
	dept.Requery();
	retCode = dept.MoveFirst();
	while((retCode == SQLITE_ROW)||(retCode == SQLITE_DONE))
	{
		if(retCode == SQLITE_DONE)
		{
			dept.m_spbm = "000";
			dept.m_blh = 0;
			dept.m_spmc = "其它";
		}
		sprintf(chDeptValue, " and SPBM = '%s' ", dept.m_spbm.c_str());//商品编码
	
		norMoney = 0;
		retMoney = 0;
		for(i = 0; i<nCount; i++)
		{
			sprintf(chValue, " and KPLX = %u ", NORMAL_INV);//发票类型
			invDet.m_filter = filterArr[i];
			invDet.m_filter += chDeptValue;
			invDet.m_filter += chValue;
			norMoney += invDet.PartMoneySum();//某部类的正票销售金额

			sprintf(chValue, " and (KPLX = %u or KPLX = %u or KPLX = %u )",
				RETURN_INV, RET_MANUAL_INV, RET_SPECIAL_INV);//发票类型
			invDet.m_filter = filterArr[i];
			invDet.m_filter += chDeptValue;
			invDet.m_filter += chValue;
			retMoney += invDet.PartMoneySum();//某部类的红票销售金额
		}
		norMoney -= retMoney;//某部类的销售金额
		sumMoney += norMoney;
		if (0 != norMoney)
		{
			DBG_PRINT(("整个时段某部类的销售金额 %d", norMoney));
			PrnDayDeptSum(&(dept.m_spmc), norMoney);//打印整个时段部类销售金额
		}

		if(retCode == SQLITE_DONE)
		{
			break;
		}
		retCode = dept.MoveNext();		
	}
	if (retCode != SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("查询部类信息失败");
		DBG_PRINT(("查询部类信息失败"));
		dept.MoveEnd();
		return FAILURE;
	}
		dept.MoveEnd();
	return SUCCESS;
}

//-----月销售统计
UINT8 PrnMonth(UINT32 sumDate)
{
	DBG_PRINT((" PrnSaler()"));
	sprintf(pGeneralPrnBuff, "月份：%04lu-%02lu", sumDate / 10000, (sumDate % 10000) / 100);
// 	sprintf(pGeneralPrnBuff, "开票日期：%s", strIssueDate->c_str());
	DBG_PRINT((" 统计月份：%u", sumDate));
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("商 品                    销售金额", sizeof("商 品                    销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	return SUCCESS;
}

UINT8 PrnMonthSaleSum(UINT32 nInvCount, INT64 nSum)
{
	DBG_PRINT((" PrnMonthSaleSum()"));
	DBG_PRINT((" nInvCount = %u, nSum = %d", nInvCount, nSum));

	sprintf(pGeneralPrnBuff, "开票数量：%u", nInvCount);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "月销售总额：%.2f", (double)nSum/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));		
	PrintReportLine("========================================", sizeof("========================================"));

	return SUCCESS;
}

//----开票统计
UINT8 PrnIssueSumHead(UINT32 nStartDate, UINT32 nStartTime, UINT32 nEndDate, UINT32 nEndTime)
{
	DBG_PRINT((" PrnSalerSumHead()"));

	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("            << 开 票 报 表 >>", sizeof("            << 开 票 报 表 >>"));
	UINT32 nCurDate, nCurTime;

	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	

	sprintf(pGeneralPrnBuff, "统计区间：   %04lu-%02lu-%02lu   %02lu:%02lu:%02lu",
									nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100, 
									nStartTime / 10000, (nStartTime % 10000) / 100,(nStartTime % 10000) % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "          至 %04lu-%02lu-%02lu   %02lu:%02lu:%02lu", 
									nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100, 
									nEndTime / 10000, (nEndTime % 10000) / 100, (nEndTime % 10000) % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	PrintReportLine("========================================", sizeof("========================================"));

// 	PrintReportLine("发票号         发票类型         销售金额", sizeof("发票号         发票类型         销售金额"));
// 	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	
	return SUCCESS;
}

UINT8 PrnIssueSumCode(string fpdm)
{
	ForwardNLine(1);

	sprintf(pGeneralPrnBuff, "发票代码: %s", fpdm.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	
	PrintReportLine("发票号         发票类型         销售金额", sizeof("发票号         发票类型         销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));

	return SUCCESS;
}

UINT8 PrnIssueSumBody(UINT32 nInvNo, UINT8 nInvType, INT64 nInvMoney)
{
	string strType = "";

	switch(nInvType) 
	{
		case NORMAL_INV:
			strType = "正票";
			break;
		case RETURN_INV:
			strType = "红票";
			break;
		case WASTE_INV:
			strType = "废票";
			break;
		case WASTE_NOR:
			strType = "正废";
			break;
		case WASTE_RET:
			strType = "红废";
			break;
		case RET_MANUAL_INV:
			strType = "手工红票";
			break;
 		case RET_SPECIAL_INV:
 			strType = "特殊红票"; 
			break;
		default:;
	}
	sprintf(pGeneralPrnBuff, "%08lu %12s %17.2f", nInvNo, strType.c_str(), 
															(double)nInvMoney / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	return SUCCESS;
}

UINT8 PrnIssueSumEnd(UINT32 nNormalNum, UINT32 nReturnNum, UINT32 nWasteNum, INT64 norMoney, INT64 retMoney)
{
	INT64 leftMoney = norMoney - retMoney;//净金额

	PrintReportLine("========================================", 
					sizeof("========================================")); 
	
	SetLineSpace(REPORT_LINE_SPACE2);	
//	PrintSeparateLine();	
	sprintf(pGeneralPrnBuff, "正票份数 : %11lu", nNormalNum);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "红票份数 : %11lu", nReturnNum);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "废票份数 : %11lu", nWasteNum);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));


	sprintf(pGeneralPrnBuff, "正票金额 : %11.2f", ((double)norMoney) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "红票金额 : %11.2f", ((double)retMoney) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "净  金额 : %11.2f", ((double)leftMoney) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	ForwardNLine(FORWARD_LINES);

	return SUCCESS;

}

UINT8 PrnInvCheckOldHead(string nsrmc, string swdjzh,TCheckOld* checkoldHead)
{
	int i = 0;
	SetLineSpace(REPORT_LINE_SPACE);
	PrintReportLine("      << 发 票 检 查 情 况 报 告 表 >>", sizeof("      <<  发 票 检 查 情 况 报 告 表 >>"));

	sprintf(pGeneralPrnBuff, "纳税人名称：%s ", nsrmc.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "纳税人识别号：%s", swdjzh.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

// 	sprintf(pGeneralPrnBuff, "开票日期：%04lu-%02lu-%02lu 至 %04lu-%02lu-%02lu",
// 		nStartDate / 10000, (nStartDate % 10000) / 100, nStartDate % 100,
// 		nEndDate / 10000, (nEndDate % 10000) / 100, nEndDate % 100);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	
	sprintf(pGeneralPrnBuff, "流水号：%s", checkoldHead[i].searchNo.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	UINT32 nCurDate, nCurTime;
	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	//nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "打印时间：%04lu-%02lu-%02lu", nCurDate / 10000,
		(nCurDate % 10000) / 100, nCurDate % 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	//PrintReportLine("########################################", sizeof("########################################"));
	//PrintReportLine("****************************************", sizeof("****************************************"));
	
	return SUCCESS;

}
UINT8 PrnTimeDeptMoney(INT64 &sumMoney,INT64 &norMoney,INT64 &retMoney, const string *strFilter)
{
	DBG_ASSERT_EXIT((strFilter != NULL), (" strFilter == NULL!"));
	
	INT32 retCode;
	// 	INT64 norMoney = 0;//某部类正票金额
	// 	INT64 retMoney = 0;//某部类退票金额
	
	INT64 NorMoney=0,RetMoney=0;
	
	INT8 chDeptValue[128];
	memset((void*)chDeptValue, 0, sizeof(chDeptValue));
	INT8 chValue[512];
	memset((void*)chValue, 0, sizeof(chValue));
	
	CInvDet invDet;
	CDept dept;
	
	//按部类进行统计,打印部类金额
	dept.Requery();
	retCode = dept.MoveFirst();
	
	while((retCode == SQLITE_ROW)||(retCode == SQLITE_DONE))
	{
		if(retCode == SQLITE_DONE)
		{
			dept.m_spbm = "0000000000000";
			//dept.m_taxCode = "00000000";
			dept.m_blh = 0;
			dept.m_spmc = "其它";
		}
		DBG_PRINT(("dept.m_spbm = %s",dept.m_spbm.c_str()));
		sprintf(chDeptValue, " and SPBM = '%s' ", dept.m_spbm.c_str());//商品编码
		
		sprintf(chValue, " and KPLX = %u ", NORMAL_INV);//发票类型
		invDet.m_filter = *strFilter;
		invDet.m_filter += chDeptValue;
		invDet.m_filter += chValue;
		DBG_PRINT(("invDet.m_filter= %s",invDet.m_filter.c_str()));
		NorMoney = invDet.PartMoneySum();//某部类的正票销售金额
		DBG_PRINT(("norMoney = %ld",norMoney));
		
		sprintf(chValue, " and (KPLX = %u or KPLX = %u or KPLX = %u)", 
			RETURN_INV, RET_MANUAL_INV, RET_SPECIAL_INV);//发票类型
		invDet.m_filter = *strFilter;
		invDet.m_filter += chDeptValue;
		invDet.m_filter += chValue;
		RetMoney = invDet.PartMoneySum();//某部类的退票销售金额
		DBG_PRINT(("retMoney = %ld",retMoney));
		
		norMoney +=NorMoney;
		DBG_PRINT(("norMoney = %ld",norMoney));
		
		retMoney+=RetMoney;
		DBG_PRINT(("retMoney = %ld",retMoney));
		
		//正-负
		NorMoney = NorMoney - RetMoney;//某部类的销售金额
		sumMoney += NorMoney;
		DBG_PRINT(("sumMoney = %ld",sumMoney));
		if (0 != norMoney)
		{
			DBG_PRINT(("整个时段某部类的销售金额 %d", norMoney));
			//PrnDayDeptSum(&(dept.m_spmc), norMoney);//打印整个时段部类销售金额
		}
		
		if(retCode == SQLITE_DONE)
		{
			break;
		}
		retCode = dept.MoveNext();		
	}
	if (retCode != SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("查询部类信息失败");
		DBG_PRINT(("查询部类信息失败"));
		dept.MoveEnd();	
		return FAILURE;
	}
	dept.MoveEnd();
	return SUCCESS;
}



UINT8 PrnInvCheckOldBody(TCheckOld* checkoldHead, int &nHeadNum, TCheckOldInfo* checkoldInfo, int &nInfoNum)
{
   // int nTemp = 0;

	for(int i=0; i<nHeadNum; i++)
	{
		PrintReportLine("========================================", sizeof("========================================"));
		sprintf(pGeneralPrnBuff, "发票名称：%s ", checkoldHead[i].invName.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		sprintf(pGeneralPrnBuff, "发票代码：%s ", checkoldHead[i].invCode.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		
		sprintf(pGeneralPrnBuff, "发票起号：%s       发票止号：%s", checkoldHead[i].invSNo.c_str(), checkoldHead[i].invENo.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		sprintf(pGeneralPrnBuff, "购买时间：%s       份数：%u", checkoldHead[i].checkDate.c_str(), checkoldHead[i].num);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
		PrintReportLine("========================================", sizeof("========================================"));
	
		UINT32 nSNo, nENo;
        nSNo =  atoi(checkoldHead[i].invSNo.c_str());
		DBG_PRINT(("nSNo = %u", nSNo));
        nENo =  atoi(checkoldHead[i].invENo.c_str());
		DBG_PRINT(("nENo = %u", nENo));
		
	    int nTemp = 0;
     
		DBG_PRINT(("nInfoNum = %u", nInfoNum));
		for(int j=nTemp; j<nInfoNum; j++)
		{
			UINT32 nStartNo, nEndNo;
			nStartNo = atoi(checkoldInfo[j].invSNo.c_str());
			DBG_PRINT(("nStartNo = %u", nStartNo));
			nEndNo  =  atoi(checkoldInfo[j].invENo.c_str());
		    DBG_PRINT(("nEndNo = %u", nEndNo));

			DBG_PRINT(("$$$$$$$$$$$$$j == %d", j));

		   if((nSNo <= nStartNo ) && (nEndNo <= nENo))
		   {
			  sprintf(pGeneralPrnBuff, "开具时间：%s        份数：%u", checkoldInfo[j].checkDate.c_str(), checkoldInfo[j].num);
			  PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
			  
			  sprintf(pGeneralPrnBuff, "发票起号：%s       发票止号：%s", checkoldInfo[j].invSNo.c_str(), checkoldInfo[j].invENo.c_str());
			  PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
			  
			  DBG_PRINT(("SE == %s", checkoldInfo[j].taxMoney.c_str()));
			  sprintf(pGeneralPrnBuff, "填开金额：%s          填开税额：%s", checkoldInfo[j].money.c_str(), checkoldInfo[j].taxMoney.c_str());
			  PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
			  
			  sprintf(pGeneralPrnBuff, "使用情况：%s ", checkoldInfo[j].resContent.c_str());
			  PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

			  PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));

	       	  //nTemp++;
		   }
// 		   else
// 		   {
// 			    
// 			   break;
// 		   }
	       nTemp++;
	  }
      
	  //PrintReportLine("****************************************", sizeof("****************************************"));

	}

	ForwardNLine(FORWARD_LINES);
	return SUCCESS;
	
}

/**************************************************************************
功能     ：打印发票票种基本信息
格式     ：int print_goodsfo(UINT8 bFindBlackMark )
输入参数 ：UINT8 bFindBlackMark 是否寻找黑标，为1时,函数会在找到下一个黑标
                               后开始打印
输出参数 ：
返回值   ：-1——失败，SUCCESS——成功
作者     ：
日期     ：
摘要     ：
**************************************************************************/
UINT8 print_invkindifo(/*struct TInvKindInfoPrnData *invkindData, */ UINT8 bFindBlackMark)
{
  
	SetLineSpace(REPORT_LINE_SPACE);
	PrintRptTitle("发 票 票 种 信 息");
    PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	
	CInvKind *pInvKind ;
	CInvKind InvKind;
	pInvKind=&InvKind;
	INT8 value[256];
	sprintf(value, "order by NO ");	
	pInvKind->m_filter=value;
	pInvKind->Requery();
    int i=0;
	INT32 errorcode = pInvKind->MoveFirst();
	DBG_PRINT(("errorcode = %u", errorcode));
	while (errorcode == SQLITE_ROW)
	{	
		DBG_PRINT(("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&i == %d", i));	
		DBG_PRINT(("发票类型代码 = %s", InvKind.m_fplxdm.c_str()));
		sprintf(pGeneralPrnBuff, "发票类型代码 : %s", InvKind.m_fplxdm.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		
		DBG_PRINT(("单张限额 = %f",((double)InvKind.m_maxSign)/SUM_EXTENSION));
		sprintf(pGeneralPrnBuff, "单张限额 : %.2f", ((double)InvKind.m_maxSign)/SUM_EXTENSION);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		DBG_PRINT(("离线锁死日 = %u", InvKind.m_Lxssr));
		sprintf(pGeneralPrnBuff, "离线锁死日 : %u", InvKind.m_Lxssr);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		DBG_PRINT(("离线开具时间(小时) = %u", InvKind.m_Lxkjsj));
		sprintf(pGeneralPrnBuff, "离线开具时间(小时) : %u", InvKind.m_Lxkjsj);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		
		DBG_PRINT(("离线正票累计限额 = %f",((double)InvKind.m_maxSum)/SUM_EXTENSION));
		sprintf(pGeneralPrnBuff, "离线正票累计限额 : %.2f", ((double)InvKind.m_maxSum)/SUM_EXTENSION);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
			
		PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
		errorcode = pInvKind->MoveNext();
		i++; 
	}
	     pInvKind->MoveEnd();
	    ForwardNLine(FORWARD_LINES);
	    ForwardNLine(FORWARD_LINES);
	
	    return 0;
}

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



INT8 print_taxiteminfo(UINT8 bFindBlackMark )

{

	UINT8 i;
	INT8 tmpCode[2 * TAX_ITME_CODE_LEN + 1];

	PrinterIni(bidirection);
	SetLineSpace(REPORT_LINE_SPACE);

	PrintRptTitle("税 种 税 目 信 息");
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	sprintf(pGeneralPrnBuff, "税档            税率");
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	CTax *pTax;
	CTax Tax;
	pTax = &Tax;
	INT8 value[256];
	sprintf(value, "order by NO ");	
	pTax->m_filter=value;
	pTax->Requery();
	INT32 errorcode = pTax->MoveFirst();
	DBG_PRINT(("errorcode = %u", errorcode));
	while (errorcode == SQLITE_ROW)
	{	
		DBG_PRINT(("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&i == %d", i));	

		sprintf(pGeneralPrnBuff, "%-2u            %.2f%%", pTax->m_no, (double)(pTax->m_sl)/100);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));	
	
		errorcode = pTax->MoveNext();
		i++; 
	}
	pTax->MoveEnd();
	ForwardNLine(FORWARD_LINES);
	ForwardNLine(FORWARD_LINES);

	return 0;

}


void DivideCNStr(string &str1, INT32 len, string &str2)
{
	if ( str1.size()<=len || len<=0 )
		return;
	
	string strTmp(str1);
	
	if( GetHalfHZCount((unsigned char *)strTmp.c_str(), len)%2 == 0 )
	{
		len=strTmp.size() > len ? len: strTmp.size();
	}
	else
	{
		len=strTmp.size() > len-1 ? len-1: strTmp.size();
	}
	
	str1.assign( strTmp, 0, len );
	str2.assign( strTmp, len, strTmp.size()-len );
}


int GetHalfHZCount(unsigned char *str,int len)
{
	int c=0;
	int p=0;
	while (p < len)
	{
		if ((str[p] >= 0x81) && (str[p] <= 0xfe))
		{
			c++;
			p++;
			if (p < len)
			{
				if ((str[p] >= 0x40) && (str[p] <= 0xfe))
				{
					c++;
					p++;
				}
			}
		}
		else
		{
			p++;
		}
	}
	
	return c;
}

INT8 PrintTitle()
{
	//	PrintReportLine("          << 通 知 信 息 表 >>", sizeof("           <<  通 知 信 息 表 >>"));
	PrintRptTitle("<< 通 知 信 息 表 >>");
}

INT8 PrintSeparator()
{
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
}

INT8 PrintDoubleSeparator()
{
	PrintReportLine("========================================", sizeof("========================================"));
}

/*
UINT8 PrnMessageDetail(const CMessage *message)
{
	DBG_PRINT((" PrnMessageDetail"));
	DBG_ASSERT_EXIT((message != NULL), (" message == NULL "));
	
	SetLineSpace(REPORT_LINE_SPACE);
	
	string strMsg = "标题: " + message->m_title;
	string tmpStr("");
	do 
	{
		DivideCNStr(strMsg, 46, tmpStr);
		sprintf(pGeneralPrnBuff, "%s", strMsg.c_str());
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		strMsg = tmpStr;
	} while (tmpStr.size() > 46);
	
	sprintf(pGeneralPrnBuff, "%s", tmpStr.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	
	strMsg = "内容: " + message->m_content;
	tmpStr = "";
	do 
	{
		DivideCNStr(strMsg, MAX_LINE_PRINT_NUM, tmpStr);
		sprintf(pGeneralPrnBuff, "%s", strMsg.c_str());
#if 0
		int i; 
		for(i=0; i<1024; i++)
		{
			printf("%2x ",pGeneralPrnBuff[i]);
		}
#endif
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
		strMsg = tmpStr;
		DBG_PRINT(("tmpStr.size = %u",tmpStr.size()));
	} while (tmpStr.size() >MAX_LINE_PRINT_NUM);
	

	sprintf(pGeneralPrnBuff, "%s", tmpStr.c_str());
     
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));


   UINT32 tmpcjsj=atoi(message->m_cjsj.substr(0,8).c_str());

	sprintf(pGeneralPrnBuff, "创建时间: %04lu-%02lu-%02lu", 
		tmpcjsj/10000, (tmpcjsj%10000)/100, tmpcjsj%100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	 UINT32 tmpxxsj=atoi(message->m_xxsj.substr(0,8).c_str());

	sprintf(pGeneralPrnBuff, "消息时间: %04lu-%02lu-%02lu", 
		tmpxxsj/10000, (tmpxxsj%10000)/100, tmpxxsj%100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	 UINT32 tmpxgsj=atoi(message->m_xgsj.substr(0,8).c_str());

	sprintf(pGeneralPrnBuff, "修改时间: %04lu-%02lu-%02lu", 
		tmpxgsj/10000, (tmpxgsj%10000)/100, tmpxgsj%100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "税务机关代码: %s",message->m_swjgdm.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "操作人员代码: %s",message->m_czrydm.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "备注: %s",message->m_remark.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	
	PrintSeparator();
	
	return SUCCESS;
}
*/
/*
//------累计金额
UINT8 PrnMoneyInfo(INT64  normalSum, INT64  returnSum, INT64  normalVolSum, INT64  returnVolSum)
{
	DBG_PRINT((" PrnMoneyInfo()"));

	SetLineSpace(REPORT_LINE_SPACE);

	PrintReportLine("            《累计金额报表》", sizeof("            《累计金额报表》"));

	UINT32 nCurDate, nCurTime;

	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	PrintReportLine("========================================", sizeof("========================================"));

	PrintReportLine("名称                     金额", sizeof("名称                     金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	sprintf(pGeneralPrnBuff, "申报期正票累计金额 : %11.2f", ((double)normalSum) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "申报期红票累计金额 : %11.2f", ((double)returnSum) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "发票卷正票累计金额 : %11.2f", ((double)normalVolSum) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "发票卷红票累计金额 : %11.2f", ((double)returnVolSum) / SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	ForwardNLine(FORWARD_LINES);
	return SUCCESS;
}
	
//----发票段销售统计

UINT8 PrnInvSaleSumHead(UINT32 nStartNo, UINT32 nEndNo, const string *strpCode)
{
	DBG_PRINT((" PrnDaySaleSumHead()"));
	SetLineSpace(REPORT_LINE_SPACE);
	
	PrintReportLine("       << 发 票 段 统 计 报 表 >>", sizeof("        << 发 票 段 统 计 报 表 >>"));
	
	UINT32 nCurDate, nCurTime;
	nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);

	sprintf(pGeneralPrnBuff, "制表日期：%04lu-%02lu-%02lu   时间：%02lu:%02lu", nCurDate / 10000,
			(nCurDate % 10000) / 100, nCurDate % 100, nCurTime / 10000, (nCurTime % 10000) / 100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	sprintf(pGeneralPrnBuff, "制 表 人：%s", g_globalArg->m_operator->m_name.c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "发票代码：   %s ", strpCode->c_str());
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "统计区间：   %08lu  至 %08lu ", nStartNo, nEndNo);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	PrintReportLine("========================================", sizeof("========================================"));

	PrintReportLine("商 品                    销售金额", sizeof("商 品                    销售金额"));
	PrintReportLine("----------------------------------------", sizeof("----------------------------------------"));
	
	return SUCCESS;
}

UINT8 PrnInvSaleSumEnd(UINT32 nInvCount, INT64 nSum)
{
	DBG_PRINT((" PrnAllDaySaleSum()"));
	DBG_PRINT((" nInvCount = %u, nSum = %d", nInvCount, nSum));

	PrintReportLine("========================================", sizeof("========================================"));

	PrintReportLine("           总 计", sizeof("           总 计"));
	sprintf(pGeneralPrnBuff, "总开票 数量： %u", nInvCount);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "累 计 金 额： %.2f", (double)nSum/100);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	return SUCCESS;
}
*/