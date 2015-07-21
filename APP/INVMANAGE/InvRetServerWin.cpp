 // InvRetServerWin.cpp: implementation of the CInvRetServerWin class.


#include "InvRetServerWin.h"
#include "CMainFrame.h"
#include "IncludeMe.h"
#include "pos_debug.h"
#include "SaleData.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "InvManageFunc.h"


CInvRetServerWin::CInvRetServerWin() : CaWindow()
{
	m_strSno = "";
	m_strEno = "";	
	m_strCode = "";
}

CInvRetServerWin::~CInvRetServerWin()
{

}
int CInvRetServerWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
	
	m_pFrame->RegsiterWin(this, INV_RET_TO_SERVER_WIN);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "发票代码:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(20);
	m_pInput1->m_InputType = m_pInput2->aCHAR; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "发票起始号码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(8);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;
	
	//创建一个Input  第三行
	strcpy(title, "发票截止号码:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(8);
	m_pInput3->m_InputType = m_pInput3->aINT; //该输入框只接受整型值
	m_pInput3->OnObject = S_OnInput3;
	
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
int CInvRetServerWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(INV_SEG_MENU);					
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}
int CInvRetServerWin::ReFresh()
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

/*! @function
*************************************************************************
<PRE>
函数名  : S_OnButton1
功能    : 按钮1相应事件
参数    : 
返回值  :
作者    : 唐日升
日期    : 2010-12-01
备注    : 
</PRE>
************************************************************************/
void CInvRetServerWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetServerWin *win=(CInvRetServerWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
/*! @function
*************************************************************************
<PRE>
函数名  : OnButton1
功能    : 按钮1相应事件
参数    : 
返回值  :
作者    : 唐日升
日期    : 2010-12-01
备注    : 
</PRE>
************************************************************************/
void CInvRetServerWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	string strErr("");
	CaProgressBar porbar("");
	CInvVol invVol;
	string strMsg1("");
	string strMsg2("");
	string strMsg3("");
	char sqlbuf[128] = {0};
	INT32 nRet;

	if (0 == g_globalArg->m_IngSegPoint)
	{
		strMsg1 = "是否发票缴销?";
		strMsg2 = "发票缴销中...";
		strMsg3 = "发票缴销成功";
	}
	else 
	{
		strMsg1 = "是否发票退回主机?";
		strMsg2 = "发票退回中...";
		strMsg3 = "发票退回成功";
	}

	if(FAILURE == YesNoBox(strMsg1))
	{
		return;
	}

	
	UINT8 ret = CheckInput(strErr);
	if (ret != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	memset(sqlbuf, 0x00, sizeof(sqlbuf));
	sprintf(sqlbuf, "where CODE = '%s' and IS_NO >= %s and IE_NO <= %s", 
		m_strCode.c_str(), m_strSno.c_str(), m_strEno.c_str());
	invVol.m_filter = sqlbuf;
	
	DBG_PRINT(("sqlbuf : %s", sqlbuf));
// 	invVol.Requery();
// 	INT32 errCode = invVol.LoadOneRecord();
// 	if (errCode == SQLITE_DONE)
// 	{
// 		CaMsgBox::ShowMsg("无该号段的发票");
// 		return;
// 	}
// 	else if(errCode != SQLITE_OK)
// 	{
// 		CaMsgBox::ShowMsg("查询数据库错误");
// 		DBG_PRINT(("errCode : %d", errCode));
// 		return;
// 	}
// 
// 	m_invVol.m_kindcode = invVol.m_kindcode;//发票种类代码

	porbar.SetText(strMsg2.c_str());
	porbar.ReFresh();
	//开始业务
	CDownLoadInvVolFun retHandle;
	if (0 == g_globalArg->m_IngSegPoint)
	{
		nRet = retHandle.InvVolRetToServer(&m_invVol, strErr);
	}
	else 
	{
		nRet = retHandle.InvVolRetToMach(&m_invVol, "0", strErr);//退回给主机（分机号0）
	}
	
	if (SUCCESS != nRet)
	{
		CaMsgBox::ShowMsg(strErr);
		DBG_PRINT(("%s" ,strErr.c_str()));
		return;
	}
	//更新本地数据库
	invVol.m_filter = sqlbuf;
	INT32 errCode = invVol.Delete();
	if (errCode != SQLITE_OK)
	{
		CaMsgBox::ShowMsg("更新数据库错误，请执行票源下载");
		DBG_PRINT(("errCode : %d", errCode));
		return;
	}

	DBG_PRINT(("发票缴销/退回成功"));
	CaMsgBox::ShowMsg(strMsg3);	
	ChangeWin(INV_SEG_MENU);	
}

void CInvRetServerWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetServerWin *win=(CInvRetServerWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvRetServerWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	ChangeWin(INV_SEG_MENU);	
}

void CInvRetServerWin::DoActive()
{
	DBG_PRINT((" CInvRetServerWin::DoActive()!"));

	m_sInvNo = 0;
	m_eInvNo = 0;
	m_strSno = "";
	m_strEno = "";	
	m_strCode = "";
	m_invVol.ResetVol();
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();//清除Input里的内容
	m_pInput3->Clear();//清除Input里的内容
	m_pInput1->SetContentBuf((UINT8*)g_globalArg->m_curInvVol->m_code.c_str(), g_globalArg->m_curInvVol->m_code.length());
	
	ReFresh();	
}

void CInvRetServerWin::S_OnActive(CaWindow *obj)
{
	((CInvRetServerWin *)obj)->DoActive();
}
UINT8 CInvRetServerWin::CheckInput(string &strErr)
{	
	if (m_pInput2->IsEmpty())
	{
		strErr = "请输入发票起始号码";
		return FAILURE;
	}
		
	if (m_pInput3->IsEmpty())
	{
		strErr = "请输入发票截止号码";
		return FAILURE;
	}

	if (m_pInput1->IsEmpty())
	{
		m_strCode = g_globalArg->m_curInvVol->m_code;
	}
	else
	{
		m_strCode = (char *)m_pInput1->m_contentBuf;
	}
	m_strSno = (char *)m_pInput2->m_contentBuf;
	m_strEno = (char *)m_pInput3->m_contentBuf;
	m_sInvNo = atoi((char *)m_pInput2->m_contentBuf);
	m_eInvNo = atoi((char *)m_pInput3->m_contentBuf);
	

	DBG_PRINT(("m_sInvNo = %d, m_eInvNo = %d", m_sInvNo, m_eInvNo));

	if(m_eInvNo <= m_sInvNo)
	{
		strErr = "截止号必须大于起始号";
		return FAILURE;
	}

	//获取号段的同一类票源标识
	CInvVol invVol;
	char sqlbuf[128] = {0};
	INT32 nRet;
	memset(sqlbuf, 0x00, sizeof(sqlbuf));
	sprintf(sqlbuf, "where CODE = '%s' and IS_NO >= %s and IE_NO <= %s", 
		m_strCode.c_str(), m_strSno.c_str(), m_strEno.c_str());
	invVol.m_filter = sqlbuf;
	
	DBG_PRINT(("sqlbuf : %s", sqlbuf));
	invVol.Requery();
	INT32 errCode = invVol.LoadOneRecord();
	if (errCode == SQLITE_DONE)
	{
		strErr = "无该号段的发票";
		return FAILURE;
	}
	else if(errCode != SQLITE_OK)
	{
		strErr = "查询数据库错误";
		DBG_PRINT(("errCode : %d", errCode));
		return FAILURE;
	}
	
	DBG_PRINT(("g_globalArg->m_kindFlag : %s", g_globalArg->m_kindFlag.c_str()));
	DBG_PRINT(("invVol.m_kindFlag : %s", invVol.m_kindFlag.c_str()));
//	if (invVol.m_kindFlag == g_globalArg->m_kindFlag)//若是可开的卷票
//	{   
		//if(((m_sInvNo%MAX_INV_VOL_NUM) != 1) ||((m_eInvNo%MAX_INV_VOL_NUM) != 0))
		if(((m_sInvNo%g_globalArg->m_perReccount) != 1) ||((m_eInvNo%g_globalArg->m_perReccount) != 0))
		{
			strErr = "必须是整卷的号段";
			return FAILURE;
		}
		
		if(((g_globalArg->m_curInvVol->m_isno>=m_sInvNo)&&(g_globalArg->m_curInvVol->m_isno<=m_eInvNo))||
			((g_globalArg->m_curInvVol->m_ieno>=m_sInvNo)&&(g_globalArg->m_curInvVol->m_ieno<=m_eInvNo)))
		{
			strErr = "号段不能包含当前卷发票";
			return FAILURE;
		}
//	}

	
	//恢复发票起止
	m_invVol.m_isno = m_sInvNo;
	m_invVol.m_ieno = m_eInvNo;
	m_invVol.m_code = m_strCode;

	return SUCCESS;
}

void CInvRetServerWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetServerWin *win=(CInvRetServerWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CInvRetServerWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetServerWin *win=(CInvRetServerWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CInvRetServerWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvRetServerWin *win=(CInvRetServerWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CInvRetServerWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CInvRetServerWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CInvRetServerWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

