
#include "InvWasteMenu.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CGlobalArg.h"

CInvWasteMenu::CInvWasteMenu():CMultiBtnCommonWin(2)
{
}

int CInvWasteMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,INV_WASTE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.空白票作废");
	SetTitle(2,"B.已开票作废");
	
	return 1;
}

//------------------------------------------------------------------------------------
//函数名称：空白票作废
//函数功能：
//函数作者：
//------------------------------------------------------------------------------------
void CInvWasteMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{

	if(g_globalArg->m_curInvVol->m_remain==0)
	{
	//	CaMsgBox::ShowMsg("请先导入发票卷！");
	//	return ;
	}
	
	ChangeWin(INV_WASTE_WIN);	
}

//------------------------------------------------------------------------------------
//函数名称：已开票作废
//函数功能：
//函数作者：
//------------------------------------------------------------------------------------
void CInvWasteMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
// 	if ((g_globalArg->m_InvServNum >= 1)&&
// 		(g_globalArg->m_operator->m_role!=DEMO_ROLE)) 
// 	{
// 		CaMsgBox::ShowMsg("网络连接失败，禁止开票");
// 		return;		
//  	}

	ChangeWin(INV_WST_QUERY_WIN);
}

//------------------------------------------------------------------------------------
//函数名称：红票作废
//函数功能：
//函数作者：
//------------------------------------------------------------------------------------
// void CInvWasteMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
// {
// }

int CInvWasteMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
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

