/*! \file  InvManageMainMenu.cpp
\brief    awe4000r发票管理的主菜单
\author   zcy
\version  1.0
\date     2015-05-07
*/

#include "InvManageMainMenu.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "InvDownloadNOWin.h"
#include "CGlobalArg.h"
#include "InvVolFunc.h" 


CInvManageMenu::CInvManageMenu():CMultiBtnCommonWin(8,2)
{
}

int CInvManageMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,INV_MANAGE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	
	SetTitle(1,"A.库存查询");
	SetTitle(2,"B.网络购票");
	SetTitle(3,"C.票源读入");
	SetTitle(4,"D.负票开具");
	SetTitle(5,"E.发票作废");
	SetTitle(6,"F.发票存根");
	SetTitle(7,"G.发票查询");
	SetTitle(8,"H.票段管理");
	
	return 1;
}

//------------------------------------------------------------------------------------
//函数名称：库存查询
//函数功能：
//函数作者：zcy
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	
	UINT32 nNum = 0;//发票卷卷数
	string strErr("");
	UINT8 ret=SUCCESS;
	
	CaProgressBar proBar("库存查询中.....");
    proBar.ReFresh();
	
	ret =INV_GetInvVol(nNum,strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	DBG_PRINT(("库存查询ret = %u, nNum = %u", ret, nNum));

	DBG_PRINT(("库存查询成功"));
	
	if (nNum ==0)
	{
		CaMsgBox::ShowMsg("库存无票源");
		return ;
	}
	
	ChangeWin(INV_DIST_QUERY_WIN);
}

//------------------------------------------------------------------------------------
//函数名称：网络购票
//函数功能：
//函数作者：xsr
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg( "机器未初始化");
		return ;
	}
	
	if(g_globalArg->m_corpInfo->m_Kpjhm != 0)
	{
		CaMsgBox::ShowMsg("分机不能网络购票!");
		return;
	}
	
	string strErr("");
	UINT8 ret=SUCCESS;

	ChangeWin(INV_WLLQFP_WIN);

	return;
}


//------------------------------------------------------------------------------------
//票源读入
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg( "机器未初始化");
		return ;
	}

	string strErr("");
	UINT8 ret=SUCCESS;

	UINT8 uJZlx=2;
	ret= INV_MediumType(uJZlx,strErr);
    if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	ret= INV_ReadInvVol(uJZlx,strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	CaMsgBox::ShowMsg( "票源读入成功!");
	return ;
}
//------------------------------------------------------------------------------------
//红票开具
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	
		
	ChangeWin(INV_RET_QUERY_WIN);
}


//------------------------------------------------------------------------------------
//发票作废
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	
	
	ChangeWin(INV_WASTE_MENU);
}

//------------------------------------------------------------------------------------
//发票存根查询
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}

	ChangeWin(INV_STUB_WIN);
	
}


//发票查询
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	
	//ChangeWin(INV_DOWNLOAD_MENU);	
	
	((CInvDownloadNOWin *)m_pFrame->GetWin(INV_DOWNLOAD_NO_WIN))->m_nFlag = 0;
	ChangeWin(INV_DOWNLOAD_NO_WIN);	
}
//------------------------------------------------------------------------------------
//票段管理
//------------------------------------------------------------------------------------
void CInvManageMenu::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	ChangeWin(INV_SEG_MENU);
}

int CInvManageMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(SYSTEM_MAIN_MENU);
		return SUCCESS;
		break;
		// 
		// 		case FORWARD_KEY:
		// 			return(ForwardKeyProc());
		// 			break;
		// 		
		// 		case BACKWARD_KEY:
		// 			ret = BackwardPaper();
		// 			if (ret != SUCCESS) 
		// 			{
		// 			   CaMsgBox::ShowMsg("无纸质发票!");
		// 			}
		// 			return SUCCESS;
		// 			break;
		
	default: break;
	}
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

