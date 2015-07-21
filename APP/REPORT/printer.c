/****************************************************************************

文件名           ：printer.c

功能             ：APL-1000打印程序。

起始日期         ：2004-09-16

作者             ：赵健青

****************************************************************************/

//#define POS_DEBUG_TO_TTY

#include <stdio.h>

#include <string.h>

#include "printer.h"

#include "PrintDriver.h"

#include "arithmetic.h"  

#include "TemplateIndep.h"

//#include "pwoffdatadesign.h"

//#include "systemdef.h"

#include "pos_log.h"



// 临时添加 BY ZL///////////

#define	BIDIRECTIONAL	3

////////////////////////////



UINT8 g_AlreadyFindBlack = 0;

extern UINT8 bidirection ;


INT8 print_buffer[1024];



INT8 *pGeneralPrnBuff = print_buffer;

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
INT8 print_volumeinfo(struct TVolSumPrnData *volsumdata,UINT8 bFindBlackMark)
{

	UINT8 invCode[INV_TYPE_CODE_LEN*2+1];

//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);


	SetLineSpace(REPORT_LINE_SPACE);

	

	PrintRptTitle("单卷发票使用汇总数据报表");

	

	sprintf(pGeneralPrnBuff, "单 位  名 称 : %s", volsumdata->CorpName);

	PrintReportLine(pGeneralPrnBuff, (UINT8)strlen(pGeneralPrnBuff));

	

// 	sprintf(pGeneralPrnBuff, "税        号 : %s", volsumdata->TaxCode);
// 	PrintReportLine(pGeneralPrnBuff, (UINT8)strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "机   器   号 : %s", volsumdata->MachineCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "开 票  时 间 : %04lu-%02lu-%02lu 至 %04lu-%02lu-%02lu", 

		volsumdata->StartDate / 10000,  (volsumdata->StartDate % 10000) / 100, volsumdata->StartDate % 100, 

		volsumdata->EndDate / 10000,  (volsumdata->EndDate % 10000) / 100, volsumdata->EndDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

//	sprintf(pGeneralPrnBuff, "发 票  代 码 :                      ");

// 	bin2char(volsumdata->TypeCode, (INT8 *)invCode, INV_TYPE_CODE_LEN);

// 	memcpy((void *)&pGeneralPrnBuff[15], (void *)(invCode), INV_TYPE_CODE_LEN*2);	
	sprintf(pGeneralPrnBuff, "发 票  代 码 : %s", volsumdata->TypeCode);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "发票起始号码 : %08lu", volsumdata->StartInvNo);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "发票终止号码 : %08lu", volsumdata->EndInvNo);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正常发票份数 : %lu", volsumdata->PosInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正常发票开具金额 : %.2f", ((double)(volsumdata->PosInvSum))/SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "废 票  份 数 : %lu", volsumdata->WasteInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负 票  份 数 : %lu", volsumdata->ReInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负 票  金 额 : %.2f", ((double)(volsumdata->ReInvSum))/SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	ForwardNLine(FORWARD_LINES);
	

/*
#if PETROL_STATION_VER == 0

//	ForwardNLine(FORWARD_LINES);

#endif*/


	

	return 0;

}



/**************************************************************************

功能     ：打印时间段发票使用汇总数据报表

格式     ：int print_dateseginvsuminfo( struct TDateSegSumPrnData * daySumData

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

                             UINT8 bFindBlackMark,UINT32 startTime, UINT32 endTime)

{

	UINT8 i;

	

//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);


	SetLineSpace(REPORT_LINE_SPACE);

	PrintRptTitle("发票使用汇总数据报表");

	

	sprintf(pGeneralPrnBuff, "单 位  名 称 : %s", dateSegSumData->CorpName);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税        号 : %s", dateSegSumData->TaxCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "机   器   号 : %s", dateSegSumData->MachineCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "开 票  时 间 : %04lu-%02lu-%02lu   %02lu:%02lu:%02lu", 

		dateSegSumData->StartDate / 10000,  (dateSegSumData->StartDate % 10000) / 100, dateSegSumData->StartDate % 100, 

		startTime / 10000,  (startTime % 10000) / 100, startTime % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "          至 : %04lu-%02lu-%02lu   %02lu:%02lu:%02lu", 

		dateSegSumData->EndDate / 10000,  (dateSegSumData->EndDate % 10000) / 100, dateSegSumData->EndDate % 100, 

		endTime / 10000,  (endTime % 10000) / 100, endTime % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正常发票份数 : %lu", dateSegSumData->PosInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正常发票开具金额 : %.2f", ((double)(dateSegSumData->PosInvSum))/SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "废 票  份 数 : %lu", dateSegSumData->WasteInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负 票 份 数 : %lu", dateSegSumData->ReInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负 票  金 额 : %.2f", ((double)(dateSegSumData->ReInvSum))/SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	
/*
	PrintSeparateLine();

	
	sprintf(pGeneralPrnBuff, "税档        正票金额       退票金额");

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));	

	for( i=0 ; i<TAX_ITEM_MAX_COUNT ; i++ )

	{

		if (dateSegSumData->TaxItem[i] == 0)

		{

			continue;

		}

		sprintf(pGeneralPrnBuff, "%3u      %11.2f    %11.2f", dateSegSumData->TaxItem[i], 

															((double)(dateSegSumData->PosInvSumByTax[i]))/SUM_EXTENSION, 

															((double)(dateSegSumData->ReInvSumByTax[i]))/SUM_EXTENSION);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}
*/


	ForwardNLine(FORWARD_LINES);

	

    return 0;

}



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

int print_daystatinfo( struct TDaySumInfoPrnData *daySumData, UINT8 bFindBlackMark )

{

	UINT8 i;



//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);


	SetLineSpace(REPORT_LINE_SPACE);

	PrintRptTitle("日 交 易 汇总数据报表");

	

	sprintf(pGeneralPrnBuff, "单 位  名 称 : %s", daySumData->CorpName);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税        号 : %s", daySumData->TaxCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "机   器   号 : %s", daySumData->MachineCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "开 票  日 期 : %04lu-%02lu-%02lu", 

		daySumData->Date / 10000,  (daySumData->Date % 10000) / 100, daySumData->Date % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "        正常票   负票      废票");

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "份数    %-6lu   %-4lu        %-4lu", daySumData->PosInvCount, 

		daySumData->ReInvCount, daySumData->WasteInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	PrintSeparateLine();

	

//	sprintf(pGeneralPrnBuff, "税档        正票金额       退票金额");
	sprintf(pGeneralPrnBuff, "正票金额       负票金额");
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "%-11.2f  %-11.2f",
		((double)(daySumData->PosInvSum))/SUM_EXTENSION, ((double)(daySumData->ReInvSum))/SUM_EXTENSION);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	
/*
	for( i=0 ; i<TAX_ITEM_MAX_COUNT ; i++ )

	{

		if (daySumData->TaxItem[i] == 0)

		{

			continue;

		}

		sprintf(pGeneralPrnBuff, "%3u      %11.2f    %11.2f", daySumData->TaxItem[i], 

			((double)(daySumData->PosInvSum[i]))/SUM_EXTENSION, ((double)(daySumData->ReInvSum[i]))/SUM_EXTENSION);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}
*/
	

	ForwardNLine(FORWARD_LINES);

	

    return 0;

}



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

int print_sbstatinfo(  struct TDeclareSumPrnData *DeclareSumPrnData )

{

	UINT8 i;

	

//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);


	SetLineSpace(REPORT_LINE_SPACE);

	PrintRptTitle("申 报 信 息 数据报表");

	

	sprintf(pGeneralPrnBuff, "单位名称 : %s", DeclareSumPrnData->CorpName);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税    号 : %s", DeclareSumPrnData->TaxCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "机 器 号 : %s", DeclareSumPrnData->MachineCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "申报日期 :  %04lu-%02lu-%02lu 至 %04lu-%02lu-%02lu", 

		DeclareSumPrnData->StartDate / 10000,  (DeclareSumPrnData->StartDate % 10000) / 100, DeclareSumPrnData->StartDate % 100, 

		DeclareSumPrnData->EndDate / 10000,  (DeclareSumPrnData->EndDate % 10000) / 100, DeclareSumPrnData->EndDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "        正常票     负票      废票");

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "份数    %-6lu     %-4lu        %-4lu", DeclareSumPrnData->PosInvCount, 

		DeclareSumPrnData->ReInvCount, DeclareSumPrnData->WasteInvCount);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	PrintSeparateLine();

	

	sprintf(pGeneralPrnBuff, "税档        正票金额       负票金额");

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	for( i=0 ; i<TAX_ITEM_MAX_COUNT ; i++ )

	{

		if (DeclareSumPrnData->TaxItem[i] == 0)

		{

			continue;

		}

		sprintf(pGeneralPrnBuff, "%-3u         %-11.2f   %-11.2f", DeclareSumPrnData->TaxItem[i], 

			((double)(DeclareSumPrnData->PosInvSum[i]))/SUM_EXTENSION, ((double)(DeclareSumPrnData->ReInvSum[i]))/SUM_EXTENSION);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}

	

	ForwardNLine(FORWARD_LINES);

	

    return 0;

}



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



int print_macinfo(struct TMachinePrnData *machineData,  UINT8 bFindBlackMark )

{

//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);
	

	SetLineSpace(REPORT_LINE_SPACE);

	PrintRptTitle("机 器 信 息");

	sprintf(pGeneralPrnBuff, "机 器 编 号 : %s", machineData->MachineNo);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "硬 件 版 本 :  %s", machineData->HardVer);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "软 件 版 本 :  %s", machineData->SoftVer);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "生 产 日 期 :  %04lu-%02lu-%02lu", machineData->MakeDate / 10000, 

		(machineData->MakeDate % 10000) / 100, machineData->MakeDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	if( machineData->IMEIFlag == 1 )
	{
		sprintf(pGeneralPrnBuff, "无线IMEI :  %s", machineData->IMEI);
		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	}

	ForwardNLine(FORWARD_LINES);

	return 0;

}



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

int print_corpinfo(struct TCorpInfoPrnData *corpData,  UINT8 bFindBlackMark )
{
//	PrinterIni(BIDIRECTIONAL);
	PrinterIni(bidirection);
	SetLineSpace(REPORT_LINE_SPACE);

	UINT8 flagBuff[64];
	PrintRptTitle(" 企 业 信 息");

	sprintf(pGeneralPrnBuff, "纳税户名称 : %s", corpData->Nsrmc);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "纳税户识别号 : %s", corpData->Nsrsbh);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "金税盘编号 : %s", corpData->Jspsbh);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	sprintf(pGeneralPrnBuff, "行业代码 : %s", corpData->Nsrhydm);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	sprintf(pGeneralPrnBuff, "行业名称 : %s", corpData->Nsrhymc);
//	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

// 	sprintf(pGeneralPrnBuff, "经营项目名称 : %s", corpData->Nsrjyxmzwmc);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "主管税务机关代码 : %s", corpData->Nsrswjgdm);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	sprintf(pGeneralPrnBuff, "主管税务机关名称 : %s", corpData->Nsrswjgmc);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "启用时间 : %s", corpData->Qysj);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "主分机 : %s", corpData->Zfbz);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "企业类型 : %s", corpData->Qylx);
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	memset(flagBuff, 0, sizeof(flagBuff));
// 	if( corpData->Yxbz == 1 )
// 		sprintf(flagBuff, "有效");
// 	else
// 		sprintf(flagBuff, "注销");
// 	sprintf(pGeneralPrnBuff, "有效标志 : %s", flagBuff);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	memset(flagBuff, 0, sizeof(flagBuff));
// 	if( corpData->Nsrlx == 1 )
// 		sprintf(flagBuff, "一般纳税人");
// 	else if( corpData->Nsrlx == 2 )
// 		sprintf(flagBuff, "小规模纳税人");
// 	sprintf(pGeneralPrnBuff, "纳税人类型 : %s", flagBuff);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 
// 	sprintf(pGeneralPrnBuff, "征收方式代码 : %s", corpData->Zsfs_dm);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	
// 	sprintf(pGeneralPrnBuff, "自定义企业类型 : %u", corpData->Zdy_qylx);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

// 	sprintf(pGeneralPrnBuff, "开票管理员 : %s", corpData->Kpgly);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

// 	sprintf(pGeneralPrnBuff, "开票模式 : %u", corpData->Kpms);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	sprintf(pGeneralPrnBuff, "开票模式名称 : %s", corpData->Kpmsmc);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	

// 	sprintf(pGeneralPrnBuff, "开票机数量 : %u", corpData->Kpjsl);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	sprintf(pGeneralPrnBuff, "开票限额 : %.2f", ((double)(corpData->MaxSum))/SUM_EXTENSION);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
// 	sprintf(pGeneralPrnBuff, "最大离线开票天数 : %u", corpData->MaxDay);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 
// 	sprintf(pGeneralPrnBuff, "最大离线开票张数 : %u", corpData->MaxNum);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 
// 	sprintf(pGeneralPrnBuff, "最大离线开票金额 : %.2f", ((double)(corpData->MaxMoney))/SUM_EXTENSION);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	ForwardNLine(FORWARD_LINES);

	return 0;
}

int print_invKindinfo(struct TInvKindInfoPrnData *invkindData,  UINT8 bFindBlackMark )
{
	//	PrinterIni(BIDIRECTIONAL);
// 	PrinterIni(bidirection);
// 	SetLineSpace(REPORT_LINE_SPACE);
// 	
// 	UINT8 flagBuff[64];
// 	PrintRptTitle(" 票 种 信 息");
// 
// 
// 	sprintf(pGeneralPrnBuff, "票种名称 : %s", invkindData->InvKindName);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	
// 	sprintf(pGeneralPrnBuff, "票种代码 : %s", invkindData->InvKindCode);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	
// 	/*DBG_PRINT(("最大金额 = %f",((double)(invkindData->InvMaxMoney))/SUM_EXTENSION));*/
// 	sprintf(pGeneralPrnBuff, "开票最大金额 : %.2f", ((double)invkindData->InvMaxMoney)/SUM_EXTENSION);
// 	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 	
// 	
// 	sprintf(pGeneralPrnBuff, "开票最小金额 : %.2f", ((double)invkindData->InvMinMoney)/SUM_EXTENSION);
// 	 PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
// 
// 	
// 	ForwardNLine(FORWARD_LINES);
	
	return 0;
}
/**************************************************************************

功能     ：打初始化回单信息

格式     ：int print_initinfo()

输入参数 ：

         ：

输出参数 ：

返回值   ：

作者     ：

日期     ：

摘要     ：

**************************************************************************/
/*		临时注释	BY ZL
void  print_initinfo(UINT8 Init_Update)

{

	UINT32 curDate = GetCurDate();

	UINT8 i, j;

	struct TTaxItemPrnData taxItemPrnData;

	struct TTaxItem	taxItem;

	INT8 tmpCode[2 * TAX_ITME_CODE_LEN + 1];

	

	PrinterIni(BIDIRECTIONAL);



	SetLineSpace(REPORT_LINE_SPACE);

	if (Init_Update == 1)

		PrintRptTitle(" 初 始 化 回 单");

	else

		PrintRptTitle(" 信息更新 回 单");

	

	//sprintf(pGeneralPrnBuff, "初始化 日 期 : %04lu-%02lu-%02lu", G_SysArg->daySumStartDate / 10000,

			//(G_SysArg->daySumStartDate % 10000) / 100, G_SysArg->daySumStartDate % 100);

	if (Init_Update == 1)

	{

		sprintf(pGeneralPrnBuff, "初始化 日 期 : %04lu-%02lu-%02lu", curDate / 10000,

			(curDate % 10000) / 100, curDate % 100);

	}

	else

	{

		sprintf(pGeneralPrnBuff, "信息更新日期 : %04lu-%02lu-%02lu", curDate / 10000,

			(curDate % 10000) / 100, curDate % 100);

	}

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "单 位  名 称 : %s", G_SysArg->corpInfo.Name);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "单 位  编 码 :                       ");

	bin2char(G_SysArg->corpInfo.CorpNo, &pGeneralPrnBuff[15], CORP_NO_LEN);	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税        号 : %s", G_SysArg->corpInfo.TaxCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税控卡 编 号 :                       ");

	bin2char(G_SysArg->corpInfo.FCardNo, &pGeneralPrnBuff[15], FCARD_NO_LEN);	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税控卡原始PIN码 :                    ");

	bin2char(G_SysArg->corpInfo.FCardPwd, &pGeneralPrnBuff[17], FCARD_PWD_LEN);	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	sprintf(pGeneralPrnBuff, "启 用  日 期 : %04lu-%02lu-%02lu", G_SysArg->corpInfo.StartDate / 10000, 

		(G_SysArg->corpInfo.StartDate % 10000) / 100, G_SysArg->corpInfo.StartDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "有 效  日 期 : %04lu-%02lu-%02lu", G_SysArg->corpInfo.ValidDate / 10000, 

		(G_SysArg->corpInfo.ValidDate % 10000) / 100, G_SysArg->corpInfo.ValidDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "主管机关编号 :                       ");

//	bin2char(corpData->ZGFJNo, &pGeneralPrnBuff[15], ZGJG_CODE_LEN);	

	sprintf(&pGeneralPrnBuff[15], "%010lu", G_SysArg->corpInfo.ZGFJNo);	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	if (G_SysArg->corpInfo.DeclareType == 1)

	{

		sprintf(pGeneralPrnBuff, "申 报  方 式 : 用户卡申报");

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}

	else

	{

		sprintf(pGeneralPrnBuff, "申 报  方 式 : 其它方式申报");

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}

	

	sprintf(pGeneralPrnBuff, "开票截止日期 : %04lu-%02lu-%02lu", G_SysArg->corpInfo.InvEndDate / 10000, 

		(G_SysArg->corpInfo.InvEndDate % 10000) / 100, G_SysArg->corpInfo.InvEndDate % 100);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	sprintf(pGeneralPrnBuff, "单张发票限额 : %11.2f", ((double)(G_SysArg->corpInfo.SingleInvLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正票累计限额 : %11.2f", ((double)(G_SysArg->corpInfo.PosInvAccLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "退票累计限额 : %11.2f", ((double)(G_SysArg->corpInfo.ReInvAccLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税档         代码            税率");

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	memset((void *)&taxItemPrnData, 0, sizeof(struct TTaxItemPrnData));

	for (i=0; i<TAX_ITEM_MAX_COUNT; i++)

	{

		if (G_SysArg->taxItemID[i] == 0)

			continue;

		

		taxItemPrnData.TaxItems[i].ID = G_SysArg->taxItemID[i];

		for (j=0; j<TAX_ITEM_RECORD_NUM; j++)

		{

			memset((void *)&taxItem, 0, sizeof(struct TTaxItem));

			if (ReadTaxItem(j, &taxItem) == FAILURE)

			{

				MsgBox("读取数据失败", g_messageArray[CASH_KEY_RETURN], E_ERRO);

				return;

			}

			

			if (taxItem.ID == G_SysArg->taxItemID[i])

			{

				memcpy((void *)taxItemPrnData.TaxItems[i].Code, (void *)taxItem.Code, TAX_ITME_CODE_LEN);

				memcpy((void *)taxItemPrnData.TaxItems[i].ChName, (void *)taxItem.ChName, TAX_ITEM_NAME_LEN);

				memcpy((void *)taxItemPrnData.TaxItems[i].EnName, (void *)taxItem.EnName, TAX_ITEM_NAME_LEN);



				taxItemPrnData.TaxItems[i].TaxRate	= taxItem.TaxRate;

				

				break;

			}

		}

	}

	for (i=0; i < TAX_ITEM_MAX_COUNT ;i++)

	{

		if (taxItemPrnData.TaxItems[i].ID == 0)

		{

			continue;

		}

		

		memset((void *)tmpCode, 0, TAX_ITME_CODE_LEN * 2 + 1);

		bin2char(taxItemPrnData.TaxItems[i].Code , tmpCode , TAX_ITME_CODE_LEN ); 

		sprintf(pGeneralPrnBuff, "%2u         %s          %.2f%%", taxItemPrnData.TaxItems[i].ID, 

			tmpCode, ((double)(taxItemPrnData.TaxItems[i].TaxRate))/100);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		

		sprintf(pGeneralPrnBuff, " 中文名称 : %s", taxItemPrnData.TaxItems[i].ChName);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

		

		sprintf(pGeneralPrnBuff, " 英文名称 : %s", taxItemPrnData.TaxItems[i].EnName);

		PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	}

	

	ForwardNLine(FORWARD_LINES);

	return;

}
*/





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



INT8 print_invoiceLimits( struct TInvLimitPrnData *invLimits, UINT8 bFindBlackMark )

{

//	PrinterIni(BIDIRECTIONAL);
PrinterIni(bidirection);


	SetLineSpace(REPORT_LINE_SPACE);

	

	PrintRptTitle("发 票  限 额");



	sprintf(pGeneralPrnBuff, "单 位  名 称 : %s", invLimits->CorpName);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "税        号 : %s", invLimits->TaxCode);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "机   器   号 : %s", invLimits->MachineNo);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	sprintf(pGeneralPrnBuff, "单张发票限额 :  %.2f", ((double)(invLimits->SigleInvLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "正票累计限额 :  %.2f", ((double)(invLimits->PosInvTotalLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负票累计限额 :  %.2f", ((double)(invLimits->ReInvTotalLimit)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	ForwardNLine(FORWARD_LINES);

	return 0;

}



/**************************************************************************

		格式化日期

		

作者     ：赵健青

日期     ：2005-04-04

**************************************************************************/



/*char * format_date(char * date)

{

	char tmpdate[9];

	

	memcpy(tmpdate,date,8);

	

	date[4] = '-' ;

	date[7] = '-' ;

	

	memcpy(date+5,tmpdate+4,2);

	memcpy(date+8,tmpdate+6,2);

	

	return  date ;

}*/



/**************************************************************************

		格式化月份

		

作者     ：赵健青

日期     ：2005-04-06

**************************************************************************/



/*char * format_month(char * month)

{

	char tmp_month[7];

	

	memcpy(tmp_month,month,6);

	

	month[4] = '-' ;

	

	memcpy(month+5,tmp_month+4,2);

	

	return  month ;

}*/





/*-------------------------------------------------------------------------

		加油站要求 《统计 报表》

		

作者     ：赵健青                       

日期     ：2005-04-06

---------------------------------【头】----------------------------------*/



/*void print_Head( struct TBegin_End_Date *Begin_End_Date, \

						   int x0,int y0,char *Title, \

						   int x1,int y1,char *FieldName_1, \

						   int x2,int y2,char *FieldName_2, \

						   int x3,int y3,char *FieldName_3 )

{

	PrinterIni();



	SetLineSpace(REPORT_LINE_SPACE);

	

	PrintRptTitle(Title);

	

	memset((void *)pGeneralPrnBuff, ' ', MAX_CHAR_NUM);

	pGeneralPrnBuff[MAX_CHAR_NUM] = 0x0;



	memcpy((void *)&pGeneralPrnBuff[x1 - strlen(FieldName_1)], (void *)FieldName_1, strlen(FieldName_1));

	memcpy((void *)&pGeneralPrnBuff[x2 - strlen(FieldName_2)], (void *)FieldName_2, strlen(FieldName_2));

	memcpy((void *)&pGeneralPrnBuff[x3 - strlen(FieldName_3)], (void *)FieldName_3, strlen(FieldName_3));

	

	SetLineSpace(REPORT_LINE_SPACE2);

	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	PrintSeparateLine();



	return;	

}





//--------------------------- 【行】-----------------------------



void print_invDetail_Line( struct TInvDetail_Line *invDetail_Line )

{

	SetLineSpace(REPORT_LINE_SPACE2);



	sprintf(pGeneralPrnBuff, "%04lu-%02lu-%02lu", invDetail_Line->Date / 10000, 

		(invDetail_Line->Date % 10000) / 100, invDetail_Line->Date % 100);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "      %010lu", invDetail_Line->InvNo);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "  %11.2f", ((double)(invDetail_Line->Sum)) / SUM_EXTENSION);

	

	PrintAsciiLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	return;		

}*/



//-----------------------------【尾】----------------------------------



void print_invDetail_End( struct TInvDetail_End *invDetail_End )

{



	SetLineSpace(REPORT_LINE_SPACE2);

	

//	PrintSeparateLine();

	

	sprintf(pGeneralPrnBuff, "正票金额 : %11.2f", ((double)(invDetail_End->PosInvSum)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "负票金额 : %11.2f", ((double)(invDetail_End->ReInvSum)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	sprintf(pGeneralPrnBuff, "净  金额 : %11.2f", ((double)(invDetail_End->NetInvSum)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	ForwardNLine(FORWARD_LINES);

	

	return;	

}





//-----------------------------------【行】-----------------------------------



/*void print_Day_Line( struct TDay *day )

{

	SetLineSpace(REPORT_LINE_SPACE2);



	sprintf(pGeneralPrnBuff, "%04lu-%02lu-%02lu", day->Date / 10000, 

		(day->Date % 10000) / 100, day->Date % 100);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "    %6lu", day->Amount);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "       %11.2f", ((double)(day->Sum)) / SUM_EXTENSION);

	

	PrintAsciiLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	return;		

}*/



//--------------------------------【尾】----------------------------------



void print_NetSum_End( INT64 NetSum , INT64 LiterSum)

{

	SetLineSpace(REPORT_LINE_SPACE2);

	

//	PrintSeparateLine();

	

	sprintf(pGeneralPrnBuff, "净  金额 : %11.2f", ((double)(NetSum)) / SUM_EXTENSION);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

#if DALIAN_VER

#else

    sprintf(pGeneralPrnBuff, "售油总量 : %11.2f", ((double)(LiterSum)) / 100);

    PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

#endif

//	ForwardNLine(FORWARD_LINES);

	

	return;

}





//-----------------------------------【行】----------------------------------- 



void print_Month_Line( struct TMonth *Month )       

{

	SetLineSpace(REPORT_LINE_SPACE2);



	sprintf(pGeneralPrnBuff, "%04lu-%02lu", Month->month / 100,  Month->month % 100);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "      %6lu", Month->Amount);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "        %11.2f", ((double)(Month->Sum)) / SUM_EXTENSION);

	

	PrintAsciiLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	return;	

}



//-----------------------------------【行】----------------------------------- ★★★



/*void print_Sales_Line( struct TSalesman *Salesman )       

{

	SetLineSpace(REPORT_LINE_SPACE2);



	sprintf(pGeneralPrnBuff, "%-10s", Salesman->Name);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %11lu", Salesman->Amount);

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], "       %11.2f", ((double)(Salesman->Sum)) / SUM_EXTENSION);

	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	return;	

}*/



/*------------------------------------------------------------------------------



商品报表 



作者: 赵健青



日期: 2005-04-07



-----------------------------------【行】-------------------------------------*/



void print_Goods_Line( struct TGoods * Goods ) // 商品报表行

{

	SetLineSpace(REPORT_LINE_SPACE2);

#if DALIAN_VER

	sprintf(pGeneralPrnBuff, "%-20s", Goods->GoodsName);

#else

	sprintf(pGeneralPrnBuff, "%-12s", Goods->GoodsName);

#endif	



#if DALIAN_VER

	sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %19.2f", ((double)(Goods->SalesSum)) / PRICE_EXTENSION);

#else

	if (memcmp((void *)Goods->GoodsName, "其它", 4)!=0)

	{	

		sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %7.2f", ((double)(Goods->Price)) / PRICE_EXTENSION);

		sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %7.2f", ((double)(Goods->GoodsNum)) / 100);

		sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %10.2f", ((double)(Goods->SalesSum)) / SUM_EXTENSION);

	}

	else

	{

		sprintf(&pGeneralPrnBuff[strlen(pGeneralPrnBuff)], " %26.2f", ((double)(Goods->SalesSum)) / SUM_EXTENSION);	

	}

#endif

	prn_log2("pGeneralPrnBuff = \"%s\"", pGeneralPrnBuff);

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));



	return;	

}



INT8 PrintRptTitle(INT8 *title)

{

	UINT8 i;

	UINT8 N;

	

	N = strlen(title);

	

	N = (MAX_CHAR_NUM - N) / 2;

	for(i=0; i<N; i++)

	{

		pGeneralPrnBuff[i] = ' ';

	}

	sprintf(&pGeneralPrnBuff[N],"%s", title);

	

	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));

	

	return 0;

}

char PrintYwtm(char *title)
{
	unsigned char i;
	unsigned char N;
	
	N = strlen(title);
	N = (46 - N) / 2;
	
	for(i=0; i<N; i++)	
	{
		
		pGeneralPrnBuff[i] = ' ';

	}
	
	sprintf(&pGeneralPrnBuff[N],"%s", title);
	
	PrintReportLine(pGeneralPrnBuff, strlen(pGeneralPrnBuff));
	
	return 0;	
}











































