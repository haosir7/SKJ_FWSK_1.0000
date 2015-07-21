/*! \file    MonthSaleSumWin.cpp
   \brief    awe4000r报表打印的月销售统计时间输入界面
   \author   Yu Yan
   \version  1.0
   \date     2008-9-19 
 */
#include "MonthSaleSumWin.h"
#include "ReportFunc.h"
#include "CGlobalArg.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CMonthSaleSumWIn::CMonthSaleSumWIn():CaWindow()
{
	m_StartDate = 0;
	m_EndDate = 0;
	m_pTjxxhz = &m_Tjxxhz;
}

CMonthSaleSumWIn::~CMonthSaleSumWIn()
{
	
}

int CMonthSaleSumWIn::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
  
	m_pFrame->RegsiterWin(this,MONTH_SALE_SUM_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "输入查询月份:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET+6;
	//curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(left_offset,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(6);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受数字
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "输入示例: 如查询2015年1月");
	curH += LINE_H;
	m_pLabel1 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel1->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLabel1->SetTitle(title, strlen(title));
	
	strcpy(title, "销售统计, 则输入201501");
	curH += LINE_H;
	m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel2->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLabel2->SetTitle(title, strlen(title));

	// 	//创建一个Input  第一行
// 	strcpy(title, "输入截止日期:");
// 	titleLen = strlen(title) * CHAR_W;
// 	curH += LINE_H;
// 	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
// 	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
// 	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
// 	m_pInput2->SetMaxLen(8);
// 	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受字符
// 	m_pInput2->OnObject = S_OnInput2;

	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽

	
	//创建一个Button  第五行
	strcpy(title, "确定");
	m_pBtn1 = new CaButton();
	curH += (6+LINE_H);
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

int CMonthSaleSumWIn::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
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

int CMonthSaleSumWIn::ReFresh()
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

void CMonthSaleSumWIn::S_OnActive(CaWindow *obj)
{
	((CMonthSaleSumWIn *)obj)->DoActive();
}


void CMonthSaleSumWIn::DoActive()
{
	DBG_PRINT((" CMonthSaleSumWIn::DoActive()!"));
	
	m_pInput1->Clear();//清除Input里的内容
	m_StartDate = 0;
	m_EndDate = 0;
	ReFresh();	
}

void CMonthSaleSumWIn::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMonthSaleSumWIn *win=(CMonthSaleSumWIn *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CMonthSaleSumWIn::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMonthSaleSumWIn *win=(CMonthSaleSumWIn *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}


void CMonthSaleSumWIn::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMonthSaleSumWIn *win=(CMonthSaleSumWIn *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CMonthSaleSumWIn::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMonthSaleSumWIn *win=(CMonthSaleSumWIn *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CMonthSaleSumWIn::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CMonthSaleSumWIn::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}


void CMonthSaleSumWIn::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 ret = 0 ;
	string strErr("");
	ret = CheckInput();
	if(ret != SUCCESS)
	{
		return;
	}
	ret = GetMonthSumCount(m_pTjxxhz, strErr);

	if ( ret !=SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	ChangeWin(MONTH_SALE_SHOW_WIN);	 //返回商业统计界面
}

void CMonthSaleSumWIn::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(FISCAL_REPORT_MENU);	 //返回商业统计界面		
}

UINT8 CMonthSaleSumWIn::CheckInput(void)
{
	DBG_PRINT(("CheckInput begin"));
	
	char *content1;
	UINT32 nNum = 0;

	content1 = (char*)(m_pInput1->m_contentBuf); 
	DBG_PRINT(("CheckInput content1 = %s", content1));

	//未输入查询月份
	if (strlen(content1)==0)
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("请输入查询日期");
		return FAILURE;
	}
	//输入日期必须是6位数
	if (strlen(content1)!=6)
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("输入日期必须是6位数");
		return FAILURE;
	}
	m_StartDate = atoi(content1);
	m_EndDate = m_StartDate;

	UINT32 year, month, day;
	year = m_StartDate/100;
	month = m_StartDate%100;
	day = 1;
	if( TDateTime::DateIsValid(year, month, day) != 1 )
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("输入日期无效");
		return FAILURE;
	}

	UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	year = nCurDate/10000;
	month = (nCurDate%10000)/100;
	day = nCurDate%100;
	nCurDate = year*100+month;

	if (1970 == year)
	{
		CaMsgBox::ShowMsg("当前日期为1970,请修改");
		return FAILURE;
	}
	if (m_StartDate>nCurDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("查询月份大于当前月份!");
		return FAILURE;		
	}

	UINT32 nInitDate = g_globalArg->m_initDate;
	year = nInitDate/10000;
	month = (nInitDate%10000)/100;
	day = nInitDate%100;
	nInitDate = year*100+month;

	if (m_StartDate < nInitDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("查询月份小于初始化月份!");
		return FAILURE;		
	}

	if (m_StartDate == nInitDate)
	{
		m_pTjxxhz->m_Qsrq = g_globalArg->m_initDate;
	}
	else
	{
		m_pTjxxhz->m_Qsrq = m_StartDate*100+1;//起始日期须是8位
	}

	m_pTjxxhz->m_Jzrq = m_pTjxxhz->m_Qsrq;//起始日期须是8位

	DBG_PRINT(("m_pTjxxhz->m_Qsrq = %u",m_pTjxxhz->m_Qsrq));
	DBG_PRINT(("m_pTjxxhz->m_Jzrq = %u",m_pTjxxhz->m_Jzrq));

	return SUCCESS;
}




















#if 0
UINT8 CMonthSaleSumWIn::CheckInput(void)
{
	DBG_PRINT(("CheckInput begin"));
	
	char *content1, *content2;
	UINT32 nNum = 0;

	content1 = (char*)(m_pInput1->m_contentBuf); 
	DBG_PRINT(("CheckInput content1 = %s", content1));
	DBG_PRINT(("CheckInput content2 = %s", content2));

	//未输入起止日期
	if ((strlen(content1)==0)||(strlen(content2)==0))
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("请输入起始/截止日期");
		return FAILURE;
	}
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

	if (m_StartDate>m_EndDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期\n大于截止日期!");
		return FAILURE;
	}

	UINT32 nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	if (m_StartDate>nCurDate) 
	{
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("起始日期大于当前日期!");
		return FAILURE;		
	}
	if(m_EndDate>nCurDate)
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("截止日期大于当前日期");
		return FAILURE;
	}

	if (m_StartDate<g_globalArg->m_initDate) 
	{
		// 输入的起始日期与初始化日期做个判断
		//m_StartDate = g_globalArg->m_initDate;//初始化日期
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
	if (m_EndDate > nCurDate) //截止日期大于当前日期
	{
		m_EndDate = nCurDate;	
	}


	//检查是否有符合条件的日汇总
// 	CInvHead invHead;
//	INT8 chValue[256];
//	memset((void*)chValue, 0, sizeof(chValue));
//	sprintf(chValue, " where I_DATE >= %u and I_DATE <= %u",m_StartDate,m_EndDate);
//	invHead.m_filter = chValue;
//	invHead.Requery();
//	INT32 errorcode = invHead.LoadOneRecord();
//	nNum = GetDaySumCount(m_StartDate, m_EndDate);
//	if (errorcode == SQLITE_OK)
//	{
//		return SUCCESS;
//	}
//	else if(errorcode == SQLITE_DONE)
//	{
//		DBG_PRINT(("该时段无交易记录"));
//		CaMsgBox::ShowMsg("该时段无交易记录");
//		return FAILURE;
//	}
//	else
//	{
//		DBG_PRINT(("查询交易记录失败"));
//		CaMsgBox::ShowMsg("查询交易记录失败");
//		return FAILURE;
//	}

	return SUCCESS;
}

UINT8 CMonthSaleSumWIn::PrnMonthSaleSumProc()
{
	DBG_PRINT(("PrnMonthSaleSumProc()"));

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

	CaProgressBar proBar("月销售统计打印中.....");
	proBar.ReFresh();

	//打印报表头
	PrnSaleSumHead(m_StartDate, 0, m_EndDate, 235959, 0);

	MonthSaleSumHandle();


	return SUCCESS;
}



UINT8 CMonthSaleSumWIn::MonthSaleSumHandle()
{
	DBG_PRINT(("MonthSaleSumHandle()"));

//	INT32 retCode;
	INT64 norMoney = 0;//某部类正票金额
//	INT64 retMoney = 0;//某部类红票金额
	INT64 SumMoney = 0;//某月的总销售金额
	INT32 ninvCount = 0;//某月的开票数量
	INT64 allSumMoney = 0;//该时段的总销售金额
	INT32 allInvCount = 0;//该时段的开票数量
	
	INT32 nInvNum = 0;
	string strTimeFilter = "";
	string strDayFilter = "";
	INT8 chDayValue[128];
	memset((void*)chDayValue, 0, sizeof(chDayValue));
	INT8 chTimeValue[128];
	memset((void*)chTimeValue, 0, sizeof(chTimeValue));
	UINT8 nCount = 0;
	UINT8 i = 0;
	CInvHead invHead;



	DBG_PRINT(("*******开始统计*******************"));
		
	//逐天打印
	UINT32 nStartDate = m_StartDate;//统计时段的起始日期
	TDateTime sDate(nStartDate, 0);
	TDateTime eDate = sDate.MonthEnd(sDate);//该月最后一天
	UINT32 nDate = eDate.FormatInt(YYYYMMDD);
	UINT32 nEndDate = 0;

//	UINT32 nDate = m_StartDate;
//	TDateTime sDate(nDate, 0);
//	sprintf(chTimeValue, " where I_DATE >= %u and I_DATE <= %u",m_StartDate,m_EndDate);
//	strTimeFilter = chTimeValue;
//	nCount = 1;//平常日只统计该日的部类销售金额
	
	while (1)
	{
		ninvCount = 0; //某操作员的开票数量清零
		SumMoney = 0; //某操作员的销售总金额清零
		
//		UINT32 nStartDate = m_StartDate;//统计时段的起始日期
//	    sDate(nStartDate, 0);
//	TDateTime eDate = sDate.MonthEnd(sDate);//该月最后一天
//	UINT32 nDate = eDate.FormatInt(YYYYMMDD);

		if (nDate>m_EndDate)
		{
			nEndDate = m_EndDate;
		}
		else
		{
			nEndDate = nDate;
		}
		DBG_PRINT(("*******一个统计时段*******************"));
		sprintf(chDayValue, "where KPRQ >= %u and KPRQ <= %u ", nStartDate, nEndDate);
		DBG_PRINT(("一个统计时段: %s", chDayValue));
		strDayFilter = chDayValue;

		//累计发票数
		invHead.m_filter = chDayValue;//时间条件
		DBG_PRINT(("invHead.m_filter = %s", invHead.m_filter.c_str()));
		//ninvCount = invHead.GetInvHeadNum();//某日的开票数量
		ninvCount = invHead.GetInvNumByDate();
		if (ninvCount<0)
		{
			CaMsgBox::ShowMsg("统计发票数量失败");
			DBG_PRINT(("统计发票数量失败"));
			return FAILURE;
		}
		allInvCount += ninvCount;//总的开票数量	
//		if (ninvCount > 0)
//		{
			PrnMonth(nEndDate);//打印日期				
// 		}
				
		//按部类进行统计并打印
		if(PrnDeptMoney(SumMoney, &strDayFilter)!=SUCCESS)
		{
			return FAILURE;
		}
	

//		if (ninvCount > 0)
//		{
			PrnMonthSaleSum(ninvCount, SumMoney); //打印当月累计数量和金额	
// 		}							    	     
		allSumMoney += SumMoney;//整个时间段的累计金额

		if (nEndDate == m_EndDate) 
		{
			break;//月份统计结束
		}
		
		(eDate)++;	//下一个统计时段的第一天
		sDate = eDate;
		nStartDate = sDate.FormatInt(YYYYMMDD);
		eDate = sDate.MonthEnd(sDate);//该月最后一天
		nDate = eDate.FormatInt(YYYYMMDD);
	}
	PrnAllDaySaleSum(allInvCount, allSumMoney); //打印整个时段累计数量和金额

	sprintf(chTimeValue, "where KPRQ >= %u and KPRQ <= %u ", m_StartDate, m_EndDate);
	DBG_PRINT(("整个统计时段: %s", chDayValue));
	strTimeFilter = chTimeValue;
	if (PrnDeptMoney(SumMoney, &strTimeFilter)!=SUCCESS) //打印总计的部类销售信息
	{
		return FAILURE;
	}
	
	ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}
#endif