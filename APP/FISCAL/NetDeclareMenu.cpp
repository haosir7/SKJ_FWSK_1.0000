/*! \file    NetDeclareMenu.cpp
   \brief    ÍøÂç³­±¨²Ëµ¥
   \author   zcy
   \version  1.0
   \date     2015-05-06
 */



#include "NetDeclareMenu.h"
#include "FiscalFun.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CNetDeclMenu::CNetDeclMenu():CMultiBtnCommonWin(2)
{

}

int CNetDeclMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,NET_DECLARE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.ÍøÂç³­Ë°");
	SetTitle(2,"B.ÍøÂçÇå¿¨");

	return 1;
}

//ÍøÂç³­Ë°
void CNetDeclMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{

	FSC_NetDeclare();

	return ;
}

//ÍøÂçÇå¿¨"
void CNetDeclMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	FSC_NetUpdateTax();
	return ;
}



int CNetDeclMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(FISCAL_MAIN_MENU);
		return SUCCESS;
	}
	 
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

