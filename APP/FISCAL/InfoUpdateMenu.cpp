/*! \file    InfoUpdateMenu.cpp
   \brief    信息更新菜单
   \author   zcy
   \version  1.0
   \date     2015-05-06
 */

#include "InfoUpdateMenu.h"
#include "FiscalFun.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CInfoUpdateMenu::CInfoUpdateMenu():CMultiBtnCommonWin(2)
{

}

int CInfoUpdateMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,INFO_UPDATE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.企业信息");
	SetTitle(2,"B.权限信息");

	return 1;
}

//企业信息
void CInfoUpdateMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{


}

//权限信息
void CInfoUpdateMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{	

}


int CInfoUpdateMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(FISCAL_MAIN_MENU);
		return SUCCESS;
	}
	 
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

