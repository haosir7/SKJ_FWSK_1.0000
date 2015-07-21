/*! \file    TemplatePrintInv.cpp
\brief    发票打印接口文件
\author   zfj
 */

//#define POS_DEBUG_TO_TTY
#include <stdio.h>
#include <string.h>
#include "TemplatePrintInv.h"
#include "PrintDriver.h"
#include "TemplateIndep.h"
#include "code128.h"
#include "powoff_protect.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


#if (PRINT_BIZNFO!=0)
#include "barcoder_encode.h"
unsigned char g_inputBuf[10*1024];
unsigned char g_outBuf1[10*1024];
unsigned char g_outBuf2[10*1024];
#endif

#define		POWER_OFF		0

UINT8 bidirection =0;


extern UINT8 CycleTempFlag;
extern TSaveTemplateInfo *pSaveTemplateInfo;
//电子存根的标志
extern UINT8 stubFlag;

#define PRINT_INV_FORWARD_LINES		8

UINT8 nLineCount ;                  //记录掉电时走的行数
UINT8 nTotalLineCount = 0;          //统计总的行数
UINT8 nGetPrintLine = 0;            //打印缓冲区内的行数

UINT8  *m_RemainLineCount;          //掉电剩余打印行个数
UINT8  *m_BeginPrintFlag;           //标记掉电时是否已经开始打印数据

/**************************************************************************
功能     ：打印发票表头
格式     ：INT8 print_invoice_head( TPrnInvoiceInfo *invData)
输入参数 ：        
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
**************************************************************************/
INT8 print_invoice_head( TPrnInvoiceInfo *invData)
{
	TPrnLineInfo *pPrnLineInfo = NULL;
	UINT16 N;
	//nLineCount = g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount;	
	nLineCount = 0;
	//prn_log2("g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = %u",g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount);
	prn_log2("m_RemainLineCount = %u", *m_RemainLineCount);
	
	PrinterIni(bidirection);
	SetLineSpace(0);
	
	while(GetUsedLines());

	ResetTempBuf();
//临时注释	ZHANGLEI////
	//Paper_Gather_Start();	//开启收纸器	
////////////////////////
	prn_log2("当前打印类型%u",invData->PrintType);	
	while(1)
	{
		N = ParseTempLine(invData, &pPrnLineInfo);

       prn_log2("N = %u", N);					
		if(N != 0)
		{
			ResetTempBuf();
			return FAILURE;
		}
		
		if (pPrnLineInfo == NULL) 
		{
			break;
		}
		
		if(pPrnLineInfo->Property == END_INVOICE_HEAD_LINE)
		{
			break;
		}

		if(print_invoice_content(pPrnLineInfo) != SUCCESS)
		{
			return FAILURE;
		}
		prn_log2("当前打印类型%u",invData->PrintType);	
				
	}

	return SUCCESS;
}
/**************************************************************************
功能     ：打印商品明细,不定长版
格式     ：INT8 print_invoice_data(TPrnInvoiceInfo *invData)
输入参数 ：        
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
**************************************************************************/
INT8 print_invoice_data(TPrnInvoiceInfo *invData)
{	
	TPrnLineInfo *pPrnLineInfo = NULL;
	UINT16 N;
	UINT16 length;
	UINT8 i;
	UINT8 num;

	length = strlen(/*(void *)*/invData->GoodsLines[0].chProductName);

	//判断是否超长，如果超长需调用两次ParseTempLine，将折行内容打出
	if(length > pSaveTemplateInfo->MaxSPMCCharacter)
	{
		num = 2;
	}
	else
	{
		prn_log("调模板一次");
		num = 1;
	}
    for (i=0;i<num;i++)
    {
		N = ParseTempLine(invData, &pPrnLineInfo);
		if(N != 0)
		{
			ResetTempBuf();
			return FAILURE;
		}

		if (pPrnLineInfo == NULL) 
		{
			return FAILURE;
		}

		if(print_invoice_content(pPrnLineInfo) != SUCCESS)
		{
			return FAILURE;
		}
    }

	return SUCCESS;
}

/***********************************************************************************
功能     ：循环打印商品明细行
格式     ：INT8 print_invoice_data_cycle(TPrnInvoiceInfo *invData,UINT8 unfixedFlag)
输入参数 ：        
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
************************************************************************************/
INT8 print_invoice_data_cycle(TPrnInvoiceInfo *invData,UINT8 unfixedFlag)
{
	TPrnLineInfo *pPrnLineInfo = NULL;
	UINT16 N;	
	UINT8  emptyLineCount = 0;

    //如果unfixedFlag为0，记录当商品行为0时需要走几个空行，否则不记录
	//unfixedFlag代表不定长标志，主要适用在打不定长存根和退票

	if (unfixedFlag != 1)
	{
		emptyLineCount = pSaveTemplateInfo->MaxGoodsCount - invData->GoodsLineCount;
	}

	while(1)
	{		
		prn_log2("打印商品行数 invData->GoodsLineCount = %u",invData->GoodsLineCount);
		if((invData->GoodsLineCount == 0) && (emptyLineCount == 0))
		{
			break;
		}
		
		N = ParseTempLine(invData, &pPrnLineInfo);
		if(N != 0)
		{
			ResetTempBuf();
			return FAILURE;
		}

		if(print_invoice_content(pPrnLineInfo) != SUCCESS)
		{
			return FAILURE;
		}
	
		if(invData->GoodsLineCount != 0)
		{
			invData->GoodsLineCount--;		
		}
		else
		{
			emptyLineCount--;
		}
		prn_log2("此时invData->GoodsLineCount = %u",invData->GoodsLineCount);		
	}

	prn_log("循环打印商品行结束");

	return SUCCESS;
}

/***********************************************************************************
功能     ：打印发票票尾
格式     ：INT8 print_invoice_tail(TPrnInvoiceInfo *invData)
输入参数 ：        
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
************************************************************************************/
INT8 print_invoice_tail(TPrnInvoiceInfo *invData)
{
	TPrnLineInfo *pPrnLineInfo = NULL;
	UINT16 N;
	UINT16 i = 0;

	CycleTempFlag = 0;
//	SeekBackBorderBlackMark();//zongcan test  以后注释掉
//	BackwardNPoint(pSaveTemplateInfo->RepairPoint);//zongcan test  以后注释掉

	prn_log("开始解析模板行");

//	g_YW_PowerOffData->pwoffdate = GetCurDate();
//	g_YW_PowerOffData->pwofftime = GetCurTime();

	while(1)
	{
		N = ParseTempLine(invData, &pPrnLineInfo);

		prn_log("解析一行结束");
		i++;			
		if(N != 0)
		{
			ResetTempBuf();
			return FAILURE;
		}
			
		if (pPrnLineInfo == NULL) 
		{
			prn_log("返回空指针");
			break;
		}
	
		if(print_invoice_content(pPrnLineInfo) != SUCCESS)
	    {
			prn_log("打印内容错误");
			return FAILURE;
		}
		prn_log("打印一行结束");
	}
	
	//prn_log2("解析模板行的个数 = %u",i);
//	while(nGetPrintLine != 0 )
//	{
//	   nGetPrintLine = GetUsedLines();
//	   POWOFF_DISABLE();
//	   g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine - 3;
//	   POWOFF_ENABLE(); 
//	}
//	POWOFF_DISABLE();

	while(GetUsedLines());
	POWOFF_DISABLE();
// 	g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = 0;	
// 	g_YW_PowerOffData->ProcData.fptk_data.BeginPrintFlag = 1;
#if POWER_OFF == 1
	if( set_power_count(0) == FAILURE )
		return FAILURE;
	if( set_power_flag(1) == FAILURE )
		return FAILURE;
#endif
	nTotalLineCount = 0;
	POWOFF_ENABLE();
	

	ResetTempBuf();

	///////////////////////////////////////////////////////////////////////////

	//打印一维条码
// 	ForwardNPoint(3);
// 	PrintLineCode(invData->chYWTM); 
// 	ForwardNPoint(50);

   ///////////////////////////////////////////////////////////////////////////
	if (invData->PrintType == STUB_PRINT)
	{
		ForwardNLine(PRINT_INV_FORWARD_LINES);
	    return 0;
	}
	
	if(pSaveTemplateInfo->FixedModeFlag == 0)
	{
		//ForwardNPoint( G_SysArg->saveTemplateInfo.UnfixedForwardPoint);
		ForwardNPoint(pSaveTemplateInfo->EndForwardPoint);
		return SUCCESS;
	}
	
	if (pSaveTemplateInfo->markFlag == 1)
	{
#if SIDA_PRINTER

 		BackwardNPoint(pSaveTemplateInfo->BackwardPoint);
#endif
		if (1 != stubFlag) 
		{
			SeekBackBorderBlackMark();
		}		
	}
	
//	if (pSaveTemplateInfo->EndForwardPoint > 0)
	if (pSaveTemplateInfo->EndBackwardFlag == 0)
	{
		ForwardNPoint(pSaveTemplateInfo->EndForwardPoint);
//		BackwardNPoint(pSaveTemplateInfo->EndForwardPoint);
	}
#if SIDA_PRINTER
	else
	{
		BackwardNPoint(pSaveTemplateInfo->EndForwardPoint);
	}
#endif
	prn_log("打印发票尾部结束");
	return SUCCESS;
}
/***********************************************************************************
功能     ：真正的打印程序
格式     ：INT8 print_invoice_content(TPrnLineInfo *pPrnLineInfo)
输入参数 ：        
输出参数 ：
返回值   ：
作者     ：
日期     ：
摘要     ：
************************************************************************************/

INT8 print_invoice_content(TPrnLineInfo *pPrnLineInfo)
{
	
	UINT16 N;

	prn_log("进入真正的打印行");	
//	g_YW_PowerOffData->pwoffdate = GetCurDate();
//	g_YW_PowerOffData->pwofftime = GetCurTime();	

	if(pPrnLineInfo->Property == NEED_SAVE_TEMPLATE_LINE)
	{
		while(GetUsedLines());
		nTotalLineCount += 1;
		prn_log2("nTotalLineCount = %u",nTotalLineCount);
		prn_log("命令是ForwardNPoint");		

		if(nTotalLineCount > nLineCount)
		{
			prn_log(".......Forward N point!!!!");
			ForwardNPoint(pPrnLineInfo->BeginForwardPoint);
	    }

//		nGetPrintLine = GetUsedLines();		
//		prn_log2("nGetPrintLine = %u",nGetPrintLine);
//		POWOFF_DISABLE();
//		if (g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount >= 3)
//		{
//			g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine - 3;
//		}
//		nGetPrintLine = GetUsedLines();		
		prn_log2("ForwardNPoint::nGetPrintLine = %u",nGetPrintLine);
		POWOFF_DISABLE();
//		g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine;
//		g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - 1;
//		prn_log2("ForwardNPoint::RemainLineCount = %u",g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount);
#if POWER_OFF == 1
		if( set_power_count(nTotalLineCount - 1) == FAILURE )
			return FAILURE;
		prn_log2("m_RemainLineCount = %u", *m_RemainLineCount);
#endif
		POWOFF_ENABLE();	
	}	

	if(pPrnLineInfo->Property == END_INVOICE_DATA_LINE)
	{
		prn_log("此行属性为循环数据结束行");
		return SUCCESS;
	}
	
	N = strlen(pPrnLineInfo->chContent);
	
	while (N > 0)
	{
		if (pPrnLineInfo->chContent[--N] == ' ')
		{
			if (N == 0)
			{
				break;
			}
			pPrnLineInfo->chContent[N] = 0x0;
		}
		else
		{
			break;
		}
	}
	prn_log2("pPrnLineInfo->chContent = \"%s\"", pPrnLineInfo->chContent);
	if (pPrnLineInfo->Property == NORMAL_TEMPLATE_LINE)
	{
		if (pPrnLineInfo->FontH != 8)
		{
			nTotalLineCount += 1;
			prn_log2("nTotalLineCount = %u",nTotalLineCount);
			prn_log("命令是PrintChineseLine");					   
		   	if(nTotalLineCount > nLineCount)
		   	{
				prn_log2("pPrnLineInfo->chContent = \"%s\"", pPrnLineInfo->chContent);
				while(GetUsedLines());
				PrintChineseLine((INT8 *)pPrnLineInfo->chContent, (UINT8)strlen(pPrnLineInfo->chContent));
			}
//			nGetPrintLine = GetUsedLines();
			prn_log2("nGetPrintLine = %u",nGetPrintLine);
			POWOFF_DISABLE();
//			g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine;
// 			g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - 1;
// 			prn_log2("PrintChineseLine::RemainLineCount = %u",g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount);
#if POWER_OFF == 1
			if( set_power_count(nTotalLineCount - 1) == FAILURE )
				return FAILURE;
			prn_log2("m_RemainLineCount = %u", *m_RemainLineCount);
#endif
			POWOFF_ENABLE();	
		}
		else
		{
			nTotalLineCount += 1;	
			prn_log2("PrintAsciiLine::nTotalLineCount = %u",nTotalLineCount);
			if(nTotalLineCount > nLineCount)
			{
				prn_log2("pPrnLineInfo->chContent = \"%s\"", pPrnLineInfo->chContent);
				while(GetUsedLines());
				PrintAsciiLine((INT8 *)pPrnLineInfo->chContent, (UINT8)strlen(pPrnLineInfo->chContent));
			}
//			nGetPrintLine = GetUsedLines();			
			prn_log2("PrintAsciiLine::nGetPrintLine = %u",nGetPrintLine);
		 	POWOFF_DISABLE();
// 			g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine;
// 			g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - 1;	
// 			prn_log2("PrintAsciiLine::RemainLineCount = %u",g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount);
#if POWER_OFF == 1
			if( set_power_count(nTotalLineCount - 1) == FAILURE )
				return FAILURE;
			prn_log2("m_RemainLineCount = %u", *m_RemainLineCount);
#endif
			POWOFF_ENABLE();

		}
	}		

	nTotalLineCount += 1;
	prn_log2("ForwardNPoint::nTotalLineCount = %u",nTotalLineCount);
	if(nTotalLineCount > nLineCount)
	{
		prn_log(".......Forward N point!!!!");
		while(GetUsedLines());
		ForwardNPoint(pPrnLineInfo->Bhigh);
	}	
//	nGetPrintLine = GetUsedLines();	
	prn_log2("nGetPrintLine = %u",nGetPrintLine);
	POWOFF_DISABLE();
//	g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - nGetPrintLine;
// 	g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = nTotalLineCount - 1;
// 	prn_log2("ForwardNPoint::RemainLineCount = %u",g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount);
#if POWER_OFF == 1
	if( set_power_count(nTotalLineCount - 1) == FAILURE )
		return FAILURE;
	prn_log2("m_RemainLineCount = %u", *m_RemainLineCount);
#endif
	POWOFF_ENABLE();		

	return SUCCESS;
}

#if (PRINT_BIZNFO!=0)
INT8 print_biznfo(UINT8 *bufbiznfo)
{
	int imgWidth = 0;
	int imgHeight = 0;
	int outWidth,outHeight;
	int res;
	
	int codeType = QR_CODE;
	int version = 5;
	int ecl = 2;
	double length = 2.5;
	
	res = barcoder_encode(codeType, 0, ecl, 1, (unsigned char *)bufbiznfo, strlen(bufbiznfo), NULL, 2048, &imgWidth, &imgHeight);
	prn_log2("res = %d\n", res);
	
	res = barcoder_encode(codeType, 0, ecl, 1, (unsigned char *)bufbiznfo, strlen(bufbiznfo), g_inputBuf, 10*1024, &imgWidth, &imgHeight);
	prn_log2("res= %d",res);		
	
	BitmapTranse(g_inputBuf, imgWidth, imgHeight, g_outBuf1,&outWidth,&outHeight,length);
	
	prn_log3("row = %d col = %d\n",outHeight,outWidth);
	
	prn_log2("QRCodeLeftMargin = %u", pSaveTemplateInfo->QRCodeLeftMargin);
	PrintBiznfo(g_outBuf1,outHeight,outWidth, pSaveTemplateInfo->QRCodeLeftMargin);
	
	
	if(pSaveTemplateInfo->FixedModeFlag == 0)
	{
		ForwardNPoint(pSaveTemplateInfo->EndForwardPoint);
		return SUCCESS;
	}
	
	if (pSaveTemplateInfo->markFlag == 1)
	{
#if SIDA_PRINTER
		
		BackwardNPoint(pSaveTemplateInfo->BackwardPoint);
#endif
		if (1 != stubFlag) 
		{
			SeekBackBorderBlackMark();
		}		
	}
	
	//if (pSaveTemplateInfo->EndForwardPoint > 0)
	if (pSaveTemplateInfo->EndBackwardFlag == 0)
	{
		ForwardNPoint(pSaveTemplateInfo->EndForwardPoint);
	}
#if SIDA_PRINTER
	else
	{
		BackwardNPoint(pSaveTemplateInfo->BackwardPoint);
	}
#endif
	
	return SUCCESS;
}
#endif		//#if (PRINT_BIZNFO!=0)


void set_power_pointer(UINT8 *remanCount, UINT8 *beginFlag)
{
	m_RemainLineCount = remanCount;
	m_BeginPrintFlag = beginFlag;
}

INT8 set_power_count(UINT8 count)
{
	if( m_RemainLineCount == NULL )
		return FAILURE;

	*m_RemainLineCount = count;

	return SUCCESS;
}
INT8 set_power_flag(UINT8 flag)
{
	if( m_BeginPrintFlag == NULL )
		return FAILURE;

	*m_BeginPrintFlag = flag;

	return SUCCESS;
}


INT8 isPaper(void)
{
//----------------------------------------------
// 由于早期机器采用讯普的打印板，判缺纸经常会死机
// 目前，没有进行缺纸判断，直接返回有纸状态。后来
// 的斯大增加了缺纸判断，通过宏来控制 2005-11-07
//----------------------------------------------
#if XUNPU_PRINTER
	uint8 ch1,ch2;
	return 0;

	while ((SC_UART1_LSR&0x01)==1)
	{
		prn_log2("SC_UART1_RBR = %u", SC_UART1_RBR);	
	}

	SC_UART1_SendByte(0x10);
	SC_UART1_SendByte(0x04);
	SC_UART1_SendByte(0x33);
	while((SC_UART1_LSR&0x01)!=1)
	{
		prn_log2("SC_UART1_LSR&0x01 = %u", SC_UART1_LSR&0x01);
		//SendCharToSerial(DEBUG_COM, 0x11);
	}
	ch1=SC_UART1_RBR;

	SC_UART1_SendByte(0x10);
	SC_UART1_SendByte(0x04);
	SC_UART1_SendByte(0x33);
	while((SC_UART1_LSR&0x01)!=1)
	{
		prn_log2("SC_UART1_LSR&0x01 = %u", SC_UART1_LSR&0x01);
		//SendCharToSerial(DEBUG_COM, 0x11);
	}

	ch2=SC_UART1_RBR;
	prn_log2("ch1 = %u", ch1);
	prn_log2("ch2 = %u", ch2);
	prn_log2("g_AlreadyFindBlack = %u", g_AlreadyFindBlack);

#if FIND_BIACKMARK 
	if(ch1==ch2)
	{
		if (g_AlreadyFindBlack == 0)
		{
			switch(ch1)
			{
			case 0x81:	return (INT8)-1;
			case 0x80:  return 0;
			default:  break;
			}
		}
		else
		{
			switch(ch1)
			{
			case 0x03:	return (INT8)-1;
			case 0x0 :
			case 0x81:
			case 0x80:  return 0;
			default:  break;
			}
		}
	}
	else
	{
		return 0;  
	}
#else
	if(ch1==ch2)
	{
		
		switch(ch1)
		{
		case 0x1:
		case 0x81:	return (INT8)-1;
		case 0x0:
		case 0x80:  return 0;
		default:  break;
		}
	}
	else
	{
		return 0;  
	}
#endif	
	return (INT8)-1;
	
#else //XUNPU_PRINTER

	UINT8 ch;
//	UINT8 i;
	
	prn_log("before GetPrinterStatus");
	
	ch = GetPrinterStatus();
	ch &= 0x01;
	prn_log2("Receive data %x", ch);
	
	switch (ch)
	{
		case 0x00:
			return 0;
		default:
			break;
	}
	return (INT8)-1;

#endif //XUNPU_PRINTER
}































