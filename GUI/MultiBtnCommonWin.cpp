#include "MultiBtnCommonWin.h"
#include "VersionConfig.h"
#ifdef WIN32
#include <conio.h>
#endif
#include "stdio.h"

CMultiBtnCommonWin::CMultiBtnCommonWin(int numOfBtn)
{
	DBG_ASSERT_EXIT(numOfBtn >= 0 && numOfBtn <=10, 
		("Num of btn in one win must be between 0~10"));	
	m_iNumOfBtn = numOfBtn;
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pBtn3 = NULL;
	m_pBtn4 = NULL;
	m_pBtn5 = NULL;
	m_pBtn6 = NULL;
	m_pBtn7 = NULL;
	m_pBtn8 = NULL;
	m_pBtn9 = NULL;
	m_pBtn10 = NULL;

/*	add by zl
	for(int i=0; i<MAX_BUTTON_NUM; i++)
	{
		m_pBtn[i] = NULL;
	}
*/
	if (numOfBtn > 0)
	{
		m_pBtnList = new CaButtonList(1);

		//m_pBtnList->Create(0,SCREEN_TOP_OFFSET,SCREEN_W - SCREEN_LEFT_OFFSET,SCREEN_H - SCREEN_TOP_OFFSET);
		m_pBtnList->Create(0,0,SCREEN_W - 2 ,SCREEN_H);
		
	}
	else
	{
		m_pBtnList = NULL;
	}
	
}

CMultiBtnCommonWin::CMultiBtnCommonWin(int numOfBtn, int colCount)
{
	DBG_ASSERT_EXIT(numOfBtn >= 0 && numOfBtn <=10, 
		("Num of btn in one win must be between 0~10"));	
	m_iNumOfBtn = numOfBtn;
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pBtn3 = NULL;
	m_pBtn4 = NULL;
	m_pBtn5 = NULL;
	m_pBtn6 = NULL;
	m_pBtn7 = NULL;
	m_pBtn8 = NULL;
	m_pBtn9 = NULL;
	m_pBtn10 = NULL;

/*	add by zl
	for(int i=0; i<MAX_BUTTON_NUM; i++)
	{
		m_pBtn[i] = NULL;
	}
*/
	if (numOfBtn > 0)
	{
		m_pBtnList = new CaButtonList(colCount);

		//m_pBtnList->Create(0,SCREEN_TOP_OFFSET,SCREEN_W - SCREEN_LEFT_OFFSET,SCREEN_H - SCREEN_TOP_OFFSET);
		m_pBtnList->Create(0,0,SCREEN_W - 2 ,SCREEN_H);
		
	}
	else
	{
		m_pBtnList = NULL;
	}
	
}

int CMultiBtnCommonWin::Create(int iX,int iY,int iW,int iH)
{	
	int curH = SCREEN_TOP_OFFSET;
	if (m_iNumOfBtn <= 0)
	{
		return 0;
	}

	CaWindow::Create(iX , iY, iW , iH);
		
	m_pBtn1 = m_pBtnList->CreateNewButton();
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;

	if (m_iNumOfBtn > 1)
	{
		m_pBtn2 = m_pBtnList->CreateNewButton();
		m_pBtn2->BoxType(CaObject::BOX_NO);
		m_pBtn2->OnObject = S_OnButton2;
	}

	if (m_iNumOfBtn > 2)
	{
		m_pBtn3 = m_pBtnList->CreateNewButton();
		m_pBtn3->BoxType(CaObject::BOX_NO);
		m_pBtn3->OnObject = S_OnButton3;
	}

	if (m_iNumOfBtn > 3)
	{
		m_pBtn4 = m_pBtnList->CreateNewButton();
		m_pBtn4->BoxType(CaObject::BOX_NO);
		m_pBtn4->OnObject = S_OnButton4;
	}

	if (m_iNumOfBtn > 4)
	{
		m_pBtn5 = m_pBtnList->CreateNewButton();
		m_pBtn5->BoxType(CaObject::BOX_NO);
		m_pBtn5->OnObject = S_OnButton5;
	}

	if (m_iNumOfBtn > 5)
	{
		m_pBtn6 = m_pBtnList->CreateNewButton();
		m_pBtn6->BoxType(CaObject::BOX_NO);
		m_pBtn6->OnObject = S_OnButton6;
	}

	if (m_iNumOfBtn > 6)
	{
		m_pBtn7 = m_pBtnList->CreateNewButton();
		m_pBtn7->BoxType(CaObject::BOX_NO);
		m_pBtn7->OnObject = S_OnButton7;
	}

	if (m_iNumOfBtn > 7)
	{
		m_pBtn8 = m_pBtnList->CreateNewButton();
		m_pBtn8->BoxType(CaObject::BOX_NO);
		m_pBtn8->OnObject = S_OnButton8;
	}

	if (m_iNumOfBtn > 8)
	{
		m_pBtn9 = m_pBtnList->CreateNewButton();
		m_pBtn9->BoxType(CaObject::BOX_NO);
		m_pBtn9->OnObject = S_OnButton9;
	}

	if (m_iNumOfBtn > 9)
	{
		m_pBtn10 = m_pBtnList->CreateNewButton();
		m_pBtn10->BoxType(CaObject::BOX_NO);
		m_pBtn10->OnObject = S_OnButton10;
	}


/* by zl
	for (int i=0; i<m_iNumOfBtn; i++)
	{
		m_pBtn[i] = m_pBtnList->CreateNewButton();
		m_pBtn[i]->BoxType(CaObject::BOX_NO);
		m_pBtn[i]->OnObject = S_OnButton;
	}
*/

	m_pBtnList->End();
	this->End();
	return 0;
}

int CMultiBtnCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{

#if (POS_TYPE==POS_APE3000R)

	if (m_iNumOfBtn<10)
	{
		if (iEvent>'0' && iEvent<=m_iNumOfBtn+0x30)
		{
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
		}
	}
	else
	{
		if (iEvent>='0' && iEvent<m_iNumOfBtn+0x30)
		{
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
		}
	}
#else

	//用字母键对应菜单的每个按钮
	switch(iEvent)
	 {
		case LETTER_A:
			iEvent = '1';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_B:
			iEvent = '2';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_C:
			iEvent = '3';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_D:
			iEvent = '4';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_E:
			iEvent = '5';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;			
		case LETTER_F:
			iEvent = '6';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_G:
			iEvent = '7';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_H:
			iEvent = '8';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_I:
			iEvent = '9';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_J:
			iEvent = '0';
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		default:
			break;
	 }
#endif
	
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CMultiBtnCommonWin::ProcShortcut(int iEvent,unsigned char *pEventData, int iDataLen)
{
//	DBG_ASSERT_EXIT(m_pBtnList->m_iColCount == 1, ("m_iColCount must be equal to 1 to use shortcut"));

	int nBtnNumOfCol = 5;//一列最多5行
	if (iEvent >= '0' && iEvent <= m_iNumOfBtn + 0x30)
	{
		if (iEvent == '1')
		{
			m_pBtnList->SetFocusToObj(m_pBtn1, nBtnNumOfCol);			
		}
		else if (iEvent == '2')
		{
			m_pBtnList->SetFocusToObj(m_pBtn2, nBtnNumOfCol);			
		}
		else if (iEvent == '3')
		{
			m_pBtnList->SetFocusToObj(m_pBtn3, nBtnNumOfCol);			
		}
		else if (iEvent == '4')
		{
			m_pBtnList->SetFocusToObj(m_pBtn4, nBtnNumOfCol);			
		}
		else if (iEvent == '5')
		{
			m_pBtnList->SetFocusToObj(m_pBtn5, nBtnNumOfCol);
	// 		CaScrollGroup::SetFocusToObj(const CaObject *targetObj)
		}
		else if (iEvent == '6')
		{
			m_pBtnList->SetFocusToObj(m_pBtn6, nBtnNumOfCol);			
		}
		else if (iEvent == '7')
		{
			m_pBtnList->SetFocusToObj(m_pBtn7, nBtnNumOfCol);			
		}
		else if (iEvent == '8')
		{
			m_pBtnList->SetFocusToObj(m_pBtn8, nBtnNumOfCol);			
		}
		else if (iEvent == '9')
		{
			m_pBtnList->SetFocusToObj(m_pBtn9, nBtnNumOfCol);			
		}
		else if (iEvent == '0')
		{
			m_pBtnList->SetFocusToObj(m_pBtn10, nBtnNumOfCol);			
		}

		m_pBtnList->ReFresh();
		m_pBtnList->CurObj()->OnObject(m_pBtnList->CurObj(), iEvent, pEventData, iDataLen);
		return 1;
	}

	return 0;

}
void CMultiBtnCommonWin::SetTitle(int num, const char *title)
{
	DBG_ASSERT_EXIT(num >= 0 && num <= m_iNumOfBtn,("Num is invalid:%d; %d", m_iNumOfBtn, num));
	
	switch(num)
	{
	case 1:
		{
			m_pBtn1->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn1->SetTitle(title, strlen(title));
		}
		break;
	case 2:
		{
			m_pBtn2->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn2->SetTitle(title, strlen(title));
		}
		break;
	case 3:
		{
			m_pBtn3->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn3->SetTitle(title, strlen(title));
		}
	    break;
	case 4:
		{
			m_pBtn4->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn4->SetTitle(title, strlen(title));
		}
	    break;
	case 5:
		{
			m_pBtn5->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn5->SetTitle(title, strlen(title));
		}
		break;
	case 6:
		{
			m_pBtn6->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn6->SetTitle(title, strlen(title));
		}
		break;
	case 7:
		{
			m_pBtn7->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn7->SetTitle(title, strlen(title));
		}
		break;
	case 8:
		{
			m_pBtn8->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn8->SetTitle(title, strlen(title));
		}
		break;
	case 9:
		{
			m_pBtn9->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn9->SetTitle(title, strlen(title));
		}
		break;
	case 0:
		{
			m_pBtn10->SetTitleAlign(CaObject::ALIGN_LEFT);
			m_pBtn10->SetTitle(title, strlen(title));
		}
		break;
	default:
	    break;
	}

//	m_pBtn[num]->SetTitleAlign(CaObject::ALIGN_LEFT);
//	m_pBtn[num]->SetTitle(title, strlen(title));

}

void CMultiBtnCommonWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton4(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton5(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton6(iEvent,pEventData,iDataLen);
}
void CMultiBtnCommonWin::S_OnButton7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton7(iEvent,pEventData,iDataLen);
}
void CMultiBtnCommonWin::S_OnButton8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton8(iEvent,pEventData,iDataLen);
}
void CMultiBtnCommonWin::S_OnButton9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton9(iEvent,pEventData,iDataLen);
}
void CMultiBtnCommonWin::S_OnButton10(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();
	win->OnButton10(iEvent,pEventData,iDataLen);
}

void CMultiBtnCommonWin::S_OnButton(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnCommonWin *win=(CMultiBtnCommonWin *)obj->GetdWin();

/*	add by zl
	for (int i=0; i<MAX_BUTTON_NUM; i++)
	{
//		if (m_pBtn1 != NULL && m_pBtn1->IsCurObj())
			break;
	}
	win->OnButton(iEvent,pEventData,iDataLen);
*/
}
void CMultiBtnCommonWin::OnButton(int iEvent, unsigned char * pEventData, int iDataLen)
{
}


void CMultiBtnCommonWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnCommonWin::OnButton10(int iEvent, unsigned char * pEventData, int iDataLen)
{
}





