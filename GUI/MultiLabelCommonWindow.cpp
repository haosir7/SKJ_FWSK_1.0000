#include "MultiLabelCommonWindow.h"
#ifdef WIN32
#include <conio.h>
#endif
#include "stdio.h"


CMultiLabelCommonWin::CMultiLabelCommonWin(int numOfBtn)
{
	DBG_ASSERT_EXIT(numOfBtn >= 0 && numOfBtn <=8, 
		("Num of btn in one win must be between 0~8"));	
	m_iNumOfBtn = numOfBtn;
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;

	if (numOfBtn > 0)
	{
		m_pBtnList = new CaButtonList(1);
		m_pBtnList->Create(0,0,SCREEN_W ,SCREEN_H-LINE_H+1);
	}
	else
	{
		m_pBtnList = NULL;
	}
	
}

int CMultiLabelCommonWin::Create(int iX,int iY,int iW,int iH)
{	
	int i=0;
	int curH = SCREEN_TOP_OFFSET;
//	CaButton *pBtn;
	if (m_iNumOfBtn <= 0)
	{
		return 0;
	}

	CaWindow::Create(iX , iY, iW , iH);
		
	for (i=0;i<m_iNumOfBtn;i++)
	{
		m_pBtn[i] = m_pBtnList->CreateNewButton();
		m_pBtn[i]->BoxType(CaObject::BOX_NO);

	}
	m_pBtnList->End();

	int m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	int m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	int left_offset = SCREEN_LEFT_OFFSET+8;
	int  leftoffset_btn = left_offset + 14;

	m_pBtn1 = new CaButton();
//	m_pBtn1->Create(20,SCREEN_H-LINE_H+1,40,LINE_H-1);
	m_pBtn1->Create(leftoffset_btn,SCREEN_H-LINE_H+1,m_iBtnW,LINE_H-1);
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;

	m_pBtn2 = new CaButton();
//	m_pBtn2->Create(120,SCREEN_H-LINE_H+1,40,LINE_H-1);
	m_pBtn2->Create(leftoffset_btn+m_iColW,SCREEN_H-LINE_H+1,m_iBtnW,LINE_H-1);
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;

	this->End();
	return 0;
}
int CMultiLabelCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{

//	ProcShortcut(iEvent,pEventData,iDataLen);
	if(iEvent == TAB_KEY)
	{
		this->OnDownKey();
		LCDRedraw();
		return 1;
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


void CMultiLabelCommonWin::SetTitle(int num, const char *title)
{
	DBG_ASSERT_EXIT(num > 0 && num <= (10),("Num is invalid"));
	

	

	if (num > 0 && num <= 8)
		{
				m_pBtn[num-1]->SetTitleAlign(CaObject::ALIGN_LEFT);
				m_pBtn[num-1]->SetTitle(title, strlen(title));
		}
	

	switch(num)
	{

	   case 9:
			{
				m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
				m_pBtn1->SetTitle(title, strlen(title));
			}
			break;
		case 10:
			{
				m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
				m_pBtn2->SetTitle(title, strlen(title));
			}
			break;
		default:

			break;
	}
}

void CMultiLabelCommonWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiLabelCommonWin *win=(CMultiLabelCommonWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CMultiLabelCommonWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiLabelCommonWin *win=(CMultiLabelCommonWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}


void CMultiLabelCommonWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiLabelCommonWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
}

void CMultiLabelCommonWin::SetBtnFocus()
{
	m_pBtn1->SetFocus();
}



