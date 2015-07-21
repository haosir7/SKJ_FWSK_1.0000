#include "FourLabelsCommonWin.h"

CFourLabelsCommonWin::CFourLabelsCommonWin(int numOfLabels)
{
	DBG_ASSERT_EXIT(numOfLabels >= 0, 
		("Num of labels in one win can't be less than 0"));

	m_iTotalLine = numOfLabels;
	m_pLabel1 = NULL;
	m_pLabel2 = NULL;
	m_pLabel4 = NULL;
	m_pLabel3 = NULL;
}

int CFourLabelsCommonWin::Create(int iX,int iY,int iW,int iH)
{	
	int curH;
	CaWindow::Create(iX , iY, iW , iH);
	if (m_iTotalLine > 0)
	{
		m_pLabel1 = new CaLabel(false,CaObject::ALIGN_LEFT);
		curH = SCREEN_TOP_OFFSET;
		m_pLabel1->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
	}
	if (m_iTotalLine > 1)
	{
		m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
		curH += LINE_H;
		m_pLabel2->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
	}
	if (m_iTotalLine > 2)
	{
		m_pLabel3 = new CaLabel(false,CaObject::ALIGN_LEFT);
		curH += LINE_H;
		m_pLabel3->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
	}
	if (m_iTotalLine > 3)
	{
		m_pLabel4 = new CaLabel(false,CaObject::ALIGN_LEFT);
		curH += LINE_H;
		m_pLabel4->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
	}

	this->End();
	return 0;
}
int CFourLabelsCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

void CFourLabelsCommonWin::SetTitle(int num, const char *title)
{
	DBG_ASSERT_EXIT(num > 0 && num <= m_iTotalLine,("Num %d is invalid", num));
	switch(num)
	{
	case 1:
		{
			m_pLabel1->SetTitle(title, strlen(title));
		}
		break;
	case 2:
		{
			m_pLabel2->SetTitle(title, strlen(title));
		}
		break;
	case 3:
		{
			m_pLabel3->SetTitle(title, strlen(title));
		}
	    break;
	case 4:
		{
			m_pLabel4->SetTitle(title, strlen(title));
		}
	    break;
	default:
	    break;
	}
}

