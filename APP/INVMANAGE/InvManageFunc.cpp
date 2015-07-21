/*! \file  InvManageFunc.cpp
\brief    awe4000r发票管理的应用函数
\author   zcy
\version  1.0
\date     2015-05-11
*/

#include "InvManageFunc.h"
#include "SaleBusinessFunc.h"
#include "CGlobalArg.h"
#include "VersionConfig.h"
#include "CaProgressBar.h"
#include "CaMsgBox.h"
#include "APIBase.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

//----------------------------------------------------
//空白发票作废
//----------------------------------------------------
UINT8 INVM_InvWasteProc()
{
	DBG_PRINT(("进入INVM_InvWasteProc函数"));
	
	UINT8 ret;

	CaProgressBar proBar("空白作废中.....");
    proBar.ReFresh();

	if (NULL==pSaleData)
	{
		pSaleData = new SaleData;
	}
	
	DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL!"));
	UINT8 nIfInvSum =0;
	ret = INVM_InvWaste(1,pSaleData,nIfInvSum); //空白作废
	
	DBG_PRINT(("ret= %u",ret));
	if (ret != SUCCESS)
	{
		pSaleData=NULL;
		return ret;
	}
    
	pSaleData=NULL;

	return  SUCCESS;	
}

UINT8 INVM_InvWaste(UINT32 nNum, SaleData *pSaleData, UINT8 &nIfInvSum)
{
	DBG_ENTER("InvWaste");
	DBG_PRINT(("进入InvWaste函数"));
	DBG_PRINT(("作废发票份数 == %u", nNum));
	UINT8 ret=SUCCESS;
	
	//发票作废
	for(UINT8 i=0; i<nNum; i++)
	{
		ret = INVM_InvWasteHandle(pSaleData);
		DBG_PRINT(("ret= %u",ret));
		if(ret != SUCCESS)
		{
			//销售相关变量复位
			pSaleData->m_tmpGoodsNum = 0;  //当前总商品行数清零
			pSaleData->InitSaleData(1);	//销售信息初始化
			pSaleData->InitInvHead();		//单张发票信息初始化
			pSaleData->m_workState = WORK_COMPLETE;
			pSaleData->m_smallInvInfo = NULL;
			DBG_PRINT(("InvWaste error"));
			DBG_RETURN(ret);
		}
	}
	
	//销售相关变量复位
	pSaleData->m_tmpGoodsNum = 0;  //当前总商品行数清零
	pSaleData->InitSaleData(1);	//销售信息初始化
	pSaleData->InitInvHead();		//单张发票信息初始化
	pSaleData->m_workState = WORK_COMPLETE;
	pSaleData->m_smallInvInfo = NULL;
	
	DBG_PRINT(("退出InvWaste函数"));
	DBG_RETURN(SUCCESS);
}

//-----------------------------------------------------------
//发票作废具体操作函数
//-----------------------------------------------------------
UINT8 INVM_InvWasteHandle(SaleData *pSaleData)
{
	DBG_ENTER("InvWasteHandle");
	DBG_PRINT(("进入InvWasteHandle函数"));
	DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL!"));
	
	UINT8 ret;
	
	//构建废票
	CInvHead curInvHead;
    pSaleData->m_smallInvInfo = &curInvHead;
	DBG_ASSERT_EXIT((pSaleData->m_smallInvInfo != NULL), (" pSaleData->m_smallInvInfo == NULL!"));
	
	INVM_BuildWasteInv(pSaleData->m_smallInvInfo);
	
	//调用开票句柄
	ret = pSaleData->MakeInvoiceHandle(0, INV_ROLL_PROC);
    DBG_PRINT(("ret= %u",ret));
	if (ret != SUCCESS)
	{
		DBG_RETURN(ret);
	}
	
	pSaleData->InitSaleData(1);
	pSaleData->m_smallInvInfo = NULL;
	DBG_PRINT(("退出InvWasteHandle函数"));
	DBG_RETURN(SUCCESS);	
}

//-----------------------------------------------------------
//构造作废发票
//-----------------------------------------------------------
void INVM_BuildWasteInv(CInvHead *pInvHead)
{
	DBG_ENTER("BuildWasteInv");	
	//DBG_ASSERT_EXIT((pInvHead != NULL), (" pInvHead == NULL!"));
	
	DBG_PRINT(("进入BuildWasteInv函数"));
	DBG_PRINT(("g_globalArg->m_curInvVol->m_code == %s", g_globalArg->m_curInvVol->m_code.c_str()));
	DBG_PRINT(("g_globalArg->m_curInvVol->m_ieno == %u", g_globalArg->m_curInvVol->m_ieno));
	DBG_PRINT(("g_globalArg->m_curInvVol->m_remain == %u", g_globalArg->m_curInvVol->m_remain));
	pInvHead->m_fpdm = g_globalArg->m_curInvVol->m_code;											//发票代码
	//pInvHead->m_fphm = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;	//发票号码
	pInvHead->m_fphm = g_globalArg->m_curInvVol->m_curInvNo; //发票号码
	DBG_PRINT(("pInvHead->m_fpdm == %s", pInvHead->m_fpdm.c_str()));
	DBG_PRINT(("pInvHead->m_fphm == %u", pInvHead->m_fphm));
    
	pInvHead->m_kplx = WASTE_INV;			//开票类型	
	pInvHead->m_yfphm = 0;					//原发票号码
	pInvHead->m_kphjje = 0;					//开票总金额 
	pInvHead->m_sphsl = 0;					//商品行个数	
    DBG_PRINT(("pInvHead->m_kplx == %u", pInvHead->m_kplx));
    DBG_PRINT(("pInvHead->m_yfphm == %u", pInvHead->m_yfphm));
	
   	pInvHead->m_kprq = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);	//开票日期
	pInvHead->m_kpsj = TDateTime::CurrentDateTime().FormatInt(HHMMSS);	//开票时间
	
	
	pInvHead->m_fkdw = "";					//付款人名称
	DBG_PRINT(("pInvHead->m_fkdw == %s", pInvHead->m_fkdw.c_str()));
	
	pInvHead->m_sky = g_globalArg->m_operator->m_name;			//操作员名称
	//	pInvHead->m_skyid = g_globalArg->m_operator->m_opid;		//操作员id	
	DBG_PRINT(("pInvHead->m_sky == %s", pInvHead->m_sky.c_str()));
	
	
	pInvHead->m_backup1 = "";		//备用字段1
	pInvHead->m_bzkz = "";
	pInvHead->m_hczt = "";		    //是否可以红冲状态 
	//	pInvHead->m_backup3 = "";		//备用字段3
	
    pInvHead->DelList();			//释放链表空间
	
	DBG_PRINT(("退出BuildWasteInv函数"));
	return;
}

UINT8 INVM_CheckIsNoDB(string invCode,UINT32 invNo,CInvHead *pInvHead)
{
	//查询所要退的发票信息
	INT8 value[256];
	memset((void*)value, 0, sizeof(value));
	INT32 errorcode=0;
	sprintf(value,"where FPDM = '%s' and FPHM = %u",invCode.c_str(), invNo);
//	errorcode = pInvHead->LoadOneRecordBySql(value);

	pInvHead->m_filter.append(value);
	errorcode = pInvHead->Load();
	
	DBG_PRINT(("errorcode= %d",errorcode));
	DBG_PRINT(("pInvHead->m_kplx= %u",pInvHead->m_kplx));
	if (errorcode != SQLITE_OK)
	{
		if (errorcode == SQLITE_DONE)
		{
		return NO_INV; 	
	}
		else
		{
			return QUERY_ERROR; 
		}	
	}
	return SUCCESS;
}
//---------------------------------------------------------------------
//检验发票是否可退
//---------------------------------------------------------------------
UINT8 INVM_CheckIsNoRet(CInvHead *pInvHead)
{
	DBG_ENTER("MKINV_CheckIsNoRet");
	DBG_PRINT(("进入MKINV_CheckIsNoRet函数"));
	
	DBG_PRINT(("pInvHead->m_kplx= %u",pInvHead->m_kplx));
	
	//该票是否在本月内//debug
	
	DBG_PRINT(("pInvHead->m_kprq= %u",pInvHead->m_kprq));

	INT32 nReturn = CmpCurMonth(pInvHead->m_kprq, pInvHead->m_kpsj);
	DBG_PRINT(("nReturn= %d",nReturn));
	if (0 == nReturn) 
	{
		DBG_PRINT(("本月发票,不得冲红%d", nReturn));
		// DBG_RETURN(BX_RET_MONTH_ERR);
	}
	
	INT8 value[256];
	memset((void*)value, 0, sizeof(value));
	INT32 errorcode;
	sprintf(value,"where FPDM = '%s' and FPHM = %u",pInvHead->m_fpdm.c_str(), pInvHead->m_fphm);
	
	//若为已退发票，返回
	CRtInv rtInv;
	rtInv.m_filter = value;
	//查询
	rtInv.Requery();
	DBG_PRINT(("pInvHead->m_bzkz = %s", pInvHead->m_bzkz.c_str()));	
	if ((rtInv.LoadOneRecord() == SQLITE_OK)||("1" == pInvHead->m_bzkz))
	{
		DBG_PRINT((" This invoice already has returned invoice!"));
		DBG_RETURN(HAVE_RT_INV);	
	}
		
	//若为红票，返回
	if ((pInvHead->m_kplx == RETURN_INV)||(pInvHead->m_kplx == RET_MANUAL_INV)||
		(pInvHead->m_kplx == RET_SPECIAL_INV)) 
	{
		DBG_RETURN(RT_INV);		
	}
	//若为废票，返回
	if( (pInvHead->m_kplx == WASTE_INV) || (pInvHead->m_kplx == WASTE_NOR) 
		|| (pInvHead->m_kplx == WASTE_RET))
	{
		DBG_RETURN(WT_INV);		
	}
	
	//不能开红票，返回
	DBG_PRINT(("pInvHead->m_hczt = %s", pInvHead->m_hczt.c_str()));	
	if ( "1" == pInvHead->m_hczt) 
	{		
		g_globalArg->m_strMsg =pInvHead->m_backup2;
		DBG_RETURN(INV_LIB_ERR);
	}
		
	DBG_PRINT(("退出CheckInvReturn函数"));
	DBG_RETURN(SUCCESS);
}

//-----------------------------------------------------------
//退本机发票
//-----------------------------------------------------------
UINT8 INVM_InvReturn(SaleData *pSale,CInvHead *pInvHead)
{
	DBG_ENTER("INVM_InvReturn");
	DBG_ASSERT_EXIT((pInvHead != NULL), (" pInvHead == NULL!"));
	
	INT8 value[128];
	memset((void*)value, 0, sizeof(value));
	
	pSale->m_singleInvInfo =pInvHead;
	
	DBG_PRINT(("pSale->m_singleInvInfo->m_fphm=%u",pSale->m_singleInvInfo->m_fphm));
	
	//组装红票信息
	pSale->m_singleInvInfo->m_yfpdm = pSale->m_singleInvInfo->m_fpdm;
	pSale->m_singleInvInfo->m_yfphm = pSale->m_singleInvInfo->m_fphm;
    pSale->m_singleInvInfo->m_kplx = RETURN_INV;
	pSale->m_singleInvInfo->m_sky = g_globalArg->m_operator->m_name;//取当前操作员
	sprintf(value, "对应正数发票代码:%s号码:%08lu",
		pSale->m_singleInvInfo->m_yfpdm.c_str(), pSale->m_singleInvInfo->m_yfphm);	
	pSale->m_singleInvInfo->m_backup1 = value;
	
	INT64 moneySum = pSale->m_singleInvInfo->m_kphjje;
	
	//单卷红票累计金额超限
//	if ((g_globalArg->m_returnVolSum + moneySum)>MAX_MONEY)
//	{
//		DBG_PRINT((" Warning: m_returnVolSum exceed the limit!"));
//		DBG_RETURN(RVM_SUM_EXCEED);
// 	}
	 	 
	UINT8 nIfInvSum; //是否执行了卷汇总操作 1：是；0：否
	UINT8 ret =pSale->PayByCash(nIfInvSum);

	DBG_PRINT(("ret= %u",ret));

	return ret; //开票
}

UINT8 INVM_InvRetProc(CInvHead *pInvHead)
{

    DBG_PRINT(("进入INVM_InvRetProc函数"));
	
	UINT8 ret;

	CaProgressBar proBar("红票打印中.....");
    proBar.ReFresh();

	if (NULL==pSaleData)//extern SaleData *pSaleData; /**< 销售数据类指针*/

	{
		pSaleData = new SaleData;
	}	
	DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL!"));

	ret = INVM_InvReturn(pSaleData,pInvHead); //退本机发票
	
	DBG_PRINT(("ret= %u",ret));
	if (ret != SUCCESS)
	{
		pSaleData=NULL;
		return ret;
	}
    
	pSaleData=NULL;

	return  SUCCESS;
}

//退非本机发票（退货）
// UINT8 ReturnGoods(SaleData *pSale, string codeNo, UINT32 invNo)
// {
// 	DBG_ENTER("InvReturn");
// 	DBG_ASSERT_EXIT((pSale != NULL), (" pSale == NULL!"));
// 	
// 	pSale->m_invtype = RET_MANUAL_INV;//RETURN_INV; 
// 	pSale->m_oldInv = invNo;
// 	pSale->m_oldCodeNo = codeNo;
// 	pSale->m_returnOther = 1;
// 	
// 	DBG_RETURN(SUCCESS);
//
//-----------------------------------------------------------
//特殊红票
//-----------------------------------------------------------
// UINT8 InvReturnSpe(SaleData *pSale)
// {
// 	DBG_ENTER("InvReturn");
// 	DBG_ASSERT_EXIT((pSale != NULL), (" pSale == NULL!"));
// 	
// 	//	pSale->m_invtype = RETURN_INV; 
// 	//退非本机发票发票类型RETURN_MANUAL_INV
// 	pSale->m_invtype = RET_SPECIAL_INV; 
// 	pSale->m_oldInv = 0;
// 	pSale->m_oldCodeNo = "";
// 	pSale->m_returnOther = 1;
// 	
// 	DBG_RETURN(SUCCESS);	
// }

//-----------------------------------------------------------
//通过发票代码查询发票代码关联号
//-----------------------------------------------------------
// UINT8 CheckInvCode(INT8 *code)
// {
// 	DBG_PRINT(("CheckInvCode"));
// 	
// 	char sqlbuf[128];
// 	CInvVol pInvVol;
// 	
// 	memset(sqlbuf, 0, sizeof(sqlbuf));
// 	sprintf(sqlbuf, "where CODE = '%s' ", code);
// 	pInvVol.m_filter.append(sqlbuf);
// 	
// 	pInvVol.Requery();
// 	int errcode = pInvVol.LoadOneRecord();
// 	if( errcode != SQLITE_OK )
// 		return FAILURE;
// 	
// 	return SUCCESS;
// }

//-------------------------------------------------------------
//获取款机数据库中记录的发票卷数
//-------------------------------------------------------------
UINT8 INVM_GetVolNum(UINT32 &nNum)
{
	DBG_ENTER("GetVolNum(UINT32 &nNum)");
	
	INT32 errorcode;
	nNum = 0;
	INT32 nTmpNum = 0;
	CInvVol curInvVol;
	CInvVol *pInvVol = &curInvVol;
	DBG_ASSERT_EXIT((pInvVol != NULL), (" pInvVol == NULL!"));
	
	//查总记录数，已分发未导入，赋值pageNum
	pInvVol->m_filter = "where USED_FLAG = 1 and OVER_FLAG = 1";
	nTmpNum = pInvVol->GetRecordNum();
	if (nTmpNum <= 0)
	{
		pInvVol = NULL;
		DBG_RETURN(FAILURE); 
	}
	
	nNum = nTmpNum;
	pInvVol = NULL;
	DBG_PRINT((" GetVolNum!"));
	DBG_RETURN(SUCCESS);
}

//------------------------------------------------------------------
//
//------------------------------------------------------------------
UINT8 YesNoBox(string strMsg)
{
	DBG_ENTER("YesNoBox(string strMsg)");
	
	// 	INT8 value[256];
	// 	memset((void*)value, 0, sizeof(value));
	//	sprintf(value,"%u卷发票未导入,查看吗?",nNum);
	
	CaMsgBox msgBox(strMsg.c_str(),CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		DBG_PRINT((" YesNoBox(string strMsg)  FAILURE"));
		DBG_RETURN(FAILURE); 
	}
	
	if(msgBox.m_iStatus == OK_PRESSED)
	{
		DBG_PRINT((" YesNoBox(string strMsg)  SUCCESS"));
		DBG_RETURN(SUCCESS);		
	}
}
//------------------------------------------------------------------
//存根打印
//------------------------------------------------------------------
UINT8 INVM_GetInvNum(string nCode, UINT32 nStartNo, UINT32 nEndNo, UINT32 &nNum)
{
	DBG_ENTER("GetInvNum(UINT32 &nNum)");
	
	nNum = 0;
	INT32 errorcode;
	INT32 nTmpNum = 0;
	CInvHead curInvHead;
	CInvHead *pInvHead = &curInvHead;
	DBG_ASSERT_EXIT((pInvHead != NULL), (" pInvHead == NULL!"));
	
	//查总记录数，已分发未导入，赋值pageNum
	
	INT8 value[256];
	memset((void*)value, 0, sizeof(value));
	pInvHead->m_filter = "where FPDM = ";
	sprintf(value,"'%s' and FPHM >= %u and FPHM <= %u",nCode.c_str(),nStartNo,nEndNo);
	pInvHead->m_filter.append(value);
	DBG_PRINT((" pInvHead->m_filter = %s", pInvHead->m_filter.c_str()));
	nTmpNum = pInvHead->GetRecordNum();
	
	pInvHead = NULL;
	
	if (nTmpNum<0)
	{
		DBG_RETURN(FAILURE); 
	}
    nNum = nTmpNum;
	DBG_RETURN(SUCCESS); 
}

//-----------------------------------------------------------
//打印发票存根
//-----------------------------------------------------------
UINT8 PrnInvStub(CInvHead *smallInvInfo, UINT8 IfPrnChar)
{

	DBG_ASSERT_EXIT((smallInvInfo != NULL), (" smallInvInfo == NULL!"));
	
	//判断装入纸质发票	
	if (isPaper() != 0)
	{		
		DBG_RETURN(NO_PAPER);
	}	
	
	//开辟打印数据结构体的内存
	TPrnInvoiceInfo *pInvPrnData;
	pInvPrnData = &g_invPrnData;;
	if(pInvPrnData==NULL)
	{
		DBG_PRINT((" malloc(sizeof(pInvPrnData) error !"));
		DBG_RETURN(MALLOC_MEM_ERROR);		
	}
	memset((void *)pInvPrnData, 0, sizeof(TPrnInvoiceInfo));//打印结构体内容清零
	if (1==IfPrnChar) 
	{
		pInvPrnData->PrintType = STUB_PRINT; //打印类型为存根打印
	}
	
	//定长版,填充
	FillPrnInvHead(pInvPrnData, smallInvInfo);
	FillPrnInvDetail(pInvPrnData, smallInvInfo);			
	FillPrnInvTail(pInvPrnData, smallInvInfo);
	
	//定长版，打印
	print_invoice_head(pInvPrnData);
	print_invoice_data_cycle(pInvPrnData, 0);				
	print_invoice_tail(pInvPrnData);
	
	pInvPrnData = NULL;
	
	DBG_RETURN(SUCCESS);
	
}

//------------------------------------------------------------------
//整卷作废
//------------------------------------------------------------------
// UINT8 WasteInvVol(void)
// {
// 	UINT8 nRet = SUCCESS;
// 	INT32 nErrcode;
// 	CInvVol invVol;
// 	string sqlstr = "";
// 	
// 	sqlstr="update INV_VOL set USED_FLAG = 1 where USED_FLAG = 0 and OVER_FLAG = 1";
// 	invVol.SetSQL(sqlstr);
// 	if (invVol.ExecuteSQL() != SQLITE_OK)
// 	{
// 		nRet = FAILURE;//整卷作废失败
// 	}
// 	else
// 	{
// 		g_globalArg->m_curInvVol->m_isno = 0;	/**< 发票起始号码 */
// 		g_globalArg->m_curInvVol->m_ieno = 0;	/**< 发票截止号码 */
// 		g_globalArg->m_curInvVol->m_remain = 0;		/**< 发票卷剩余份数 */	
// 		g_globalArg->m_curInvVol->m_usedflag = 1;	/**< 发票卷导入标志 1:未导入 0:导入 */
// 		g_globalArg->m_curInvVol->m_overflag = 1;	/**< 发票卷用完标志 1:未用完 0:用完 */
// 	}
// 	
// 	return nRet;
// }

//是否本月
INT32 CmpCurMonth(UINT32 nDate, UINT32 nTime)
{
	INT32 nRet = 1;
	//该票是否在本月内???????
	UINT32 curYear, curMonth, curDay;
	UINT32 tmpYear, tmpMonth, tmpDay;
	TDateTime::CurrentDateTime().DecodeDate(curYear, curMonth, curDay);
	DBG_PRINT(("curYear=%u, curMonth = %u, curDay = %u", curYear, curMonth, curDay));
	TDateTime invDate(nDate, nTime);
	invDate.DecodeDate(tmpYear, tmpMonth, tmpDay);
	DBG_PRINT(("tmpYear=%u, tmpMonth = %u, tmpDay = %u", tmpYear, tmpMonth, tmpDay));
	
	if (tmpYear < curYear) 
	{
		DBG_PRINT(("tmpYear < curYear"));
		nRet = -1;		
	}
	else if (tmpYear == curYear) 
	{
		if (tmpMonth < curMonth) 
		{
			DBG_PRINT(("tmpMonth < curMonth"));
			nRet = -1;			
		}
		else if (tmpMonth == curMonth) 
		{
			DBG_PRINT(("tmpMonth == curMonth"));
			nRet = 0;
		}		
	}
	
	return nRet;
}

//已开发票作废
UINT8 INVM_WstInv(CInvHead* pInvHead, string &strErr)
{
	DBG_ASSERT_EXIT((pInvHead != NULL), (" pInvHead == NULL!"));
	INT32 ret = SUCCESS;
    INT8 val[256];
	string strsql("");
	UINT8 waste_type = 0;
	UINT8 tmpType = pInvHead->m_kplx;
	
	switch(pInvHead->m_kplx) 
	{
	case NORMAL_INV:
		waste_type = WASTE_NOR;//正票可以作废，成为正废票
		break;
	case RETURN_INV:
	case RET_MANUAL_INV:
	case RET_SPECIAL_INV:
		waste_type = WASTE_RET;//红票(负票)也可以作废，成为负废票
		break;
	default:;
	}
	
	UINT8 zflx= 1; //已开发票作废
	ret= g_pAPIBase->MakeWasteInv_API(*g_YwXmlArg, pInvHead, zflx, strErr);
	
	if (SUCCESS != ret)
	{			
		DBG_PRINT(("strErr = %s", strErr.c_str()));
		return FAILURE;
	}


	POWOFF_DISABLE(); //屏蔽掉电中断
	//更新INV_HEAD发票类型
	
	memset((void*)val, 0x00, sizeof(val));
	sprintf(val, "where FPDM='%s' AND FPHM=%u", pInvHead->m_fpdm.c_str(), pInvHead->m_fphm);
	strsql = val;
	
	pInvHead->m_kplx = waste_type;
	pInvHead->m_kprq = pInvHead->m_zfrq;
	pInvHead->m_kpsj = pInvHead->m_zfsj;

	ret = pInvHead->Update(strsql, &(pInvHead->m_kplx), &(pInvHead->m_kprq), &(pInvHead->m_kpsj), NULL);

	if ( ret!= SQLITE_OK)
	{
		strErr = "更新发票信息表错误";
		return FAILURE;
	}

	DBG_PRINT(("更新INV_HEAD发票类型strsql = %s", strsql.c_str()));

	//更新INV_DET发票类型
	CInvDet pInvDet;

	pInvDet.m_kplx = waste_type;
	pInvDet.m_kprq = pInvHead->m_zfrq;
	pInvDet.m_kpsj = pInvHead->m_zfsj;

	ret = pInvDet.Update(strsql, &(pInvDet.m_kplx), &(pInvDet.m_kprq), &(pInvDet.m_kpsj), NULL);
	
	if ( ret!= SQLITE_OK)
	{
		strErr = "更新发票信息表错误";
		return FAILURE;
	}

	DBG_PRINT(("更新INV_DET发票类型strsql = %s", strsql.c_str()));
	
	//删除RT_INV表中，红票对应的原发票代码和号码
	DBG_PRINT(("pInvHead->m_kplx = %d", pInvHead->m_kplx));//开票类型并不从金税盘中返回
	if(tmpType == RETURN_INV)
	{
		CRtInv rtInv;
		memset(val, 0, sizeof(val));
		sprintf(val, "where FPDM = '%s' and FPHM = %u ", pInvHead->m_yfpdm.c_str(), pInvHead->m_yfphm);
		DBG_PRINT(("val = %s", val));
		rtInv.m_filter = val;
		if( rtInv.Delete() != SQLITE_OK )
		{
			DBG_PRINT(("Delete RT_INV info error"));
		}
		DBG_PRINT(("Delete RT_INV info succeed"));
	}
	
	POWOFF_ENABLE(); //开掉电中断
	
	DBG_PRINT(("WstInv() SUCCESS"));
		
	return SUCCESS;
}	
	
//按发票号码查询发票明细
UINT8 INVM_InvDetailNOQuery(string invCode,UINT32 invNO)
{		
	INT32 Ret=SUCCESS;
	string strErr("");
	string strsql("");

	INT32 ret=0;

	CaProgressBar proBar("发票信息查询中.....");
    proBar.ReFresh();
	
	//发票明细查询
	CInvHead Invhead;
	UINT32 invNum=0;//发票张数
	UINT8 cxfs=0;  //0：按发票号码段查询
	string cxtj(""); // cxfs为0时：12发票代码+8位起始号码+8位终止号码
	cxtj =invCode;
	INT8 tmpChar[16];
    memset((void *)tmpChar,0x00,sizeof(tmpChar));
	sprintf(tmpChar,"%08u",invNO);
	cxtj.append(tmpChar); 
	cxtj.append(tmpChar);
	DBG_PRINT(("cxtj= %s",cxtj.c_str()));

	Ret=g_pAPIBase->GetInvHeadInfo_API(*g_YwXmlArg, cxfs, cxtj, invNum, &Invhead, strErr);
	DBG_PRINT(("Ret= %d",Ret));
	if (Ret !=SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return FAILURE;
	}

	//查看本地是否有该发票信息
	CInvHead invheadtmp;
	INT8 sqlbuf[128];
	memset(sqlbuf, 0x00, sizeof(sqlbuf));
	sprintf(sqlbuf, "where FPDM = '%s' and FPHM = %u", invCode.c_str(), invNO);
	invheadtmp.m_filter = sqlbuf;
	invheadtmp.Requery();
	if (invheadtmp.LoadOneRecord() == SQLITE_OK)
	{
		// 		DBG_PRINT(("本地已有该发票!"));
		//CaMsgBox::ShowMsg("本地已有该发票!");
	   //	return FAILURE; 
      //判断本地数据库的数据类型是否与转换器一致
		DBG_PRINT(("invheadtmp.m_kplx= %u",invheadtmp.m_kplx));
	    DBG_PRINT(("Invhead.m_kplx= %u",Invhead.m_kplx));
      if (invheadtmp.m_kplx != Invhead.m_kplx )
      {
		  strsql = sqlbuf;
		  invheadtmp.m_kplx = Invhead.m_kplx;
		  ret = invheadtmp.Update(strsql, &(invheadtmp.m_kplx), NULL);
		  DBG_PRINT(("ret= %d",ret));
		  if ( ret!= SQLITE_OK)
		  {
			  strErr = "更新发票头信息表错误";
			  return FAILURE;
		  }

		  //更新INV_DET发票类型
		  CInvDet invDet;
		  invDet.m_kplx =invheadtmp.m_kplx;
		  ret = invDet.Update(strsql, &(invDet.m_kplx), NULL);
		  DBG_PRINT(("ret= %d",ret));
		  if ( ret!= SQLITE_OK)
		  {
			  strErr = "更新发票明细信息表错误";
			  return FAILURE;
		  }

      }

	}
	else
	{
		//发票信息保存
		if (SUCCESS != Invhead.Save())
		{
			CaMsgBox::ShowMsg("查询发票存储失败!");
				 return FAILURE;
		  }
		DBG_PRINT(("Invhead.m_kplx= %u",Invhead.m_kplx));
		if(Invhead.m_kplx == RETURN_INV)
		{
			CRtInv rtInv;
		     rtInv.m_fpdm =Invhead.m_yfpdm;
			 rtInv.m_fphm =Invhead.m_yfphm;
			 ret = rtInv.AddNew();				//写已退发票信息表
			 DBG_PRINT(("ret= %d",ret));
			 if ( ret!= SQLITE_OK)
			 {
				 strErr = "查询红票，存储蓝票信息表错误";
				 return FAILURE;
		  }
		
	}
	}

	CaMsgBox::ShowMsg("查询发票成功");

	return SUCCESS;
}

void INVM_ErrMsgBox(UINT8 ret)
{
	string pText("");
	
	switch(ret)
	{
	case INV_SUM_ERROR:
		pText = "卷汇总失败";
		break;
	case PRN_INV_ERROR:
		pText = "开票失败";
		break;
	case RETURN_NUM_ERROR:
		pText = "红票号码不合理";
		break;
	case NO_INV:
		pText = "非本机所开发票";
		break;
	case RT_INV:
		pText = "红票不能被开红票";
		break;
	case WT_INV:
		pText = "废票不能被开红票";
		break;
	case BX_RET_MONTH_ERR:
		pText="本月发票不能开红票";
		break;
	case RM_SUM_EXCEED:
		pText = "红票累计金额超限";
		break;
	case RVM_SUM_EXCEED:
		pText = "单卷红票累计金额超限";
		break;
	case HAVE_RT_INV:
		pText = "已开过红票";
		break;
	case CODE_NO_ERROR:
		pText = "发票代码非法";
		break;
	case NO_PAPER:
		pText = "没有纸质发票";
		break;
	case NO_ISSUED_INV:
		pText = "本卷未开发票不能退回";
		break;
	case DB_FULL:
		pText = "存储空间已满，\n无法开票请尽快申报";
		break;
	case HAVE_ROLLED:
		pText = "信息整理成功";
		break;
	case UPDATE_INV_VOL_ERROR:
		pText = "更新发票领用信息表失败";
		break;
	   case GET_FISCAL_CODE_ERROR:
		   pText = "生成防伪码失败";
		   break;
	   case DB_SAVE_ERROR:
		   pText = "存数据库失败";
		   break;
	   case MALLOC_MEM_ERROR:
		   pText = "申请动态内存失败";
		   break;
	   case INV_LIB_ERR:
		   pText = g_globalArg->m_strMsg;
		   g_globalArg->m_strMsg = "";
		   break; 
	   case INV_ZERO:
		   pText = "无可用发票！";
		   break;
	   case GX_SV_NUM_EXCEED:
		   pText = "离线开票张数超限";
		   break;
	   case GX_SV_SUM_EXCEED:
		   pText = "离线开票金额超限";
		   break;
	   case GX_SV_TIME_EXCEED:
		   pText = "离线开票时间超限";
		   break;
	   case NM_EXCEED_MIN:
		   pText =  "总金额低于最小开票金额";
	   case NO_ISSUED_VOL:
		   pText = "发票未开具,不能开红票";
		   break;
	   case NM_EXCEED:
		   pText = "单张开票金额超限";
		   break;
	   case DIF_PYCODE:
		   pText = "正票与当前卷发票票样不同";
		   break;
	case WASTE_NUM_EXCEED:
		pText = "超过剩余发票份数";
		break;
	case WASTE_NUM_ERROR:
		pText = "作废份数不合理";
		break;
	case MUST_RE_LOGIN:
		pText = "发票卷已用完，\n必须重登录";
		break;
	case PRE_INV_ERROR:
		pText = "查询上笔交易失败";
		break;
	case NO_DIST_INV:
		pText = "当前0卷发票未导入";
		break;
	case QUERY_ERROR:
		pText = "数据库查询失败";
		break;
	   default:
		   pText="其他错误";
		   break;
	}  
	
	CaMsgBox::ShowMsg(pText);
	return ;
}