#include"PosFrame.h"
#include "LOGCTRL.h"
#include "pos_debug.h"
#include "CaIMEWin.h"
#include "pinyin.h"
#include "CaMultiLineInput.h"

CaWindow * CPosFrame::m_pCurrWin=NULL;
CaWindow * CPosFrame::m_wpArray[WIN_MAX_NUM]={
0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0	};
unsigned char CPosFrame::m_wCreatedFlag[WIN_MAX_NUM]={
0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0	};
// CaWindow * CPosFrame::m_pQuWeiWin=NULL;
// CaWindow * CPosFrame::m_pPinYinWin=NULL;
CaIMEWin * CPosFrame::m_pIMEWin= new CaIMEWin;

CPosFrame * CPosFrame::pThis=NULL;
CPosFrame::CPosFrame()
{
//	unsigned int errorcode = py_init();
//
//	DBG_ASSERT_EXIT(errorcode != -1,("ÊäÈë·¨³õÊ¼Ê§°Ü!"));
	m_pCurrWin=NULL;
	pThis=this;
}
CPosFrame::~CPosFrame()
{
	
}
int CPosFrame::ProcEvent(int iEvent, unsigned char *pEventData, int iDataLen)
{
	DBG_ASSERT_EXIT(m_pCurrWin != NULL,("m_pCurrWin is NULL!") );
	int res=m_pCurrWin->ProcEvent(iEvent,pEventData,iDataLen);
//	if(res == 1 )
//	{
//		m_pCurrWin->ReFresh();
//	}
	return res;
}
bool CPosFrame::Create()
{
	return 0;
}
void CPosFrame::ChangeWin( CaWindow *win)
{
	DBG_ASSERT_EXIT(win != NULL,("win is NULL!") );
	if(m_pCurrWin !=NULL )
	{
		m_pCurrWin->ChangeWin(win);
	}
	else
	{
		m_pCurrWin=win;
		if( m_pCurrWin->OnActive != NULL )
		{
			m_pCurrWin->OnActive(m_pCurrWin);
		}
		m_pCurrWin->ReFresh();
	}
}
void CPosFrame::ChangeWin( int id )
{
	ChangeWin(GetWin(id));
}
CaWindow *CPosFrame::GetWin(int id)
{
	DBG_ASSERT_EXIT( id >= 0 && id < WIN_MAX_NUM ,(" id=%d is invalid!",id));
	if(m_wCreatedFlag[id] != 0 )
	{
		return m_wpArray[id];
	}
	else
	{
//		DBG_ASSERT_WARNING(false,
//			("window %s  id=%d has been not created!,GetWinName(id),id"));
		return NULL;
	}

}
void CPosFrame::ReleaseWin(int id)
{
	CaWindow *win = GetWin(id);
	m_wpArray[id] = NULL;
	delete win;
}
void CPosFrame::ReleaseWin(CaWindow *win)
{
	int i;
	for(i=0; i<WIN_MAX_NUM; i++)
	{
		if (win == m_wpArray[i])
		{
			break;
		}
	}
	
	if (i < WIN_MAX_NUM)
	{
		delete win;
		m_wpArray[i] = NULL;
	}
}
bool CPosFrame::RegsiterWin(CaWindow *win,int id)
{
	DBG_ASSERT_EXIT(win != NULL,("win is NULL!"));
	DBG_ASSERT_EXIT( id >= 0 && id < WIN_MAX_NUM ,(" id=%d is invalid!",id));
	if(m_wCreatedFlag[id] == 0 )
	{
		m_wpArray[id]=win;
		m_wCreatedFlag[id]=1;
		return true;
	}
	else
	{
		DBG_ASSERT_WARNING(false,
			("window %s  id=%d has been created!,GetWinName(id),id"));
		return false;
	}
}
void CPosFrame::OnChangeIME(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	int len,maxLen;
	unsigned char *buf;
	CaInput *inputObj;
	CaMultiLineInput *multiInputObj;

	maxLen = CaInput::MAX_BUF_LEN;
	if (maxLen < CaMultiLineInput::MAX_BUF_LEN)
	{
		maxLen = CaMultiLineInput::MAX_BUF_LEN;
	}
	buf = new unsigned char[maxLen];
	DBG_ASSERT_EXIT(obj != NULL, ("Pointer to cainput obj can't be NULL"));
	if (m_pIMEWin == NULL)
	{
		m_pIMEWin = new CaIMEWin;
	}

	if (obj->m_iType == CA_INPUT)
	{
		inputObj = (CaInput *) obj;
		m_pIMEWin->m_pIMEInput->m_InputType = inputObj->m_InputType;
		m_pIMEWin->m_pIMEInput->SetMaxLen(inputObj->m_iConMaxLen - inputObj->m_iConLen);
		m_pIMEWin->Show();
		if (strlen((char *)m_pIMEWin->m_pIMEInput->m_contentBuf) > 0)
		{
			len = inputObj->m_iCurConPos;
			memset(buf, 0, CaInput::MAX_BUF_LEN);
			memcpy(buf, inputObj->m_contentBuf, inputObj->m_iCurConPos);
			memcpy(buf + len, m_pIMEWin->m_pIMEInput->m_contentBuf, m_pIMEWin->m_pIMEInput->m_iConLen);
			len += m_pIMEWin->m_pIMEInput->m_iConLen;
			memcpy(buf + len, inputObj->m_contentBuf + inputObj->m_iCurConPos, inputObj->m_iConLen - inputObj->m_iCurConPos);
			
			len = inputObj->m_iConLen + m_pIMEWin->m_pIMEInput->m_iConLen;
			inputObj->SetContentBuf(buf,len);
		}
	}
	else if (obj->m_iType == CA_MULTILINE_INPUT)
	{
		multiInputObj = (CaMultiLineInput *) obj;
		m_pIMEWin->m_pIMEInput->m_InputType = (CaInput::_INPUT_TYPE)multiInputObj->m_InputType;
		m_pIMEWin->m_pIMEInput->SetMaxLen(multiInputObj->m_iAllConMaxLen - multiInputObj->m_iConLen);
	m_pIMEWin->Show();
	if (strlen((char *)m_pIMEWin->m_pIMEInput->m_contentBuf) > 0)
	{
			len = multiInputObj->m_iCurConPos;
			memset(buf, 0, CaMultiLineInput::MAX_BUF_LEN);
			memcpy(buf, multiInputObj->m_contentBuf, multiInputObj->m_iCurConPos);
		memcpy(buf + len, m_pIMEWin->m_pIMEInput->m_contentBuf, m_pIMEWin->m_pIMEInput->m_iConLen);
		len += m_pIMEWin->m_pIMEInput->m_iConLen;
			memcpy(buf + len, multiInputObj->m_contentBuf + multiInputObj->m_iCurConPos, multiInputObj->m_iConLen - multiInputObj->m_iCurConPos);

			len = multiInputObj->m_iConLen + m_pIMEWin->m_pIMEInput->m_iConLen;
			multiInputObj->SetContentBuf(buf,len);
		}
	}
	delete buf;
	m_pIMEWin->EndProcess();
	if (CPosFrame::CurrWin() != NULL)
	{
	CPosFrame::CurrWin()->ReFresh();
         }
}

void CPosFrame::SetWinResetFlag(int id, bool flag)
{
 	CaWindow *win = GetWin(id);
	win->m_bResetFlag = flag;
}
