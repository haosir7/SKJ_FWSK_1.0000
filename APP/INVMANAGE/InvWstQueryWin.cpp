/*! \file    InvWstQueryWin.cpp
   \brief    awe4000r发票管理档的已开发票作废查询界面
   \author   Yu Yan 
   \version  1.0
   \date     2008-2-28
 */


#include "InvWstQueryWin.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CInvWstQueryWin::CInvWstQueryWin():CaWindow()
{
	m_ret = 0;
	m_code = "";
	m_invStartNo = 0;
	m_invEndNo = 0;
	
}

CInvWstQueryWin::~CInvWstQueryWin()
{
	
}


int CInvWstQueryWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,INV_WST_QUERY_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个Input  第一行
	strcpy(title, "发票代码:");
	titleLen = strlen(title) * CHAR_W;
	curH = SCREEN_TOP_OFFSET;
	curH += LINE_H;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(12);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;
	
	//创建一个Input  第二行
	strcpy(title, "发票号码:");
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
	strcpy(title, "查询");
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

int CInvWstQueryWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  
		ChangeWin(INV_WASTE_MENU);			
		return SUCCESS;
		break;
		
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CInvWstQueryWin::ReFresh()
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

UINT8 CInvWstQueryWin::ErrMsgBox(UINT8 ret)
{
	string pText;
	switch(ret)
	{
		case CODE_NO_ERROR:
		    pText = "未使用过该发票代码";
			break;
		case NO_MATCHED_INV:
		    pText = "无匹配的发票存根";
			break;
    	case NO_END_INV:
		pText = "请输入发票截止号码";
			break;
		case NO_INV_NO:
		    pText = "请输入发票号码";
			break;
		case QUERY_ERROR:
			 pText = "查询数据库失败";
			break;
		case HAVE_FU_INV:
			pText = "该票已充红";
			break;
		default:;
	}  

	if (FAILURE != ret)
	{
		CaMsgBox::ShowMsg(pText);
	}
	
	return FAILURE;	
}



void CInvWstQueryWin::S_OnActive(CaWindow *obj)
{
	((CInvWstQueryWin *)obj)->DoActive();
}


void CInvWstQueryWin::DoActive()
{
	m_pInput1->Clear();//清除Input里的内容
	m_pInput2->Clear();
	m_code = "";
	m_invStartNo = 0;
	m_invEndNo = 0;

	m_pInput1->SetContentBuf((UINT8*)g_globalArg->m_curInvVol->m_code.c_str(), g_globalArg->m_curInvVol->m_code.length());

	ReFresh();	
}



void CInvWstQueryWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWstQueryWin *win=(CInvWstQueryWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CInvWstQueryWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWstQueryWin *win=(CInvWstQueryWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CInvWstQueryWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWstQueryWin *win=(CInvWstQueryWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CInvWstQueryWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWstQueryWin *win=(CInvWstQueryWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvWstQueryWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CInvWstQueryWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CInvWstQueryWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 ret = CheckInput();
	if(ret == SUCCESS)
	{
		ChangeWin(INV_WST_WIN);	 //已开发票作废界面
	}
	else
	{
		INVM_ErrMsgBox(ret);
	//	ErrMsgBox(ret);
	}
	
}

void CInvWstQueryWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(INV_WASTE_MENU);	 //返回发票管理界面		
}

UINT8 CInvWstQueryWin::CheckInput(void)
{
	DBG_PRINT(("CheckInput begin"));
	
	UINT8 ret = SUCCESS;
	char *content1, *content2, *content3;
	INT32 tmp;

	content1 = (char*)(m_pInput1->m_contentBuf); 
	content2 = (char*)(m_pInput2->m_contentBuf);
	//content3 = (char*)(m_pInput2->m_contentBuf);

	//未输入发票号码
	if (strlen(content2)==0)
	{
		m_pInput2->SetFocus();
		return(NO_INV_NO);
	}

	m_invStartNo = atoi(content2);
	m_invEndNo = atoi(content2);

	//未输入发票代码
	if (strlen(content1)==0)
	{
		m_pInput2->SetFocus();
		return(CODE_NO_ERROR);
	}

	//未输入发票代码
// 	if(strlen(content1)==0)
// 	{
// 		m_code = g_globalArg->m_curInvVol->m_code;	
// 		DBG_PRINT(("g_globalArg->m_curInvVol->m_code = %s", g_globalArg->m_curInvVol->m_code.c_str()));
// 	}
// 	else //检查发票代码合法性
// 	{

		m_code = content1;
//	}

	//检查是否有符合条件的发票存根
	UINT32 nTmpNum = 0;
	CInvHead invHead;
	DBG_PRINT(("m_code = %s", m_code.c_str()));
//	ret = GetInvNum(m_code, m_invStartNo, m_invEndNo, nTmpNum);
	ret = INVM_CheckIsNoDB(m_code, m_invStartNo,&invHead);
	if(ret != SUCCESS)
	{
		return ret;
	}
// 	if (ret==FAILURE)
// 	{
// 		return(QUERY_ERROR);		
// 	}
// 	else if((ret==SUCCESS)&&(nTmpNum == 0))
// 	{
// 		return(NO_MATCHED_INV);		
// 	}

	return SUCCESS;
}

