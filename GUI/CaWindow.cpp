#include"CaWindow.h"
#include"CaObject.h"
#include"PosFrame.h"

#include"LOGCTRL.h"
#define NO_POS_DEBUG
#include"pos_debug.h"

#ifndef WIN32
extern "C" void LCDRedraw();
#endif
bool CaWindow::m_bShowMsgBox = false;

CaWindow *CaObject::GetdWin()
{
	for (CaObject *o = m_pOwner; o !=NULL ; o = o->m_pOwner)
	{
		if (o->m_iType >= CA_WINDOW)
		{
			return (CaWindow*)o;
		}
	}
	DBG_ASSERT_WARNING(false,("not found CA_END_WINDOW parent !"));
	return NULL;
}
CaWindow::CaWindow(LABEL_POS lbPos,int lbW):CaGroup()
{
	m_labelPos=lbPos;
	m_iLbWidth=lbW;
	m_iWinID=0;
	m_iType=CA_WINDOW;
	m_pPreWin=CaObject::GetdWin(); //缺省为父窗体
	m_pFrame= CPosFrame::Frame();
	OnActive = NULL;
	m_bResetFlag = true;
}
CaWindow::CaWindow():CaGroup()
{
	m_iWinID=0;
	m_iType=CA_WINDOW;
	m_pPreWin=CaObject::GetdWin();//缺省为父窗体
	m_pFrame= CPosFrame::Frame();
	OnActive = NULL;
	m_bResetFlag = true;
}
CaWindow::~CaWindow()
{
	if(m_pFrame->CurrWin() == this )
	{
		m_pFrame->CurrWin(NULL);
	}

}
int  CaWindow::Create(int iX,int iY,int iW,int iH)
{
	return CaGroup::Create(iX,iY,iW,iH);
}
int CaWindow::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	int res= CaGroup::ProcEvent(iEvent,pEventData,iDataLen);

	if(res == 1 && GetRefreshFlag() && m_pFrame->CurrWin() == this )
	{
		LCDRedraw();
		DBG_PRN("------------",("LCDRedraw"));
		ClearRefreshFlag();
	}
	return res;
}
void CaWindow::ChangeWin( CaWindow *pNewWin)
{
	DBG_ASSERT_EXIT(m_pFrame->CurrWin(),("current win is NULL!"));
	DBG_ASSERT_EXIT(pNewWin, ("pNewWin is NULL"));
	if(pNewWin == this )
	{
		return ;
	}
	CaWindow *tempWin;
	tempWin = m_pFrame->CurrWin();
	m_pFrame->CurrWin(pNewWin);
	pNewWin->m_pPreWin = tempWin;

	pNewWin->ResetFocus();//每次切换都把焦点置于第一个控件

	if( pNewWin->OnActive != NULL )
	{
		pNewWin->OnActive(pNewWin);
	}
	pNewWin->ReFresh();
}

void CaWindow::ChangeWin(int id)
{
	ChangeWin(m_pFrame->GetWin(id));
}
int  CaWindow::ReFresh(bool force)
{
	static CaWindow *pLastRefreshWin=this;
	static int refresh_flag_s=0;
	if( m_pFrame->CurrWin() == this )
	{
		ReFreshTitle();
		CaGroup::ReFresh();
		if(m_pCurObj != NULL )
		{
 			m_pCurObj->SetFocusIn();
		}
		
		if (force || 
			(GetRefreshFlag() && (pLastRefreshWin != this || refresh_flag_s==0)))
		{
			refresh_flag_s = 1;
			LCDRedraw();
			DBG_PRN("++++++++++++",("LCDRedraw %u", this));
			ClearRefreshFlag();
			pLastRefreshWin = this;
		}
		

		return 1;
	}
	else
	{
		return 0;
	}
}


//int  CaWindow::ReFresh(int id)
//{
//	m_pFrame->GetWin(id)->ReFresh();
//	return 1;
//}

int  CaWindow::SetFocus(void)
{
	return 1;
}
//int  CaWindow::SetTitle(unsigned char * pData,int iLen)
//{
//	return 1;
//}
int CaWindow::OnUpKey(void)
{
	return CaGroup::OnUpKey();
}
int CaWindow::OnDownKey(void)
{
	return CaGroup::OnDownKey();
}
int CaWindow::OnLeftKey(void)
{
	return -1;
}
int CaWindow::OnRightKey(void)
{
	return -1;
}
int CaWindow::OnTabKey(void)
{
	return CaGroup::OnDownKey();	
}
int CaWindow::OnPgUpKey()
{
	return -1;
}
int CaWindow::OnPgDnKey()
{
	return -1;
}
bool CaWindow::IsCurWin()
{
 return ((m_pFrame->CurrWin() == this) || (m_pFrame->CurrWin() == NULL));
}

void CaWindow::ResetWin()
{
}

bool CaWindow::IsShowMsgBox(void)
{
	return m_bShowMsgBox;
}

void CaWindow::SetShowMsgBox(bool show)
{
	m_bShowMsgBox = show;
}
