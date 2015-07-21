/**
 @file  Net3GWlenSetWin.cpp
 @brief 界面文件，3G帐户设置
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "netManager.h"
#include "3GConnection.h"
#include "Net3GWlenSetWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CNet3GWlenSetWin::CNet3GWlenSetWin():CaWindow()
{
	m_strUsrPwd = "";
}

CNet3GWlenSetWin::~CNet3GWlenSetWin()
{
}


int CNet3GWlenSetWin::Create(int iX,int iY,int iW,int iH)
{
	int curH=0;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	int inputW = SCREEN_W - left_offset*2;


	m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,NET_3GWLEN_SET_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	//创建一个Input  第一行
	strcpy(title, "3G账号:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET+4;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(20);
	m_pInput1->m_InputType = m_pInput1->aCHAR; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;

	//创建一个Input  第二行
	strcpy(title, "密  码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H+4;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(20);
	m_pInput2->m_InputType = m_pInput2->aCHAR; //该输入框只接受字符
	m_pInput2->OnObject = S_OnInput2;

	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽

	//创建一个Button  第五行
	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += LINE_H*2;
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

int CNet3GWlenSetWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearWin();
		ChangeWin(NET_3G_WLEN_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CNet3GWlenSetWin::ReFresh()
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


void CNet3GWlenSetWin::S_OnActive(CaWindow *obj)
{
	((CNet3GWlenSetWin *)obj)->DoActive();
}


void CNet3GWlenSetWin::DoActive()
{
	Disp3GInfo();
	ReFresh();
}

void CNet3GWlenSetWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNet3GWlenSetWin *win=(CNet3GWlenSetWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CNet3GWlenSetWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNet3GWlenSetWin *win=(CNet3GWlenSetWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CNet3GWlenSetWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}
void CNet3GWlenSetWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}

void CNet3GWlenSetWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNet3GWlenSetWin *win=(CNet3GWlenSetWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CNet3GWlenSetWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNet3GWlenSetWin *win=(CNet3GWlenSetWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CNet3GWlenSetWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int ret=0;
	string strErr("");
	CaProgressBar proBar("");
	
	string username = (char *)m_pInput1->m_contentBuf;
	string userpwd  = (char *)m_pInput2->m_contentBuf;

	if (g_netManager->GetConnectionMode() != NET_3G)
	{
		CaMsgBox::ShowMsg("当前非3G模式,无法修改");
		return;
	}
	if (g_netManager->checkConnect() != CONNECT_OFF)
	{
		CaMsgBox::ShowMsg("3G连接尚未断开,无法修改");
		return;
	}
	
	proBar.SetText("3G用户信息设置中...");
	proBar.Show();
	if( username.length() <= 0 )
	{
		CaMsgBox::ShowMsg("3G账号不能为空");
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

	CNetConnection *p = g_netManager->GetConnectionHandle();
	ret = p->setUserNamePwd(username, userpwd, strErr);
	if (ret != NET_SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	else
	{
 		CaMsgBox::ShowMsg("3G信息配置成功");
 		ClearWin();
		ChangeWin(NET_3G_WLEN_MENU);
	}

}

void CNet3GWlenSetWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearWin();
	ChangeWin(NET_3G_WLEN_MENU);
}

void CNet3GWlenSetWin::Disp3GInfo()
{
	INT32 ret=0;
	string name(""), pwd("");
	string strErr("");

	ret = CNetDBWork::load3gUserNamePwd(name, pwd, strErr);
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
}

void CNet3GWlenSetWin::ClearWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
}





