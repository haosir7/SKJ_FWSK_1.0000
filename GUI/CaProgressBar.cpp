#include "CaProgressBar.h"
#include "StringWrap.h"

#include"LOGCTRL.h"
#include"pos_debug.h"

CaProgressBar::CaProgressBar(const char *text,bool withBar):CaWindow()
{
	m_iType = CA_PROGRESSBAR;
	m_iBoxType = BOX_ALL;	

	if (withBar)
	{
		Create(PROGRESSBAR_X,PROGRESSBAR_Y,PROGRESSBAR_W,PROGRESSBAR_H*2);
		m_pBar = new CaLabelBar();
		m_pBar->Create(2,4,PROGRESSBAR_W - 4,GRID_LINE_H / 2);
		m_pLabel = new CaLabel(false, ALIGN_CENTER);
 		m_pLabel->Create(0, (GRID_LINE_H + 2) * 2, PROGRESSBAR_W, CHAR_H);
 		m_pLabel->SetTitle(text, strlen(text));
	}
	else
	{
		int maxwidth = (PROGRESSBAR_W - 1 * WORD_W) / CHAR_W;
		CStringWrap strwrap(text,maxwidth);
		strwrap.Wrap();
		
		if (strwrap.Lines() > 1)
		{//多行的话，就增加高度
			int h = PROGRESSBAR_H + (GRID_LINE_H + 1)*(strwrap.Lines() - 1);
			h = h < SCREEN_H ? h:(SCREEN_H * 5 / 6);
			Create(PROGRESSBAR_X,PROGRESSBAR_Y / strwrap.Lines(),
				   PROGRESSBAR_W,h);
			for(int i=0;i<strwrap.Lines();i++)
			{
				m_pLabel = new CaLabel(false, ALIGN_CENTER);
 				m_pLabel->Create(0, 4 + (GRID_LINE_H + 1) * i, PROGRESSBAR_W, CHAR_H);
				DBG_PRN("info",("line[%d]=%s",i,strwrap.Line(i)));
 				m_pLabel->SetTitle(strwrap.Line(i), strlen(strwrap.Line(i)));
			}
		}
		else
		{
			Create(PROGRESSBAR_X,PROGRESSBAR_Y,PROGRESSBAR_W,PROGRESSBAR_H);
			m_pLabel = new CaLabel(false, ALIGN_CENTER);
 			m_pLabel->Create(0, 4, PROGRESSBAR_W, CHAR_H);
 			m_pLabel->SetTitle(text, strlen(text));
		}
		
		m_pBar = NULL;			
	}
	End();
}

CaProgressBar::~CaProgressBar()
{
	if (m_pBar != NULL)
	{
		delete m_pBar;
	}
	m_pBar = NULL;
}

int CaProgressBar::SetFocusIn()
{
	return 1;
}
int CaProgressBar::SetFocus()
{
	SetFocusIn();
	return 1;
}
int CaProgressBar::ReFresh()
{
// 	CaWindow::ReFresh();
	if (m_pBar != NULL)
	{
		m_pBar->ReFresh();
	}

		ReFreshTitle();
		CaGroup::ReFresh();
		if(m_pCurObj != NULL )
		{
 			m_pCurObj->SetFocusIn();
		}
		
 		LCDRedraw();
	return 1;
}

int CaProgressBar::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	return 1;
}

void CaProgressBar::SetText(const char *text)
{
	m_pLabel->SetTitle(text, strlen(text));
}
void CaProgressBar::SetProgress(int pos)
{
	if (m_pBar != NULL)
	{
		m_pBar->UpdateCurPos(pos);
	}
}
void CaProgressBar::Show()
{
	ReFresh();
}

bool CaProgressBar::IsCurWin()
{
	return true;
}


