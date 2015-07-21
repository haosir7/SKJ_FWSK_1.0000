#include "CaMultiPageWindow.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CaMultiPageWindow::CaMultiPageWindow()
{

//	m_pHeadSubWin=NULL;
//	m_pEndSubWin=NULL;
//	m_pCurSubWin=NULL;

}
CaMultiPageWindow::~CaMultiPageWindow()
{

}
void CaMultiPageWindow::Add(CaObject *pObj)
{
	DBG_ASSERT_EXIT((pObj != NULL ),("pObj is NULL!"));
//	DBG_ASSERT_EXIT((pObj->m_iType >= CA_PAGE),("CaMultiPageWindow addobj pObj must be a Window ") );
	 CaGroup::Add(pObj);
}
CaWindow *CaMultiPageWindow::CreateNewSubWin(int index)
{
	DBG_ASSERT_EXIT((FindSubWin(index) == NULL ),("index =%d is existed!"));
	CaWindow *pWin=new CaWindow();
	pWin->Create(m_iX,m_iY,m_iW,m_iH);
	pWin->WinID(index);
	pWin->m_iType=CA_PAGE;
	return pWin;
}
CaWindow *CaMultiPageWindow::CreateNewSubWin(int iX,int iY,int iW,int iH,int index)
{
	DBG_ASSERT_EXIT((FindSubWin(index) == NULL ),("index =%d is existed!"));
	CaWindow *pWin=new CaWindow();
	pWin->Create(m_iX,m_iY,m_iW,m_iH);
	pWin->m_iType=CA_PAGE;
	pWin->WinID(index);
	return pWin;
}
CaWindow *CaMultiPageWindow::FindSubWin(int index)
{
	CaObject*const* a = ObjList();
	int i; 
	for (i=0; i < m_iObjCount; i++)
	{
		if ( (*(a+i))->m_iType >= CA_WINDOW  && ((CaWindow *)*(a+1))->WinID() == index)
		{
			break;
		}
	}
	if(i >= m_iObjCount ) //not found
	{
		return NULL;
	}
	return (CaWindow *)(*(a+i));

}
int CaMultiPageWindow::OnPgDnKey()
{
	if(m_iObjCount < 1)
	{
		DBG_ASSERT_WARNING(false,("m_iObjCount=%d ",m_iObjCount));
		return -1;
	}
	CaWindow * pWin =(CaWindow *)NextFocusObj(m_pCurObj);
	if(pWin == NULL)
	{
		DBG_ASSERT_WARNING(false,("next win is NULL "))
		return -1;
	}
	pWin->ReFresh();
	return 1;
}
int CaMultiPageWindow::OnPgUpKey()
{
	if(m_iObjCount < 1)
	{
		DBG_ASSERT_WARNING(false,("m_iObjCount=%d ",m_iObjCount));
		return -1;
	}	
	CaWindow * pWin =(CaWindow *)PreFocusObj(m_pCurObj);
	if(pWin == NULL )
	{
		DBG_ASSERT_WARNING(false,("pre win is NULL "))
		return -1;
	}
	pWin->ReFresh();
	return 1;
}
int CaMultiPageWindow::ShowSubWin(int index)
{
	CaWindow *pWin=FindSubWin(index);
	if(pWin != NULL )
	{
		pWin->ReFresh();
		return 1;
	}
	else
	{
		DBG_ASSERT_WARNING(false,(" index=%d window  is NULL ",index))
		return -1;
	}
}

//CaObject* CaMultiPageWindow::ProcEvent(int iEvent,unsigned char *pEventData,
//									   int iDataLen)
//{
//	switch(iEvent)
//	{
//		case KEY_PGUP:            //前一页
//			PgUp();			
//			break;
//		case KEY_PGDN:            //后一页
//			PgDn();			
//			break;
//		default:
//			break;
//	}
//	return ((CaGroup *)m_pOwner)->m_pCurObj;
//}


