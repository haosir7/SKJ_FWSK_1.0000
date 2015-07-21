// InvInfoDownloadWin.cpp: implementation of the CFpblWin class.
//
//////////////////////////////////////////////////////////////////////

#include "FpblWin.h"
#include "FiscalFun.h"
#include "TDateTime.h"
#include "CMainFrame.h"
#include "LOGCTRL.h"
#include "pos_debug.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFpblWin::CFpblWin() : CaWindow()
{

	m_StartDate=0;
	m_EndDate=0;
}

CFpblWin::~CFpblWin()
{

}

int CFpblWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
	
	m_pFrame->RegsiterWin(this, FPBL_WIN);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "报税起始日期:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(8);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "报税终止日期:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "确定");
	m_pBtn1 = new CaButton();
	curH += 2*LINE_H;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;
	
	OnActive = S_OnActive;
	
	this->End();  
	return 0;
}

int CFpblWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(FISCAL_MAIN_MENU);					
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CFpblWin::ReFresh()
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

void CFpblWin::S_OnActive(CaWindow *obj)
{
	((CFpblWin *)obj)->DoActive();
}


void CFpblWin::DoActive()
{
	DBG_PRINT((" CFpblWin::DoActive()!"));
	m_pInput1->Clear();
	m_pInput2->Clear();
	ReFresh();	
}


void CFpblWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CFpblWin *win=(CFpblWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CFpblWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CFpblWin *win=(CFpblWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}


void CFpblWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CFpblWin *win=(CFpblWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CFpblWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CFpblWin *win=(CFpblWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CFpblWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CFpblWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CFpblWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 ret = 0;
	string strErr("");

	ret = CheckInput(strErr);	
	if (ret != SUCCESS)
	{
		//CaMsgBox::ShowMsg(strErr);
		return;
	}

	CaProgressBar proBar("发票补录中...");
	proBar.Show();

	DBG_PRINT(("传入报税起始日期 = %u", m_StartDate));
	DBG_PRINT(("传入报税终止日期 = %u",m_EndDate));

	ret = FSC_FpblProc(m_StartDate, m_EndDate, strErr);

	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	else
	{
		CaMsgBox::ShowMsg("发票补录成功");	
	}

	ChangeWin(FISCAL_MAIN_MENU);	 //返回发票管理界面	
	
}


void CFpblWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(FISCAL_MAIN_MENU);	 //返回发票管理界面		
}

UINT8 CFpblWin::CheckInput(string& strErr)
{

	DBG_PRINT(("CheckInput begin"));
	
	char *content1, *content2;

	content1 = (char*)(m_pInput1->m_contentBuf); //报税起始日期
	content2 = (char*)(m_pInput2->m_contentBuf); //报税截止日期

	//输入日期必须是8位数
	if ((strlen(content1)!=8)||(strlen(content2)!=8))
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("输入日期必须是8位数");
		return FAILURE;
	}

	m_StartDate = atoi(content1);
	m_EndDate = atoi(content2);


	unsigned int year, month, day;
	year = m_StartDate/10000;
	month = (m_StartDate%10000)/100;
	day = m_StartDate%100;
	if( TDateTime::DateIsValid(year, month, day) != 1 )
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期无效");
		return FAILURE;
	}
	year = m_EndDate/10000;
	month = (m_EndDate%10000)/100;
	day = m_EndDate%100;
	if( TDateTime::DateIsValid(year, month, day) != 1 )
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("截止日期无效");
		return FAILURE;
	}
 /*
	UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	if (m_StartDate>nCurDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期大于当前日期!");
		return FAILURE;		
	}


	if (m_StartDate<g_globalArg->m_initDate) //起始日期小于初始化日期
	{
		//起始日期与初始化日期做判断
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期\n小于初始化日期!");
		return FAILURE;	
	}


	if (m_EndDate<g_globalArg->m_initDate) 
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("截止日期\n小于初始化日期!");
		return FAILURE;		
	}
	if (m_EndDate>nCurDate) 
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("截止日期\n大于当前日期!");
		return FAILURE;		
	}
	*/

	if (m_EndDate < m_StartDate) 
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("起始日期大于截止日期!");
		return FAILURE;		
	}

	return SUCCESS;
}