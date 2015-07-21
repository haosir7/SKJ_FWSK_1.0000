/****************************************************************************
文件名           ：PowOffRecover.cpp
功能             ：awe4000r销售中的应用函数。
起始日期         ：2008-01-31
作者             ：Yu Yan     
****************************************************************************/

#include "PowOffRecover.h"

#include "CaMsgBox.h"
#include "CaProgressBar.h"
//#include "CSystemInit.h"
//#include "CDeclareProc.h"
//#include "VolSumInfoWin.h"
#include "VersionConfig.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

UINT8 InvStuctToClass(struct TInvFullInfo *invFullInfo, CInvHead *smallInvInfo)
{
	DBG_ENTER("InvStuctToClass");

	DBG_ASSERT_EXIT((invFullInfo != NULL)&&(smallInvInfo != NULL), (" invFullInfo == NULL or smallInvInfo = NULL "));
		
//-----转换发票整体信息 
	
	/*发票代码关联号 */
	smallInvInfo->m_codeNo = invFullInfo->head.TypeCodeNo;
	DBG_PRINT(("发票代码关联号 : %u ", smallInvInfo->m_codeNo));

	/*发票号码                      */
	smallInvInfo->m_InvNo = invFullInfo->head.InvNo; 
	DBG_PRINT(("发票号码  : %u ", smallInvInfo->m_InvNo));

	/*开票类型                      */
	smallInvInfo->m_issuetype = invFullInfo->head.Type;
	DBG_PRINT(("开票类型 : %u ", smallInvInfo->m_issuetype));

	/*开票日期 CCYYMMDD             */
	smallInvInfo->m_issueDate = invFullInfo->head.Date;
	DBG_PRINT(("开票日期  : %u ", smallInvInfo->m_issueDate));

	smallInvInfo->m_issueTime = invFullInfo->head.Time;
    DBG_PRINT(("开票时间  : %u ", smallInvInfo->m_issueTime));

	/*原发票号码                    */
	smallInvInfo->m_oldInv = invFullInfo->head.PosInvNo;
	DBG_PRINT(("原发票号码  : %u ", smallInvInfo->m_oldInv));

	/*付款人 40 ASC                 */
	smallInvInfo->m_payer = (INT8*)(invFullInfo->head.ClientName);
	DBG_PRINT(("付款人 : %s ", smallInvInfo->m_payer.c_str()));
	
	/*开票员姓名，10 ASC       */
	smallInvInfo->m_operator = (INT8*)(invFullInfo->head.OperatorName);
    DBG_PRINT(("开票员姓名 : %s ", smallInvInfo->m_operator.c_str()));

	/*开票员ID      */
	smallInvInfo->m_opId = invFullInfo->head.OpId;
    DBG_PRINT(("开票员ID : %u ", smallInvInfo->m_opId));

	/**<商品行个数        	*/ 
	smallInvInfo->m_goodsCount = invFullInfo->head.detailCount;
	DBG_PRINT(("商品行个数 = %u", smallInvInfo->m_goodsCount));

	/**<开票总金额 单位：分  */
	smallInvInfo->m_moneySum = invFullInfo->head.TotalSum;
	DBG_PRINT(("开票总金额 小写 = %d", smallInvInfo->m_moneySum));

	/**<税控码 HEX            */
	memcpy((void *)smallInvInfo->m_fCode, (void *)invFullInfo->head.TaxCtrCode, INV_TAX_CTRL_CODE_LEN);
    DBG_PRINT(("税控码Hex : %s ", smallInvInfo->m_fCode));

	/**<付款方式  */    
	smallInvInfo->m_payType = invFullInfo->head.PayMode;
	DBG_PRINT(("付款方式  : %u ", smallInvInfo->m_payType));

//-----转换发票明细信息

	CInvDet *p;
    //若为非空链表
	for(UINT8 i=0; i<invFullInfo->head.detailCount; i++)
	{
//		p = new CInvDet;
		p = p->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
		DBG_ASSERT_EXIT((p != NULL), (" p == NULL"));

		p->m_property = invFullInfo->detail[i].Property;    /**< 商品行属性 */
		p->m_quantity = invFullInfo->detail[i].Amount;  /**< 商品数量 */		   
		p->m_price = invFullInfo->detail[i].Price;	    /**< 商品单价 */		
		p->m_moneySum = invFullInfo->detail[i].Sum;		/**< 总金额 */
		p->m_taxId = invFullInfo->detail[i].TaxItem;	/**< 税目 */
		p->m_code = (INT8*)(invFullInfo->detail[i].GoodsCode); /**< 商品代码（PLU代码） */
		p->m_name = (INT8*)(invFullInfo->detail[i].GoodsName); /**< 商品名称 */
		
		p->m_opId = smallInvInfo->m_opId;
		p->m_codeNo = smallInvInfo->m_codeNo;
		p->m_InvNo = smallInvInfo->m_InvNo;
		p->m_issueDate = smallInvInfo->m_issueDate;
		p->m_issueTime = smallInvInfo->m_issueTime;
		p->m_issuetype = smallInvInfo->m_issuetype;
		DBG_PRINT(("商品行属性: %u ", p->m_property));
		DBG_PRINT(("商品数量  : %f ", p->m_quantity));
		DBG_PRINT(("商品单价  : %f ", p->m_price));
		DBG_PRINT(("商品行总金额  : %d ", p->m_moneySum));
		DBG_PRINT(("商品税目  : %u ", p->m_taxId));
		DBG_PRINT(("商品PLU码 : %s ", p->m_code.c_str()));
		DBG_PRINT(("商品名称  : %s ", p->m_name.c_str()));

		smallInvInfo->InsertNode(p);	//插入节点
	}
	DBG_RETURN(SUCCESS);
}


UINT8 InvSumPrn(void)
{
	UINT8 ret = SUCCESS;
	UINT32 nCodeNo = g_globalArg->m_curInvVol->m_codeNo;
	UINT32 nStartInvNo = g_globalArg->m_curInvVol->m_InvStartNo;
	UINT32 nEndInvNo = g_globalArg->m_curInvVol->m_InvEndNo;
	INT8 chValue[256];
	memset((void *)chValue, 0, sizeof(chValue));	
	sprintf(chValue,"where CODE_NO = %u and IS_NO = %u and IE_NO = %u",nCodeNo, nStartInvNo, nEndInvNo);

	CInvSum invSum;
	invSum.m_filter = chValue;
	invSum.Requery();
	invSum.LoadOneRecord();

    //给标题赋值
//	sprintf(title_arr[0], "起止号码:%u--%u ", invSum.m_InvStartNo, invSum.m_InvEndNo);
//	sprintf(title_arr[1], "正票数:%u 红票数:%u 废票数:%u",invSum.m_normalInvSum, invSum.m_returnInvSum, invSum.m_wasteInvSum);
//	sprintf(title_arr[2], "正票金额:%.2lf", (double)(invSum.m_nomalMoneySum)/100.0);
//	sprintf(title_arr[3], "红票金额:%.2lf", (double)(invSum.m_returnMoneySum)/100.0);
//	sprintf(title_arr[4], "");
//	sprintf(title_arr[5], "");
//
//	ChangeTitle();
//	ReFresh();

//#ifndef WIN32
//	usleep(3000000);
//#endif
//	if (isPaper() != 0)
//	{	
//		CaMsgBox::ShowMsg("打印机未检测到纸");
//	}
	CaProgressBar proBar("单卷汇总打印中.....");
	proBar.ReFresh();

	struct TVolSumPrnData volSumPrnData;
	ret = CVolSumInfoWin::PrnVolSumInfo(&volSumPrnData, &invSum);	

	DBG_PRINT(("InvSumPrn();显示卷汇总并打印"));
#ifndef WIN32
	CommonSleep(9000);
#endif
	
	return ret;
}

UINT8 MakeInvRecover(struct TInvFullInfo *invFullInfo, UINT8 nChildId)
{
	DBG_ENTER("InvStuctToClass");
	DBG_ASSERT_EXIT((invFullInfo != NULL), (" invFullInfo == NULL "));
	DBG_PRINT(("begin MakeInvRecover"));
	
	UINT8 nIfPrn = 1;
	UINT8 ret;
	
	DBG_PRINT(("new SaleData"));
	if(pSaleData == NULL)
	{
		DBG_PRINT(("new SaleData"));
		pSaleData = new SaleData;
	}	
	DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL "));
	CInvHead smallInvHead;
	pSaleData->m_smallInvInfo = &smallInvHead; //存储发票内容
	DBG_ASSERT_EXIT((pSaleData->m_smallInvInfo != NULL), (" pSaleData->m_smallInvInfo == NULL "));

	DBG_PRINT(("begin InvStuctToClass"));
	InvStuctToClass(invFullInfo, pSaleData->m_smallInvInfo);
	if (pSaleData->m_smallInvInfo->m_issuetype==WASTE_INV)
	{
		nIfPrn = 0;
	}

	DBG_PRINT(("begin MakeInvoiceHandle"));
	//调用开票句柄
	ret = pSaleData->MakeInvoiceHandle(nIfPrn, nChildId);

	//----销售相关变量复位
    pSaleData->m_tmpGoodsNum = 0;  //当前总商品行数清零
	pSaleData->InitSaleData(1);	//销售信息初始化
	pSaleData->InitInvHead();		//单张发票信息初始化
	pSaleData->m_workState = WORK_COMPLETE;
	pSaleData->m_smallInvInfo = NULL;
	
	//判断是否进行卷汇总
	if(g_globalArg->m_curInvVol->m_remain == 0)
	{
		UINT8 nIfSum = 0;
		POWOFF_DISABLE(); //屏蔽掉电中断
		if(!pSaleData->InvSum(nIfSum))
		{
			delete pSaleData;
			pSaleData = NULL;
			DBG_PRINT(("Get volume Sum error !"));
			POWOFF_ENABLE(); //开掉电中断
			DBG_RETURN(FAILURE);
		}
		POWOFF_ENABLE(); //开掉电中断
		if (1==nIfSum) 
		{
			InvSumPrn();//显示卷汇总并打印
		}
	}
    
//	if (pSaleData != NULL) 
//	{
//		DBG_PRINT(("delete pSaleData"));
//		delete pSaleData;
//		pSaleData = NULL;
//	}

	if (ret != SUCCESS)
	{
		CGlobalArg::SysLog(EVENT_POWEROFF_INV, FAILURE);//写系统日志
		DBG_RETURN(FAILURE);
	}	

	DBG_PRINT(("end MakeInvRecover"));
	CGlobalArg::SysLog(EVENT_POWEROFF_INV, SUCCESS);//写系统日志
	DBG_RETURN(SUCCESS);
}

UINT8 RollRecover(void)
{
	DBG_PRINT((" ****滚动掉电恢复，开始***** "));
//	POWOFF_DISABLE(); //屏蔽掉电中断
	POWOFF_ENABLE();//开掉电中断
	SetPowerOffFlag();
	SetPowerOffProcID_M(INV_ROLL_PROC);//设主过程ID
	
	CInvHead curInvHead;
	CRtInv curRtInv;
	CInvSum curInvSum;

	UINT32 nInvDelDate = g_YW_PowerOffData->invRollPwOffData.invDatelimit;/**<InvHead表滚动删除的截止日期  */ 
	UINT32 nSumDelDate = g_YW_PowerOffData->invRollPwOffData.sumDatelimit;/**<InvSum表滚动删除的截止日期  */  
	UINT32 nRtDetNo = g_YW_PowerOffData->invRollPwOffData.rtNolimit;/**<RtInv表滚动删除的截止号码  */  
	UINT32 nRefDate = g_YW_PowerOffData->invRollPwOffData.nRefDate; /**<上次申报的截止日期  */ 
	//滚动
	curInvHead.Roll(nInvDelDate, nRefDate);
	curInvSum.Roll(nSumDelDate);
	curRtInv.Roll(nRtDetNo);

	CleanPowerOffFlag();
//	POWOFF_ENABLE();

	DBG_PRINT(("end RollRecover"));
	CGlobalArg::SysLog(EVENT_POWEROFF_ROLL, SUCCESS);//写系统日志
	DBG_RETURN(SUCCESS);
}


UINT8 poweroff_recover()
{
	UINT8 fatherID;
	UINT8 childID;
	UINT8 ret = SUCCESS;
	/*内存浪费 修改 XPK*/
	CSystemInit systemInit;
//	CInvoiceDistribute invDistribute;
//	CUpdateTaxData updateTax;
	CDeclareProc declareProc;

	DBG_PRINT(("begin to poweroff data recover"));
	
	GetPowerOffProcID_M(fatherID);
	GetPowerOffProcID_C(childID);

	DBG_PRINT(("fatherID: %u", fatherID));
	DBG_PRINT(("childID: %u", childID));
	
	
	if(g_YW_PowerOffData == NULL )
	{
		DBG_PRINT(("g_PowerOffData no Init!"));
		return SUCCESS;
	}
	
	if( GetPowerOffFlag() != 1 )
	{
        DBG_PRINT(("power off flag is 0 !"));
		return FAILURE;
	}
	

	DBG_PRINT(("g_YW_PowerOffData->main_proc_id = %u", g_YW_PowerOffData->main_proc_id ));
	
	if ((g_globalArg->m_initFlag == 0) && (g_YW_PowerOffData->main_proc_id != INIT_PROC))
	{
		return SUCCESS;
	}
	
	CaProgressBar proBar("掉电恢复中.....");
    string strError;

	switch(g_YW_PowerOffData->main_proc_id)
	{
	case INIT_PROC:  //初始化过程
		proBar.SetText("掉电恢复：初始化...");
		proBar.ReFresh();
		ret = systemInit.InitPowerOffRecover(childID);
		break;
		
	case FPFF_PROC:  //发票分发过程
//		proBar.SetText("掉电恢复：发票分发...");
//		proBar.ReFresh();
//		ret = invDistribute.InvPowerOffRecover(childID);
		break;
		
	case FPDR_PROC:  //发票导入过程
		break;
		
	case FPTK_NORMAL_PROC:  //发票填开过程
		proBar.SetText("掉电恢复：发票填开...");
		proBar.ReFresh();
		ret = MakeInvRecover(&(g_YW_PowerOffData->ProcData.fptk_data.posInvInfo), childID);
		break;
		
	case FPTK_RETURN_PROC:  //发票填开过程
		break;
		
	case FPTK_WASTE_PROC:  //发票填开过程
		break;
		
	case SB_PROC:    //申报过程
		proBar.SetText("掉电恢复：申报过程...");
		proBar.ReFresh();
		ret = declareProc.DecPowerOffRecover(childID);		
		break;
		
	case WS_PROC:    //完税过程
//		proBar.SetText("掉电恢复：完税过程...");
//		proBar.ReFresh();
//		ret = updateTax.UpdateTaxPowerOffRecover(childID);
		break;

	case INV_ROLL_PROC:    //滚动过程
		proBar.SetText("掉电恢复：数据库滚动...");
		proBar.ReFresh();
		ret = RollRecover();
		break;
		
	default:
		return 5;
	}
	
	if (ret == SUCCESS) 
	{
		DBG_PRINT(("掉电恢复成功！"));
		ShowMsg("掉电恢复成功！");
	}
	else
	{
		DBG_PRINT(("掉电恢复失败！"));
		ShowMsg("掉电恢复失败！");
	}

	return SUCCESS;	
}

void ShowMsg(string strInfo)
{
	CaMsgBox msgBox(strInfo.c_str(),CaMsgBox::MB_OK);
	msgBox.ShowBox();
	if (msgBox.m_iStatus == OK_PRESSED)
	{
//		ReFresh();
	}

}



