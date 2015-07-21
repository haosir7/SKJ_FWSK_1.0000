#include "MultiBtnNewCommonWin.h"
#include "VersionConfig.h"
#ifdef WIN32
#include <conio.h>
#endif
#include "stdio.h"



CMultiBtnNewCommonWin::CMultiBtnNewCommonWin(unsigned int colCount)
{
	DBG_ASSERT_EXIT(colCount >= 0 && colCount <=2, 
		("Num of col in one win must be between 1~2"));

	m_iNumOfCol=colCount;
	m_nNum=0; 	
	
	for(int i=0; i<MAX_BUTTON_NUM; i++)
	{
		m_pBtn[i] = NULL;
	}
	
    for(int j=0; j<MAX_BUTTON_NUM; j++)
	{
		m_pTitle[j] = new char[MAX_TITLE_NUM +1];
	}
}

CMultiBtnNewCommonWin::~CMultiBtnNewCommonWin()
{	
    for (int i = 0; i < MAX_BUTTON_NUM; i++)
	{
		if (m_pTitle[i] != NULL)
		{
			delete []m_pTitle[i];
			m_pTitle[i] = NULL;
		}
		
	}	
	
}

void CMultiBtnNewCommonWin::SetTitle(const char *title)
{
	m_nNum++; // 菜单个数
	printf("m_nNum= %u \n",m_nNum);
	
	DBG_ASSERT_EXIT(m_nNum >= 1 && m_nNum <=10, 
		("Num of title in one win must be equal or lesser than 10"));

#if(POS_TYPE == POS_APE3000R)
	if (m_nNum ==10)
	{
		sprintf(m_pTitle[m_nNum-1],"%d.",0); //显示字母
	}
	else
	{
		sprintf(m_pTitle[m_nNum-1],"%d.",m_nNum); //显示字母
	}
#else
	sprintf(m_pTitle[m_nNum-1],"%c.",m_nNum+0x40); //显示字母
#endif
	strcat (m_pTitle[m_nNum-1], title);
	
	printf("%s \n", m_pTitle[m_nNum-1]);
	
	return;	
}

int CMultiBtnNewCommonWin::Create(int iX,int iY,int iW,int iH)
{	
	printf("iX= %d,iY= %d,iW= %d,iH= %d\n",iX,iY,iW,iH);

	if (m_iNumOfCol ==0)
	{
		m_iNumOfCol =(m_nNum >5)? 2:1;
	}
	
	m_pBtnList = new CaButtonList(m_iNumOfCol);
	
	m_pBtnList->Create(0,0,SCREEN_W - 2 ,SCREEN_H);	
	CaWindow::Create(iX , iY, iW , iH);
	
	return 0;
}

void CMultiBtnNewCommonWin::SetButton()
{
	printf("m_nNum= %u \n",m_nNum);
	
	for (int i=0; i< m_nNum;i++)
	{
		m_pBtn[i] = m_pBtnList->CreateNewButton();
		m_pBtn[i]->BoxType(CaObject::BOX_NO);
		m_pBtn[i]->SetTitleAlign(CaObject::ALIGN_LEFT);
		m_pBtn[i]->SetTitle(m_pTitle[i], strlen(m_pTitle[i]));
		
		if (i == 0)
		{
			m_pBtn[i]->OnObject = S_OnButton1;
		}
		if (i == 1)
		{
			m_pBtn[i]->OnObject = S_OnButton2;
		}
		
		if (i == 2)
		{
			m_pBtn[i]->OnObject = S_OnButton3;
		}
		
		if (i == 3)
		{
			m_pBtn[i]->OnObject = S_OnButton4;
		}
		
		if (i == 4)
		{
			m_pBtn[i]->OnObject = S_OnButton5;
		}
		
		if (i == 5)
		{
			m_pBtn[i]->OnObject = S_OnButton6;
		}
		
		if (i == 6)
		{
			m_pBtn[i]->OnObject = S_OnButton7;
		}
		
		if (i == 7)
		{
			m_pBtn[i]->OnObject = S_OnButton8;
		}
		
		if (i == 8)
		{
			m_pBtn[i]->OnObject = S_OnButton9;
		}
		
		if (i == 9)
		{
			m_pBtn[i]->OnObject = S_OnButton10;
		}
		
	}
	
	m_pBtnList->End();
	this->End();
	
	return ;	
}


int CMultiBtnNewCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	
	printf("iEvent= %0x \n",iEvent);
	printf("m_nNum= %0x \n",m_nNum);
	
#if (POS_TYPE==POS_APE3000R)
	
	//用字母键对应菜单的每个按钮
	if ((iEvent >= 0x30)&&(iEvent<=m_nNum+0x30))
	{
		switch(iEvent)
		{
		case DIGIT1: 
			iEvent = 1;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT2:
			iEvent = 2;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT3:
			iEvent = 3;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT4:
			iEvent = 4;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT5:
			iEvent = 5;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;			
		case DIGIT6:
			iEvent = 6;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT7:
			iEvent = 7;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT8:
			iEvent = 8;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT9:
			iEvent = 9;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case DIGIT0:
			if (m_nNum ==10)
			{
				iEvent = 10;
				ProcShortcut(iEvent,pEventData,iDataLen);
				return 1;
			}
			break;
		default:
			break;
		}
	}
#else //POS_TYPE
	
	if ((iEvent >= 0x90)&&(iEvent<m_nNum+0x90))
	{
		switch(iEvent)
		{
		case LETTER_A:
			iEvent = 1;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_B:
			iEvent = 2;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_C:
			iEvent = 3;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_D:
			iEvent = 4;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_E:
			iEvent = 5;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;			
		case LETTER_F:
			iEvent = 6;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_G:
			iEvent = 7;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_H:
			iEvent = 8;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_I:
			iEvent = 9;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		case LETTER_J:
			iEvent = 10;
			ProcShortcut(iEvent,pEventData,iDataLen);
			return 1;
			break;
		default:
			break;
		}
	}
	
#endif //POS_TYPE
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CMultiBtnNewCommonWin::ProcShortcut(int iEvent,unsigned char *pEventData, int iDataLen)
{
	
	int nBtnNumOfCol = 5;//一列最多5行
	
	printf("ProcShortcut  iEvent= %0x\n",iEvent);
	
	if (iEvent > 0 && iEvent <= MAX_BUTTON_NUM)
	{
		m_pBtnList->SetFocusToObj(m_pBtn[iEvent-1], nBtnNumOfCol);
		m_pBtnList->ReFresh();
		m_pBtnList->CurObj()->OnObject(m_pBtnList->CurObj(), iEvent, pEventData, iDataLen);
		return 1;
	}
	
	return 0;
	
}

void CMultiBtnNewCommonWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton4(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton5(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton6(iEvent,pEventData,iDataLen);
}
void CMultiBtnNewCommonWin::S_OnButton7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton7(iEvent,pEventData,iDataLen);
}
void CMultiBtnNewCommonWin::S_OnButton8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton8(iEvent,pEventData,iDataLen);
}
void CMultiBtnNewCommonWin::S_OnButton9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton9(iEvent,pEventData,iDataLen);
}
void CMultiBtnNewCommonWin::S_OnButton10(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	win->OnButton10(iEvent,pEventData,iDataLen);
}

void CMultiBtnNewCommonWin::S_OnButton(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiBtnNewCommonWin *win=(CMultiBtnNewCommonWin *)obj->GetdWin();
	
	/*	add by zl
	for (int i=0; i<MAX_BUTTON_NUM; i++)
	{
	//		if (m_pBtn1 != NULL && m_pBtn1->IsCurObj())
	break;
	}
	win->OnButton(iEvent,pEventData,iDataLen);
	*/
}
void CMultiBtnNewCommonWin::OnButton(int iEvent, unsigned char * pEventData, int iDataLen)
{
}


void CMultiBtnNewCommonWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{
}
void CMultiBtnNewCommonWin::OnButton10(int iEvent, unsigned char * pEventData, int iDataLen)
{
}





