/*! \file    InvStubPrnWin.h
   \brief    awe4000r发票管理档的发票存根打印界面
   \author   Yu Yan 
   \version  1.0
   \date     2008-2-14 
 */

#include "InvStubPrnWin.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>
#include "CaProgressBar.h"
#include "keypad.h"

CInvStubPrnWin::CInvStubPrnWin():CaWindow()
{
	m_pageIndex = 1;
	m_pageNum = 0;

	m_pInvHead = &m_invHead;
	DBG_ASSERT_EXIT((m_pInvHead != NULL), (" m_pInvHead == NULL!"));	
}

CInvStubPrnWin::~CInvStubPrnWin()
{
	    m_pInvHead = NULL;	
}

int CInvStubPrnWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,INV_STUB_PRN_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	//创建一个标签  第一行	
	label[0] = new CaLabel(false,CaObject::ALIGN_RIGHT);
	curH = SCREEN_TOP_OFFSET;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
  
	//创建一个标签  第二行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[1]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));

	//创建一个标签  第三行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[2]->Create(0,curH, SCREEN_W, CHAR_H);
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	
	//创建一个标签  第四行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[3]->Create(0,curH, m_iBtnW-24, CHAR_H);
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));

	//创建一个标签  第四行
	label[4] = new CaLabel(false,CaObject::ALIGN_LEFT);
	label[4]->Create(m_iColW-20,curH, m_iBtnW+20, CHAR_H);
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));

	m_iBtnW = (SCREEN_W - 40)/3 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/3; //Button的列宽
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

	strcpy(title, "补打");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn + m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;

	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn3 = new CaButton();
	m_pBtn3->Create(leftoffset_btn + m_iColW*2,curH,m_iBtnW,WORD_H); 
	m_pBtn3->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn3->SetTitle(title, strlen(title));
	m_pBtn3->BoxType(CaObject::BOX_NO);
	m_pBtn3->OnObject = S_OnButton3;

	OnActive = S_OnActive;
	
	this->End();  
	return 0;
}

void CInvStubPrnWin::S_OnActive(CaWindow *obj)
{
	((CInvStubPrnWin *)obj)->DoActive();
}

void CInvStubPrnWin::DoActive()
{
	DBG_PRINT(("进入CInvStubPrnWin::DoActive函数!"));
	
	m_pPSWin = (CInvStubWin*)m_pFrame->GetWin(INV_STUB_WIN);
	m_code = m_pPSWin->m_code;
	m_invStartNo = m_pPSWin->m_invStartNo;
    m_invEndNo = m_pPSWin->m_invEndNo;
	
	m_pageIndex = 1;
	INVM_GetInvNum(m_code, m_invStartNo, m_invEndNo, m_pageNum);//获得发票数
	QueryShow(m_pInvHead, 0, m_pageIndex, m_pageNum, 0);//组织屏幕显示信息
	ChangeTitle();
	ReFresh();	
	DBG_PRINT(("退出CInvStubPrnWin::DoActive函数!"));
}

int CInvStubPrnWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	UINT32 ii;
	char *content;

	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to INPUTWIN
		 ChangeWin(INV_STUB_WIN);			
		 return SUCCESS;
	 	 break;

	case PAGEUP_KEY:
		 DBG_PRINT(("PAGEUP_KEY"));
		 if ((m_pageIndex-1)<1)
		 {
			 return FAILURE;
		 }
		 m_pageIndex--;
		 QueryShow(m_pInvHead, m_pInvHead->m_fphm, m_pageIndex, m_pageNum, 1);
		 ChangeTitle();
	     ReFresh();	
		 DBG_PRINT(("PAGEUP_KEY"));
		 return SUCCESS;
		 break;

	case PAGEDOWN_KEY:
		 if ((m_pageIndex+1)>m_pageNum)
		 {
			 return FAILURE;
		 }
		 m_pageIndex++;
		 QueryShow(m_pInvHead, m_pInvHead->m_fphm, m_pageIndex, m_pageNum, 0);
		 ChangeTitle();
	     ReFresh();	
		 DBG_PRINT(("PAGEDOWN_KEY"));
		 return SUCCESS;
		 break;

	default: 
			break;
	}  

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

UINT8 CInvStubPrnWin::QueryShow(CInvHead *pInvHead, UINT32 nIndex, UINT32 nPageIndex, UINT32 nPageNum, UINT8 nOrder)
{
	DBG_ASSERT_EXIT((pInvHead != NULL), (" pInvHead == NULL!"));
	
	char value[256];
	INT32 errorcode = 0;

	DBG_PRINT((" nPageIndex = %u", nPageIndex));
	//页码序号超限
	if ((nPageIndex<1)||(nPageIndex>nPageNum))
	{
		return FAILURE;
	}

	//查出一张发票存根
	INT8 val[256];
	memset((void*)val, 0, sizeof(val));
	pInvHead->m_filter = "where FPDM = ";
	sprintf(val,"'%s' and FPHM >= %u and FPHM <= %u ",m_code.c_str(),m_invStartNo,m_invEndNo);
	pInvHead->m_filter.append(val);
	
	if (nOrder==0)//升序
	{
		memset((void*)value, 0, sizeof(value));
		sprintf(value,"and FPHM > %u ",nIndex);
		pInvHead->m_filter.append(value);
		pInvHead->m_filter.append("limit 1 ");
	}
	else
	{
		memset((void*)value, 0, sizeof(value));
		sprintf(value,"and FPHM < %u ",nIndex);
		pInvHead->m_filter.append(value);
		pInvHead->m_filter.append("order by FPHM desc limit 1 ");
	}
	
	pInvHead->Requery();  //查询
	errorcode = pInvHead->LoadOneRecord();
	if (errorcode != SQLITE_OK)
	{
		INVM_ErrMsgBox(QUERY_ERROR);
		return FAILURE;
	}

	switch(pInvHead->m_kplx)
	{
		case NORMAL_INV:
			 sprintf(title_array[3], "类型: 正票");
			 break;
		case RET_MANUAL_INV:
		case RET_SPECIAL_INV:
		case RETURN_INV:
			 sprintf(title_array[3], "类型: 红票");
			 break;
		case WASTE_INV:
		case WASTE_NOR:
		case WASTE_RET:
			 sprintf(title_array[3], "类型: 废票");
			 break;
		default:;
	}

	TDateTime tmpDate(pInvHead->m_kprq, pInvHead->m_kpsj);
    string strTmpDate = tmpDate.FormatString(YYYYMMDDHHMMSS);
	
	sprintf(title_array[0], "[页%u/%u]", nPageIndex, nPageNum);
	sprintf(title_array[1], "发票号码: %08u", pInvHead->m_fphm);	
	sprintf(title_array[2], "开票时间: %s", strTmpDate.c_str());
	if ((pInvHead->m_kplx==RETURN_INV)||(pInvHead->m_kplx==RET_MANUAL_INV)||(pInvHead->m_kplx==RET_SPECIAL_INV)) 
	{
		sprintf(title_array[4], "金额: -%.2lf", (double)(pInvHead->m_kphjje)/100.0);
	}
	else
	{
		sprintf(title_array[4], "金额: %.2lf", (double)(pInvHead->m_kphjje)/100.0);
	}
	
	return SUCCESS;
}

int CInvStubPrnWin::ReFresh()
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

UINT8 CInvStubPrnWin::ChangeTitle()
{
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));
	
	return SUCCESS;
}

void CInvStubPrnWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvStubPrnWin *win=(CInvStubPrnWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
} 

void CInvStubPrnWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvStubPrnWin *win=(CInvStubPrnWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvStubPrnWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvStubPrnWin *win=(CInvStubPrnWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CInvStubPrnWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	PrintInv(1);
}

void CInvStubPrnWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	PrintInv(0);
}

void CInvStubPrnWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(INV_STUB_WIN);	
}

void CInvStubPrnWin::PrintInv(UINT8 printType)
{
	UINT8 ret = 0;
	INT32 errorcode = 0;
    INT8 val[256];
	memset((void*)val, 0, sizeof(val));
	string strMsg = "";
	
	if( (m_pInvHead->m_kplx == WASTE_INV) || (m_pInvHead->m_kplx == WASTE_NOR) 
		|| (m_pInvHead->m_kplx == WASTE_RET) )
	{
		CaMsgBox::ShowMsg("废票不能打印");
		return;
	}

	if( printType == 1 )
		strMsg = "请放白纸，是否打印?";
	else
		strMsg = "请放对应纸质票，是否打印?";
	
	CaMsgBox msgBox(strMsg.c_str(),CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return;
	}
    else if(msgBox.m_iStatus == OK_PRESSED)
	{
		sprintf(val,"where FPDM = '%s' and FPHM = %u",m_pInvHead->m_fpdm.c_str(),m_pInvHead->m_fphm);
		
		m_pInvHead->m_filter.append(val);
		errorcode = m_pInvHead->Load();
		if (errorcode != SQLITE_OK)
		{
			INVM_ErrMsgBox(QUERY_ERROR);
			return;
		}
		ret = PrnInvStub(m_pInvHead, printType);
		
		if (ret != SUCCESS)
		{
			INVM_ErrMsgBox(ret);
		}	
	}  	
	return;
}

/*
void CInvStubPrnWin::ContinuousIssue()
{
	UINT8 ret;
	INT32 errorcode;
    INT8 val[128];
	UINT32 nIndex = m_invStartNo;
	CaProgressBar proBar("连续打印电子存根...");
	proBar.ReFresh();
	for(nIndex = m_invStartNo;nIndex<=m_invEndNo; nIndex++)
	{
		memset((void*)val, 0, sizeof(val));
		sprintf(val,"where CODE_NO = %u and INV_NO = %u",m_codeNo,nIndex);

		m_pInvHead->m_filter = val;
		errorcode = m_pInvHead->Load();
		ret = PrnInvStub(m_pInvHead, 0);
		if (ret != SUCCESS)
		{
			ErrMsgBox(ret);
		}
	}
	proBar.SetText("打印结束");
	proBar.ReFresh();

}*/


/*
void CInvStubPrnWin::ContinuousIssueOne()
{
	UINT8 ret;
	INT32 errorcode;
    INT8 val[128];
	UINT32 nIndex = m_invStartNo;
	UINT8 nKeyTime = 0;
	CaProgressBar proBar("连续打印一张电子存根");
	proBar.ReFresh();
	memset((void*)val, 0, sizeof(val));
	sprintf(val,"where CODE_NO = %u and INV_NO = %u",m_codeNo,nIndex);

	m_pInvHead->m_filter = val;
	errorcode = m_pInvHead->Load();
	do 
	{
		//连续按两次确认键，则退出开票程序
		if(CheckKey(nKeyTime)==SUCCESS)
		{
			break;
		}
		ret = PrnInvStub(m_pInvHead, 0);		
	} while(ret == SUCCESS);
	
	if (ret != SUCCESS)
	{		
		CaMsgBox::ShowMsg("打印失败");
	}
    return;
//	proBar.SetText("打印结束");
//	proBar.ReFresh();

}*/


/*
UINT8 CInvStubPrnWin::CheckKey(UINT8 &nKeyTime)
{
	INT32 nKey = 0x0;
	if((nKey = ReadKeypad())&&(RETURN_MAIN_MENU == nKey) )
   	{
		nKeyTime++;
		if (nKeyTime==2)
		{
			nKeyTime = 0;
			return SUCCESS;
		}		
   	}
	return FAILURE;
}
*/




