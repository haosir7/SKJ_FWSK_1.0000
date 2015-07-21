/*! \file  ProgramMainMenu.cpp
\brief    功能编程主界面
\author   xsr
\version  1.0
\date     2010-7-28
*/

#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"

#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "ProgramMainMenu.h"
#include "APIBase.h"


//调用转换器接口，
#if (TYPE_MODE == ZHQ_MODE)
CProgramCtrlMenu::CProgramCtrlMenu():CMultiBtnCommonWin(10, 2)
{
}
#else
CProgramCtrlMenu::CProgramCtrlMenu():CMultiBtnCommonWin(8, 2)
{
}
#endif

int CProgramCtrlMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, PROGRAM_CTRL_MAIN_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);

	SetTitle(1,  "A.商品管理");
	SetTitle(2,  "B.开票员管理");
	SetTitle(3,  "C.客户管理");
	SetTitle(4,  "D.参数编程");
	SetTitle(5,  "E.网络管理");
	SetTitle(6,  "F.数据传输");
	SetTitle(7,  "G.软件升级");
	SetTitle(8,  "H.密码修改");
#if TYPE_MODE == ZHQ_MODE
	SetTitle(9,  "I.转换器设置");
	SetTitle(0,  "J.联机测试");
#endif	
	return 1;
}

//------------------------------------------------------------------------------------
//商品编程，参考4000的PLU编程。所以删掉了原本的DEPT_EDIT_WIN
//------------------------------------------------------------------------------------
void CProgramCtrlMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}
	ChangeWin(DEPT_EDIT_WIN);
}

//开票员管理
void CProgramCtrlMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(OPERATOR_EDIT_WIN);
}

//客户管理
void CProgramCtrlMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(CLIENT_EDIT_WIN);	
}

//参数设置
void CProgramCtrlMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	// 参数编程不对未初始化限制
// 	if (g_globalArg->m_initFlag == 0)
// 	{
// 		CaMsgBox::ShowMsg("机器未初始化");
// 		return ;
// 	}
	m_pFrame->GetWin(SYS_ARG_EDIT_WIN);
	ChangeWin(SYSTEM_ARG_MENU);
}

//网络设置
void CProgramCtrlMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_MANAGE_MENU);
}

//数据传输
void CProgramCtrlMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	//ChangeWin(TEMPLATE_TRANS_MENU);
	ChangeWin(DATA_IO_MENU);
}


//软件升级
void CProgramCtrlMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	ChangeWin(SYSTEM_UPGRADE_WIN);	
}


//修改密码
void CProgramCtrlMenu::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
	{
	ChangeWin(PASSWD_EDIT_WIN);	
}


//转换器参数设置
void CProgramCtrlMenu::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{	

#if TYPE_MODE == ZHQ_MODE
	
	ChangeWin(ZHQ_PARA_SET_MENU);	
#endif

	return ;
}

//联机测试
void CProgramCtrlMenu::OnButton10(int iEvent, unsigned char * pEventData, int iDataLen)
{	
#if TYPE_MODE == ZHQ_MODE
	INT32 ret;
	string strErr = "";
	ret = g_globalArg->OnLineTest(strErr);
	if (SUCCESS == ret)
	{
		CaMsgBox::ShowMsg("联机测试成功");
	}
	else
	{
		CaMsgBox::ShowMsg(strErr);
		DBG_PRINT(("strErr : %s", strErr.c_str()));
		//CaMsgBox::ShowMsg("联机测试失败");
	}
#endif
	return ;
}


int CProgramCtrlMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(SYSTEM_MAIN_MENU);
		return SUCCESS;
	}
	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}
