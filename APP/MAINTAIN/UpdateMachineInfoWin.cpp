#include "SysMacDef.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaInput.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "UpdateMachineInfoWin.h"
#include "TDateTime.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CUpdateMachindInfoWin::CUpdateMachindInfoWin():CaWindow()
{
}

CUpdateMachindInfoWin::~CUpdateMachindInfoWin()
{
}

int CUpdateMachindInfoWin::Create(int iX,int iY,int iW,int iH)
{
	int curH, left_offset;
	int inputW, leftoffset_btn;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	m_pFrame->RegsiterWin(this, UPDATE_MACHINE_INFO_WIN);    
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	curH = SCREEN_TOP_OFFSET+4;
	left_offset = SCREEN_LEFT_OFFSET;
	inputW = SCREEN_W - left_offset*2;

	strcpy(title, "软件版本:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput1=new CaInput(CaObject::ON_LEFT, titleLen);
	m_pInput1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput1->SetTitle(title, titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(8);
	m_pInput1->m_InputType = CaInput::aCHAR;
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "硬件版本:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput2->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput2->SetTitle(title, titleLen / CHAR_W);	
 	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = CaInput::aCHAR;
	m_pInput2->OnObject = S_OnInput2;
	
	strcpy(title, "出厂日期:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput3=new CaInput(CaObject::ON_LEFT, titleLen);
	curH += LINE_H;
	m_pInput3->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput3->SetTitle(title, titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(10);
	m_pInput3->m_InputType = CaInput::aCHAR;
	m_pInput3->OnObject = S_OnInput3;
	
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

int CUpdateMachindInfoWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ChangeWin(SELF_TEST_MENU);
		return SUCCESS;
		break;		
	default: 
		break;		
	}

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CUpdateMachindInfoWin::IsCurWin()
{
	return true;
}

int CUpdateMachindInfoWin::ReFresh()
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


void CUpdateMachindInfoWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CUpdateMachindInfoWin *win=(CUpdateMachindInfoWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CUpdateMachindInfoWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CUpdateMachindInfoWin *win=(CUpdateMachindInfoWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}
void CUpdateMachindInfoWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CUpdateMachindInfoWin *win=(CUpdateMachindInfoWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}
void CUpdateMachindInfoWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CUpdateMachindInfoWin *win=(CUpdateMachindInfoWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CUpdateMachindInfoWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CUpdateMachindInfoWin *win=(CUpdateMachindInfoWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CUpdateMachindInfoWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInput2->SetFocus();
}

void CUpdateMachindInfoWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInput3->SetFocus();
}
void CUpdateMachindInfoWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pBtn1->SetFocus();
}

void CUpdateMachindInfoWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;
	string pDate = (char*)(m_pInput3->m_contentBuf);

	if(pDate.length() == 0)
	{
		CaMsgBox::ShowMsg("日期不能为空");
		m_pInput3->SetFocus();
		return;
	}
	if(pDate.length() != 8)
	{
		CaMsgBox::ShowMsg("日期必须是8位数");
		m_pInput3->SetFocus();
		return;
	}
	TDateTime datetime(pDate.c_str());
	if (0 == datetime.DateIsValid())
	{
		CaMsgBox::ShowMsg("出厂日期无效");
		m_pInput3->SetFocus();
		return;
	}

    string strMachine = "INSERT INTO machine values('";
	strMachine += g_globalArg->m_machine->m_machineNo;
	strMachine += "', '";
	strMachine += pDate;
	strMachine += "', '";
	strMachine += g_globalArg->m_machine->m_hVer;
	strMachine += "', '";
	strMachine += g_globalArg->m_machine->m_sVer;
	strMachine += "');";
	DBG_PRINT((" strMachine = %s", strMachine.c_str()));
	FILE *fp1; 
	if( (fp1=fopen("machine.sql","w"))==NULL ) 
	{ 
		fclose(fp1); 
		strErr = "打开machine.sql文件错误！";
		DBG_PRINT((" 打开machine.sql文件错误"));
		CaMsgBox::ShowMsg(strErr);
		return;
	} 
	INT32 nRealLen = strMachine.length();
	INT32 nLen = 0;
	nLen = fwrite(strMachine.c_str(),1, nRealLen,fp1); 
	if (nLen!=nRealLen) 
	{
		fclose(fp1); 
		strErr = "写machine.sql文件错误！";
		DBG_PRINT((" 写machine.sql文件错误"));
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	fclose(fp1);

	char sqlbuf[128];
	int errcode;
	CMachine mach;
	memset(sqlbuf, 0, sizeof(sqlbuf));
	sprintf(sqlbuf, "update machine set P_DATE = '%s'", pDate.c_str());
	DBG_PRINT(("sqlbuf = %s", sqlbuf));
	mach.SetSQL(sqlbuf);
	errcode = mach.ExecuteSQL();
	if( errcode != SQLITE_OK )
	{
		CaMsgBox::ShowMsg("更新出厂信息错误");
		return;
	}

	g_globalArg->m_machine->m_pDate = pDate;

	CaMsgBox::ShowMsg("更新出厂信息成功");
}

void CUpdateMachindInfoWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(SELF_TEST_MENU);
}


UINT8 CUpdateMachindInfoWin::CheckInputValid(string &strErr)
{	
	return SUCCESS;
}

void CUpdateMachindInfoWin::S_OnActive(CaWindow *obj)
{
	((CUpdateMachindInfoWin *)obj)->DoActive();
}

void CUpdateMachindInfoWin::DoActive()
{
	m_pInput1->SetContentBuf((UINT8 *)g_globalArg->m_machine->m_sVer.c_str(), g_globalArg->m_machine->m_sVer.length());
	m_pInput2->SetContentBuf((UINT8 *)g_globalArg->m_machine->m_hVer.c_str(), g_globalArg->m_machine->m_hVer.length());
	m_pInput3->SetContentBuf((UINT8 *)g_globalArg->m_machine->m_pDate.c_str(), g_globalArg->m_machine->m_pDate.length());
	m_pInput1->m_bReadOnly = true;
	m_pInput2->m_bReadOnly = true;
	m_pInput3->SetFocus();
}
