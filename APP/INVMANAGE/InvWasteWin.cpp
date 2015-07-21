/*! \file    InvWasteWin.cpp
\brief    awe4000r发票管理档的空白发票作废界面
\author   Yu Yan 
\version  1.0
\date     2007-11-28
*/

#include "InvWasteWin.h"

//#include "VolSumInfoWin.h"
#include "CaProgressBar.h"
#include <string>
#include "CaMsgBox.h"
#include "InvManageFunc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CInvWasteWin::CInvWasteWin():CaWindow()
{
	
}

CInvWasteWin::~CInvWasteWin()
{
	
}

int CInvWasteWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 32) ;	//Label的宽度
	m_iColW = m_iBtnW + 4;			//Label的列宽
	
	m_pFrame->RegsiterWin(this,INV_WASTE_WIN);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	//创建一个标签  第一行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH = SCREEN_TOP_OFFSET;
	label[1]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	
	//创建一个标签  第二行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[2]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	
	//创建一个标签  第三行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	sprintf(title_array[3], "");
	curH += LINE_H;
	label[3]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	
	//创建一个Input  第四行
	strcpy(title, "作废份数:");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(16);
	m_pInput2->m_InputType = m_pInput2->aINT; //该输入框只接受整型值
	m_pInput2->OnObject = S_OnInput2;
	m_pInput2->Visible(false); //不可见
	
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
	m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	
	//创建一个Button  第五行
	strcpy(title, "作废");
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
	m_pBtn2->Create(leftoffset_btn + m_iColW ,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;
	
	OnActive = S_OnActive;
	
	this->End();  
	return 0;
}

int CInvWasteWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	UINT32 ii;
	char *content;
	
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ChangeWin(INV_WASTE_MENU);			
		return SUCCESS;
		break;
//#if (RELEASE_VER == 0)
//		case DISCOUNT_KEY:
//			DBG_PRINT(("进入DISCOUNT_KEY"));
//			UINT8 nIfInvSum;
//			AutoIssue(nIfInvSum);	
//			break;
//#endif
	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

void CInvWasteWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWasteWin *win=(CInvWasteWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CInvWasteWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWasteWin *win=(CInvWasteWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}


void CInvWasteWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CInvWasteWin *win=(CInvWasteWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CInvWasteWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); //切换焦点到下一个控件
	return;		
}

void CInvWasteWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{

	UINT8 ret=SUCCESS;

	//检查是否满足空白条件
//	ret=INVM_CheckWaste();
//	DBG_PRINT(("ret= %u",ret));
//	if (ret !=SUCCESS)
//	{
//		INVM_ErrMsgBox(ret);
//		return ;
// 	}

    //空白作废
	ret =INVM_InvWasteProc(); 
	DBG_PRINT(("ret= %u",ret ));

	if (ret != SUCCESS)
	{
		INVM_ErrMsgBox(ret);
		DBG_PRINT(("发票作废失败"));
		return;
	}

	CaMsgBox::ShowMsg("发票作废成功");
	DBG_PRINT(("发票作废成功"));

   //ChangeWin(INV_WASTE_MENU);	 //返回发票作废界面				
   return;
   
}


void CInvWasteWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	
	ChangeWin(INV_WASTE_MENU);			
	
}


int CInvWasteWin::ReFresh()
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



void CInvWasteWin::S_OnActive(CaWindow *obj)
{
	((CInvWasteWin *)obj)->DoActive();
}


void CInvWasteWin::DoActive()
{
	DBG_PRINT(("进入CInvWasteWin::DoActive函数"));
	INT32 nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	DBG_PRINT(("nCurrentInvNo = %u",nCurrentInvNo));
	
	string strErr;
	UINT8 ret =SUCCESS;
	UINT8 uFlag=0;
	if (0 == g_globalArg->m_curInvVol->m_curInvNo) //避免反复读盘
	{
       ret = SALE_GetCurInv(g_globalArg->m_curInvVol,strErr);
 		if (ret ==FAILURE)
 		{
 			uFlag=1;
			if (strErr.length()>OBJ_TITLE_MAX_LEN)
			{
				strErr = strErr.substr(0,OBJ_TITLE_MAX_LEN);
			}
			sprintf(title_array[1], strErr.c_str()); //错误
			sprintf(title_array[2], "作废发票代码: 空");
			sprintf(title_array[3], "作废发票号码: 空");	
 		} 
		
	}

	if (0 ==uFlag)
	{
		sprintf(title_array[1], "");
		sprintf(title_array[2], "作废发票代码:%s ", g_globalArg->m_curInvVol->m_code.c_str());
		sprintf(title_array[3], "作废发票号码:%08u", g_globalArg->m_curInvVol->m_curInvNo);	
	}

	
	ChangeTitle();
	m_pBtn1->SetFocus();
	ReFresh();
	DBG_PRINT(("退出CInvWasteWin::DoActive函数"));
}

UINT8 CInvWasteWin::ChangeTitle()
{
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	
	return SUCCESS;
}
