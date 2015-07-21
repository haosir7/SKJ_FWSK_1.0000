/*! \file   InvSegMenu.cpp
\brief    awe4000r发票管理下其它功能的主菜单
\author   Yu Yan
\version  1.0
\date     2010-12-26
*/

#include "InvSegMenu.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "InvVolFunc.h" 

CInvSegMenu::CInvSegMenu():CMultiBtnCommonWin(3)
{
}

int CInvSegMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,INV_SEG_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.发票分发");  
	SetTitle(2,"B.发票退回");
	SetTitle(3,"C.发票收回");  

	return 1;
}

void CInvSegMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (0 != g_globalArg->m_corpInfo->m_Kpjhm)
	{
		CaMsgBox::ShowMsg("非主机，不能分发发票");
		return ;
	}
	ChangeWin(HOST_TO_INV_WIN);

	return ;
}

void CInvSegMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
// 	if (0 == g_globalArg->m_corpInfo->m_Kpjhm)
// 	{
//  		CaMsgBox::ShowMsg("非分机，不能退回发票");
// 		return ;
// 	}
	ChangeWin(RET_TO_INV_WIN);
	return ;
}
void CInvSegMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");
	UINT8 ret=SUCCESS;

	if (0 != g_globalArg->m_corpInfo->m_Kpjhm)
	{
		CaMsgBox::ShowMsg("非主机，不能收回发票");
		return ;
	}

    UINT8 uJZlx=2;//金税盘介质
	
	ret= INV_MediumType(uJZlx,strErr);
    if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

	CaMsgBox::ShowMsg("请保持主分税控设备同时连接款机");

	CaProgressBar proBar("");
	proBar.SetText("发票收回中...");
    proBar.Show();
	
	ret = INV_HostGetInvVol(uJZlx, strErr);

	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
        CaMsgBox::ShowMsg("发票收回成功");
	return ;
}



int CInvSegMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(INV_MANAGE_MENU);
		return SUCCESS;
		break;
	default: break;
	}
	
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}



