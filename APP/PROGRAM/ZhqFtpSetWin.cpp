/**
 @file  FTPServerSetWin.cpp
 @brief 界面文件，FTP服务器设置
*/
#include "CMainFrame.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CaProgressBar.h"

#include "ZhqFtpSetWin.h"
#include "setToFileProc.h"
#include "ZhqParaSetMenu.h"
#include "APIBase.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CZhqFtpSetWin::CZhqFtpSetWin():CMultiInputCommonWin(4)
{
	m_iNumOfInput = 4;
	m_iNumOfBtn = 2;
}

CZhqFtpSetWin::~CZhqFtpSetWin()
{

}

int CZhqFtpSetWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, ZHQ_FTP_SET_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH); 

	SetObjPara(1, "FTP地址：",	CaInput::aCHAR,15);
	SetObjPara(2, "端 口 号：",	CaInput::aCHAR,6);
	SetObjPara(3, "用 户 名：",	CaInput::aCHAR,16);
	SetObjPara(4, "密    码：",		CaInput::aPASSWORD,16);

	SetObjPara(10," 确认 ");
	SetObjPara(11," 上一步 ");

	OnActive = S_OnActive;
	this->ReFresh();

	return 0;
}

int CZhqFtpSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		{
			CaMsgBox msgBox("确定放弃转换器网络设置?",CaMsgBox::MB_YESNO);
			msgBox.ShowBox();
			if (msgBox.m_iStatus == OK_PRESSED)
			{
				ClearFTPWin();
				ChangeWin(ZHQ_PARA_SET_MENU);						
				return SUCCESS;
			}								
		}
		break;		
	default: 
		break;		
	}
   return CMultiInputCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

void CZhqFtpSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
}

void CZhqFtpSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CZhqFtpSetWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput4);
	ReFresh(); 
}

void CZhqFtpSetWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}

int CZhqFtpSetWin::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();

	return 1;
}

void CZhqFtpSetWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr("");
        INT32 nRet=0;
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	if (changeFTPInfoProc(strErr) != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return ;
	}
	
//	CaMsgBox::ShowMsg("设置FTP服务器成功");
	
		//调用转换器接口，更改转换器网络设置
#if TYPE_MODE == ZHQ_MODE

	CaProgressBar proBar("网络设置中...");	
	proBar.Show();

	 nRet = g_pAPIBase->NetParaManage_API(m_netpara, strErr);

	DBG_PRINT(("SetIPpara: nRet = %d",nRet));
	if (nRet != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
#endif

	DBG_PRINT(("m_netpara: m_IDDHCP = %u", m_netpara->m_IsDhcp));
	DBG_PRINT(("m_netpara: m_LocalIP = %s", m_netpara->m_LocalIP.c_str()));
	DBG_PRINT(("m_netpara: m_LocalMask = %s", m_netpara->m_LocalMask.c_str()));
	DBG_PRINT(("m_netpara: m_LocalGate = %s", m_netpara->m_LocalGate.c_str()));
	DBG_PRINT(("m_netpara: m_LocalDNS = %s", m_netpara->m_LocalDNS.c_str()));
	DBG_PRINT(("m_netpara: m_ServIP = %s", m_netpara->m_ServIP.c_str()));
	DBG_PRINT(("m_netpara: m_ServPort = %s", m_netpara->m_ServPort.c_str()));
	DBG_PRINT(("m_netpara: m_ServAddr = %s", m_netpara->m_ServAddr.c_str()));
	DBG_PRINT(("m_netpara: m_FtpIP = %s", m_netpara->m_FtpIP.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPort = %s", m_netpara->m_FtpPort.c_str()));
	DBG_PRINT(("m_netpara: m_FtpUser = %s", m_netpara->m_FtpUser.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPwd = %s", m_netpara->m_FtpPwd.c_str()));
	INT8 chvalue[128];
	memset((void*)chvalue, 0x00, sizeof(chvalue));
	sprintf(chvalue, "where 1");
	string strsql = chvalue;
	//更新转换器网络配置表
	nRet = m_netpara->Update(strsql, &(m_netpara->m_IsDhcp), &(m_netpara->m_LocalIP), &(m_netpara->m_LocalMask), &(m_netpara->m_LocalGate), \
		&(m_netpara->m_LocalDNS), &(m_netpara->m_ServIP), &(m_netpara->m_ServPort), &(m_netpara->m_ServAddr),\
		&(m_netpara->m_FtpIP), &(m_netpara->m_FtpPort), &(m_netpara->m_FtpUser), &(m_netpara->m_FtpPwd), NULL);

	if ( nRet!= SQLITE_OK)
	{
		DBG_PRINT(("m_netpara: update_nRet = %d", nRet));
		CaMsgBox::ShowMsg("更新数据库失败");
		return;
	}
	DBG_PRINT(("NetPara表更新成功"));
	CaMsgBox::ShowMsg("转换器网络设置成功");

	ChangeWin(ZHQ_PARA_SET_MENU);
}

void CZhqFtpSetWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearFTPWin();
	ChangeWin(ZHQ_SERVERIP_SET_WIN);
}

UINT8 CZhqFtpSetWin::CheckInputValid(string &strErr)
{	
	if (!(m_pInput1->IsEmpty()))
	{	
		DBG_PRINT(("FTP_IP : %s",m_pInput1->m_contentBuf));

		if (isIPValid((INT8 *)m_pInput1->m_contentBuf) != NET_SUCCESS)
		{	
			DBG_PRINT(("FTP_IP : %s",m_pInput1->m_contentBuf));
			strErr = "FTP服务器地址输入无效";
			m_pInput1->SetFocus();
			return FAILURE;
		}
	
		if (m_pInput2->IsEmpty())
		{	
			strErr = "未输入端口号";
			m_pInput2->SetFocus();
			return FAILURE;
		}
	
		if (m_pInput3->IsEmpty())
		{	
				strErr = "未输入用户名";
			m_pInput3->SetFocus();
			return FAILURE;
		}
	
		if (m_pInput4->IsEmpty())
		{	
			strErr = "未输入密码";
			m_pInput4->SetFocus();
			return FAILURE;
		}
	}
	return SUCCESS;
}


void CZhqFtpSetWin::DispFTPInfo()
{	
	m_pInput1->SetContentBuf((UINT8 *)m_netpara->m_FtpIP.c_str(), m_netpara->m_FtpIP.length());
	m_pInput2->SetContentBuf((UINT8 *)m_netpara->m_FtpPort.c_str(), m_netpara->m_FtpPort.length());
	m_pInput3->SetContentBuf((UINT8 *)m_netpara->m_FtpUser.c_str(), m_netpara->m_FtpUser.length());
	m_pInput4->SetContentBuf((UINT8 *)m_netpara->m_FtpPwd.c_str(), m_netpara->m_FtpPwd.length());

	DBG_PRINT(("m_netpara: m_FtpIP = %s", m_netpara->m_FtpIP.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPort = %s", m_netpara->m_FtpPort.c_str()));
	DBG_PRINT(("m_netpara: m_FtpUser = %s", m_netpara->m_FtpUser.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPwd = %s", m_netpara->m_FtpPwd.c_str()));

}

void CZhqFtpSetWin::ClearFTPWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput4->Clear();
}

int CZhqFtpSetWin::changeFTPInfoProc(string &strErr)
{
	int ret = SUCCESS;
	m_netpara->m_FtpIP = (INT8 *)m_pInput1->m_contentBuf;
	m_netpara->m_FtpPort = (INT8 *)m_pInput2->m_contentBuf;
	m_netpara->m_FtpUser = (INT8 *)m_pInput3->m_contentBuf;
	m_netpara->m_FtpPwd = (INT8 *)m_pInput4->m_contentBuf;

	DBG_PRINT(("m_netpara: m_FtpIP = %s", m_netpara->m_FtpIP.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPort = %s", m_netpara->m_FtpPort.c_str()));
	DBG_PRINT(("m_netpara: m_FtpUser = %s", m_netpara->m_FtpUser.c_str()));
	DBG_PRINT(("m_netpara: m_FtpPwd = %s", m_netpara->m_FtpPwd.c_str()));


	//更新数据库

	return ret;
}

void CZhqFtpSetWin::S_OnActive(CaWindow *obj)
{
	((CZhqFtpSetWin *)obj)->DoActive();
}

void CZhqFtpSetWin::DoActive()
{		
	m_netpara =  ((CZhqParaSetMenu *)m_pFrame->GetWin(ZHQ_PARA_SET_MENU))->m_ComNetpara;
	DispFTPInfo();
	this->ReFresh();

	return;
}
