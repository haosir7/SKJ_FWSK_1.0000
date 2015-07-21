/*! \file    DaySaleSumWin.cpp
   \brief    awe4000r报表打印的日销售统计时间输入界面
   \author   Yu Yan
   \version  1.0
   \date     2008-9-19 
 */


#include "DaySaleSumWin.h"
#include "ReportFunc.h"
#include "sqlite3.h"
#include "CGlobalArg.h"
#include "CaProgressBar.h"
#include "CInvHead.h"
#include "PrintDriver.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CDaySaleSumWin::CDaySaleSumWin():CaWindow()
{
	m_StartDate = 0;
	m_EndDate = 0;
	m_StartTime = 0;
	m_EndTime = 0;

}

CDaySaleSumWin::~CDaySaleSumWin()
{
	
}


int CDaySaleSumWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,DAY_SALE_SUM_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "输入起始日期:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(8);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;

	//创建一个Input  第一行
	strcpy(title, "输入起始时间:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(6);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受字符
	m_pInput2->OnObject = S_OnInput2;

	//创建一个Input  第一行
	strcpy(title, "输入截止日期:");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput3=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput3->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput3->SetTitle(title,titleLen / CHAR_W);	
	m_pInput3->SetMaxLen(8);
	m_pInput3->m_InputType = m_pInput3->aINT; //该输入框只接受字符
	m_pInput3->OnObject = S_OnInput3;

	//创建一个Input  第一行
	strcpy(title, "输入截止时间:");
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

int CDaySaleSumWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
			ChangeWin(COMMERCE_REPORT_MENU);			
			return SUCCESS;
		  break;		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}



int CDaySaleSumWin::ReFresh()
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

void CDaySaleSumWin::S_OnActive(CaWindow *obj)
{
	((CDaySaleSumWin *)obj)->DoActive();
}


void CDaySaleSumWin::DoActive()
{
	DBG_PRINT((" CDaySaleSumWin::DoActive()!"));
	
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();//清除Input里的内容
	m_pInput3->Clear();//清除Input里的内容
	m_pInput4->Clear();//清除Input里的内容

	m_pInput2->SetContentBuf((UINT8 *)"000000", 6);
	m_pInput4->SetContentBuf((UINT8 *)"235959", 6);

	m_StartDate = 0;
	m_EndDate = 0;
	m_StartTime = 0;
	m_EndTime = 0;
	ReFresh();	
}



void CDaySaleSumWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CDaySaleSumWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CDaySaleSumWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CDaySaleSumWin::S_OnInput4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}


void CDaySaleSumWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CDaySaleSumWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CDaySaleSumWin *win=(CDaySaleSumWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CDaySaleSumWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CDaySaleSumWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CDaySaleSumWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CDaySaleSumWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CDaySaleSumWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 retCode;
	UINT8 ret = CheckInput();
	DBG_PRINT(("CheckInput() = %u", ret));
	if(ret == SUCCESS)
	{
		CaProgressBar proBar("信息获取中.....");
		proBar.ReFresh();
		retCode = PrnDaySaleSumProc();	 //打印日销售报表
		if (SUCCESS == retCode) 
		{
			ChangeWin(COMMERCE_REPORT_MENU);	 //返回商业统计界面
		}		
	}
	
}


void CDaySaleSumWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(COMMERCE_REPORT_MENU);	 //返回商业统计界面		
}



UINT8 CDaySaleSumWin::CheckInput(void)
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
		m_pInput1->SetFocus();
		CaMsgBox::ShowMsg("输入日期必须是8位数");
		return FAILURE;
	}

	//输入时间必须是6位数
	if ((strlen(content2)!=6)||(strlen(content4)!=6))
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("输入时间必须是6位数");
		return FAILURE;
	}
	m_StartDate = atoi(content1);
	m_StartTime = atoi(content2);
	m_EndDate = atoi(content3);
	m_EndTime = atoi(content4);

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
		m_pInput3->SetFocus();
		CaMsgBox::ShowMsg("截止日期无效");
		return FAILURE;
	}
    //检验时分秒的有效性
	if (IsTimeValid(m_StartTime) != SUCCESS)
	{
		m_pInput2->SetFocus();
		CaMsgBox::ShowMsg("起始时间无效!");
		return FAILURE;
	}
	if (IsTimeValid(m_EndTime) != SUCCESS)
	{
		m_pInput4->SetFocus();
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
		m_pInput2->SetFocus();
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
		CaMsgBox::ShowMsg("起始时间大于当前时间!");
		return FAILURE;	
	}

	if (m_StartDate<g_globalArg->m_initDate) //起始日期小于初始化日期
	{
		//起始日期与初始化日期做判断
// 		m_StartDate = g_globalArg->m_initDate;//初始化日期
// 		m_StartTime = 0;
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
	

	//检查是否有符合条件的日汇总
//	CInvHead invHead;
//	INT8 chValue[256];
//	memset((void*)chValue, 0, sizeof(chValue));
//	sprintf(chValue, " where I_DATE >= %u and I_DATE <= %u",m_StartDate,m_EndDate);
//	invHead.m_filter = chValue;
//	invHead.Requery();
//	INT32 errorcode = invHead.LoadOneRecord();
////	nNum = GetDaySumCount(m_StartDate, m_EndDate);
//	if ((errorcode == SQLITE_OK)||(errorcode == SQLITE_DONE))
//	{
//		DBG_PRINT(("errorcode == SQLITE_OK or errorcode == SQLITE_DONE"));
//		return SUCCESS;
//	}
//	else if(errorcode == SQLITE_DONE)
//	{
//		DBG_PRINT(("该时段无交易记录"));
//		CaMsgBox::ShowMsg("该时段无交易记录!");
//		return FAILURE;
// 	}
//	else
//	{
//		DBG_PRINT(("查询交易记录失败"));
//		CaMsgBox::ShowMsg("查询交易记录失败!");
//		return FAILURE;
//	}

	return SUCCESS;
}

UINT8 CDaySaleSumWin::PrnDaySaleSumProc()
{
	DBG_PRINT(("PrnDaySaleSumProc()"));

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

	CaProgressBar proBar("日销售统计打印中.....");
	proBar.ReFresh();

	//打印报表头
	PrnSaleSumHead(m_StartDate, m_StartTime, m_EndDate, m_EndTime, 1);

	DaySaleSumHandle();


	return SUCCESS;
}



UINT8 CDaySaleSumWin::DaySaleSumHandle()
{
	DBG_PRINT(("DaySaleSumHandle()"));

//	INT32 retCode;
	INT64 norMoney = 0;//某部类正票金额
//	INT64 retMoney = 0;//某部类红票金额
	INT64 SumMoney = 0;//某日的总销售金额
	INT32 ninvCount = 0;//某日的开票数量
	INT64 allSumMoney = 0;//该时段的总销售金额
	INT32 allInvCount = 0;//该时段的开票数量
	
	INT32 nInvNum = 0;
	string strTimeFilter = "";
	string strTimeFilter2 = "";
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
	UINT32 nDate = m_StartDate;
	TDateTime sDate(nDate, 0);
	sprintf(chTimeValue, " and KPSJ >= %u ",m_StartTime);
	strTimeFilter = chTimeValue; //起始天
	sprintf(chTimeValue, " and KPSJ <= %u",m_EndTime);
	strTimeFilter2 = chTimeValue; //截止天
	nCount = 1;//平常日只统计该日的部类销售金额
	while (nDate<=m_EndDate)
	{
		ninvCount = 0; //某操作员的开票数量清零
		SumMoney = 0; //某操作员的销售总金额清零
			
		DBG_PRINT(("*******一天*******************"));
		sprintf(chDayValue, "where KPRQ = %u ", nDate);
		strDayFilter = chDayValue;
		if (m_StartDate==nDate) 
		{
			strDayFilter += strTimeFilter;			
		}
	
		if (m_EndDate == nDate) 
		{
			strDayFilter += strTimeFilter2;
		}

		//累计发票数
		invHead.m_filter = strDayFilter;//时间条件
		DBG_PRINT(("invHead.m_filter = %s", invHead.m_filter.c_str()));
		//ninvCount = invHead.GetInvHeadNum();//某日的开票数量
		ninvCount = invHead.GetInvNumByDate();
		if (ninvCount<0)
		{
			CaMsgBox::ShowMsg("统计发票数量失败");
			DBG_PRINT(("统计发票数量失败"));
			return FAILURE;
		}
		allInvCount += ninvCount;//该时段总的开票数量	

		PrnDate(&(sDate.FormatString(YYYYMMDD)));//打印日期				

		DBG_PRINT(("*******PrnDeptMoney*******************"));		
		//按部类进行统计
		if(PrnDeptMoney(SumMoney, &strDayFilter)!=SUCCESS)
		{
			return FAILURE;
		}
		DBG_PRINT(("*******PrnDaySaleSum*******************"));	
		PrnDaySaleSum(ninvCount, SumMoney); //打印当日累计数量和金额	
							    	     
		allSumMoney += SumMoney;//整个时间段的累计金额
		(sDate)++;	//下一天
		nDate = sDate.FormatInt(YYYYMMDD);
	}
	PrnAllDaySaleSum(allInvCount, allSumMoney); //打印该时段累计数量和金额

	if (PrnPeriodDeptMoney(SumMoney, m_StartDate, m_StartTime, m_EndDate, m_EndTime)!=SUCCESS) //打印总计的部类销售信息
	{
		return FAILURE;
	}

	ForwardNLine(FORWARD_LINES);
	return SUCCESS;

}