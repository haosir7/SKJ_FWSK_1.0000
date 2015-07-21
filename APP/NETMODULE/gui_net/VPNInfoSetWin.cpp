/**
 @file  VPNInfoSetWin.cpp
 @brief 界面文件，VPN帐户设置
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "setToFileProc.h"
#include "netManager.h"
#include "VPNConnection.h"
#include "VPNInfoSetWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CVPNInfoSetWin::CVPNInfoSetWin():CaWindow()
{
	m_strUsrPwd = "";
}

CVPNInfoSetWin::~CVPNInfoSetWin()
{
}


int CVPNInfoSetWin::Create(int iX,int iY,int iW,int iH)
{
	int curH=0;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	int inputW = SCREEN_W - left_offset*2;

	m_iBtnW = (SCREEN_W/2 - 0) ; 
	m_iColW = m_iBtnW + 0;        
  
	m_pFrame->RegsiterWin(this,VPN_INFO_SET_WIN);
	CaWindow::Create(iX,iY,iW,iH); 

	//创建一个Input  第一行
	strcpy(title, "VPN帐号:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET+4;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(50);
	m_pInput1->m_InputType = m_pInput1->aCHAR; 
	m_pInput1->OnObject = S_OnInput1;

	//创建一个Input  第二行
	strcpy(title, "密  码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(20);
	m_pInput2->m_InputType = m_pInput2->aCHAR; //该输入框只接受字符
	m_pInput2->OnObject = S_OnInput2;

	//创建一个Input  第三行
	strcpy(title, "服务器:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(20);
	m_pInput3->m_InputType = m_pInput3->aCHAR; //该输入框只接受字符
	m_pInput3->OnObject = S_OnInput2;

	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽

	//创建一个Button  第五行
	strcpy(title, "确定");
	m_pBtn1 = new CaButton();
	curH += LINE_H+4;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn + m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;

	OnActive = S_OnActive;
	this->End();  
	return 0;
}

int CVPNInfoSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearWin();
		ChangeWin(NETWORK_SET_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CVPNInfoSetWin::ReFresh()
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


void CVPNInfoSetWin::S_OnActive(CaWindow *obj)
{
	((CVPNInfoSetWin *)obj)->DoActive();
}


void CVPNInfoSetWin::DoActive()
{
	DispVPNInfo();
	ReFresh();
}

void CVPNInfoSetWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CVPNInfoSetWin *win=(CVPNInfoSetWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CVPNInfoSetWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CVPNInfoSetWin *win=(CVPNInfoSetWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CVPNInfoSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}
void CVPNInfoSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}

void CVPNInfoSetWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CVPNInfoSetWin *win=(CVPNInfoSetWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CVPNInfoSetWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CVPNInfoSetWin *win=(CVPNInfoSetWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CVPNInfoSetWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int ret=0;
	string strErr("");
	CaProgressBar proBar("");
	
	string username = (char *)m_pInput1->m_contentBuf;
	string userpwd  = (char *)m_pInput2->m_contentBuf;
	string server  = (char *)m_pInput3->m_contentBuf;

	if (g_netManager->GetConnectionModeL2() != NET_VPN)
	{
		CaMsgBox::ShowMsg("未开启VPN模式,无法修改");
		return;
	}
	
	proBar.SetText("VPN信息设置中...");
	proBar.Show();
	if( username.length() <= 0 )
	{
		CaMsgBox::ShowMsg("用户名不能为空");
		m_pInput1->SetFocus();
		this->ReFresh();
		return;
	}
	if( userpwd.length() <= 0 )
	{
		CaMsgBox::ShowMsg("密码不能为空");
		m_pInput2->SetFocus();
		this->ReFresh();
		return;
	}
	if( userpwd == "********" )
	{
		userpwd = m_strUsrPwd;
	}
	if (isIPValid(server.c_str()) != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg("服务器IP输入无效");
		m_pInput3->SetFocus();
		this->ReFresh();
		return;
	}

	VPNConnection *p = (VPNConnection *)g_netManager->GetConnectionHandleL2();
	ret = p->setVPNip(server, strErr);
	if (ret != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	ret = p->setUserNamePwd(username, userpwd, strErr);
	if (ret != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

 	CaMsgBox::ShowMsg("VPN信息配置成功");
 	ClearWin();
 	ChangeWin(NETWORK_SET_MENU);
}

void CVPNInfoSetWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearWin();
	ChangeWin(NETWORK_SET_MENU);
}

void CVPNInfoSetWin::DispVPNInfo()
{
	INT32 ret=0;
	string name(""), pwd(""), server("");
	string strErr("");

	ret = CNetDBWork::loadVPNUserNamePwd(name, pwd, strErr);
	DBG_PRINT(("ret = %d", ret))
	if (ret != SQLITE_OK)
	{
		DBG_PRINT(("ret = %d", ret))
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	m_pInput1->SetContentBuf((UINT8 *)name.c_str(), name.length());
	if (pwd!="")
		m_pInput2->SetContentBuf((UINT8 *)"********", strlen("********"));
	else
		m_pInput2->SetContentBuf((UINT8 *)"", strlen(""));
	m_strUsrPwd = pwd;

	ret = CNetDBWork::loadVPNServer(server, strErr);
	if (ret != SQLITE_OK)
	{
		DBG_PRINT(("ret = %d", ret))
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	m_pInput3->SetContentBuf((UINT8 *)server.c_str(), server.length());
}

void CVPNInfoSetWin::ClearWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
}



