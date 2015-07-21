/*! \file    ReportMainMenu.cpp
   \brief    awe4000r报表打印的主菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */


#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "SaleFunc.h"
#include "ReportMainMenu.h"

CReportMainMenu::CReportMainMenu():CMultiBtnCommonWin(4)
{
}

int CReportMainMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,REPORT_MAIN_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.基本信息");
	SetTitle(2,"B.商业统计");
    SetTitle(3,"C.税控统计");
	SetTitle(4,"D.系统日志");

	
	return 1;
}

void CReportMainMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(BASIC_REPORT_MENU);	

}

void CReportMainMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;
	
	if (g_globalArg->m_initFlag == 0)
	{
		strErr = "机器未初始化";
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	ChangeWin(COMMERCE_REPORT_MENU);	
}

void CReportMainMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag != 1)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
   ChangeWin(FISCAL_REPORT_MENU);
}
void CReportMainMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}

	ChangeWin(SYS_LOG_DATE_WIN);		
}


int CReportMainMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(SYSTEM_MAIN_MENU);
		return SUCCESS;
		break;

	case FORWARD_KEY:
			return(ForwardKeyProc());
			break;
		
	case BACKWARD_KEY:
			ret = BackwardPaper();
			if (ret != SUCCESS) 
			{
			   CaMsgBox::ShowMsg("无纸质发票!");
			}
			return SUCCESS;
			break;

	default: break;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}


UINT8 CReportMainMenu::ForwardKeyProc()
{
	UINT8 ret;
	
		ret = NoMarkForwardPaper();
		if (ret != SUCCESS) 
		{
		   CaMsgBox::ShowMsg("无纸质发票!");
			return FAILURE;
		}		

	ReFresh();
	return SUCCESS;
}

