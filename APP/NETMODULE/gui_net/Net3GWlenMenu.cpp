/**
 @file  Net3GWlenMenu.cpp
 @brief 界面文件，3G管理菜单
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"

#include "Net3GWlenMenu.h"


CNet3GWlenMenu::CNet3GWlenMenu():CMultiBtnCommonWin(2)
{
}

int CNet3GWlenMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,NET_3G_WLEN_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.运营商选择");
	SetTitle(2,"B.拨号账户设置");
	
	return 1;
}

void CNet3GWlenMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_WLEN_CARD_WIN);
}

void CNet3GWlenMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_3GWLEN_SET_WIN);
}

int CNet3GWlenMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(NETWORK_SET_MENU);
		return SUCCESS;
		break;	
		
	default: break;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}



