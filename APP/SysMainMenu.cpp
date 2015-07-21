#include "SysMainMenu.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CGlobalArg.h"
#include "TDateTime.h"
#include "SaleData.h"

//////////////////////////////////////////////////////
#include "ManageBusinessFunc.h"
#include "InvBusinessFunc.h"
#include "DeclareBusinessFunc.h"
#include "SaleBusinessFunc.h"
#include "YWXMLGY.h"
//////////////////////////////////////////////////////

CSysMainMenu::CSysMainMenu():CMultiBtnCommonWin(5)
{
}

int CSysMainMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,SYSTEM_MAIN_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.发票开具");
	SetTitle(2,"B.发票管理");
	SetTitle(3,"C.系统管理");
	SetTitle(4,"D.报表打印");
	SetTitle(5,"E.税务管理");
	
	return 1;
}

void CSysMainMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
 	if (g_globalArg->m_operator->m_role!=DEMO_ROLE) 
 	{
 		//判断系统参数，是否进入下一级界面
 		if (g_globalArg->m_initFlag == 0)//&&(g_globalArg->m_operator->m_role!=DEMO_ROLE))
 		{
 			DBG_PRINT(("机器未初始化"));
 			CaMsgBox::ShowMsg("机器未初始化");
 			return ;
 		}
		g_globalArg->m_curInvVol->ResetVol();
	
 	}
 	DBG_PRINT(("---****m_remain = %u", g_globalArg->m_curInvVol->m_remain));

	ChangeWin(PRODUCT_SALE_MAIN_MENU);	
	
}

void CSysMainMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_operator->m_role == DEMO_ROLE) 
	{
		CaMsgBox::ShowMsg("无操作权限!");
		return;
	}
	ChangeWin(INV_MANAGE_MENU);	
}

void CSysMainMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_operator->m_role == DEMO_ROLE) 
	{
		CaMsgBox::ShowMsg("无操作权限!");
		return;
	}
//	if (g_globalArg->m_operator->m_role == NORMAL_ROLE) 
//	{
//		CaMsgBox::ShowMsg("无操作权限!");
//		return;
//	}
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);		
}
void CSysMainMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_operator->m_role == DEMO_ROLE) 
	{
		CaMsgBox::ShowMsg("无操作权限!");
		return;
	}
	if (g_globalArg->m_operator->m_role == NORMAL_ROLE) 
	{
		CaMsgBox::ShowMsg("无操作权限!");
		return;
	}
	ChangeWin(REPORT_MAIN_MENU);
	
	
}
void CSysMainMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_operator->m_role == DEMO_ROLE) 
	{
		CaMsgBox::ShowMsg("无操作权限!");
		return;
	}
	ChangeWin(FISCAL_MAIN_MENU);	
}


int CSysMainMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{

	INT8 chValue[64];
	memset((void *)chValue,0x00,sizeof(chValue));
	UINT32 uServNum;
	string strErr("");

	if (ESC_KEY == iEvent || LOCKED_KEY == iEvent )
	{
		CaMsgBox msgBox("退出当前操作员?",CaMsgBox::MB_YESNO);
		msgBox.ShowBox();	
		if (msgBox.m_iStatus == OK_PRESSED)
		{	
			g_globalArg->m_threadIn = 0;//关闭次线程的待上传发票查找
			ChangeWin(LOGIN_WIN);
			return SUCCESS;
		}
	}

	if(FIND_KEY == iEvent)
	{
		UINT8 ret= GetServNum(uServNum,strErr);
		if ( ret !=SUCCESS)
		{
			CaMsgBox::ShowMsg(strErr);
		}
		else
		{
			DBG_PRINT(("缓存张数 uServNum= %u", uServNum));
			if (uServNum ==0)
			{
				sprintf(chValue, "无待上传发票！");
			}
			else
			{
				sprintf(chValue, " 未上传发票张数: %u", uServNum);
			}
			CaMsgBox::ShowMsg(chValue);
		}

		return SUCCESS;
	}
	

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}


