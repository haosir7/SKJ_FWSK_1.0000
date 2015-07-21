#include <stdio.h>
#include <string>
#ifndef WIN32
	#include <unistd.h>
	#include  <sys/timeb.h>
	#include "keypad.h"
	#include "lcd_tiny.h"
#else
	//#include <windows.h>
#endif

#include "CaIMEWin.h"
#include "IncludeMe.h"
#include "pinyin.h"
#include "CaMsgBox.h"
#include "handwrite.h"
#include "VersionConfig.h"
#include "SerialPort.h"
#include"commonFunc.h"
extern "C"
{
#include "wubi.h"

extern unsigned char getch_pos(void);
extern unsigned char getch_usb(void);
extern unsigned char clear_key_buffer(void);
extern unsigned char clear_key_buffer_usb(void);
extern unsigned char HaveKey(void);

#include "T9_pinyin.h"
#include "T9_bihua.h"
}

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"
#include "pos_debug.h"


extern int match_keystroke_normal(unsigned char *inpinfo, unsigned int inpinfo_size,wchar_t *outbuff,unsigned int *outsize);

#ifdef WIN32
extern "C"{
	#include <conio.h>
};
#endif

char * CaIMEWin::s_ImeNames[9] = {"拼音","小写","大写","笔画","区位","手写","五笔","T9拼","T9笔"};
#if (POS_TYPE == POS_APE3000R)
ASC_Key_List CaIMEWin::s_ascKeyList[9] = {
	{7, "pqrs"},	{8, "tuv"},		{9, "wxyz"},
	{4, "ghi"},		{5, "jkl"},		{6, "mno"},
	{1, " @#$%^&*()-<>./_~\\"}, 
					{2, "abc"},		{3, "def"}
};
#else //POS_TYPE
#if (KEYPAD_TYPE == 1)
ASC_Key_List CaIMEWin::s_ascKeyList[9] = {
	{7, "pqrs"},	{8, "tuv"},		{9, "wxyz"},
	{4, "ghi"},		{5, "jkl"},		{6, "mno"},
	{1, " @#$%^&*()-<>./_~\\"}, 
					{2, "abc"},		{3, "def"}
};
#else
ASC_Key_List CaIMEWin::s_ascKeyList[9] = {
	{7, " @#$%^&*()-<>./_~\\"},     
				{8, "abc"},  {9, "def"},
	{4, "ghi"},  {5, "jkl"},  {6, "mno"},
	{1, "pqrs"}, {2, "tuv"},  {3, "wxyz"}
};
#endif
#endif //POS_TYPE


static INT8 *BihuaHZDis[5] = { "一", "丨", "丿", "丶", "乙" };
UINT8 g_nHWType = 1;  //手写板模式 0 = 胜创通不带屏， 1=胜创通带屏。

CaIMEWin::CaIMEWin()
{
	char title[5];

	m_IMEType = IME_PIN_YIN;//切换时的第一种输入法
	m_bIMEInputFocused = true;
	m_iBoxType = BOX_ALL;

	m_bLenovoOn = false;
	m_T9LenovoState	=0;

	m_bRecurSpecChar = false;
	
	m_iX = IMEWIN_X;
	m_iY = IMEWIN_Y;
	m_iH = IMEWIN_H;
	m_iW = IMEWIN_W;
	Create(m_iX, m_iY, m_iW, m_iH);
	
	memset(title, 0, 5);
	sprintf(title,"%s:",s_ImeNames[IME_PIN_YIN]);
	m_pIMEInput = new CaInput(ON_LEFT,5*CHAR_W);
	m_pIMEInput->Create(SCREEN_LEFT_OFFSET,SCREEN_TOP_OFFSET,IMEWIN_W - CHAR_W,WORD_H);
	m_pIMEInput->SetTitle(title,strlen(title));

	m_pPYDisInput = new CaInput;
	m_pPYDisInput->Create(1,SCREEN_TOP_OFFSET + LINE_H,CHAR_W*8,WORD_H);
	m_pPYDisInput->SetMaxLen(6);//拼音输入法默认最多输入6个字符

	m_pIMELabel = new CaLabel;
	m_pIMELabel->Create(CHAR_W*7,SCREEN_TOP_OFFSET + LINE_H,WORD_W*12,WORD_H);

	m_T9Label = new CaLabelSelect;
	m_T9Label->Create(CHAR_W*8,SCREEN_TOP_OFFSET + LINE_H,WORD_W*11+CHAR_W,WORD_H);

	End();

}

CaIMEWin::~CaIMEWin()
{
}
int CaIMEWin::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
	if (m_T9state != 1)
		m_pIMELabel->ReFresh();
 	
	LCDRedraw();
	return 1;
}

int CaIMEWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	DBG_PRN("info", ("iEvent = %x", iEvent));
	
	int res=0;
	if (iEvent == CHANGE_INPUT_METHOD_EVENT)
	{
		ChangeInputMethod();
		return 1;
	}

	if (m_bLenovoOn == true)
	{
		res = LenovoProc(iEvent, pEventData, iDataLen);

		if(res == 1)
		{
			//上翻下翻键TAB键：由LenovoProc处理之后直接返回
			if (iEvent == PAGEUP_KEY || iEvent == PAGEDOWN_KEY || iEvent == TAB_KEY)
			{
				ReFresh();
				return 1;
			}
			//数字键：选择词汇后，置回拼音输入模式，再返回
			else if (iEvent >= 0x30 && iEvent <= 0x39 && m_T9LenovoState==1)
			{
				ReFresh();
				m_T9LenovoState = 0;
				return 1;
			}
		}
		else
		{
			//联想内没有处理，但有作用的按键
			if(iEvent == CLEAR_ONE_EVENT || iEvent == CLEAR_ALL_EVENT)	
			{;
			}
			else
			{
				return 0;
			}
		}
	}

	switch(m_IMEType)
	{
	case IME_PIN_YIN:
		{
		//	res = RecPinyin(iEvent, pEventData, iDataLen);
			res = RecT9py(iEvent, pEventData, iDataLen);
		}
		break;
//	case IME_WU_BI:
//		{
//			res = RecWB(iEvent, pEventData, iDataLen);
//		}
//		break;
	case IME_LOWER_CASE:
		{
			res = ProcDirectInput(iEvent, pEventData, iDataLen);
		}
		break;
	case IME_UPPER_CASE:
		{
			res = ProcDirectInput(iEvent, pEventData, iDataLen);
		}
		break;
	case IME_QU_WEI:
		{
			res = RecQW(iEvent, pEventData, iDataLen);
		}
		break;
	case IME_BI_HUA:
		{
			res = RecT9Bihua(iEvent, pEventData, iDataLen);
		}
		break;
	case IME_HAND_WRITE:
		{
			if (1 == g_nHWType)
			{
				res = RecScreenHandWrite(iEvent, pEventData, iDataLen);
			}
			else if (0 == g_nHWType) 
			{
				res = RecHandWrite(iEvent, pEventData, iDataLen);
			}
		}
		break;
	default:
		{
			res = ProcDirectInput(iEvent, pEventData, iDataLen);
		}
	    break;
	}

	if (res == 1)
		ReFresh();

	if(res == 1 && GetRefreshFlag())
	{
		LCDRedraw();
		DBG_PRN("------------",("LCDRedraw"));
		ClearRefreshFlag();
	}
		
	return res;
}

void CaIMEWin::Show()
{
	CaWindow::SetShowMsgBox(true);
	BeginChn();
	BeginT9py();
	this->ReFresh();
#ifndef WIN32	
	clear_key_buffer();
	clear_key_buffer_usb();
#endif

	while (1)
	{
#ifdef WIN32
		int key=0;	
		
		if (m_IMEType == IME_HAND_WRITE)
		{
			if ((0 == g_nHWType) &&
				(HwRevHANZI((UINT8 *)m_caPinyin_out, (UINT8 *)&m_iHZMaxLen) == 1))
			{
				key = HW_REV_EVENT;
			}
			else if ((1 == g_nHWType)&&(get_rev_count(UART0)>0))
			{
				key = HW_REV_EVENT;
			}
			else
			{
				if(_kbhit())
				{
					key = _getche();
				}
			}
		}
		else
		{
			if(_kbhit())
			{
				key = _getche();
			}
		}

		if (key != 0)
		{
			if (ProcEvent(key,NULL,0) == 0)
			{
				if (key == ENTER_KEY && m_bIMEInputFocused)
				{						
					DBG_PRN("info", (" end of ime"));
					break;
				}
				else if (RETURN_MAIN_MENU == key)
				{
					m_pIMEInput->Clear();
					break;
				}
			}
		}

#else
//		usleep(1000);
		CommonSleep(10);
		
		int key,res;	
		
		if (m_IMEType == IME_HAND_WRITE)
		{
		//	DBG_PRN("info",("show--m_IMEType == IME_HAND_WRITE"));
			if (0 == g_nHWType)//不带屏手写板
			{
				res= HwRevHANZI((UINT8 *)m_caPinyin_out, (UINT8 *)&m_iHZMaxLen);
//			usleep(100);
			CommonSleep(10);
				if (res == 1)
				{
					key = HW_REV_EVENT;
				}
				else
				{
					key = ReadKeypad();
				}
			}
			else if (1 == g_nHWType) //带屏手写板
			{
//				usleep(100);
				CommonSleep(10);
				res= get_rev_count(UART0);
				if (res > 0)
				{
					DBG_PRN("info",("get_rev_count(UART0) = %d", res));
					key = HW_REV_EVENT;
				}
				else
				{
					key = ReadKeypad();
				}				
			}			
		}
		else
		{
			key = ReadKeypad();
		}
		
		if (key != 0)
		{
			WB_LCM_BKL_ON();
			if (ProcEvent(key,NULL,0) == 0)
			{
				if (key == ENTER_KEY && m_bIMEInputFocused)
				{
					if (m_IMEType == IME_HAND_WRITE)
						HwRelease();
					
					DBG_PRN("info", (" end of ime"));
					this->SetShowMsgBox(false);
					break;
				}
				else if (RETURN_MAIN_MENU == key)
				{
					if (m_IMEType == IME_HAND_WRITE)
					{
						HwRelease();
					}

					m_pIMEInput->Clear();
					this->SetShowMsgBox(false);
					break;
				}
			}
		}
#endif
	}
}

bool CaIMEWin::IsCurWin()
{
	return true;
}

void CaIMEWin::EndProcess()
{
	char title[5];
	m_IMEType = IME_PIN_YIN;
	memset(title, 0, 5);
	sprintf(title,"%s:",s_ImeNames[m_IMEType]);
	m_pIMEInput->SetTitle(title,strlen(title));
	m_pIMEInput->Clear();
}

void CaIMEWin::ChangeInputMethod()
{
	char title[5];

	if (m_IMEType == IME_HAND_WRITE)
	{
		HwRelease();
	}

	m_bLenovoOn = false;
	m_IMEType = (m_IMEType + 1) % IME_DISABLE;
	memset(title, 0, 5);
	sprintf(title,"%s:",s_ImeNames[m_IMEType]);
	m_pIMEInput->SetTitle(title,strlen(title));
	m_pPYDisInput->Clear();
	m_pIMELabel->SetTitle("",0);
	m_pIMEInput->SetFocus();
	m_bIMEInputFocused = true;
	memset(m_RandLenBuf, 0, PHRASE_BUFFER_SIZE);
	m_iLineIndex = 0;
	ReFresh();
	
	switch(m_IMEType)
	{
	case IME_PIN_YIN:
		BeginChn();		
		BeginT9py();			
		break;
//	case IME_WU_BI:
//		{
//			BeginWB();			
//		}
//		break;
	case IME_LOWER_CASE:
	case IME_UPPER_CASE:
		BeginT9ASC();
	    break;
	case IME_QU_WEI:
		BeginQW();
		break;
	case IME_BI_HUA:
		BeginT9Bihua();
		break;
	case IME_HAND_WRITE:
		BeginChn();
		int res;
		DBG_PRINT(("g_nHWType = %u", g_nHWType));
		if (1 == g_nHWType) 
		{
			res = ScreenHwInit();//有屏手写板初始化
			DBG_PRINT(("res = %d", res));
		}
		else if (0 == g_nHWType) 
		{
			res = HwInit();
			DBG_PRINT(("res = %d", res));
		}		
	    break;
	default:
	    break;
	}
}

void CaIMEWin::BeginChn()
{
	memset(m_caPinyin_ChnBuf,0,PINYIN_CHN_BUF_LEN);
	memset(m_caPinyin_out,0,PINYIN_OUT_LEN);	
	memset(m_caPinyin_in,0,PINYIN_IN_LEN);
	memset(m_RandLenBuf, 0, PHRASE_BUFFER_SIZE);
	
	m_iLineIndex = 0;
	m_iHZMaxLen=0;
	m_iCurHZIndex = 0;
	m_iPinyinLen = 0;

	m_bLenovoOn = false;

	m_pPYDisInput->Clear();
	m_pIMELabel->SetTitle("",0);

	if (!m_pIMEInput->IsCurObj())
	{
		m_pIMEInput->SetFocus();
		m_bIMEInputFocused = true;
	}	

	m_handwrite_flag = 0;
}

void CaIMEWin::BeginT9py()
{
	m_caT9PYLen = 0;	
	memset(m_caT9PYBuf, 0, PINYIN_BUF_LEN);	
	memset(PYBuf, 0, sizeof(PYBuf));
	PY_num = 0;		
	PY_CurPos = 0;		
	m_T9state = 1;
	m_T9Label->SetTitle("",0);

	m_T9LenovoState = 0;
}

void CaIMEWin::BeginT9ASC()
{
	m_T9ASC_lastkey = 0;	
	m_T9ASC_clickCount = 0;		
	m_T9ASC_lastTime = 0;
	m_T9Label->SetTitle("",0);
	m_pIMELabel->SetTitle("",0);
	ReFresh();
}


void CaIMEWin::BeginWB()
{
	BeginChn();
}

void CaIMEWin::BeginQW()
{
	m_iQwLen = 0;
	memset((void*)m_caQwBuf, 0, sizeof(m_caQwBuf));
	m_pPYDisInput->Clear();
	m_pIMELabel->SetTitle("",0);
	m_pIMEInput->SetFocus();
	m_bIMEInputFocused = true;
}

void CaIMEWin::BeginT9Bihua()
{
	m_iHZMaxLen=0;
	m_iCurHZIndex = 0;
	m_iPinyinLen = 0;

	T9BH_Pageup_Could=0;
	T9BH_Pagedown_Could=0;

	T9BH_state = 0;

	m_pPYDisInput->Clear();
	m_pIMELabel->SetTitle("",0);
	m_pIMEInput->SetFocus();

	memset(m_caPinyin_in, 0, sizeof (m_caPinyin_in));
}

void CaIMEWin::ChangeFocus(int iEvent, unsigned char *pEventData, int iDataLen)
{
	switch(m_IMEType)
	{
	case IME_PIN_YIN:
// 	case IME_WU_BI:
		{
			if((iEvent > 0x7A || iEvent < 0x61) && m_iPinyinLen == 0)
			{
				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
			}
			else /*if(iEvent <= 0x7A && iEvent >= 0x61)*/
			{
				m_pPYDisInput->SetFocus();
				m_bIMEInputFocused = false;
			}
		}
		break;
	case IME_UPPER_CASE:
	case IME_LOWER_CASE:
		{
			m_pIMEInput->SetFocus();
			m_bIMEInputFocused = true;
		}
		break;
	case IME_QU_WEI:
		{
			if (m_iQwLen == 0 && (iEvent < 0x30 || iEvent > 0x39))
			{
				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
			}
			else
			{
				m_pPYDisInput->SetFocus();
				m_bIMEInputFocused = false;
			}			
		}
		break;
	case IME_BI_HUA:
		{
			//不是笔画输入事件，则将焦点置于汉字显示框；否则焦点置于拼音输入框
			if ((iEvent < 0x31 || iEvent > 0x35) && m_iPinyinLen == 0)
			{
				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
			}
			else
			{
				m_pPYDisInput->SetFocus();
				m_bIMEInputFocused = false;
			}			
		}
		break;
	default:
	    break;
	}
}

int CaIMEWin::RecQW(int iEvent, unsigned char *pEventData, int iDataLen)
{
	int res; 
	ChangeFocus(iEvent,pEventData,iDataLen);

	if (iEvent >= 0x30 && iEvent <= 0x39)
	{
		if (m_iQwLen >= 0 && m_iQwLen < 4)
		{
			m_caQwBuf[m_iQwLen ++]=(unsigned char)(iEvent);
			res = CaGroup::ProcEvent(iEvent, pEventData,iDataLen);
			if (m_iQwLen == 4)
			{
				unsigned char  buf[4]={0,0,0,0};
  				buf[0]=(m_caQwBuf[0]-'0')*10+(m_caQwBuf[1]-'0');
  				buf[1]=(m_caQwBuf[2]-'0')*10+(m_caQwBuf[3]-'0');

				//0xB0-0xF7(176-247) 
				//0xA0-0xFE（160-254）
				if ( buf[0]>247-160 || buf[1]>254-160)
				{
					return 0;
				}

  				buf[0] = buf[0]+160;
  				buf[1] = buf[1]+160;

				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
				res = CaGroup::ProcEvent(INPUT_EVENT,buf,2);				
				BeginQW();
				return res;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}

	if (m_iQwLen > 0)
	{
		if (iEvent == CLEAR_ONE_EVENT)
		{
			m_iQwLen --;
			res = CaGroup::ProcEvent(iEvent, pEventData, iDataLen);
			if (m_iQwLen == 0)
			{
				BeginQW();
			}
			return res;
		}
		else if (iEvent == CLEAR_ALL_EVENT)
		{
			res = CaGroup::ProcEvent(iEvent, pEventData, iDataLen);
			BeginQW();
			return res;
		}
	}
	else
	{
		if (iEvent==CLEAR_ONE_EVENT || iEvent == CLEAR_ALL_EVENT)
		{
			return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
		}
	}
	
	return 0;
}

int CaIMEWin::RecT9Bihua(int iEvent, unsigned char *pEventData, int iDataLen)
{
	static unsigned char cout_size;
	unsigned char i,errorcode;
	unsigned char *pPinyinOut;
	unsigned int tmpIndex=0;
	int res; 
	UINT8 tmp;
	char tmpWord[3];

	ChangeFocus(iEvent,pEventData,iDataLen);
	prn_log("Enter SelectHanzi Function.");

	pPinyinOut=(unsigned char *)m_caPinyin_out;
	
	if( iEvent<=0x39 && iEvent>=0x31 )			
	{
		// 输入数字键选择汉字
		if (T9BH_state == 1)
		{
			if (iEvent<=0x35 && iEvent>=0x31)
			{
				if ( m_iPinyinLen>0 )
				{
					iEvent -= 0x31;	
					if(iEvent>PINYIN_PAGE_SIZE-1)    //输入>4,及超出可选汉字数
					{
						return 0;
					}
					if(iEvent < PINYIN_PAGE_SIZE && iEvent + m_iCurHZIndex < m_iHZMaxLen)  //判断是否在共选择的字数以内       
					{	
						m_pIMEInput->SetFocus();
						m_bIMEInputFocused = true;
						res = CaGroup::ProcEvent(INPUT_EVENT,m_caPinyin_ChnBuf+iEvent*4+2,2);				
						
						//复位各种变量，开始新一轮输入
						BeginT9Bihua();
						memset((void*)tmpWord, 0, sizeof(tmpWord));
						memcpy(tmpWord, m_caPinyin_ChnBuf+iEvent*4+2,2);
						LenovoProc('F', (unsigned char *)tmpWord, strlen(tmpWord));	
						return res;
					}	
				}
			}
		}
		// 输入了新的笔画
		else
		{
			if (iEvent<=0x35 && iEvent>=0x31)
			{
				m_caPinyin_in[m_iPinyinLen] = iEvent; 
				m_iPinyinLen++;
				prn_log3("Enter BH = %s, m_iPinyinLen = %d", m_caPinyin_in, m_iPinyinLen);
				
				if(m_iPinyinLen < PINYIN_BUF_LEN)		
				{
					//查找汉字
					errorcode = T9_Strokes_enter(m_caPinyin_in, (UINT8 *)m_caPinyin_out, &cout_size);
					prn_log3("m_caPinyin_out=%s, cout_size=%d", m_caPinyin_out, cout_size);
					
					T9BH_Pageup_Could = 0;
					if (cout_size == 6)
					{
						T9BH_Pagedown_Could = 1;
					}
					else
					{
						T9BH_Pagedown_Could = 0;
					}
					if (cout_size == 6)
					{
						cout_size = 5;
					}
					
					//显示汉字选择条
					m_iHZMaxLen = cout_size;          
					m_iCurHZIndex = 0;
					tmpIndex = 0;
					for(i=0; (i<cout_size && i<PINYIN_PAGE_SIZE); i++) //在m_caPinyin_ChnBuf中生成用来选择汉字的列表。如：0日1月2水3火
					{		
						if (T9BH_state == 1)
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i+1);
						else
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"  ");
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
						if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
							m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
					}	
					
					m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
					
					//在输入框显示已经输入的笔画
					string bihuaDisBuf("");
					for(i=0; i<m_iPinyinLen; i++)
					{
						tmp = m_caPinyin_in[i]-0x31;
						bihuaDisBuf += BihuaHZDis[tmp];
					}
					m_pPYDisInput->SetContentBuf((unsigned char*)bihuaDisBuf.c_str(), m_iPinyinLen*2);
				}
			}
		}
	}

	// 消除键，删除最后一个输入的拼音字母
	else if(iEvent==CLEAR_ONE_EVENT)
	{	
		T9BH_state = 0;
		
		//若有已输入拼音，消去最后输入的字母
		if(m_iPinyinLen>0)			
		{
			m_iPinyinLen--;
			m_caPinyin_in[m_iPinyinLen] = 0;
			
			if (m_iPinyinLen != 0)
			{
				//重新查找汉字		
				errorcode = T9_Strokes_enter(m_caPinyin_in, (UINT8 *)m_caPinyin_out, &cout_size);
				prn_log3("m_caPinyin_out=%s, cout_size=%d", m_caPinyin_out, cout_size);
				
				T9BH_Pageup_Could = 0;
				if (cout_size == 6)
				{
					T9BH_Pagedown_Could = 1;
				}
				else
				{
					T9BH_Pagedown_Could = 0;
				}
				if (cout_size == 6)
				{
					cout_size = 5;
				}

				//显示汉字选择条
				tmpIndex=0;
				m_iHZMaxLen = cout_size;
				m_iCurHZIndex = 0;
				for(i=0; (i<cout_size && i<PINYIN_PAGE_SIZE); i++)
				{	
					if (T9BH_state == 1)
						tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i+1);
					else
						tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"  ");
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
					if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
						m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
				}
				m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
				
				//在输入框显示已经输入的笔画
				string bihuaDisBuf("");
				for(i=0; i<m_iPinyinLen; i++)
				{
					tmp = m_caPinyin_in[i]-0x31;
					bihuaDisBuf += BihuaHZDis[tmp];
				}
				m_pPYDisInput->SetContentBuf((unsigned char*)bihuaDisBuf.c_str(), m_iPinyinLen*2);
				m_pPYDisInput->SetFocus();
				m_bIMEInputFocused = false;
			}
			else
			{
				m_pIMELabel->SetTitle("", 0);
				m_pPYDisInput->SetContentBuf((unsigned char*)"", 0);
				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
			}

		}
		//尚无拼音输入
		else					
		{
			return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
		}
	}
	
	// 清除键
//	else if(iEvent==CLEAR_ALL_EVENT)
//	{;
//	}

	// 上翻键，向前翻字
	else if(iEvent==PAGEUP_KEY)
	{		
		if(m_iPinyinLen>0)
		{
			prn_log2("T9BH_Pageup_Could = %d", T9BH_Pageup_Could);
			if (T9BH_Pageup_Could == 1)
			{
				T9_Strokes_pageUP((UINT8 *)m_caPinyin_out, &cout_size);
				prn_log3("m_caPinyin_out=%s, cout_size=%d", m_caPinyin_out, cout_size);
				m_iHZMaxLen=cout_size;
				m_iCurHZIndex=0;
				
				if (cout_size == 6)
				{
					T9BH_Pageup_Could = 1;
				}
				else
				{
					T9BH_Pageup_Could = 0;
				}
				
				if (cout_size != 0)
				{
					T9BH_Pagedown_Could = 1;
				}
														
				if (cout_size != 0)
				{
					if (cout_size == 6)
					{
						cout_size = 5;
					}	
					
					tmpIndex=0;	
					for(i=0; i<cout_size; i++)
					{		
						if (T9BH_state == 1)
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i+1);
						else
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"  ");
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
						if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
							m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
					}							
					m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
				}
			}
		}
	}
	
	// 下翻键，向后翻字
	else if(iEvent==PAGEDOWN_KEY)
	{		
		if(m_iPinyinLen>0)
		{
			if (T9BH_Pagedown_Could == 1)
			{
				T9_Strokes_pageDown((UINT8 *)m_caPinyin_out, &cout_size);
				prn_log3("m_caPinyin_out=%s, cout_size=%d", m_caPinyin_out, cout_size);
				m_iHZMaxLen=cout_size;
				m_iCurHZIndex=0;
				
				if (cout_size == 6)
				{
					T9BH_Pagedown_Could = 1;
				}
				else
				{
					T9BH_Pagedown_Could = 0;
				}
				
				if (cout_size != 0)
				{
					T9BH_Pageup_Could = 1;
				}
														
				if (cout_size != 0)
				{
					if (cout_size == 6)
					{
						cout_size = 5;
					}	
					
					tmpIndex=0;	
					for(i=0; i<cout_size; i++)
					{		
						if (T9BH_state == 1)
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i+1);
						else
							tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"  ");
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
						if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
							m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
						tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
					}			
					m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
				}
			}
		}
	}

	//切换键：输入笔画或选择汉字
	else if(iEvent == TAB_KEY && m_iPinyinLen != 0)	
	{
		if (T9BH_state == 0)
		{
			T9BH_state = 1;
		}
		else
		{
			T9BH_state = 0;
		}

		// 重新刷新选字条
		tmpIndex=0;
		for(i=0; (i<cout_size && i<PINYIN_PAGE_SIZE); i++)
		{	
			if (T9BH_state == 1)
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i+1);
			else
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"  ");
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
			if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
				m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
		}
		m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
	}

	else
	{
		return 0;
	}
	
	return 1;
}

void CaIMEWin::DisT9py(void)
{
	INT32 disPY_num_max, disPY_num;
	INT8 i, j, m, dispbuf[32];

	memset(dispbuf, 0x20, sizeof(dispbuf));
	dispbuf[sizeof(dispbuf)-1] = '\0';

	// 计算一行内需要显示的拼音数
	disPY_num_max = T9_MAX_DIS_LEN / (m_caT9PYLen+1);
	disPY_num = (PY_num < disPY_num_max) ? PY_num : disPY_num_max;

	m = 0;
	// 生成“←”
	if (PY_CurPos > disPY_num_max-1)
	{
		memcpy(dispbuf+m, "← ", sizeof("← "));
	}
	m += 3;

	// 生成显示拼音字符串
	j = PY_CurPos / disPY_num_max;
	j = j * disPY_num_max;
	for(i=0; i<disPY_num; i++)
	{
		DBG_PRINT(("PYBuf[%d]: %s", j, PYBuf[j]));
		memcpy(dispbuf+m, PYBuf[j], m_caT9PYLen);
		j++;
		m += m_caT9PYLen+1;
	}

	m = 21;
	// 生成“→”
	if (PY_num > j/*PY_num-PY_CurPos > disPY_num_max*/)
	{
		memcpy(dispbuf+m, "→", sizeof("→"));
	}
	m += 2;

	// 显示
	m_T9Label->SetTitle(dispbuf, m);

	// 反色显示当前选中的拼音
	if (PY_num >= 1)
	{
		m_T9Label->SetAntiString((char *)PYBuf[PY_CurPos]);
	}
}

void CaIMEWin::ClearT9py(void)
{
	INT8 dispbuf[32];
	memset(dispbuf, 0x0, sizeof(dispbuf));
	m_T9Label->SetTitle(dispbuf, 0);
	m_T9Label->ReFresh();
}

int CaIMEWin::RecT9py(int iEvent, unsigned char *pEventData, int iDataLen)
{
	UINT8 Enterkey;
	INT8 i, result;
	INT32 tmpEvent, tmpLen, res=1;
	
	Enterkey = (UINT8)iEvent;

	if(Enterkey == ENTER_KEY)			
	{
		if (m_T9state == 1)
		{
			tmpLen = strlen((char *)PYBuf[PY_CurPos]);
			if (PY_CurPos != PY_num-1)
			{
				for(i=0; i<tmpLen; i++)
				{
					tmpEvent = PYBuf[PY_CurPos][i];
					if (i==0)
						tmpEvent += 0x20;
					res = RecPinyin(tmpEvent, pEventData, iDataLen);
				}
				m_T9state++;
				m_T9Label->SetTitle("",0);
			}
			else
			{
				for(i=0; i<tmpLen; i++)
				{
					tmpEvent = PYBuf[PY_CurPos][i];
					res = CaGroup::ProcEvent(tmpEvent,pEventData,iDataLen);
				}
				BeginT9py();
				m_T9state = 3;
			}
		}
		else
		{
			res = 0;
		}
	}			

#if (POS_TYPE==POS_APE3000R)
	else if(Enterkey>=0x30 && Enterkey<=0x39) 
#else // POS_TYPE
		
#if (KEYPAD_TYPE == 1)
	else if(Enterkey>=0x31 && Enterkey<=0x39) 
#else
	else if(Enterkey>=0x31 && Enterkey<=0x39 && Enterkey!=0x37) 
#endif
	
#endif  //POS_TYPE
	{

#if (KEYPAD_TYPE == 1)
		if (m_T9state!=2 && Enterkey==0x31)
		{
			return 0;
		}
#endif
		if (m_T9state==3)
		{
			BeginT9py();
		}
		if (m_caT9PYLen == 0)
		{
			m_T9state = 1;
		}

		if (m_T9state == 1)
		{
			DBG_PRINT(("receive a T9 char."));
			if (m_caT9PYLen == PY_MAX_LENGTH-1)
				return 0;
				
			// 查找输入组合的全部拼音
			m_caT9PYLen++;
			m_caT9PYBuf[m_caT9PYLen-1] = Enterkey - 0x30;
			result = InputT9PY(m_caT9PYBuf, m_caT9PYLen, &PY_num, PYBuf); 
			
			DBG_PRINT(("m_caT9PYLen=%u; m_caT9PYBuf[m_caT9PYLen-1]=%u", m_caT9PYLen, m_caT9PYBuf[m_caT9PYLen-1]));
			DBG_PRINT(("PY_num = %d", PY_num));
			DBG_PRINT(("T9 search complete."));
			
			if (result == SUCCESS)	
			{	
				DBG_PRINT(("search success, and display PinYin."));
				PY_CurPos = 0;
				DisT9py();
			}
			else
			{
				DBG_PRINT(("Pinyin search failure"));
				m_caT9PYBuf[m_caT9PYLen-1] = 0;
				m_caT9PYLen--;
			}
		}
		else
		{
			m_T9state++;
			res = RecPinyin(Enterkey, pEventData, iDataLen);
		}
	}
	else if(Enterkey == CLEAR_ONE_EVENT)
	{	
		if (m_T9state == 3)
		{
			return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);		
		}

		if (m_T9state == 2)
		{
			m_T9state = 1;
		}
		else
		{
			if (m_caT9PYLen>0)
				m_caT9PYLen--;
		}

		if(m_caT9PYLen>0)			
		{
			DBG_PRINT(("cancel last input"));
			// 查找输入组合的全部拼音
			m_caT9PYBuf[m_caT9PYLen] = 0;
			result = InputT9PY(m_caT9PYBuf, m_caT9PYLen, &PY_num, PYBuf); 
			
			if (result == SUCCESS)		//有对应拼音 
			{
				PY_CurPos = 0;
				DisT9py();
			}
			m_pPYDisInput->Clear();

			BeginChn();
		}
		else					
		{
			ClearT9py();
			m_pPYDisInput->Clear();
			BeginChn();
			m_T9state = 3;
		}
	}
	else if (iEvent == CLEAR_ALL_EVENT)
	{
		if (m_T9state == 3)
		{
			return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);		
		}
		else if (m_T9state == 2)
		{
			m_T9state = 1;
		}

		m_caT9PYLen=0;
		memset(m_caT9PYBuf, 0, sizeof(m_caT9PYBuf));
		ClearT9py();
		m_pPYDisInput->Clear();
		BeginChn();
		m_T9state = 3;
	}
#if (POS_TYPE==POS_APE3000R)
	else if(Enterkey == RIGHT_KEY || Enterkey == PAGEDOWN_KEY )
#else
          else if(Enterkey == TAB_KEY || Enterkey == PAGEDOWN_KEY )
#endif
	{		
		DBG_PRINT(("Enterkey = %x, m_T9state = %d", TAB_KEY, m_T9state));
		if (m_T9state == 1)
		{
			DBG_PRINT(("execute pageup."));

			if (PY_num > 1)			
			{	
				if (PY_CurPos == PY_num-1)
				{
					PY_CurPos = 0;
				}
				else
				{
					PY_CurPos++;
				}

				DisT9py();
				m_pPYDisInput->Clear();
				BeginChn();
			}
		}
		else
		{
			res = RecPinyin(Enterkey, pEventData, iDataLen);
		}
	}
#if (POS_TYPE==POS_APE3000R)
		else if (Enterkey == LEFT_KEY || Enterkey == PAGEUP_KEY)
#else
       	else if (Enterkey == PAGEUP_KEY)
#endif
	{
		if (m_T9state == 1)
		{
			DBG_PRINT(("execute pageup."));
			if (PY_num > 1)			
			{		
				if (PY_CurPos == 0)
				{
					PY_CurPos = PY_num-1;
				}
				else
				{
					PY_CurPos--;
				}	

				DisT9py();		
				m_pPYDisInput->Clear();
				BeginChn();
			}
		}
		else
		{
			res = RecPinyin(Enterkey, pEventData, iDataLen);
		}
	}
	else
	{
		return 0;
	}

	return res;	
}

int CaIMEWin::RecPinyin(int iEvent, unsigned char *pEventData, int iDataLen)
{
	unsigned int outSize;
	int tmpIndex, i, j, errorcode, res;
	unsigned char * pPinyinOut;
	unsigned char tmpWord[3];

	outSize = 0;
	tmpIndex = 0;
	
	pPinyinOut = (unsigned char *)m_caPinyin_out;
	ChangeFocus(iEvent,pEventData,iDataLen);
	
	if (m_iPinyinLen > 0)
	{
		if (iEvent == CLEAR_ONE_EVENT)//删除
		{
			m_iPinyinLen --;
			m_caPinyin_in[m_iPinyinLen] = 0;
			if (m_iPinyinLen == 0)
			{
				m_pIMELabel->SetTitle("",0);
				res = CaGroup::ProcEvent(iEvent,pEventData,iDataLen);//删除

				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
				
				return res;
			}
			else
			{
				errorcode = match_keystroke_normal(m_caPinyin_in, m_iPinyinLen,(wchar_t *)m_caPinyin_out,&outSize);
				DBG_ASSERT_EXIT(errorcode == 1,("查询汉字失败!"));
				
				m_iHZMaxLen = outSize;
				m_iCurHZIndex = 0;
				DBG_PRN("info",("共查出 %d 个汉字", outSize));
				tmpIndex = 0;
				for(i = 0; (i < m_iHZMaxLen && i < PINYIN_PAGE_SIZE ); i++)
				{		
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i + 1);
#ifdef WIN_MMI
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
#else
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4+1]);
#endif
				}

				DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
				m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
				return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
			}
		}
		else if (iEvent <= 0x39 && iEvent >= 0x31)//选泽指定的汉字
		{	
			iEvent -= 0x30;
			iEvent --;

			if(iEvent < PINYIN_PAGE_SIZE && iEvent + m_iCurHZIndex < m_iHZMaxLen)
			{	
				m_pIMEInput->SetFocus();
				m_bIMEInputFocused = true;
				res = CaGroup::ProcEvent(INPUT_EVENT,m_caPinyin_ChnBuf+iEvent*4+2,2);				
				memset(tmpWord, 0, sizeof(tmpWord));
				memcpy(tmpWord, m_caPinyin_ChnBuf+iEvent*4+2,2);
				BeginChn();	
				LenovoProc('F', tmpWord, strlen((char *) tmpWord));	
				return res;
			}
			else
			{
				return 0;
			}
		}
		else if(iEvent == PAGEDOWN_KEY)//下翻
		{
			if (m_iCurHZIndex + PINYIN_PAGE_SIZE < m_iHZMaxLen)
			{
				tmpIndex = 0;
				j = 0;
				m_iCurHZIndex += PINYIN_PAGE_SIZE;
				
				for(i = m_iCurHZIndex; (i < m_iCurHZIndex + PINYIN_PAGE_SIZE)  && (i < m_iHZMaxLen); i++)
				{	
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",j + 1);
#ifdef WIN_MMI
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
#else
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4+1]);
#endif
					j ++;
				}

				DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
				m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
			}
			
			return 1;
		}
		else if(iEvent == PAGEUP_KEY)//上翻
		{	
			if (m_iCurHZIndex - PINYIN_PAGE_SIZE >= 0)
			{
				tmpIndex = 0;
				j = 0;
				m_iCurHZIndex -= PINYIN_PAGE_SIZE;			
				for(i = m_iCurHZIndex; i < m_iCurHZIndex + PINYIN_PAGE_SIZE; i++)
				{		
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",j + 1);
#ifdef WIN_MMI
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
#else
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4]);
					tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4+1]);
#endif
					j ++;
				}

				DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
				m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
			}

			return 1;
		}
		else if (iEvent == CLEAR_ALL_EVENT)
		{
			res = CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
			BeginChn();
			BeginT9py();
			m_pIMEInput->SetFocus();
			m_bIMEInputFocused = true;
			return res;
		}
	}
	else
	{;	//由于有联想输入,所以这里还要初始化一下
		DBG_PRINT(("step1: iEvent = %x", iEvent));
		//BeginChn();
		//ChangeFocus(iEvent,pEventData,iDataLen);
	}

	if(iEvent <= 0x7A && iEvent >= 0x61) //输入了拼音
	{
		if (m_iPinyinLen >= MAX_PINYIN_LEN)
		{
			return 0;
		}
		m_caPinyin_in[m_iPinyinLen]=(unsigned char)(iEvent);

		m_iPinyinLen++;

		errorcode = match_keystroke_normal(m_caPinyin_in, m_iPinyinLen,(wchar_t *)m_caPinyin_out,&outSize);
		DBG_ASSERT_EXIT(errorcode == 1,("查询汉字失败!"));
		
		m_iHZMaxLen = outSize;
		m_iCurHZIndex = 0;
		DBG_PRN("info",("共查出 %d 个汉字", outSize));
		for(i = 0; (i < m_iHZMaxLen && i < PINYIN_PAGE_SIZE ); i++)
		{		
			tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",i + 1);
#ifdef WIN_MMI
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
#else
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4]);
			tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*4+1]);
#endif
		}
		m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}
	
	DBG_PRINT(("m_bIMEInputFocused = %d", m_bIMEInputFocused));
	if (!m_bIMEInputFocused)
	{
		return 0;//拼音输入框中其它的事件不予处理
	}
	else
	{
		if (iEvent == TAB_KEY)	//加入联想后，TAB键不再此处处理。
		{
			return 0;
		}
	}
	
	return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
}

int CaIMEWin::RecWB(int iEvent, unsigned char *pEventData, int iDataLen)
{
	return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
}

int CaIMEWin::ProcDirectInput(int iEvent, unsigned char *pEventData, int iDataLen)
{
	int ascByte;
	int i;
	
	ChangeFocus(iEvent,pEventData,iDataLen);

	if (((m_IMEType == IME_UPPER_CASE) || (m_IMEType == IME_LOWER_CASE)) 
		&& ((iEvent >= '1') && (iEvent <= '9')))
	{
		for (i=0; i<9; i++)
		{
			if (CaIMEWin::s_ascKeyList[i].key == iEvent-0x30)
				break;
		}
		
		UINT64 fTime;
#ifndef WIN32
		struct timeb tp;
		ftime(&tp);
		fTime = tp.time * 1000 + tp.millitm;
#else
		//fTime=GetTickCount();
#endif
		DBG_PRINT(("iEvent = %x",iEvent));
		if (iEvent == m_T9ASC_lastkey && fTime-m_T9ASC_lastTime<T9ASC_DELAY_TIME)
		{
			ascByte = *(CaIMEWin::s_ascKeyList[i].list + m_T9ASC_clickCount);
			if (ascByte == '\0')
			{
				m_T9ASC_clickCount = 0;
				ascByte = *(CaIMEWin::s_ascKeyList[i].list + m_T9ASC_clickCount);
			}
			m_T9ASC_clickCount++;
			CaGroup::ProcEvent(CLEAR_ONE_EVENT,pEventData,iDataLen);			
		}
		else
		{
			m_T9ASC_clickCount = 0;
			ascByte = *(CaIMEWin::s_ascKeyList[i].list + m_T9ASC_clickCount);
			m_T9ASC_clickCount++;
		}

		m_T9ASC_lastTime = fTime;
		m_T9ASC_lastkey = iEvent;
		
		if ( m_IMEType==IME_UPPER_CASE && (ascByte>='a' && ascByte<='z') )
			ascByte -= 0x20;

		DBG_PRINT(("ascByte = %c",ascByte));
		return CaGroup::ProcEvent(ascByte,pEventData,iDataLen);
	}
	else if (iEvent == CLEAR_ONE_EVENT || iEvent == CLEAR_ALL_EVENT )
	{
		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}
	else
	{
		return 0;
	}
}

int CaIMEWin::RecHandWrite(int iEvent, unsigned char *pEventData, int iDataLen)
{
	unsigned char cout_size;
	int tmpIndex, i, j, res;
	unsigned char * pPinyinOut;
	unsigned char tmpWord[3];
	unsigned char *pchar;
	unsigned char showlen=2;
	
	DBG_PRINT(("Enter handwrite Function."));
	
	pPinyinOut=(unsigned char *)m_caPinyin_out;
	cout_size = m_iHZMaxLen;
	
	// 手写板接收
	if (iEvent == HW_REV_EVENT)
	{
		if (cout_size==1 && pPinyinOut[0]==0x00 && pPinyinOut[1]==0x08)		//退格手势	
		{
			return CaGroup::ProcEvent(CLEAR_ONE_EVENT,pEventData,iDataLen);
		}
		else if (cout_size==1 && pPinyinOut[0]==0x00 && pPinyinOut[1]==0x0d)	//回车手势	
		{
			return CaGroup::ProcEvent(ENTER_KEY,pEventData,iDataLen);
		}
		else if (cout_size==1 && pPinyinOut[0]==0x00 && pPinyinOut[1]==0x20)	//空格手势	
		{
			return CaGroup::ProcEvent(' ',pEventData,iDataLen);
		}
		else if (cout_size==0xff)			//cout_size=0xff, 表示选择了汉字0
		{
			if (m_handwrite_flag == 1)
			    iEvent = 0x31;
		}
		else									//输入了手写字符
		{
			tmpIndex = 0;
			j = 0;
			m_iCurHZIndex=0;
			for(i = m_iCurHZIndex; i < m_iCurHZIndex + PINYIN_PAGE_SIZE; i++)
			{		
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",j + 1);
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
				if (m_caPinyin_ChnBuf[tmpIndex-1] == 0)
					m_caPinyin_ChnBuf[tmpIndex-1] = 0x20;
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
				j ++;
			}

			DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
			m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);

			m_handwrite_flag = 1;
			return 1;
		}
	}
	
	// 输入数字键选择汉字
	if( iEvent<=0x39 && iEvent>=0x30 )			
	{
		iEvent -= 0x30;	
		iEvent--;

		if(iEvent < PINYIN_PAGE_SIZE && iEvent + m_iCurHZIndex < m_iHZMaxLen)
		{	
			m_pIMEInput->SetFocus();
			m_bIMEInputFocused = true;
			pchar = m_caPinyin_ChnBuf+iEvent*4+2;
			if (0x20 == *pchar)
			{
				pchar++;
				showlen = 1;
			}
			res = CaGroup::ProcEvent(INPUT_EVENT, pchar, showlen);				
			BeginChn();
			memset(tmpWord, 0, sizeof(tmpWord));
			memcpy(tmpWord, pchar, showlen);
			//LenovoProc('F', tmpWord, strlen((char *) tmpWord));	
			return res;
		}
		else
		{
			return 0;
		}
	}

	// 上翻键，向前翻字
	else if(iEvent==PAGEUP_KEY)
	{		
		if (m_iCurHZIndex - PINYIN_PAGE_SIZE >= 0)
		{
			tmpIndex = 0;
			j = 0;
			m_iCurHZIndex -= PINYIN_PAGE_SIZE;			
			for(i = m_iCurHZIndex; i < m_iCurHZIndex + PINYIN_PAGE_SIZE; i++)
			{		
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",j + 1);
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
				j ++;
			}

			DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
			m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
		}

		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}
	
	// 下翻键，向后翻字
	else if(iEvent==PAGEDOWN_KEY)
	{		
		if (m_iCurHZIndex + PINYIN_PAGE_SIZE < m_iHZMaxLen)
		{
			tmpIndex = 0;
			j = 0;
			m_iCurHZIndex += PINYIN_PAGE_SIZE;
			
			for(i = m_iCurHZIndex; (i < m_iCurHZIndex + PINYIN_PAGE_SIZE)  && (i < m_iHZMaxLen); i++)
			{	
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," %d",j + 1);
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2]);
				tmpIndex +=sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%c",pPinyinOut[i*2+1]);
				j ++;
			}

			DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
			m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
		}

		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}
	
	//删除最后字符
	else if (iEvent == CLEAR_ONE_EVENT || iEvent == CLEAR_ALL_EVENT )
	{
		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}
	
	else 
	{
		return 0;
	}

}

int CaIMEWin::RecScreenHandWrite(int iEvent, unsigned char *pEventData, int iDataLen)
{
	int cout_size = 0;
	int tmpIndex, i, j, res;
	unsigned char * pPinyinOut;
	unsigned char tmpWord[3];
	
	DBG_PRINT(("Enter handwrite Function."));
	DBG_PRN("info",("RecScreenHandWrite"));
	
//	pPinyinOut=(unsigned char *)m_caPinyin_out;
//	cout_size = m_iHZMaxLen;
	
	// 手写板接收
	if (iEvent == HW_REV_EVENT)
	{
		memset((void*)m_chHwBuf, 0, sizeof(m_chHwBuf));
		cout_size = HwDataAdjust(m_chHwBuf, sizeof(m_chHwBuf));//获取并解析手写数据
		DBG_PRN("info",("cout_size = %d", cout_size));
		if (cout_size == 0)
		{
			DBG_PRN("info",(" cout_size == 0"));
			return FAILURE;
		}
		m_pIMEInput->SetFocus();
		m_bIMEInputFocused = true;
#if(POS_TYPE==POS_APE3000R)
	DBG_PRN("info",(" 字符串m_chHwBuf[2]起 = %s", &m_chHwBuf[2]));
		res = CaGroup::ProcEvent(INPUT_EVENT,&m_chHwBuf[3],cout_size);//input中显示 航信手写板
#else //POS_TYPE
#if (SCREEN_HW_TYPE == 0)
		DBG_PRN("info",(" 字符串m_chHwBuf[2]起 = %s", &m_chHwBuf[2]));
		res = CaGroup::ProcEvent(INPUT_EVENT,&m_chHwBuf[2],cout_size);//input中显示	
#else
		printf("字符串m_chHwBuf[3]起 = %s\n", &m_chHwBuf[3]);
		res = CaGroup::ProcEvent(INPUT_EVENT,&m_chHwBuf[3],cout_size);//input中显示	
#endif
#endif //POS_TYPE

		DBG_PRN("info",(" CaGroup::ProcEvent  res = %d", res));
		BeginChn();
		return 1;
	}	
	//删除最后字符
	else if (iEvent == CLEAR_ONE_EVENT || iEvent == CLEAR_ALL_EVENT )
	{
		return CaGroup::ProcEvent(iEvent,pEventData,iDataLen);
	}	
	else 
	{
		return 0;
	}
}

int CaIMEWin::LenovoProc(int iEvent, unsigned char *pEventData, int iDataLen)
{
	int errorcode, tmpIndex, res;
	UINT8 outSize;
	int i, j;
	unsigned char tmpbuff[128];
	unsigned char tmpWord[3];
	int wordLen = 0;

	//选泽指定的词组
	if (iEvent<=0x39 && iEvent>=0x30)
	{
		iEvent -= 0x30;

		if (m_T9LenovoState == 1)
		{
			if(iEvent < m_iMaxWordsSize)
			{	
				memset(tmpbuff, 0, sizeof(tmpbuff));
				sprintf((char *)tmpbuff, "%s", Phrase_Result(m_iCurHZIndex + iEvent));
				wordLen = strlen(Phrase_Result(m_iCurHZIndex + iEvent));

				res = CaGroup::ProcEvent(INPUT_EVENT,tmpbuff,wordLen);	

				memset(tmpWord, 0, sizeof(tmpWord));
				memcpy(tmpWord, tmpbuff+wordLen-2, 2);
				DBG_PRN("info", ("tmpWord = %s", tmpWord));
	
				m_iPinyinLen=0;

				switch(m_IMEType)
				{
				case IME_PIN_YIN:
					BeginChn();		
					break;
				case IME_BI_HUA:
					BeginT9Bihua();
					break;
				case IME_QU_WEI:
					BeginQW();
					break;			
				case IME_HAND_WRITE:
					BeginChn();
					HwInit();
					break;			
				default:
					break;
				}

				m_T9LenovoState = 0;  //使联想时不显示选择序号
				LenovoProc('F', tmpWord, strlen((char *) tmpWord));
				m_T9LenovoState = 1;	//由于m_T9LenovoState＝0 时return也为1，
										//所以此处将m_T9LenovoState置为1状态，便于在proevent中处理(临时性处理). zl
				return res;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			switch(m_IMEType)
			{
			case IME_PIN_YIN:
#if (POS_TYPE==POS_APE3000R)
				BeginChn();	
#else //POS_TYPE
#if (KEYPAD_TYPE == 1)
		if (iEvent!=0x0 && iEvent!=0x1)
#else
		if (iEvent!=0x0 && iEvent!=0x7)
#endif
			BeginChn();		
			else
			return 0;
#endif //POS_TYPE	
				break;
			case IME_BI_HUA:
				BeginT9Bihua();
				break;
			case IME_QU_WEI:
				BeginQW();
				break;				
			case IME_HAND_WRITE:
				BeginChn();
				HwInit();
				break;				
			default:
				break;
			}		
			m_iLineIndex = 0;
			m_bLenovoOn = false;
			return 1;
		}
	}
	//iEvent = 'F'代表查找联系词汇
	else if (iEvent == 'F' && iDataLen == 2)
	{
		errorcode = Phrase_Strokes_enter((char *)pEventData,&outSize);
		DBG_PRN("info",("共查出 %d 个字(词)", outSize));

		m_iHZMaxLen = (int)outSize;

		m_iCurHZIndex = 0;
		tmpIndex = 0;
		m_iMaxWordsSize = 0;
		m_iLineIndex = 0;
		memset(m_RandLenBuf, 0, PHRASE_BUFFER_LEN);

		if( m_iHZMaxLen <= 0 )
		{
			m_bLenovoOn = false;
			return 0;
		}

		memset(m_caPinyin_ChnBuf, 0, sizeof(m_caPinyin_ChnBuf));
		for(i=0; i<m_iHZMaxLen; i++)
		{
			DBG_PRN("info", ("Phrase_Result(%d) = '%s'",i,Phrase_Result(i)));

//			if( m_IMEType == BIHUA )
//			{
//				if ( i >= BH_MAX_WORDS_SIZE ||				//笔画最多能显示0----4
//					tmpIndex + strlen(Phrase_Result(i))+2 > LENOVO_MAX_DIS_LEN)
//				{
//					break;//不能全部显示
//				}
//			}
//			else
//			{
				if (tmpIndex + strlen(Phrase_Result(i))+2 > LENOVO_MAX_DIS_LEN)
				{
					break;//不能全部显示
				}
//			}
			m_iMaxWordsSize ++;

			if (m_T9LenovoState)
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%d",i);
			else
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," ");
			
			tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%s", Phrase_Result(i));
		}

		if (tmpIndex == 0)//第一个显示就超长了
		{
			m_iMaxWordsSize ++;
			if (m_T9LenovoState)
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf,"1");
			else
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf," ");
			tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,	"%s", Phrase_Result(i));
		}

		DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
		m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);

		m_bLenovoOn = true;
	}
	//下翻
	else if(iEvent == PAGEDOWN_KEY)
	{
		if (m_iCurHZIndex + m_iMaxWordsSize < m_iHZMaxLen)
		{
			m_iCurHZIndex += m_iMaxWordsSize;
			m_RandLenBuf[m_iLineIndex ++] = m_iMaxWordsSize;

			tmpIndex = 0;
			j = 0;				
			m_iMaxWordsSize = 0;

			memset(m_caPinyin_ChnBuf, 0, sizeof(m_caPinyin_ChnBuf));
			for(i=m_iCurHZIndex; i<m_iHZMaxLen; i++)
			{
				DBG_PRN("info", ("Phrase_Result(%d) = '%s'", i, Phrase_Result(i)));
//				if( m_pCurrWin->m_InputMode == BIHUA )
//				{
//					if ( j >= BH_MAX_WORDS_SIZE ||				//笔画最多能显示0----4
//						tmpIndex + strlen(Phrase_Result(i))+2 > LENOVO_MAX_DIS_LEN)
//					{
//						break;//不能全部显示
//					}
//				}
//				else
//				{
					if (tmpIndex + strlen(Phrase_Result(i))+2 > LENOVO_MAX_DIS_LEN)
					{
						break;//不能全部显示
					}
//				}
				
				m_iMaxWordsSize ++;
				
				if (m_T9LenovoState)
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%d",j);
				else
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," ");

				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,	"%s", Phrase_Result(i));
				j ++;
			}
			if (tmpIndex == 0)//第一个字(词)就超长
			{
				m_iMaxWordsSize ++;
				if (m_T9LenovoState)
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf,"1");
				else
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf," ");
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,	"%s", Phrase_Result(m_iCurHZIndex));
			}

			DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
			m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);

			return 1;
		}
	}
	//上翻
	else if(iEvent == PAGEUP_KEY)
	{	
		if (m_iCurHZIndex != 0 && m_iLineIndex > 0)//上翻还有汉字
		{
			tmpIndex = 0;			
			j = 0;
			m_iMaxWordsSize = m_RandLenBuf[-- m_iLineIndex];
			m_iCurHZIndex -= m_iMaxWordsSize;

			memset(m_caPinyin_ChnBuf, 0, sizeof(m_caPinyin_ChnBuf));
			for(i = m_iCurHZIndex; i<m_iCurHZIndex + m_iMaxWordsSize; i++)
			{
				if (m_T9LenovoState)
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%d",j);
				else
					tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," ");

				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,	"%s", Phrase_Result(i));
				j ++;
			}

			DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
			m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);

			return 1;
		}
	}
	//切换键：输入下一个拼音或选定联想词汇
	else if(iEvent == TAB_KEY)	
	{
		j=0;
		if (m_T9LenovoState == 0)
		{
			m_T9LenovoState = 1;
		}
		else
		{
			m_T9LenovoState = 0;
		}

		// 重新刷新选字条
		m_iMaxWordsSize = 0;
		tmpIndex = 0;
		for(i=m_iCurHZIndex; i<m_iHZMaxLen; i++)
		{
			DBG_PRN("info", ("Phrase_Result(%d) = '%s'",i,Phrase_Result(i)));
			if (tmpIndex + strlen(Phrase_Result(i))+2 > LENOVO_MAX_DIS_LEN)
			{
				break;//不能全部显示
			}
			m_iMaxWordsSize ++;

			if (m_T9LenovoState)
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%d",j);
			else
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex," ");
			
			tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,"%s", Phrase_Result(i));

			j++;
		}

		if (tmpIndex == 0)//第一个显示就超长了
		{
			m_iMaxWordsSize ++;
			if (m_T9LenovoState)
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf,"1");
			else
				tmpIndex += sprintf((char *)m_caPinyin_ChnBuf," ");
			tmpIndex += sprintf((char *)m_caPinyin_ChnBuf+tmpIndex,	"%s", Phrase_Result(i));
		}
		
		DBG_PRN("info",("m_caPinyin_ChnBuf = %s", m_caPinyin_ChnBuf));
		m_pIMELabel->SetTitle((char *)m_caPinyin_ChnBuf, tmpIndex);
		return 1;
	}

	return 0;
}

unsigned char CaIMEWin::InputT9PY(unsigned char *key, unsigned char enterNum, 
									unsigned char *outNum, unsigned char (*PYBuf)[PY_MAX_LENGTH])
{
	unsigned char result;
	int j;
	
	if (key[enterNum-1] != 0 && enterNum<=6)
	{
		result = T9_Input(key, outNum, PYBuf);	
		if (result != SUCCESS)	
		{
			return FAILURE;
		}

		//将PYBuf[outNum]设置为按键对应的数字 by zl 20100818
//		for (j=*outNum; j>0; j--)
//		{
//			memcpy(PYBuf[j], PYBuf[j-1], sizeof(PYBuf[j]));
//		}
		j=0;
		while(*(key+j) != 0)
		{
			PYBuf[*outNum][j] = *(key+j)+0x30;
			j++;
		}
		(*outNum) ++;
	}
	else
	{
		memset(PYBuf[0], 0, sizeof(PYBuf[0]));
		for (j=0; j<enterNum; j++)
		{
			PYBuf[0][j] = *(key+j)+0x30;
		}
		*outNum = 1;
	}

	return SUCCESS;

}
