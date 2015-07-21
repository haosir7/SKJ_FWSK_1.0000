#include "Scanner.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "ClientEditWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CClientEditWin::CClientEditWin():CaWindow()
{	
	m_Client_edit = new CClient;
	m_isClientExist = false;
}

CClientEditWin::~CClientEditWin()
{
	if (m_Client_edit != NULL)
	{
		delete m_Client_edit;
		m_Client_edit = NULL;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
int CClientEditWin::Create(int iX,int iY,int iW,int iH)
{
    int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/3 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,CLIENT_EDIT_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "编号:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(3);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受数字
	m_pInput1->OnObject = S_OnInput1;

	//创建一个Input  第二行
	strcpy(title, "名称:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->m_InputType = m_pInput2->aCHAR; //该输入框只接受字符
    m_pInput2->SetMaxLen(PAYER_NAME_LEN);
	m_pInput2->OnObject = S_OnInput2;

	//创建一个Input  第三行
	strcpy(title, "税号:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->m_InputType = m_pInput3->aINT; //该输入框只接受数字
    m_pInput3->SetMaxLen(TAX_CODE_LEN);
	m_pInput3->OnObject = S_OnInput3;
	
	m_iBtnW = (SCREEN_W - 40)/3 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/3; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	

	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	curH += LINE_H;
	m_pBtn1->Create(leftoffset_btn, curH, m_iBtnW, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	strcpy(title, "删除");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW, curH, m_iBtnW, WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;	

	strcpy(title, "返回");
	m_pBtn3 = new CaButton();
	m_pBtn3->Create(leftoffset_btn+m_iColW*2, curH, m_iBtnW, WORD_H); 
	m_pBtn3->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn3->SetTitle(title, strlen(title));
	m_pBtn3->BoxType(CaObject::BOX_NO);
	m_pBtn3->OnObject = S_OnButton3;		

	OnActive = S_OnActive;
	this->End();
	return 0;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
int CClientEditWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearClientWin();
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CClientEditWin::IsCurWin()
{
	return true;
}

int CClientEditWin::ReFresh()
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


void CClientEditWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CClientEditWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}
void CClientEditWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CClientEditWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CClientEditWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}
void CClientEditWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CClientEditWin *win=(CClientEditWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CClientEditWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInput2->SetFocus();
}

void CClientEditWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInput3->SetFocus();
}

void CClientEditWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pBtn1->SetFocus();
}

void CClientEditWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int errcode;
	string strErr;
	char sqlbuf[512];
	string sqlstr;	
	
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	m_Client_edit->m_khbh = (char *)m_pInput1->m_contentBuf;
	m_Client_edit->m_khmc = (char *)m_pInput2->m_contentBuf;
	m_Client_edit->m_khsh = (char *)m_pInput3->m_contentBuf;
	
	if (m_isClientExist == false)
	{
		errcode = m_Client_edit->AddNew();
		DBG_PRINT(("errorcode=%d", errcode));
		if (errcode == SQLITE_OK)
		{
			CaMsgBox::ShowMsg("客户信息添加成功");
		}
		else
		{
			CaMsgBox::ShowMsg("客户信息添加错误！");
		}
	}
	else
	{
//		sprintf(sqlbuf, "update CLIENT set KH_DM = '%s', KH_JM = '%s', KH_MC = '%s' where KH_BH = '%s'", 
//					m_Client_edit->m_khdm.c_str(), m_Client_edit->m_khjm.c_str(), 
//					m_Client_edit->m_khmc.c_str(), m_Client_edit->m_khbh.c_str());

		sprintf(sqlbuf, "update CLIENT set KH_MC = '%s', KH_SH = '%s' where KH_BH = '%s'", 
					m_Client_edit->m_khmc.c_str(), m_Client_edit->m_khsh.c_str(), m_Client_edit->m_khbh.c_str());

		sqlstr=sqlbuf;
		m_Client_edit->SetSQL(sqlstr);
		errcode = m_Client_edit->ExecuteSQL();
		if (errcode != SQLITE_OK)
		{
			strErr = "客户信息修改错误！";			
			return ;
		}		

		CaMsgBox::ShowMsg("客户信息修改成功");
	}

	ClearClientWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}

void CClientEditWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	INT8 tmpstr[32];
	INT32 res;

	if (m_isClientExist == true)
	{
		m_Client_edit->m_khbh = (char *)m_pInput1->m_contentBuf;
		sprintf(tmpstr, "WHERE KH_BH = '%s'", m_Client_edit->m_khbh.c_str());
		m_Client_edit->m_filter.append(tmpstr);
		res = m_Client_edit->Delete();
		if (res == SQLITE_OK)
		{
			CaMsgBox::ShowMsg("删除成功");
		}
		else
		{
			CaMsgBox::ShowMsg("删除失败");
		}
	}
	else
	{
		CaMsgBox::ShowMsg("尚无此客户的信息");
	}
	
	ClearClientWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}

void CClientEditWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearClientWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}


UINT8 CClientEditWin::CheckInputValid(string &strErr)
{	
	//客户编码是否输入正确
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入客户编码";
		m_pInput1->SetFocus();
		return FAILURE;
	}

	//客户名称是否输入正确
	if (m_pInput2->IsEmpty())
	{	
		strErr = "未输入客户名称";
		m_pInput2->SetFocus();
		return FAILURE;
	}

	return SUCCESS;
}

void CClientEditWin::DispClientInfo(CClient *oper)
{
	
	m_pInput1->SetContentBuf((UINT8 *)oper->m_khbh.c_str(), oper->m_khbh.length());
	m_pInput2->SetContentBuf((UINT8 *)oper->m_khmc.c_str(), oper->m_khmc.length());
	m_pInput3->SetContentBuf((UINT8 *)oper->m_khsh.c_str(), oper->m_khsh.length());
	
	m_pInput2->SetFocus();
}

//
void CClientEditWin::ClearClientWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput2->SetFocus();
}

void CClientEditWin::S_OnActive(CaWindow *obj)
{
	((CClientEditWin *)obj)->DoActive();
}

void CClientEditWin::DoActive()
{
	INT8 editFlag = 0;
	UINT8 errorcode=0;
	INT32 status=0;
	INT8 value[32];
	string ClientNo("");
	INT8 nLen  = 0;
	string code = "";

	DBG_ENTER("CClientEditWin::DoActive()");
	ReFresh();

	CaMsgBox msgBox("客户编号:",CaMsgBox::MB_INPUT);
	msgBox.SetInputType(CaInput::aINT);
	msgBox.SetMaxInputLen(3);

	while (1)
	{
		code = "";
		msgBox.ShowBox();
		status = msgBox.m_iStatus;
		ClientNo = (INT8 *)msgBox.GetInputContent();
		DBG_PRINT(("status=%d, ClientNo=%s", status, ClientNo.c_str()));
		if (status != OK_PRESSED || ClientNo != "")//能进入这里一定按了OK_PRESSED键
		{
	if (status == OK_PRESSED)
	{
		nLen = CLENT_CODE_LEN - ClientNo.length();
		if (nLen > 0)//不足3位，则前补零到3位
		{
			while(nLen > 0)
			{
				code += "0";
				nLen--;
			}
		}	
		code += ClientNo;
				
				if( code == DErrorCode() )
				{
					CaMsgBox::ShowMsg("客户编码不合法");
					this->ReFresh();
					continue;//到这里说明先按了一个不是OK_PRESSED的键，最后按了OK_PRESSED键，导致deptNo非空，但如果code无效，需继续输入
				}
			}
			break;
		}
		CaMsgBox::ShowMsg("请输入客户编码");//只按了OK_PRESSED键
	}

	if (status == OK_PRESSED)
	{
		ClientNo = code;

		sprintf(value, "WHERE KH_BH = '%s'", ClientNo.c_str());
		m_Client_edit->m_filter.append(value);
		m_Client_edit->Requery();
		errorcode = m_Client_edit->LoadOneRecord();
		DBG_PRINT(("errorcode = %u", errorcode));
		
		switch(errorcode)
		{
		case SQLITE_DONE:
			{
				CaMsgBox msgBox("无此客户信息,是否添加?", CaMsgBox::MB_YESNO);
				msgBox.ShowBox();
				if (msgBox.m_iStatus == OK_PRESSED)
				{
					m_pInput1->SetContentBuf((UINT8 *)ClientNo.c_str(), ClientNo.length());
					m_isClientExist = false;
					editFlag = 1;
				}
				else
				{
					editFlag = 0;
				}
			}
			break;
			
		case SQLITE_OK:
			DispClientInfo(m_Client_edit);
			m_isClientExist = true;
			editFlag = 1;
			break;

		default:
			CaMsgBox::ShowMsg("客户信息表错误");
			editFlag = 0;
			break;
		}
	}
	else
	{
		editFlag = 0;
	}
	
	
	if (editFlag == 1)
	{
		m_pInput2->SetFocus();
		m_pInput1->m_bReadOnly = true;
		ReFresh(); 
	}
	else
	{
		m_pInput1->m_bReadOnly = false;
		ClearClientWin();
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);
	}
}

string CClientEditWin::DErrorCode()
{
	string errorcode("");
	for (UINT32 tempi=1; tempi<=CLENT_CODE_LEN; tempi++)
	{
		errorcode +="0";
	}
	return errorcode;
}