/*! \file    IssueSumDateWin.cpp
   \brief    awe4000r报表打印的开票统计时间输入界面
   \author   Yu Yan
   \version  1.0
   \date     2008-9-5 
 */

#include "IssueSumDateWin.h"
#include "ReportFunc.h"
#include "sqlite3.h"

#include "CGlobalArg.h"
#include "CInvHead.h"
#include "CaProgressBar.h"
#include "PrintDriver.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CIssueSumDateWin::CIssueSumDateWin():CaWindow()
{
	m_StartDate = 0;
	m_EndDate = 0;
	m_StartTime = 0;
	m_EndTime = 0;
	m_strTimeSeg = "";

	m_iPrnType = 0;
}

CIssueSumDateWin::~CIssueSumDateWin()
{
	
}


int CIssueSumDateWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,ISSUE_SUM_DATE_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "起始日期:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(8);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "起始时间:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(6);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受字符
	m_pInput2->OnObject = S_OnInput2;
	
	//创建一个Input  第三行
	strcpy(title, "截止日期:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(8);
	m_pInput3->m_InputType = m_pInput3->aINT; //该输入框只接受字符
	m_pInput3->OnObject = S_OnInput3;

	//创建一个Input  第四行
	strcpy(title, "截止时间:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput4=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput4->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput4->SetTitle(title,titleLen / CHAR_W);	
	m_pInput4->SetMaxLen(6);
	m_pInput4->m_InputType = m_pInput4->aINT; //该输入框只接受字符
	m_pInput4->OnObject = S_OnInput4;
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "打印");
	m_pBtn1 = new CaButton();
	curH += LINE_H;
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

int CIssueSumDateWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
			ChangeWin(FISCAL_REPORT_MENU);			
			return SUCCESS;
		  break;		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CIssueSumDateWin::ReFresh()
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

void CIssueSumDateWin::S_OnActive(CaWindow *obj)
{
	((CIssueSumDateWin *)obj)->DoActive();
}


void CIssueSumDateWin::DoActive()
{
	DBG_PRINT((" CIssueSumDateWin::DoActive()!"));
	
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();//清除Input里的内容
	m_pInput3->Clear();//清除Input里的内容
	m_pInput4->Clear();//清除Input里的内容

	m_pInput2->SetContentBuf((UINT8 *)"000000", 6);
	m_pInput4->SetContentBuf((UINT8 *)"235959", 6);
	
	m_StartDate = 0;
	m_EndDate = 0;
	ReFresh();	
}



void CIssueSumDateWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CIssueSumDateWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CIssueSumDateWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CIssueSumDateWin::S_OnInput4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnInput4(iEvent,pEventData,iDataLen);
}


void CIssueSumDateWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CIssueSumDateWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CIssueSumDateWin *win=(CIssueSumDateWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CIssueSumDateWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CIssueSumDateWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CIssueSumDateWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CIssueSumDateWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CIssueSumDateWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 retCode;
	UINT8 ret = CheckInput();
	if(ret == SUCCESS)
	{
		INT32 status;
		CaMsgBox msgBox("发票类型(1正2红3废):",CaMsgBox::MB_INPUT);
		msgBox.SetInputType(CaInput::aINT);
		msgBox.SetMaxInputLen(1);

		msgBox.ShowBox();
		status = msgBox.m_iStatus;
		if (status != OK_PRESSED)
		{
			return;
		}
		m_iPrnType = atoi((INT8 *)msgBox.GetInputContent());
		DBG_PRINT(("m_iPrnType = %d", m_iPrnType));
		if( (m_iPrnType > 3)||(m_iPrnType == 0) )
		{
			//m_iPrnType = 0;
			CaMsgBox::ShowMsg("请输入正确的发票类型");
			return ;
		}
		CaProgressBar proBar("信息获取中.....");
		proBar.ReFresh();
		//统计并打印日开票统计报表
		retCode = PrnIssueSumProc();
		if (SUCCESS == retCode) 
		{
			ChangeWin(FISCAL_REPORT_MENU);	 //返回税控信息界面	
		}
	}	
}


void CIssueSumDateWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(FISCAL_REPORT_MENU);	 //返回税控信息界面		
}


UINT8 CIssueSumDateWin::CheckInput(void)
{
	DBG_PRINT(("CheckInput begin"));
	
	char *content1, *content2, *content3, *content4;
	UINT32 nNum = 0;

	content1 = (char*)(m_pInput1->m_contentBuf); 
	content2 = (char*)(m_pInput2->m_contentBuf); 
	content3 = (char*)(m_pInput3->m_contentBuf); 
	content4 = (char*)(m_pInput4->m_contentBuf); 
	DBG_PRINT(("CheckInput content1 = %s", content1));
	DBG_PRINT(("CheckInput content2 = %s", content2));


	//未输入
	if ((strlen(content1)==0)||(strlen(content2)==0))
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("请输入起始日期/时间");
		return FAILURE;
	}
	if ((strlen(content3)==0)||(strlen(content4)==0))
	{
		m_pInput3->SetFocus();
		CaMsgBox::ShowMsg("请输入截止日期/时间");
		return FAILURE;
	}

	//输入日期必须是8位数
	if ((strlen(content1)!=8)||(strlen(content3)!=8))
	{
		CaMsgBox::ShowMsg("输入日期必须是8位数");
		return FAILURE;
	}

	//输入时间必须是6位数
	if ((strlen(content2)!=6)||(strlen(content4)!=6))
	{
		CaMsgBox::ShowMsg("输入时间必须是6位数");
		return FAILURE;
	}
	m_StartDate = atoi(content1);
	m_StartTime = atoi(content2);
	m_EndDate = atoi(content3);
	m_EndTime = atoi(content4);

    //检验时分秒的有效性
	if (IsTimeValid(m_StartTime) != SUCCESS)
	{
		CaMsgBox::ShowMsg("起始时间无效!");
		return FAILURE;
	}
	if (IsTimeValid(m_EndTime) != SUCCESS)
	{
		CaMsgBox::ShowMsg("截止时间无效!");
		return FAILURE;
	}


	if (m_StartDate>m_EndDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期\n大于截止日期!");
		return FAILURE;
	}
	else if ((m_StartDate==m_EndDate)&&(m_StartTime>m_EndTime)) 
	{
		CaMsgBox::ShowMsg("起始时间\n大于截止时间!");
		return FAILURE;
	}

	UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	UINT32 nCurTime = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
	if (m_StartDate>nCurDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期大于当前日期!");
		return FAILURE;		
	}
	else if((m_StartDate==nCurDate)&&(m_StartTime>nCurTime))
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("起始时间\n大于当前时间!");
		return FAILURE;	
	}

	if (m_StartDate<g_globalArg->m_initDate) //起始日期小于初始化日期
	{
// 		m_StartDate = g_globalArg->m_initDate;//初始化日期
// 		m_StartTime = 0;
		DBG_PRINT(("m_StartDate = %u", m_StartDate));
		DBG_PRINT(("m_initDate = %u", g_globalArg->m_initDate));
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期\n小于初始化日期!");
		return FAILURE;	
	}

	if (m_EndDate<g_globalArg->m_initDate) 
	{
		m_pInput3->SetFocus();
		CaMsgBox::ShowMsg("截止日期\n小于初始化日期!");
		return FAILURE;		
	}
	if (m_EndDate>nCurDate) 
	{
		m_pInput3->SetFocus();
		CaMsgBox::ShowMsg("截止日期\n大于当前日期!");
		return FAILURE;		
	}
	else if((m_EndDate==nCurDate)&&(m_EndTime>nCurTime))//大于当前时间
	{
		m_EndTime = nCurTime;
	}

    
	//检查是否有符合条件的时间段汇总记录
	INT8 chValue[256];
	memset((void*)chValue, 0, sizeof(chValue));

	if (m_StartDate!=m_EndDate) //起始日和截至日不是同一天
	{
		//中间时段
		UINT32 startDay, endDay;
		TDateTime sDateTime(m_StartDate, 0);
		TDateTime eDateTime(m_EndDate, 0);
		TDateTime *sDate = &sDateTime;
		TDateTime *eDate = &eDateTime;
		DBG_ASSERT_EXIT((sDate != NULL), (" sDate == NULL!"));
		DBG_ASSERT_EXIT((eDate != NULL), (" eDate == NULL!"));
		(*sDate)++;
		(*eDate)--;
		startDay = sDate->FormatInt(YYYYMMDD);
		endDay = eDate->FormatInt(YYYYMMDD);
		sprintf(chValue, " where KPRQ >= %u and KPRQ <= %u",startDay,endDay);
		m_strTimeSeg = chValue;//保存，统计时用
		sDate = NULL;
		eDate = NULL;
	}

	return SUCCESS;
}


UINT8 CIssueSumDateWin::PrnIssueSumProc()
{
	DBG_PRINT(("PrnSalerSumProc()"));

	UINT8 ret;
	string strTime = "";
	INT8 chValue[256];
	memset((void*)chValue, 0, sizeof(chValue));

	//白纸检查
	ret = YesNoMsgBox("请放入白纸，是否打印？");
	if (ret==FAILURE)
	{
		return FAILURE;//不打印
	}
	if (isPaper() != 0)
	{	
		CaMsgBox::ShowMsg("打印机未检测到纸");
		return FAILURE;
	}

	CaProgressBar proBar("开票统计打印中.....");
	proBar.ReFresh();

	//打印报表头
	PrnIssueSumHead(m_StartDate, m_StartTime, m_EndDate, m_EndTime);

	if (m_StartDate!=m_EndDate) //起始日和截至日不是同一天
	{
		ret = IssueSumHandle(0);//已预存时间段的sql语句
	}
	else
	{
		ret = IssueSumHandle(1);
	}

	return ret;
}



UINT8 CIssueSumDateWin::IssueSumHandle(UINT8 nIfSameDay)
{
	DBG_PRINT(("SalerSumHandle()"));

	INT32 retCode;
	UINT32 nNorInvNum = 0;//正票份数
	UINT32 nRetInvNum = 0;//红票份数
	UINT32 nWasteInvNum = 0;//废票份数
	INT64 norMoney = 0;//正票金额
	INT64 retMoney = 0;//红票金额
	INT8 chValue[512];
	memset((void*)chValue, 0, sizeof(chValue));
	UINT8 nCount = 0;
	UINT8 i = 0;
	string filterArr[3];

	if (1 == nIfSameDay) 
	{
		DBG_PRINT(("是同一天"));
		nCount = 1;
		sprintf(chValue, "where KPRQ = %u and KPSJ >= %u and KPSJ <= %u ", m_StartDate, m_StartTime, m_EndTime);		
		filterArr[0] = chValue;
		DBG_PRINT(("filterArr[0] = %s", filterArr[0].c_str()));
	}
	else
	{
		DBG_PRINT(("不是同一天"));
		nCount = 3;
		//起始日
		sprintf(chValue, "where KPRQ = %u and KPSJ >= %u ", m_StartDate, m_StartTime);
		filterArr[0] = chValue;	
		DBG_PRINT(("filterArr[0] = %s", filterArr[0].c_str()));
		//中间时段
		filterArr[1] = m_strTimeSeg;//已预存时间段的sql语句
		DBG_PRINT(("filterArr[1] = %s", filterArr[1].c_str()));
		//截止日
		sprintf(chValue, "where KPRQ = %u and KPSJ <= %u ", m_EndDate, m_EndTime);
		filterArr[2] = chValue;	
		DBG_PRINT(("filterArr[2] = %s", filterArr[2].c_str()));
		
	}
	
	DBG_PRINT(("*******开始统计*******************"));
	
	CInvHead invHead;
	string tmpfpdm = "";

	for(i = 0; i<nCount; i++)
	{
		invHead.m_filter = filterArr[i];
		invHead.m_filter += " order by FPDM, FPHM";
		invHead.Requery();
		retCode = invHead.MoveFirst();
		if( strcmp(tmpfpdm.c_str(), invHead.m_fpdm.c_str()) != 0 )
		{
			tmpfpdm = invHead.m_fpdm;
			PrnIssueSumCode(tmpfpdm);
		}

		while (retCode == SQLITE_ROW) 
		{
			if( strcmp(tmpfpdm.c_str(), invHead.m_fpdm.c_str()) != 0 )
			{
				tmpfpdm = invHead.m_fpdm;
				PrnIssueSumCode(tmpfpdm);
			}

			if( m_iPrnType == 0 )
				PrnIssueSumBody(invHead.m_fphm, invHead.m_kplx, invHead.m_kphjje);
			switch(invHead.m_kplx) 
			{
			case NORMAL_INV:
				if( m_iPrnType == 1 )
					PrnIssueSumBody(invHead.m_fphm, invHead.m_kplx, invHead.m_kphjje);
				nNorInvNum++;
				norMoney += invHead.m_kphjje;
				break;
			case RETURN_INV:
			case RET_MANUAL_INV:
			case RET_SPECIAL_INV:
				if( m_iPrnType == 2 )
					PrnIssueSumBody(invHead.m_fphm, invHead.m_kplx, invHead.m_kphjje);
				nRetInvNum++;
				retMoney += invHead.m_kphjje;
				break;
			case WASTE_INV:
			case WASTE_NOR:
			case WASTE_RET:
				if( m_iPrnType == 3 )
					PrnIssueSumBody(invHead.m_fphm, invHead.m_kplx, invHead.m_kphjje);
				nWasteInvNum++;
			default:;
			}
			retCode = invHead.MoveNext();
		}
		invHead.MoveEnd(); 
		if (retCode!=SQLITE_DONE) 
		{
			CaMsgBox::ShowMsg("查询数据库出错");
			return FAILURE;
		}
	}
	PrnIssueSumEnd(nNorInvNum, nRetInvNum, nWasteInvNum, norMoney, retMoney);

	DBG_PRINT(("开票统计成功"));

	return SUCCESS;

}