#include "CaMsgBox.h"
#include "StringWrap.h"
#ifndef WIN32
#include <unistd.h>
#include "keypad.h"
#include "Scanner.h"
#include "lcd_tiny.h"
#endif
#include"commonFunc.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

extern "C"
{
extern unsigned char getch_pos(void);
extern unsigned char getch_usb(void);
extern unsigned char clear_key_buffer(void);
extern unsigned char clear_key_buffer_usb(void);
extern unsigned char HaveKey(void);
}

CaMsgBox::CaMsgBox(const char *text, int type, bool model):CaWindow()
{
	DBG_ASSERT_EXIT(text !=NULL && strlen(text) > 0
					&& type >= MB_OK && type <= MB_REP_INPUT,("init msg box wrong!"));
	m_iLastLineH = 0;
	m_iStatus = 0;

	int textLen;	
	
	m_iType = CA_MSGBOX;
	m_iBoxType = BOX_ALL;
	m_iMsgBoxType = type;
	m_bModel = model;

	textLen = strlen(text);
	m_iH = MSGBOX_H;
	m_iW = MSGBOX_W;
	m_iX = (SCREEN_W - m_iW) / 2;
	m_iY = (SCREEN_H - m_iH) / 2 + 1;

	int maxwidth = (MSGBOX_W - 2*WORD_W) / CHAR_W + 1;
	if (textLen > maxwidth && type != MB_INPUT && type!= MB_REP_INPUT)
	{	//多行且不是Input，就增加高度
		int lineCount = 0;
		CStringWrap strwrap(text,maxwidth);
		strwrap.Wrap();
		lineCount = strwrap.Lines();
		DBG_PRN("info",("linecount = %d",lineCount));
		
		if (lineCount > 1)
		{
			int h = 4 + (GRID_LINE_H + 3)*(lineCount + 1);
			h = h < MSGBOX_H ? h:(SCREEN_H * 4 / 5);
			m_iY = SCREEN_H/16;
			m_iH = h;
			m_iLastLineH = h - GRID_LINE_H - 4;
			Create(m_iX, m_iY, m_iW, m_iH+4);			
		}

		for(int i=0;i<lineCount;i++)
		{
			char title[OBJ_TITLE_MAX_LEN + 1];
			memset(title, 0, sizeof(title) * sizeof(char));
			CaLabel *label = new CaLabel(false, ALIGN_CENTER);
			label->Create(0, 4 + (GRID_LINE_H + 2) * i, MSGBOX_W , WORD_H);
			strcpy(title,strwrap.Line(i));
			DBG_PRN("info",("line[%d]=%s",i,title));
			DBG_PRN("info",("strlen(line[%d])=%d",i,strlen(title)));
 			label->SetTitle(title, strlen(title));
		}
	}
	else
	{
		Create(m_iX, m_iY, m_iW, m_iH+4);
		m_iLastLineH = 5 + LINE_H + 6;
		if (type == MB_INPUT || type == MB_REP_INPUT)
		{
			m_pInput = new CaInput(CaObject::ON_LEFT,textLen*CHAR_W);
			m_pInput->Create(4,6,MSGBOX_W - 4,WORD_H);
			m_pInput->SetTitle(text,textLen);
			m_pInput->ReFresh();
			m_pInput->m_InputType=CaInput::aCHAR;
			m_pInput->m_bReadOnly=false;
			m_pInput->OnObject = s_onOK;
		} else
		{	
			m_pLabel = new CaLabel(false, ALIGN_CENTER);
			m_pLabel->Create(0, 6, MSGBOX_W, WORD_H);
			m_pLabel->SetTitle(text, strlen(text));
		}
	}	
	InitBtn(type);
	End();
}

void CaMsgBox::InitBtn(int type)
	{
		
		CaButton *btn;		 
		switch(type)
		{		
		case MB_OK:
			{
				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20,m_iLastLineH,3*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("确定",4);
				btn->OnObject = s_onOK;				
			}
			break;
		case MB_INPUT:
		case MB_OKCANCEL:
			{
 				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 - 2*WORD_W,m_iLastLineH,3*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("确定",4);
				btn->OnObject = s_onOK;						

				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 + 2*WORD_W,m_iLastLineH,3*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("取消",4);
				btn->OnObject = s_onCANCEL;
			}
			break;
		case MB_REP_INPUT:
			{
				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 + 2*WORD_W,m_iLastLineH,3*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("确定",4);
				btn->OnObject = s_onOK;
			}
			break;
		case MB_YESNO:
			{ 				
				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 - 2*WORD_W,m_iLastLineH,2*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("是",2);
				btn->OnObject = s_onOK;

				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 + 2*WORD_W,m_iLastLineH,2*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("否",2);
				btn->OnObject = s_onNO;
			}
			break;
		case MB_YESNOCANCEL:
			{
 				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 - 3*WORD_W,m_iLastLineH,3*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("取消",4);
				btn->OnObject = s_onCANCEL;

				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 + WORD_W,m_iLastLineH,2*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("否",2);
				btn->OnObject = s_onNO ;

				btn = new CaButton(this);
				btn->Create(m_iW/2 - 20 + 4*WORD_W,m_iLastLineH,2*WORD_W,WORD_H);
				btn->BoxType(CaObject::BOX_ALL);
				btn->SetTitleAlign(CaObject::ALIGN_CENTER);
				btn->SetTitle("是",2);
				btn->OnObject = s_onOK;
			}
			break;
		default:
		    break;
		}
	}

void CaMsgBox::SetText(const char *text)
{
  	m_pLabel->SetTitle(text, strlen(text));
}


CaMsgBox::~CaMsgBox()
{
}

unsigned char * CaMsgBox::GetInputContent()
{
	if (MB_INPUT == m_iMsgBoxType && m_pInput)
	{
		return m_pInput->m_contentBuf;
	}
	return NULL;
}
int CaMsgBox::SetFocusIn()
{
	return 1;
}
int CaMsgBox::SetFocus()
{
	return 1;
}
int CaMsgBox::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
 		LCDRedraw();
	return 1;
}

int CaMsgBox::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
//	DBG_PRN("info",("iEvent=%d",iEvent));
	int res= CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	if(res == 0)
	{
		//press ESC_KEY can renturn CANCEL_PRESSED. add by zhanglei 080829
		if (MB_OK != m_iMsgBoxType) 
		{
			if (iEvent == ESC_KEY)
			{
				m_iStatus = CANCEL_PRESSED;
				res = 1;
			}
		}
		
	}

	if(res == 1 && GetRefreshFlag())
	{
		LCDRedraw();
		DBG_PRN("------------",("LCDRedraw"));
		ClearRefreshFlag();
	}
	return res;
}

void CaMsgBox::ShowBox()
{	
	bool imeCallFlag = false;
	CaWindow::SetShowMsgBox(true);

	m_iStatus = 0;
	this->ReFresh();
#ifndef WIN32	
	clear_key_buffer();
	clear_key_buffer_usb();
#endif
	if (m_bModel)
	{
		while (1)
		{
		#ifdef WIN32
			if(_kbhit())
			{
				int key =_getche();
				if ((m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT))
				{	
					if(key == CHANGE_INPUT_METHOD_EVENT)
					{
						imeCallFlag = true;//有可能调用了ime
					}				
				}
				ProcEvent(key,NULL,0);
				if (imeCallFlag)//调用了ime就用刷新
				{
					imeCallFlag = false;
					ReFresh();
				}
				if (m_iStatus)
				{	
					this->SetShowMsgBox(false);
					break;
				}
			}
		#else
//			usleep(1000);
           CommonSleep(10);
			int key;
			//读扫描枪
			if (m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT)
			{
				if(	ScannerStateCheck() )
				{
					char scanEndSTR[2] = {0x0d, 0x00};
					unsigned char *p_ScannerBuf;
					if (ScannerPressed_mode2(scanEndSTR) == SUCCESS)
					{
						int i=0;
						p_ScannerBuf = ScannerCode();
						while ( *(p_ScannerBuf+i) != NULL)
						{
							ProcEvent(*(p_ScannerBuf+i), NULL, 0);
							i++;
						}
						ProcEvent(CASH_KEY, NULL, 0);
						if (m_iStatus)
						{		
							if (m_pFrame->CurrWin() != NULL)
							{
								m_pFrame->CurrWin()->ReFresh(true);
							}
							CaWindow::SetShowMsgBox(false);
							break;
						}
					}
				}
			}
		
			//读键盘值
			if(key = ReadKeypad()) 
			{	
				WB_LCM_BKL_ON();
				if ((m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT))
				{	
					if(key == CHANGE_INPUT_METHOD_EVENT)
					{
						imeCallFlag = true;//有可能调用了ime
					}				
				}
				ProcEvent(key,NULL,0);
				if (imeCallFlag)//调用了ime就用刷新
				{
					imeCallFlag = false;
					ReFresh();
				}
				if (m_iStatus)
				{		
					if (m_pFrame->CurrWin() != NULL)
					{
						m_pFrame->CurrWin()->ReFresh(true);
					}
					CaWindow::SetShowMsgBox(false);
					break;
				}
			}
		#endif
		}
	}
	else
	{
		m_pFrame->CurrWin(this);
	}
}
void CaMsgBox::SetInputType(CaInput::_INPUT_TYPE inputType)
{
	if (m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT)
	{
		m_pInput->m_InputType = inputType;
	}	
}

void CaMsgBox::SetMaxInputLen(int len)
{
	if (m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT)
	{
		m_pInput->SetMaxLen(len);
	}	
}

void CaMsgBox::SetInputText(const char *text)
{
	if (m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT)
	{
		m_pInput->SetContentBuf((UINT8 *)text, strlen(text));
	}
}
void CaMsgBox::ClearInput()
{
	if (m_iMsgBoxType == MB_INPUT || m_iMsgBoxType == MB_REP_INPUT)
	{
		m_pInput->Clear();
	}
}
void CaMsgBox::s_onCANCEL(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CaMsgBox *msgBox=(CaMsgBox *)obj->GetdWin();
	msgBox->OnCANCEL(iEvent,pEventData,iDataLen);
}
void CaMsgBox::s_onOK(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CaMsgBox *msgBox=(CaMsgBox *)obj->GetdWin();
	msgBox->OnOK(iEvent,pEventData,iDataLen);
}
void CaMsgBox::s_onNO(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CaMsgBox *msgBox=(CaMsgBox *)obj->GetdWin();
	msgBox->OnNO(iEvent,pEventData,iDataLen);
}
// void CaMsgBox::s_onInput(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
// {
// 	CaMsgBox *msgBox=(CaMsgBox *)obj->GetdWin();
// 	msgBox->OnInput(iEvent,pEventData,iDataLen);
// }

void CaMsgBox::OnOK(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iStatus = OK_PRESSED;
}
void CaMsgBox::OnNO(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iStatus = NO_PRESSED;
}
void CaMsgBox::OnCANCEL(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iStatus = CANCEL_PRESSED;
}
// void CaMsgBox::OnInput(int iEvent, unsigned char * pEventData, int iDataLen)
// {
// }
bool CaMsgBox::IsCurWin()
{
	return true;
}

int CaMsgBox::OnLeftKey(void)
{
	return OnUpKey();
}
int CaMsgBox::OnRightKey(void)
{
	return OnDownKey();
}

void CaMsgBox::ShowMsg(string strInfo)
{
	if( strInfo.empty() )
		strInfo = "NULL strInfo";

	CaMsgBox msgBox(strInfo.c_str(),CaMsgBox::MB_OK);
	msgBox.ShowBox();
	if (msgBox.m_iStatus == OK_PRESSED)
	{
		CPosFrame::Frame()->CurrWin()->ReFresh(true);
	}
}




