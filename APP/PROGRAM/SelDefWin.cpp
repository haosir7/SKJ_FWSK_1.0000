#include "SelDefWin.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "TemplateMacro.h"
#include "CGlobalArg.h"

#include "LOGCTRL.h"
#include "pos_debug.h"


CSelDefWin::CSelDefWin() :CMultiInputCommonWin(5)
{
	m_iNumOfInput = 5;
	m_iNumOfBtn = 2;
}

CSelDefWin::~CSelDefWin()
{

}

int CSelDefWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, SELF_DEF_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH);
	
	SetObjPara(1,"自定义1",	CaInput::aCHAR, SELF_DEF_TAB_LEN);
	SetObjPara(2,"自定义值1",	CaInput::aCHAR, SELF_DEF_CONT_LEN);
	SetObjPara(3,"自定义2",	CaInput::aCHAR, SELF_DEF_TAB_LEN);
	SetObjPara(4,"自定义值2",	CaInput::aCHAR, SELF_DEF_CONT_LEN);
	SetObjPara(5,"备用",	CaInput::aCHAR, REMARKS_LEN);
	
	SetObjPara(10," 确认 ");
	SetObjPara(11," 返回 ");
	
	OnActive = S_OnActive;
	
	this->ReFresh();
	
	return 0;
}
int CSelDefWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
			ChangeWin(PROGRAM_CTRL_MAIN_MENU);					
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}



int CSelDefWin::ReFresh()
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

void CSelDefWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
}

void CSelDefWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CSelDefWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput4);
	ReFresh(); 
}

void CSelDefWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput5);
	ReFresh(); 
}

void CSelDefWin::OnInput5(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}

void CSelDefWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;
	if( CheckInputValid(strErr) != SUCCESS )
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	m_prnInfo.m_zdyTab1 = (char*)(m_pInput1->m_contentBuf); 
	m_prnInfo.m_zdyCont1 = (char*)(m_pInput2->m_contentBuf); 
	m_prnInfo.m_zdyTab2 = (char*)(m_pInput3->m_contentBuf); 
	m_prnInfo.m_zdyCont2 = (char*)(m_pInput4->m_contentBuf); 
	m_prnInfo.m_remarks = (char*)(m_pInput5->m_contentBuf); 

	DBG_PRINT(("m_prnInfo.m_zdyTab1 = %u", m_prnInfo.m_zdyTab1.c_str()));

	m_prnInfo.Delete();
	INT32 errCode = m_prnInfo.AddNew();
	if (SQLITE_OK != errCode)
	{
		CaMsgBox::ShowMsg("票样设置信息存储错误");
		DBG_PRINT(("m_prnInfo errCode = %u", errCode));
		return;
	}
		
	g_globalArg->m_prnInfo->Requery();
	errCode = g_globalArg->m_prnInfo->LoadOneRecord();
	if (SQLITE_OK != errCode)
	{
		CaMsgBox::ShowMsg("票样设置信息读取错误");
		DBG_PRINT(("m_prnInfo errCode = %u", errCode));
	}

	CaMsgBox::ShowMsg("票样设置信息成功");
	DBG_PRINT(("m_prnInfo errCode = %u", errCode));
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
	return;
}

void CSelDefWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearPrnInfoWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);	
}
	
void CSelDefWin::S_OnActive(CaWindow *obj)
{
	((CSelDefWin *)obj)->DoActive();
}

void CSelDefWin::DoActive()
{
	DispPrnInfo();
	ReFresh(); 
}

void CSelDefWin::DispPrnInfo()
{
	INT8 tempstr[32];
	
	m_pInput1->SetContentBuf((UINT8 *)g_globalArg->m_prnInfo->m_zdyTab1.c_str(), g_globalArg->m_prnInfo->m_zdyTab1.length());

	m_pInput2->SetContentBuf((UINT8 *)g_globalArg->m_prnInfo->m_zdyCont1.c_str(),g_globalArg->m_prnInfo->m_zdyCont1.length());
	
	m_pInput3->SetContentBuf((UINT8 *)g_globalArg->m_prnInfo->m_zdyTab2.c_str(), g_globalArg->m_prnInfo->m_zdyTab2.length());

	m_pInput4->SetContentBuf((UINT8 *)g_globalArg->m_prnInfo->m_zdyCont2.c_str(), g_globalArg->m_prnInfo->m_zdyCont2.length());

	m_pInput5->SetContentBuf((UINT8 *)g_globalArg->m_prnInfo->m_remarks.c_str(), g_globalArg->m_prnInfo->m_remarks.length());

	m_pInputList->SetFocusToObj(m_pInput1);

}

UINT8 CSelDefWin::CheckInputValid(string &strErr)
{
	if( (!m_pInput1->IsEmpty() && m_pInput2->IsEmpty()) ||
		(m_pInput1->IsEmpty() && !m_pInput2->IsEmpty()) )
	{
		strErr = "自定义1不完整";
		return FAILURE;
	}

	if( (!m_pInput3->IsEmpty() && m_pInput4->IsEmpty()) ||
		(m_pInput3->IsEmpty() && !m_pInput4->IsEmpty()) )
	{
		strErr = "自定义2不完整";
		return FAILURE;
	}

	return SUCCESS;
}

void CSelDefWin::ClearPrnInfoWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput4->Clear();
	m_pInput5->Clear();
//	m_pInput6->Clear();
//	m_pInput7->Clear();
//	m_pInput8->Clear();
//	m_pInput9->Clear();
	m_pInputList->SetFocusToObj(m_pInput1);
}