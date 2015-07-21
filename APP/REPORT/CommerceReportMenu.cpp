/*! \file    CommerceReportMenu.cpp
   \brief    awe4000r报表打印的商业统计菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#include "CommerceReportMenu.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CGlobalArg.h"
#include "ReportFunc.h"
#include "PrintDriver.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

CCommerceReportMenu::CCommerceReportMenu():CMultiBtnCommonWin(3)
{
}

int CCommerceReportMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,COMMERCE_REPORT_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.商品信息");
	SetTitle(2,"B.客户信息");
	SetTitle(3,"C.操作员信息");

	return 1;
}

void CCommerceReportMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
 	string strErr;
	if (g_globalArg->m_initFlag == 0)
	{
		strErr = "机器未初始化";
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	PrnDept();
}

void CCommerceReportMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;

	if (g_globalArg->m_initFlag == 0)
	{
		strErr = "机器未初始化";
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	PrnClient();
}

void CCommerceReportMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;

	if (g_globalArg->m_initFlag == 0)
	{
		strErr = "机器未初始化";
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	PrnOperator();
}


int CCommerceReportMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	 {	
		ChangeWin(REPORT_MAIN_MENU);
		return SUCCESS;
	 }
		
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

UINT8 CCommerceReportMenu::PrnClient()
{
	UINT8 ret;

	//查询是否有客户记录
	CClient client;
	client.Requery();
	INT32 errorcode = client.LoadOneRecord();
	if (errorcode == SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("没有客户信息");
		return SUCCESS;
	}
	else if (errorcode != SQLITE_OK) 
	{
		CaMsgBox::ShowMsg("查询客户信息失败");
		return FAILURE;
	}
	
	//白纸检查
	ret = YesNoMsgBox("请放入白纸，是否打印？");
	if (ret==FAILURE)
	{
		return SUCCESS;//不打印
	}
	if (isPaper() != 0)
	{	
		CaMsgBox::ShowMsg("打印机未检测到纸");
		return FAILURE;
	}

	CaProgressBar proBar("客户信息打印中.....");

	//打印报表头
	PrnClientHead();

	//逐个记录打印明细
	client.Requery();
	errorcode = client.MoveFirst();
	while (errorcode == SQLITE_ROW)
	{
		PrnClientDetail(&client);			    	     
		errorcode = client.MoveNext();
	}
	client.MoveEnd(); 
	ForwardNLine(FORWARD_LINES);

	if (errorcode==SQLITE_DONE) 
	{		
		return SUCCESS;
	}
	DBG_PRINT(("查询客户信息出错，errorcode = %d", errorcode));	
	return FAILURE;	
}

UINT8 CCommerceReportMenu::PrnDept()
{
	UINT8 ret;

	//查询是否有PLU记录
	CDept dept;
	dept.Requery();
	INT32 errorcode = dept.LoadOneRecord();
	if (errorcode == SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("没有商品信息");
		return SUCCESS;
	}
	else if (errorcode != SQLITE_OK) 
	{
		CaMsgBox::ShowMsg("查询商品信息失败");
		return FAILURE;
	}
	
	//白纸检查
	ret = YesNoMsgBox("请放入白纸，是否打印？");
	if (ret==FAILURE)
	{
		return SUCCESS;//不打印
	}
	if (isPaper() != 0)
	{	
		CaMsgBox::ShowMsg("打印机未检测到纸");
		return FAILURE;
	}

	CaProgressBar proBar("商品信息打印中.....");

	//打印报表头
	PrnDeptHead();

	//逐个记录打印明细
	dept.Requery();
	errorcode = dept.MoveFirst();
	while (errorcode == SQLITE_ROW)
	{
		PrnDeptDetail(&dept);			    	     
		errorcode = dept.MoveNext();
	}
	dept.MoveEnd(); 
	ForwardNLine(FORWARD_LINES);

	if (errorcode==SQLITE_DONE) 
	{		
		return SUCCESS;
	}
	DBG_PRINT(("查询商品信息出错，errorcode = %d", errorcode));	
	return FAILURE;	
}

UINT8 CCommerceReportMenu::PrnOperator()
{
	UINT8 ret;

	//查询是否有操作员记录
	COperator oper;
	oper.Requery();
	INT32 errorcode = oper.LoadOneRecord();
	if (errorcode == SQLITE_DONE) 
	{
		CaMsgBox::ShowMsg("没有员工信息");
		return SUCCESS;
	}
	else if (errorcode != SQLITE_OK) 
	{
		CaMsgBox::ShowMsg("查询员工信息失败");
		return FAILURE;
	}
	
	//白纸检查
	ret = YesNoMsgBox("请放入白纸，是否打印？");
	if (ret==FAILURE)
	{
		return SUCCESS;//不打印
	}
	if (isPaper() != 0)
	{	
		CaMsgBox::ShowMsg("打印机未检测到纸");
		return FAILURE;
	}

	CaProgressBar proBar("员工信息打印中.....");

	//打印报表头
	PrnOperatorHead();

	//逐个记录打印明细
	oper.Requery();
	errorcode = oper.MoveFirst();
	while (errorcode == SQLITE_ROW)
	{
		//若非系统操作员，则打印
		if (SYSTEM_ADMINISTRATOR != oper.m_role) 
		{
			PrnOperatorDetail(&oper);	

		}			    	     
		errorcode = oper.MoveNext();
	}
	oper.MoveEnd(); 
	ForwardNLine(FORWARD_LINES);

	if (errorcode==SQLITE_DONE) 
	{		
		return SUCCESS;
	}
	DBG_PRINT(("查询员工信息出错，errorcode = %d", errorcode));	
	return FAILURE;	 
}
