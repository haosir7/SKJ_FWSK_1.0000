
#include "InvDownloadMenu.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CGlobalArg.h"

CInvDownloadMenu::CInvDownloadMenu():CMultiBtnCommonWin(2)
{
}

int CInvDownloadMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,INV_DOWNLOAD_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.号码段查询");
	SetTitle(2,"B.时间段查询");
	
	return 1;
}

//------------------------------------------------------------------------------------
//函数名称：号码段查询
//函数功能：
//函数作者：
//------------------------------------------------------------------------------------
void CInvDownloadMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{

	if(g_globalArg->m_curInvVol->m_remain==0)
	{
	//	CaMsgBox::ShowMsg("请先导入发票卷！");
	//	return ;
	}
	
	ChangeWin(INV_DOWNLOAD_NO_WIN);	
}

//------------------------------------------------------------------------------------
//函数名称：时间段查询
//函数功能：
//函数作者：
//------------------------------------------------------------------------------------
void CInvDownloadMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
// 	if ((g_globalArg->m_InvServNum >= 1)&&
// 		(g_globalArg->m_operator->m_role!=DEMO_ROLE)) 
// 	{
// 		CaMsgBox::ShowMsg("网络连接失败，禁止开票");
// 		return;		
//  	}

	CaMsgBox::ShowMsg("暂不支持,此功能");
	return;
	ChangeWin(INV_DOWNLOAD_DATE_WIN);
}


int CInvDownloadMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
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

