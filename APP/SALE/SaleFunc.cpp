/*! \file    SaleFunc.cpp
\brief    awe4000r销售中的应用函数
\author   zcy
\version  1.0
\date     2015-05-13
*/



#include "SaleFunc.h"
//#include "CUserMore.h"
#include "CInvServ.h"
#include "CInvSum.h"
#include "APIBase.h"
#include "powoff_protect.h"
#include "SaleBusinessFunc.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"




UINT8 SALE_GetCurInv(CInvVol *pInvVol,string &strErr)
{
	
	INT32 ret=SUCCESS;

	ret=g_pAPIBase->GetCurInvInfo_API(*g_YwXmlArg, pInvVol, strErr);

	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
	g_YwXmlArg->m_fplxdm = pInvVol->m_fplxdm;
	DBG_PRINT(("g_YwXmlArg->m_fplxdm = %s", g_YwXmlArg->m_fplxdm.c_str()));
  

	return SUCCESS;
}

UINT8 SALE_MakeInvHand(CInvHead *pInvHead,string &strErr)
{

	INT32 ret=SUCCESS;

	DBG_PRINT(("pInvHead->m_kplx= %u",pInvHead->m_kplx));
	if((pInvHead->m_kplx == NORMAL_INV)||(pInvHead->m_kplx ==RETURN_INV))
	{	
		ret= g_pAPIBase->MakeNormalInv_API(*g_YwXmlArg, pInvHead, strErr);
	}
	else if (pInvHead->m_kplx == WASTE_INV)
	{	
		//执行盘的发票作废
		UINT8 zflx= 0; //空白作废
		ret= g_pAPIBase->MakeWasteInv_API(*g_YwXmlArg, pInvHead, zflx, strErr);
	}
				
	DBG_PRINT(("ret= %d",ret));
	if (ret !=SUCCESS)
	{
		return FAILURE;
	}
		
	return SUCCESS;
}

//---------------------------------------------------------------------------
//填充打印发票结构体的发票头
//---------------------------------------------------------------------------
UINT8 FillPrnInvHead(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo)
{
	DBG_ENTER("SaleData::FillPrnInvHead");
	DBG_PRINT(("进入FillPrnInvHead函数"));
	UINT8 N;
	INT32 nLen, errorcode;
	
	DBG_ASSERT_EXIT((smallInvInfo != NULL), (" smallInvInfo == NULL!"));
	DBG_ASSERT_EXIT((pPrnInvInfo != NULL), (" pPrnInvInfo == NULL!"));
	
	//发票代码由 20 ASC
	memset((void *)pPrnInvInfo->chTypeCode, 0, sizeof(pPrnInvInfo->chTypeCode));
	memcpy((void *)pPrnInvInfo->chTypeCode, (void *)(smallInvInfo->m_fpdm.c_str()), smallInvInfo->m_fpdm.length());
	DBG_PRINT(("发票代码  : %s ", pPrnInvInfo->chTypeCode));
	
	//发票号码
	pPrnInvInfo->InvNo = smallInvInfo->m_fphm; 
	DBG_PRINT(("发票号码  : %d ", pPrnInvInfo->InvNo));
	
	//开票日期 CCYYMMDD
	pPrnInvInfo->m_Date = smallInvInfo->m_kprq;
	DBG_PRINT(("开票日期  : %u ", pPrnInvInfo->m_Date));
	
	//开票时间
	pPrnInvInfo->m_Time = smallInvInfo->m_kpsj;
    DBG_PRINT(("开票时间  : %u ", pPrnInvInfo->m_Time));
	
	//开票类型
	pPrnInvInfo->InvType = smallInvInfo->m_kplx;
	DBG_PRINT(("开票类型  : %d ", pPrnInvInfo->InvType));
	
	//原发票号码
	pPrnInvInfo->PosInvNo = smallInvInfo->m_yfphm;
	DBG_PRINT(("原发票号码: %u ", pPrnInvInfo->PosInvNo));
	
	//付款单位
	nLen = smallInvInfo->m_fkdw.length() + 1;
	// 	if (smallInvInfo->m_kplx == RETURN_INV)
	// 	{
	// 		sprintf(pPrnInvInfo->chClientName, "%08lu", pPrnInvInfo->PosInvNo);
	// 	}
	// 	else
	//	{
	memcpy((void *)pPrnInvInfo->chClientName, (void *)smallInvInfo->m_fkdw.c_str(), nLen);
	//	}	
	N = strlen((INT8 *)pPrnInvInfo->chClientName);
	DBG_PRINT(("付款人  : %s ", pPrnInvInfo->chClientName));
	// 注释下面是为了不填充空格
	
	if (N > 0)
	{
		if (N <= 40)
		{
			memset((void *)&pPrnInvInfo->chClientName[N], ' ', 40 - N);
			pPrnInvInfo->chClientName[40] = '\0';
		}
		else
		{
			for(;;)
			{
				if (pPrnInvInfo->chClientName[--N] == ' ')
				{
					pPrnInvInfo->chClientName[N] = 0x0;
					if (N == 0 || N == 40)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	
	//收款员
	nLen = smallInvInfo->m_sky.length() + 1;
	memcpy((void *)pPrnInvInfo->chOperatorName, (void *)smallInvInfo->m_sky.c_str(), nLen);
    DBG_PRINT(("收款员  : %s ", pPrnInvInfo->chOperatorName));
	
	//收款员ID
	
	//付款方式
	
	//商品行数量
	
	//税率
	
	//征收率
	
	//票种类别
	
	
	
	//防伪税控码
	nLen = smallInvInfo->m_fwm.length();
	memset((void *)pPrnInvInfo->chTaxCtrCode, 0, sizeof(pPrnInvInfo->chTaxCtrCode));
	memcpy((void *)pPrnInvInfo->chTaxCtrCode, (void *)smallInvInfo->m_fwm.c_str(), nLen);
    DBG_PRINT(("防伪税控码  : %s ", pPrnInvInfo->chTaxCtrCode));
	
	//打印次数
		
	//纳税人名称 40 ASC
	nLen = g_globalArg->m_corpInfo->m_Nsrmc.length() + 1;
	memcpy((void *)pPrnInvInfo->chCorpName, (void *)g_globalArg->m_corpInfo->m_Nsrmc.c_str(), nLen);
	N = strlen((INT8 *)pPrnInvInfo->chCorpName);
    DBG_PRINT(("纳税人名称  : %s ", pPrnInvInfo->chCorpName));
	// 纳税人名称（收款单位）注释下面是为了不填充空格
	
	if (N > 0)
	{
		if (N <= 40)
		{
			memset((void *)&pPrnInvInfo->chCorpName[N], ' ', 40 - N);
			pPrnInvInfo->chCorpName[40] = '\0';
		}
		else
		{
			for(;;)
			{
				if (pPrnInvInfo->chCorpName[--N] == ' ')
				{
					pPrnInvInfo->chCorpName[N] = 0x0;
					if (N == 0 || N == 40)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	
	
	//纳税人识别号 16ASC
	memset(pPrnInvInfo->chCorpCode, 0, sizeof(pPrnInvInfo->chCorpCode));
	memcpy((void *)pPrnInvInfo->chCorpCode, (void *)g_globalArg->m_corpInfo->m_Nsrsbh.c_str(), g_globalArg->m_corpInfo->m_Nsrsbh.length());
	DBG_PRINT(("纳税人识别号  : %s ", pPrnInvInfo->chCorpCode));	
	
	//机器编码 16 ASC
	memset(pPrnInvInfo->chMachineNo, 0, sizeof(pPrnInvInfo->chMachineNo));
	memcpy((void *)pPrnInvInfo->chMachineNo, (void *)g_globalArg->m_machine->m_machineNo.c_str(), g_globalArg->m_machine->m_machineNo.length());
	DBG_PRINT(("机器编码  : %s ", pPrnInvInfo->chMachineNo));	
	
	
	//付款方识别号 16ASC
	if (smallInvInfo->m_fkdwsh == PAYER_NAME_DEF)
	{
		smallInvInfo->m_fkdwsh = "";
	}
	memset(pPrnInvInfo->chClientCode, 0, sizeof(pPrnInvInfo->chClientCode));
	memcpy((void *)pPrnInvInfo->chClientCode, (void *)smallInvInfo->m_fkdwsh.c_str(), smallInvInfo->m_fkdwsh.length());
	DBG_PRINT(("付款方识别号  : %s ", pPrnInvInfo->chClientCode));	
	
	//备用字段
	if( smallInvInfo->m_kplx == RET_SPECIAL_INV )
	{
		smallInvInfo->m_backup1 = "特殊红票  " + smallInvInfo->m_backup1;
	}
	memset(pPrnInvInfo->chRemarks, 0, sizeof(pPrnInvInfo->chRemarks));
	memcpy((void *)pPrnInvInfo->chRemarks, (void *)smallInvInfo->m_backup1.c_str(), smallInvInfo->m_backup1.length());
	if ((smallInvInfo->m_kplx==RETURN_INV)||(smallInvInfo->m_kplx==RET_SPECIAL_INV)||(smallInvInfo->m_kplx==RET_MANUAL_INV))
	{
		memset(pPrnInvInfo->chClientName, 0, sizeof(pPrnInvInfo->chClientName));
		memcpy((void *)pPrnInvInfo->chClientName, (void *)smallInvInfo->m_backup1.c_str(), smallInvInfo->m_backup1.length());
	}
	DBG_PRINT(("付款单位  : %s ", pPrnInvInfo->chClientName));	
	
	//自定义标签1
	string strZdy("");
	if("" != g_globalArg->m_prnInfo->m_zdyTab1)
	{
		strZdy = g_globalArg->m_prnInfo->m_zdyTab1;
		strZdy += " : ";
		
	}
	strZdy += g_globalArg->m_prnInfo->m_zdyCont1;
	
	memset(pPrnInvInfo->chSelfDefTab1, 0, sizeof(pPrnInvInfo->chSelfDefTab1));
	memcpy((void *)pPrnInvInfo->chSelfDefTab1, (void *)strZdy.c_str(), strZdy.length());
	DBG_PRINT(("自定义标签1  : %s ", pPrnInvInfo->chSelfDefTab1));
	
	
	
	//自定义内容1
	// 	memset(pPrnInvInfo->chSelfDefCont1, 0, sizeof(pPrnInvInfo->chSelfDefCont1));
	// 	memcpy((void *)pPrnInvInfo->chSelfDefCont1, (void *)g_globalArg->m_prnInfo->m_zdyCont1.c_str(),g_globalArg->m_prnInfo->m_zdyCont1.length());
	// 	DBG_PRINT(("自定义标签1  : %s ", pPrnInvInfo->chSelfDefCont1));	
	
	strZdy = "";
	if("" != g_globalArg->m_prnInfo->m_zdyTab2)
	{
		strZdy = g_globalArg->m_prnInfo->m_zdyTab2;
		strZdy += " : ";
	}
	strZdy += g_globalArg->m_prnInfo->m_zdyCont2;
	//自定义标签2
	
	memset(pPrnInvInfo->chSelfDefTab2, 0, sizeof(pPrnInvInfo->chSelfDefTab2));
	memcpy((void *)pPrnInvInfo->chSelfDefTab2, (void *)strZdy.c_str(), strZdy.length());
	DBG_PRINT(("自定义标签2  : %s ", pPrnInvInfo->chSelfDefTab2));	
	
	
	//自定义内容2
	// 	memset(pPrnInvInfo->chSelfDefCont2, 0, sizeof(pPrnInvInfo->chSelfDefCont2));
	// 	memcpy((void *)pPrnInvInfo->chSelfDefCont2, (void *)g_globalArg->m_prnInfo->m_zdyCont2.c_str(),g_globalArg->m_prnInfo->m_zdyCont2.length());
	// 	DBG_PRINT(("自定义标签2  : %s ", pPrnInvInfo->chSelfDefCont2));	
	
	
	DBG_PRINT(("退出FillPrnInvHead函数"));
    DBG_RETURN(SUCCESS);	
}

//---------------------------------------------------------------------------
//填充打印发票结构体的发票明细
//---------------------------------------------------------------------------
UINT8 FillPrnInvDetail(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo)
{
	DBG_PRINT(("进入FillPrnInvDetail函数"));
	DBG_ENTER("FillPrnInvDetail");
	
	UINT32 detailCount;
	UINT8 j=0;
	UINT8 i;
	UINT8 k = 0; //本张发票打印行数
	INT32 nLen;
	
	//单张发票打印行数目
	detailCount = smallInvInfo->m_sphsl;
	DBG_PRINT(("单张发票明细行数 == %u", detailCount));
	
	
	CInvDet *p = smallInvInfo->pHead;
	
    //若为空链表
	if (p == NULL)
	{
		DBG_PRINT((" smallInvInfo->pHead = NULL !"));
		DBG_RETURN(SUCCESS);		
	}
	
    //若为非空链表
	for( i=0; i<detailCount; i++, p = p->pNext )
	{
		//将带有折让的商品行分成两行来打
		pPrnInvInfo->GoodsLines[j].Type = p->m_property;							//商品行属性
	
		DBG_PRINT(("m_spsl= %f",p->m_spsl));
		p->m_dotNum = AmountRound(&(p->m_spsl));									//商品数量四舍五入
         DBG_PRINT(("m_dotNum= %d",p->m_dotNum));

		pPrnInvInfo->GoodsLines[j].Amount = FormatAmount(p->m_dotNum, p->m_spsl);	//商品数量
		DBG_PRINT(("商品数量          : %u ", pPrnInvInfo->GoodsLines[j].Amount));

		pPrnInvInfo->GoodsLines[j].Price = double2int(p->m_spdj*PRICE_EXTENSION);	//商品单价
		if( (p->m_property==DETAIL_DISCOUNT) || (p->m_property==DETAIL_REDUCTION_TEMPLATE) ) 
		{
			pPrnInvInfo->GoodsLines[j].Sum = 0 - p->m_spje;  //本商品行总金额
		}
		else
		{
			pPrnInvInfo->GoodsLines[j].Sum = p->m_spje;		 //本商品行总金额
		}
		pPrnInvInfo->GoodsLines[j].TaxSum = p->m_spse;	 //税额
		pPrnInvInfo->GoodsLines[j].TaxItemIndex = p->m_sl;	 //税率
		
		memcpy((void *)pPrnInvInfo->GoodsLines[j].chProductPLUCode, (void *)p->m_spbm.c_str(), p->m_spbm.length()); //商品编码
		
		nLen = p->m_spmc.length() + 1;
		memcpy((void *)pPrnInvInfo->GoodsLines[j].chProductName, (void *)p->m_spmc.c_str(), nLen);				/**< 商品名称 */
		
		DBG_PRINT(("-----------------------------"));
		DBG_PRINT(("商品行属性        : %u ", pPrnInvInfo->GoodsLines[j].Type));
		DBG_PRINT(("商品数量          : %u ", pPrnInvInfo->GoodsLines[j].Amount));
		DBG_PRINT(("商品数量小数位数  : %d ", p->m_dotNum));
		DBG_PRINT(("商品单价		  : %lld ", pPrnInvInfo->GoodsLines[j].Price));
		DBG_PRINT(("商品行总金额      : %lld ", pPrnInvInfo->GoodsLines[j].Sum));
		DBG_PRINT(("商品行总税额      : %lld ", pPrnInvInfo->GoodsLines[j].TaxSum));
		DBG_PRINT(("商品税率          : %f ", pPrnInvInfo->GoodsLines[j].TaxItemIndex));
		DBG_PRINT(("商品编码          : %s ", pPrnInvInfo->GoodsLines[j].chProductPLUCode));
		DBG_PRINT(("商品名称          : %s ", pPrnInvInfo->GoodsLines[j].chProductName));
		
		j++;
		k++;
		
        //商品名称超长
		if ((nLen-1) > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter)//xsr暂时注掉，因为没有模板。
			//	if ((nLen-1) > 10)
		{
			k++;
			DBG_PRINT(("商品名称超长"));
			DBG_PRINT(("商品名称最长 == %u", g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter));
		}
	}
	
	//打印商品行数
	pPrnInvInfo->GoodsLineCount =  k;
	DBG_PRINT(("单张发票打印商品行数目 == %u，k == %u", pPrnInvInfo->GoodsLineCount,k));
	
	DBG_PRINT(("退出FillPrnInvDetail函数"));
	DBG_RETURN(SUCCESS);
}


UINT8 FillPrnInvTail(TPrnInvoiceInfo *pPrnInvInfo, CInvHead *smallInvInfo)
{
	DBG_PRINT(("进入FillPrnInvTail函数"));
	DBG_ENTER("FillPrnInvTail");
	
	//开票价税合计 单位：分
	pPrnInvInfo->InvSumLowcase = smallInvInfo->m_kphjje;
	DBG_PRINT(("开票价税合计 小写 == %lld", pPrnInvInfo->InvSumLowcase));
	
	//开票价税合计 单位：分  大写
	if (smallInvInfo->m_kphjje != 0)
	{
		ChineseAmount((INT64)smallInvInfo->m_kphjje, (INT8 *)pPrnInvInfo->chInvSumUppercase, 64);
		DBG_PRINT(("开票价税合计 大写 = %s", pPrnInvInfo->chInvSumUppercase));
	}
	else
	{
		sprintf(pPrnInvInfo->chInvSumUppercase, "零圆整");
	}
	
	//开票合计税额 单位：分
	pPrnInvInfo->InvTaxLowcase = smallInvInfo->m_kpse;
	DBG_PRINT(("开票合计税额 小写 == %lld", pPrnInvInfo->InvTaxLowcase));
	
	
	//开票合计金额 单位：分
	pPrnInvInfo->InvMoneyLowcase = smallInvInfo->m_kphjje - smallInvInfo->m_kpse;
	DBG_PRINT(("开票合计金额 小写 == %lld", pPrnInvInfo->InvMoneyLowcase));
	
	//一维条码 20ASC
	memset(pPrnInvInfo->chYWTM, 0, sizeof(pPrnInvInfo->chYWTM));
	memcpy((void *)pPrnInvInfo->chYWTM, (void *)smallInvInfo->m_fwm.c_str(), smallInvInfo->m_fwm.length());
	DBG_PRINT(("一维条码  : %s ", pPrnInvInfo->chYWTM));
	
	
	DBG_PRINT(("退出FillPrnInvTail函数"));
	DBG_RETURN(SUCCESS);
}

UINT8 CheckIfFull(INT32 nInvHeadNum, INT32 nInvSumNum)
{
	CInvHead curInvHead;
// 	CInvSum curInvSum;
	
	INT32 errcode;
	
	//需要滚动
	if (nInvHeadNum>=INV_HEAD_MAX) 
	{
		curInvHead.Requery();
		errcode = curInvHead.LoadOneRecord();
		if (errcode==SQLITE_OK)
		{
			DBG_PRINT(("curInvHead.m_kprq = %u ", curInvHead.m_kprq));
			DBG_PRINT(("g_globalArg->m_startDecDate = %u ", g_globalArg->m_startDecDate));
			//第一条记录就是未申报的
			if( curInvHead.m_kprq >= g_globalArg->m_startDecDate ) 
			{
				DBG_PRINT(("INV_HEAD表已满，必须申报! "));
				DBG_RETURN(SUCCESS);
			}
		}
	}
	
// 	if (nInvSumNum>=INV_SUM_MAX) 
// 	{
// 		curInvSum.Requery();
// 		errcode = curInvSum.LoadOneRecord();
// 		if (errcode==SQLITE_OK)
// 		{
// 			DBG_PRINT(("curInvSum.m_issueEndDate = %u ", curInvSum.m_issueEndDate));
// 			DBG_PRINT(("g_globalArg->m_startDecDate = %u ", g_globalArg->m_startDecDate));
// 			if (curInvSum.m_issueEndDate>=g_globalArg->m_startDecDate) 
// 			{
// 				DBG_PRINT(("INV_SUM表已满，必须申报! "));
// 				DBG_RETURN(SUCCESS);
// 			}
// 		}
// 	}
	
	DBG_RETURN(FAILURE);
}


UINT8 ForwardPaper(void)
{
	DBG_ENTER("SaleData::ForwardPaper");
	if (isPaper() != 0)
	{
		DBG_RETURN(FAILURE);
	}
	
	if(g_globalArg->pSaveTemplateInfo->markFlag == 1)
	{
#if VOL_INV_PRN
		SeekBackBorderBlackMark();
#endif
		ForwardNPoint(g_globalArg->pSaveTemplateInfo->EndForwardPoint);	
		//	BackwardNPoint(g_globalArg->pSaveTemplateInfo->EndForwardPoint);
	}
	else
	{
		ForwardNPoint(5);
	}

	DBG_RETURN(SUCCESS);
}

UINT8 BackwardPaper(void)
{
	DBG_ENTER("SaleData::BackwardPaper");
	if (isPaper() != 0)
	{
		DBG_RETURN(FAILURE);
	}
	BackwardNPoint(10);

	DBG_RETURN(SUCCESS);
}

UINT8 NoMarkForwardPaper(void)
{
	DBG_ENTER("SaleData::NoMarkForwardPaper");
	if (isPaper() != 0)
	{
		DBG_RETURN(FAILURE);
	}
	
    ForwardNLine(2);
	
	DBG_RETURN(SUCCESS);
}

UINT8 MakeInvLog(UINT8 nInvType, UINT32 EventResult, string Backup)
{
	UINT8 Eventtype;
	
	switch(nInvType)
	{
	case NORMAL_INV:
		Eventtype = EVENT_MAKE_INVOICE;
		break;
	case RET_MANUAL_INV:
	case RETURN_INV:
	case RET_SPECIAL_INV:
		Eventtype = EVENT_MAKE_RET_INVOICE;
		break;
	case WASTE_INV:
	case WASTE_NOR:
	case WASTE_RET:
		Eventtype = EVENT_MAKE_WAS_INVOICE;
		break;
	default:;
	}
	
	if ((SUCCESS==EventResult)&&(EVENT_MAKE_INVOICE==Eventtype))
	{
		return SUCCESS;//开正票成功不记入系统日志
	}
	CGlobalArg::SysLog(Eventtype , EventResult, Backup);//写系统日志
	return SUCCESS;
}

UINT8 GetServNum(UINT32 &uNum,string &strErr)
{
	CInvServ curInvServ;
    uNum = curInvServ.GetRecordNum();
	DBG_PRINT(("缓存张数 uNum= %u", uNum));
	
	if (uNum <0)
	{
		strErr="查询未上传发票失败！";
		return FAILURE;
	}
	
	return SUCCESS;
	
}

UINT8 is_Money(const char *str)
{
	UINT32 PointNum=0;
	UINT32 len = strlen(str);
		
	if ((*str == '.')||(*(str+len-1)=='.'))
	{
		return FAILURE;
	}
	while(len > 0) 
	{
		if ((*str < '0' || *str > '9')&&(*str != '.')) 
		{
			return FAILURE;
		}
		else if (*str == '.')
		{
			PointNum++;
			if(PointNum>1)
			{
				return FAILURE;
			}
		}

		str++;
		len--;
	}
	return SUCCESS; 
}

#ifndef WIN32
void * NetCommunicate(void *arg)
{
#if TYPE_MODE ==  SKJ_MODE
	CSaleBusinessFunc saleFunc;
	CYWXML_GY ywXml_Gy;
	INT32 ret = SUCCESS;
	string strErr;
	UINT32 nTime = 10000;
	
	while(1)
	{
		if( (g_globalArg->m_pthreadFlag == 1) )
		{
			ywXml_Gy.m_nsrsbh = g_globalArg->m_pthreadNsrsbh;
			ywXml_Gy.m_sksbbh = g_globalArg->m_pthreadSksbbh;
			ywXml_Gy.m_sksbkl = g_globalArg->m_pthreadSksbkl;
			ywXml_Gy.m_fplxdm = g_globalArg->m_pthreadFplxdm;
			ywXml_Gy.m_jqbh = g_globalArg->m_pthreadJqbh;
			ywXml_Gy.m_kpjh = g_globalArg->m_pthreadKpjh;
			ywXml_Gy.m_zskl = g_globalArg->m_pthreadZskl;

			ret = saleFunc.InvoiceUpload(ywXml_Gy, strErr);
			if (SUCCESS != ret)
			{
				DBG_PRINT(("strErr = %s", strErr.c_str()));
				//g_globalArg->m_pthreadFlag = 0;
				g_globalArg->m_pthreadErr = strErr;
				CommonSleep(nTime);
			}
		}
		else
		{
			CommonSleep(nTime);
		}
	}
#endif
}

UINT8 UploadInvProc()
{
/*
DBG_PRINT(("UploadInvProc()"));
UINT32 nLIndex = 0;//连接失败计数器清零
UINT32 nLmt = 3;
INT32 errorcode = 0;
UINT32 nTime = 10000;
INT8 chValue[128];
CInvServ invServ;
CInvHead curInv;
//	CInvcodeLink codeLink;	
string strErr = "";	
string strCode = "";
UINT8 nLinkFlag = 0;//网络连通标志 0：连通，1：不通
int ii=0;
UINT32 nMinute = 0;
int i;
CInvDet *p;
UINT8 ret, TransRet;
XmlFpsc FpscSend;
CNetTrans tmpTrans;
string strRet = "";

	DBG_PRINT(("thread in "));
	DBG_PRINT(("g_globalArg->m_netOn = %u",g_globalArg->m_netOn));
	DBG_PRINT(("m_initFlag = %u, m_threadIn = %u",g_globalArg->m_initFlag, g_globalArg->m_threadIn));
	DBG_PRINT(("m_InvServNum = %u",g_globalArg->m_InvServNum));
	while(1)
	{
	//		if ((g_globalArg->m_InvServNum>0)&&(0==g_globalArg->m_threadIn))
	//		{
	//			nMinute = TDateTime::CurrentDateTime().Minute();
	//			if ((30<= nMinute)&&(nMinute<=35)) //时针指向30分到35分之间时
	//			{
	//				DBG_PRINT(("时针指向30分到35分之间时 "));
	//				g_globalArg->m_threadIn = 1;//允许连线，尝试再次上传
	//			}
	//		}
	
	 //若初始化 登录成功才可以上传发票
	 if ((1==g_globalArg->m_initFlag)&&(1==g_globalArg->m_threadIn))
	 {
	 //DBG_PRINT(("in "));
	 //建立连接
	 if (1 == nLinkFlag)//网络不通,增大循环间隔
	 {
	 DBG_PRINT(("网络不通,增大循环间隔 begin "));
	 CommonSleep(10*nTime);
	 DBG_PRINT(("网络不通,增大循环间隔 end "));
	 //		continue;
	 }
	 
	  if (0 >= g_globalArg->m_InvServNum) //InvServ表里无数据
	  {
	  //		DBG_PRINT(("InvServ表里无数据 "));
	  CommonSleep(nTime);
	  continue;
	  }
	  
	   //一张一张读取，分类处理
	   invServ.Requery();
	   errorcode = invServ.LoadOneRecord();
	   while (SQLITE_OK == errorcode)
	   {
	   DBG_PRINT(("InvServ表里一条记录 "));
	   //				if (0 == invServ.m_upFlag)//未上传
	   //				{
	   DBG_PRINT(("发票，待上传"));
	   //从CInvHead中查出该发票
	   memset((void*)chValue, 0, sizeof(chValue));
	   sprintf(chValue, "where FPDM = '%s' and FPHM = %u ", invServ.m_code.c_str(), invServ.m_InvNo);
	   DBG_PRINT(("invServ.m_code = %s",invServ.m_code.c_str()));
	   DBG_PRINT(("invServ.m_InvNo = %d",invServ.m_InvNo));
	   curInv.m_filter = chValue;
	   errorcode = curInv.Load();
	   DBG_PRINT(("errorcode= %d",errorcode));
	   if (SQLITE_OK != errorcode) 
	   {
	   g_globalArg->m_strMsg = "上传发票查找失败";
	   g_globalArg->m_ifMsg = 1;
	   break;				
	   }
	   
		//判断INV_SERV与INV_HEAD中的发票类型是否一致
		
		 DBG_PRINT(("curInv.m_fphm = %d",curInv.m_fphm));
		 DBG_PRINT(("invServ.m_InvNo = %d",invServ.m_InvNo));
		 
		  // 				if(curInv.m_fphm == invServ.m_InvNo)
		  // 				{
		  // 					if(curInv.m_kplx != invServ.m_issuetype)
		  // 					{
		  // 				    	curInv.m_kplx=invServ.m_issuetype;
		  // 					DBG_PRINT(("curInv.m_kplx = %d",curInv.m_kplx));
		  // 			       	DBG_PRINT(("invServ.m_issuetype = %d",invServ.m_issuetype));
		  // 					}    	
		  // 				}
		  // 				DBG_PRINT(("curInv.m_kplx = %d",curInv.m_kplx));
		  // 			    DBG_PRINT(("invServ.m_issuetype = %d",invServ.m_issuetype));
		  DBG_PRINT(("开始上传发票 "));
		  string resCode("");
		  FpscSend.m_invHead = &curInv;
		  
		   FpscSend.XmlFpscRequest( strErr);
		   
			tmpTrans.NetTransMutexLock();
			g_globalArg->m_sendInvFlag = 1;
			TransRet = tmpTrans.TransToSever(NULL, strErr);	//22代表单户信息更新
			g_globalArg->m_sendInvFlag = 0;
			tmpTrans.NetTransMutexUnlock();
			if (TransRet != SUCCESS)//网络交互失败
			{
			DBG_PRINT(("strErr = %s", strErr.c_str()));
			g_globalArg->m_strMsg = strErr;
			g_globalArg->m_ifMsg = 1;
			nLIndex++;
			if (nLIndex >= nLmt) 
			{
			nLinkFlag = 1;
			nLIndex=0;
			g_globalArg->m_threadIn =0;//进入离线状态，必须重启机器才能上传
			break;
			}
			break;						
			}
			
			 DBG_PRINT(("ret = %d", ret));
			 errorcode = FpscSend.XmlFpscRespond( DEF_RSP_FILE, strRet, strErr);
			 
			  DBG_PRINT(("strRet = %s", strRet.c_str()));
			  
			   if (strRet != "Y")
			   {
			   DBG_PRINT(("strErr = %s", strErr.c_str()));
			   g_globalArg->m_strMsg = strErr;
			   g_globalArg->m_ifMsg = 1;
			   nLIndex++;
			   if (nLIndex >= nLmt) 
			   {
			   nLinkFlag = 1;
			   nLIndex=0;
			   g_globalArg->m_threadIn =0;//进入离线状态，必须重启机器才能上传
			   break;
			   }
			   break;			
			   }
			   else
			   {
			   
				DBG_PRINT(("*****网络上传成功***** "));
				POWOFF_DISABLE(); //屏蔽掉电中断
				
				 //上传成功
				 //删除InvServ该条记录
				 nLIndex = 0;
				 nLinkFlag = 0;
				 sprintf(chValue, "where NO = %u", invServ.m_no);
				 invServ.m_filter = chValue;
				 invServ.Delete();
				 
				  //缓存张数减少
				  if(0 != g_globalArg->m_InvServNum)
				  {
				  g_globalArg->m_InvServNum--;
				  }
				  //					if ((NORMAL_INV == curInv.m_kplx)||(RETURN_INV ==curInv.m_kplx ))//正票
				  if ((NORMAL_INV == curInv.m_kplx ))//正票
				  {
				  g_globalArg->m_InvServSum -= invServ.m_moneySum;//缓存金额减少
				  }
				  POWOFF_ENABLE();  //开掉电中断
				  DBG_PRINT(("删除成功  g_globalArg->m_InvServNum =%u", g_globalArg->m_InvServNum));
				  
				   //取InvServ中下一条记录
				   sprintf(chValue, "where NO > %u limit 1", invServ.m_no);
				   invServ.m_filter = chValue;
				   invServ.Requery();
				   errorcode = invServ.LoadOneRecord();
				   }
				   }										
				   CommonSleep(nTime);
				   }
				   CommonSleep(1000);
				   }
*/
}
#endif