#include "CaLabel.h"
#include "MultiInputCommonWin.h"
#include "CaMsgBox.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

CMultiInputCommonWin::CMultiInputCommonWin(int numOfInput)
{
	DBG_ASSERT_EXIT(numOfInput >= 0 && numOfInput <=9, 
		("Num of Input in one win must be between 0~9"));	
	m_iNumOfInput = numOfInput;
	m_pInput1 = NULL;
	m_pInput2 = NULL;
	m_pInput3 = NULL;
	m_pInput4 = NULL;
	m_pInput5 = NULL;
	m_pInput6 = NULL;
	m_pInput7 = NULL;
	m_pInput8 = NULL;
	m_pInput9 = NULL;

	m_pBtn1=NULL;
    m_pBtn2=NULL;
	m_pBtn3=NULL;

	m_pInputList = NULL;
}
	
CMultiInputCommonWin::~CMultiInputCommonWin()
{
//	if (m_pInput1 != NULL){delete m_pInput1;}
//	if (m_pInput2 != NULL){delete m_pInput2;}
//	if (m_pInput3 != NULL){delete m_pInput3;}
//	if (m_pInput4 != NULL){delete m_pInput4;}
//	if (m_pInput5 != NULL){delete m_pInput5;}
//	if (m_pInput6 != NULL){delete m_pInput6;}
//	if (m_pInput7 != NULL){delete m_pInput7;}
//	if (m_pInput8 != NULL){delete m_pInput8;}
//
//	if (m_pBtn1 != NULL){delete m_pBtn1;}
//
//	if (m_pBtn2 != NULL){delete m_pBtn2;}
//
//	if (m_pBtn3 != NULL){delete m_pBtn3;}
//
//	if (m_pInputList != NULL){delete m_pInputList;}
//	
//	m_pInput1 = NULL;
//	m_pInput2 = NULL;
//	m_pInput3 = NULL;
//	m_pInput4 = NULL;
//	m_pInput5 = NULL;
//	m_pInput6 = NULL;
//	m_pInput7 = NULL;
//	m_pInput8 = NULL;
//
//	m_pBtn1=NULL;
//    m_pBtn2=NULL;
//	m_pBtn3=NULL;
//
//	m_iNumOfInput=0;
//	m_iNumOfBtn = 0;
// 	m_pInputList = NULL;
}

int CMultiInputCommonWin::Create(int iX,int iY,int iW,int iH)
{	
	int curH = SCREEN_TOP_OFFSET;

	if (m_iNumOfInput <= 0)
	{
		return 0;
	}

	CaWindow::Create(iX , iY, iW , iH);
		
	m_pInputList = new CaInputList(1, m_iNumOfBtn);
	m_pInputList->Create(0, SCREEN_TOP_OFFSET+2, SCREEN_W - CHAR_W, SCREEN_H - SCREEN_TOP_OFFSET);
		
	m_pInput1 = m_pInputList->CreateNewInput();
//	m_pInput1->SetMaxLen(16);

	m_pInput1->OnObject = S_OnInput1;

	if (m_iNumOfInput > 1)
	{
		m_pInput2 = m_pInputList->CreateNewInput();
//		m_pInput2->SetMaxLen(100);
		m_pInput2->OnObject = S_OnInput2;
	}

	if (m_iNumOfInput > 2)
	{
		m_pInput3 = m_pInputList->CreateNewInput();
//		m_pInput3->SetMaxLen(6);
		m_pInput3->OnObject = S_OnInput3;
	}

	if (m_iNumOfInput > 3)
	{
		m_pInput4 = m_pInputList->CreateNewInput();
//		m_pInput4->SetMaxLen(20);
		m_pInput4->OnObject = S_OnInput4;
	}

	if (m_iNumOfInput > 4)
	{
		m_pInput5 = m_pInputList->CreateNewInput();
//		m_pInput5->SetMaxLen(40);
		m_pInput5->OnObject = S_OnInput5;
	}

	if (m_iNumOfInput > 5)
	{
		m_pInput6 = m_pInputList->CreateNewInput();
//		m_pInput6->SetMaxLen(32);
		m_pInput6->OnObject = S_OnInput6;
	}

	if (m_iNumOfInput > 6)
	{
		m_pInput7 = m_pInputList->CreateNewInput();
		m_pInput7->OnObject = S_OnInput7;
//		m_pInput7->SetMaxLen(20);
	}

	if (m_iNumOfInput > 7)
	{
		m_pInput8 = m_pInputList->CreateNewInput();
//		m_pInput8->SetMaxLen(1);
		m_pInput8->OnObject = S_OnInput8;
	}

	if (m_iNumOfInput > 8)
	{
		m_pInput9 = m_pInputList->CreateNewInput();
		//m_pInput9->SetMaxLen(1);
		m_pInput9->OnObject = S_OnInput9;
	}

	if (m_iNumOfBtn > 0)
	{
		m_pBtn1 = m_pInputList->CreateNewButton();
		m_pBtn1->OnObject = S_OnBtn1;
	}

	if (m_iNumOfBtn > 1)
	{
		m_pBtn2 = m_pInputList->CreateNewButton();
		m_pBtn2->OnObject = S_OnBtn2;
	}
	
	if (m_iNumOfBtn > 2)
	{
		m_pBtn3 = m_pInputList->CreateNewButton();
		m_pBtn3->OnObject = S_OnBtn3;
	}
		
	m_pInputList->End();
	this->End();
	this->ReFresh();
	return 0;
}

int CMultiInputCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	DBG_PRN("info",("iEvent = %u", iEvent));
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::SetObjPara(int num, const char *title, CaInput::TYPE inputtype,int len)
{
	DBG_ASSERT_EXIT((num==10 || num==11 || num==12)||(num >= 0 && num <= m_iNumOfInput),("Num is invalid"));
	
	switch(num)
	{
	case 10:
		{		
			m_pBtn1->SetTitle(title, strlen(title));
			//m_pBtn1->BoxType(BOX_ALL);
		}
		break;
	case 11:
		{		
			m_pBtn2->SetTitle(title, strlen(title));
			//m_pBtn2->BoxType(BOX_ALL);
		}
		break;
	case 12:
		{		
			m_pBtn3->SetTitle(title, strlen(title));
			//m_pBtn3->BoxType(BOX_ALL);
		}
		break;
		
	case 1:
		{
			m_pInput1->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pInput1->SetTitle(title, strlen(title));
			m_pInput1->SetMaxLen(len);
			m_pInput1->m_InputType=inputtype;
			DBG_PRN("info",("m_pInput1:%d",(int)m_pInput1->m_pLabel));
		}
		break;
	case 2:
		{
			m_pInput2->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput2->SetTitle(title, strlen(title));
			m_pInput2->SetMaxLen(len);
			m_pInput2->m_InputType=inputtype;
			DBG_PRN("info",("m_pInput2:%d",(int)m_pInput2->m_pLabel));
		}
		break;
	case 3:
		{
			m_pInput3->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput3->SetTitle(title, strlen(title));
			m_pInput3->SetMaxLen(len);
			m_pInput3->m_InputType=inputtype;
		}
	    break;
	case 4:
		{
			m_pInput4->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput4->SetTitle(title, strlen(title));
			m_pInput4->SetMaxLen(len);
			m_pInput4->m_InputType=inputtype;
		}
	    break;
	case 5:
		{
			m_pInput5->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput5->SetTitle(title, strlen(title));
			m_pInput5->SetMaxLen(len);
			m_pInput5->m_InputType=inputtype;
		}
		break;
	case 6:
		{
			m_pInput6->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput6->SetTitle(title, strlen(title));
			m_pInput6->SetMaxLen(len);
			m_pInput6->m_InputType=inputtype;
		}
		break;
	case 7:
		{
			m_pInput7->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput7->SetTitle(title, strlen(title));
			m_pInput7->SetMaxLen(len);
			m_pInput7->m_InputType=inputtype;
		}
		break;
	case 8:
		{
			m_pInput8->SetTitleAlign(CaObject::ALIGN_LEFT);
		 	m_pInput8->SetTitle(title, strlen(title));
			m_pInput8->SetMaxLen(len);
			m_pInput8->m_InputType=inputtype;
		}
		break;
	case 9:
		{
			m_pInput9->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pInput9->SetTitle(title, strlen(title));
			m_pInput9->SetMaxLen(len);
			m_pInput9->m_InputType=inputtype;
		}
		break;
	default:
	    break;
	}

}

void CMultiInputCommonWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput2(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput3(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput4(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput5(iEvent,pEventData,iDataLen);
	
}

void CMultiInputCommonWin::S_OnInput6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput6(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput7(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput8(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnInput9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnInput9(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnBtn1(CaObject *obj, int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnBtn1(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnBtn2(CaObject *obj, int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnBtn2(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::S_OnBtn3(CaObject *obj, int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiInputCommonWin *win=(CMultiInputCommonWin *)obj->GetdWin();
	win->OnBtn3(iEvent,pEventData,iDataLen);
}

void CMultiInputCommonWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput5(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput6(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput7(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput8(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnInput9(int iEvent, unsigned char * pEventData, int iDataLen)
{
}

void CMultiInputCommonWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiInputCommonWin::OnBtn3(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
int CMultiInputCommonWin::ClearContentBuf(int n)
{
	switch(n) {
	case 9:
		m_pInput9->Clear();
	case 8:
		m_pInput8->Clear();
	case 7:
		m_pInput7->Clear();
	case 6:
		m_pInput6->Clear();
	case 5:
		m_pInput5->Clear();
	case 4:
		m_pInput4->Clear();
	case 3:
		m_pInput3->Clear();
	case 2:
		m_pInput2->Clear();
	case 1:
		m_pInput1->Clear();
	default:
		return 0;
		break;
	}
	return 1;
}
void CMultiInputCommonWin::SetFocusToObj(const CaObject *targetObj)
{
	m_pInputList->SetFocusToObj(targetObj);
}

void CMultiInputCommonWin::ReSetFocus()
{
	m_pInputList->ReSetFocus();
	//input and btn both inputlist
}



