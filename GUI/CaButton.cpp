#include "CaButton.h"
#include "CaLabel.h"
#include "LOGCTRL.h"
#include "pos_debug.h"


CaButton::CaButton(CaGroup *pOwner):CaObject(CA_BUTTON,true,pOwner)
{
	m_iBoxType=BOX_ALL;
}
//CaButton::CaButton(bool bFrame)
//{
//	m_bFocus=true;
//	m_iBoxType=BOX_ALL;
//}
CaButton::~CaButton()
{

}
int CaButton::Create(int iX,int iY,int iW,int iH)
{
	CaObject::Create(iX, iY, iW, iH);
	m_pLabel=new CaLabel(false,ALIGN_CENTER,CA_INNER_LABEL);
	m_pLabel->Create(m_iX, m_iY, m_iW, m_iH);
	return 0;
}

int CaButton::SetFocusIn()
{
	DBG_ASSERT_EXIT((m_pLabel != 0),("m_pLabel is null!"));
	m_pLabel->SetFocus();
	return 1;
}
int CaButton::SetFocus()
{
	SetFocusIn();
    return	CaObject::SetFocus();
}

int CaButton::ReFresh()
{
	DBG_ASSERT_EXIT((m_pLabel != 0),("m_pLabel is null!"));
	if(!m_Visible)
	{
		return 0;
	}
	m_pLabel->Create(m_iX, m_iY, m_iW, m_iH);
	m_pLabel->ReFresh();
	DrawBox();
	return 1;
}
int CaButton::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if( iEvent == BTN_ENTER_KEY )
	{
		return	CaObject::ProcEvent(iEvent,pEventData,iDataLen);
	}
	else
	{
		return 0;
	}
}



