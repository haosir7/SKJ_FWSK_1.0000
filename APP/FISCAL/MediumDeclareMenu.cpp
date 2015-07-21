/*! \file    MediumDeclareMenu.cpp
   \brief    介质抄报菜单
   \author   zcy
   \version  1.0
   \date     2015-05-06
 */


#include "MediumDeclareMenu.h"
#include "FiscalFun.h"
#include "InvVolFunc.h" 
#include "CaMsgBox.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CMedDeclMenu::CMedDeclMenu():CMultiBtnCommonWin(2)
{

}

int CMedDeclMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,MEDIUM_DECLARE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.介质抄税");
	//SetTitle(2,"B.非征期抄税");
	SetTitle(2,"B.清零解锁");
	return 1;
}


//介质抄税
void CMedDeclMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	UINT8 ret=SUCCESS;
	UINT8 uJZlx=2;
	string strErr;

// 	ret =  FSC_CheckDeclare(strErr);
// 	if (ret != SUCCESS)
// 	{
// 		CaMsgBox::ShowMsg(strErr);
// 		return ;
// 	}

	ret = INV_MediumType(uJZlx,strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}

  FSC_DiskDeclare(uJZlx);
	
return ;
}

//清零解锁
void CMedDeclMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	FSC_DiskUpdateTax();
return ;
}


int CMedDeclMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(FISCAL_MAIN_MENU);
		return SUCCESS;
	}
	 
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

