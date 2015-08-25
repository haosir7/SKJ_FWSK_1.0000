/**
 @file  ZhqParaSetMenu.cpp
 @brief 界面文件，转换器参数设置主菜单
 @author  lzh
 @date  20150608
*/
#include "APIBase.h"
#include "ZhqParaSetMenu.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CGlobalArg.h"


CZhqParaSetMenu::CZhqParaSetMenu():CMultiBtnCommonWin(4)
{
	m_ComNetpara = &m_Netpara;
}
CZhqParaSetMenu::~CZhqParaSetMenu()
{
}

int CZhqParaSetMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, ZHQ_MANAGE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);

	SetTitle(1,  "A.串口设置");
	SetTitle(2,  "B.联机测试");
	SetTitle(3,  "C.网络设置");
	SetTitle(4,  "D.网络测试");

	return 1;
}

void CZhqParaSetMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (TYPE_MODE != ZHQ_MODE)
	{
		CaMsgBox::ShowMsg("非转换器运行模式");
		return;
	}
	ChangeWin(ZHQ_BTL_SET_WIN);
}

//联机测试
void CZhqParaSetMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{	
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
}

void CZhqParaSetMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		DBG_PRINT(("机器未初始化"));
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}

	INT32 errorcode = 0;
	m_ComNetpara->Requery();
	errorcode = m_ComNetpara->LoadOneRecord();
	DBG_PRINT(("errorcode = %d", errorcode));
	if ((errorcode!=SQLITE_DONE)&&(errorcode!=SQLITE_OK))
	{
		CaMsgBox::ShowMsg("数据库查询错误");
}
	DBG_PRINT(("m_ComNetpara: m_IDDHCP = %u", m_ComNetpara->m_IsDhcp));
	DBG_PRINT(("m_ComNetpara: m_LocalIP = %s", m_ComNetpara->m_LocalIP.c_str()));
	DBG_PRINT(("m_ComNetpara: m_ServIP = %s", m_ComNetpara->m_ServIP.c_str()));

	ChangeWin(ZHQ_IPADDR_SET_WIN);
}

void CZhqParaSetMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		DBG_PRINT(("机器未初始化"));
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}


	BAR_DEF();
	BAR_SHOW("网络连接测试...");

	INT32 ret = SUCCESS;
	string strErr = "";

	ret = g_pAPIBase->LXXXUpdatePro_API(*g_YwXmlArg, strErr);
	if(ret == SUCCESS)
	{
		CaMsgBox::ShowMsg("网络连接测试成功!");	
	}
	else
	{
		CaMsgBox::ShowMsg(strErr);
	}
}


int CZhqParaSetMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);
		return SUCCESS;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}
