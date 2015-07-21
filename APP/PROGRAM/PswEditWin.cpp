#include "SysMacDef.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "PswEditWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CPswEditWin::CPswEditWin():CaWindow()
{
}

CPswEditWin::~CPswEditWin()
{
}

int CPswEditWin::Create(int iX,int iY,int iW,int iH)
{
	int curH, left_offset;
	int inputW, leftoffset_btn;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	m_pFrame->RegsiterWin(this, PASSWD_EDIT_WIN);    
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	curH = SCREEN_TOP_OFFSET;
	left_offset = SCREEN_LEFT_OFFSET+8;
	inputW = SCREEN_W - left_offset*2;

	strcpy(title, "新 密 码:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput1=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput1->SetTitle(title, titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(6);
	m_pInput1->m_InputType = CaInput::aPASSWORD;
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "密码确认:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput2->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput2->SetTitle(title, titleLen / CHAR_W);	
 	m_pInput2->SetMaxLen(6);
	m_pInput2->m_InputType = CaInput::aPASSWORD;
	m_pInput2->OnObject = S_OnInput2;
	
 	m_iBtnW = (SCREEN_W - 40)/2 - 4; 
 	m_iColW = (SCREEN_W - 40)/2; 
	leftoffset_btn = left_offset + 14;

	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += LINE_H+5;
	m_pBtn1->Create(leftoffset_btn, curH, m_iBtnW, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW, curH, m_iBtnW, WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;	

	OnActive = S_OnActive;
	
	this->End();
	return 0;
}

int CPswEditWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearPswEditWin();
		BacktoUpperWin();
		return SUCCESS;
		break;		
	default: 
		break;		
	}

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CPswEditWin::IsCurWin()
{
	return true;
}

int CPswEditWin::ReFresh()
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


void CPswEditWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CPswEditWin *win=(CPswEditWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CPswEditWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CPswEditWin *win=(CPswEditWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}
void CPswEditWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CPswEditWin *win=(CPswEditWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CPswEditWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CPswEditWin *win=(CPswEditWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CPswEditWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInput2->SetFocus();
}

void CPswEditWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pBtn1->SetFocus();
}

void CPswEditWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int errcode;
	string strErr;
	string sqlstr;	
	char sqlbuf[128];
	
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	g_globalArg->m_operator->m_passwd = (char *)(m_pInput1->m_contentBuf);

	sprintf(sqlbuf, "update OPERATOR set PASSWD = '%s' where OP_ID = '%u'", 
				g_globalArg->m_operator->m_passwd.c_str(), 
				g_globalArg->m_operator->m_opid );
	sqlstr=sqlbuf;
	g_globalArg->m_operator->SetSQL(sqlstr);
	errcode = g_globalArg->m_operator->ExecuteSQL();
	if (errcode != SQLITE_OK)
	{
		strErr = "密码修改错误！";			
		return ;
	}		

	CaMsgBox::ShowMsg("密码修改成功");

	ClearPswEditWin();
	BacktoUpperWin();
}

void CPswEditWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearPswEditWin();
	BacktoUpperWin();
}

void CPswEditWin::ClearPswEditWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
}

void CPswEditWin::BacktoUpperWin()
{
	if (g_globalArg->m_operator->m_opid != SYSTEM_OPERATOR_ID)
	{
		ChangeWin(INV_MANAGE_MENU);
	}
	else
	{
		ChangeWin(MAINTENANCE_MENU);
	}
}

UINT8 CPswEditWin::CheckInputValid(string &strErr)
{	
	//密码长度是否正确
	if (m_pInput1->m_iConLen != 6)
	{	
		strErr = "密码长度不到6位!";
		m_pInput1->SetFocus();
		return FAILURE;
	}

	//两次密码是否一致
	if (memcmp(m_pInput1->m_contentBuf, m_pInput2->m_contentBuf, 6) != 0)
	{	
		strErr = "两次密码输入不一致!";
		m_pInput1->SetFocus();
		return FAILURE;
	}
	
	return SUCCESS;
}

void CPswEditWin::S_OnActive(CaWindow *obj)
{
	((CPswEditWin *)obj)->DoActive();
}

void CPswEditWin::DoActive()
{
	INT32 status;
	string passwd("");
	int nCount = 0;

	ReFresh();

	CaMsgBox msgBox("输入旧密码:",CaMsgBox::MB_INPUT);
	msgBox.SetInputType(CaInput::aPASSWORD);
	msgBox.SetMaxInputLen(6);
	while (1)
	{
		msgBox.ShowBox();
		status = msgBox.m_iStatus;
		passwd = (INT8 *)msgBox.GetInputContent();
// 		if (status != OK_PRESSED || passwd.length() != 0)
// 			break;

		if( status == OK_PRESSED && passwd.length() == 0 )
		{
			CaMsgBox::ShowMsg("请输入旧密码");
			continue;
		}
		
		if (status == OK_PRESSED)
		{
			if (passwd == g_globalArg->m_operator->m_passwd)
			{
				m_pInput1->SetFocus();
				ReFresh();
				return;
			}		
			else	
			{
				CaMsgBox::ShowMsg("旧密码输入不正确");
				if( nCount++ >= 3 )
					break;
			}
		}
		else
			break;
	}
	
	ClearPswEditWin();
	BacktoUpperWin();
	return;
}
