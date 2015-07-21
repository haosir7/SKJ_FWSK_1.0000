/*! \file    BasicReportMenu.cpp
   \brief    awe4000r报表打印的基本信息菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "BasicReportMenu.h"

CBasicReportMenu::CBasicReportMenu():CMultiBtnCommonWin(4)
{
}

int CBasicReportMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,BASIC_REPORT_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.机器信息");
	SetTitle(2,"B.企业信息");
	SetTitle(3,"C.票种信息");
	SetTitle(4,"D.税率信息");
	
	return 1;
}

void CBasicReportMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(MACHINE_INFO_WIN);	
}

void CBasicReportMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	ChangeWin(CORP_INFO_WIN);	
}

void CBasicReportMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;

	if (g_globalArg->m_initFlag == 0)
	{
		strErr = "机器未初始化";
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	ChangeWin(INV_KIND_INFO_WIN);		
}

void CBasicReportMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{

	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	ChangeWin(TAX_INFO_WIN);		
}


int CBasicReportMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	 {	
		ChangeWin(REPORT_MAIN_MENU);
		return SUCCESS;
	 }
		
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}




