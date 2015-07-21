/*! \file    SelfTestMenu.cpp
   \brief    awe4000r销售程序中机器自检主菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-3-19
 */

#include "SelfTestMenu.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CGlobalArg.h"
#include "lcd_tiny.h"
#include "MaintenanceFunc.h"
#include "SysArgMenu.h"
#include "SysArgMac.h"
#include "commonFunc.h"
#include "TemplatePrintInv.h"
#include "PrintDriver.h"

extern UINT8 bidirection;

CSelfTestMenu::CSelfTestMenu()
{
	ifLightOn = 1;
	int i;
#if (POS_TYPE == POS_APE5020R)
	m_iNumOfBtn = 14;
#else
        m_iNumOfBtn = 15;
#endif
	for(i = 0; i < m_iNumOfBtn; i++)
	{
		m_pBtn[i] = NULL;
	}
	m_pBtnList = new CaButtonList(2);
	m_pBtnList->Create(0,0,SCREEN_W ,SCREEN_H);

}

CSelfTestMenu::~CSelfTestMenu()
{
}

int CSelfTestMenu::Create(int iX,int iY,int iW,int iH)
{
	int curH, i;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

    int m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	int m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,SELF_TEST_MENU);

	CaWindow::Create(iX,iY,iW,iH); // creat a window

	for(i = 0; i < m_iNumOfBtn; i++)
	{
		m_pBtn[i] = m_pBtnList->CreateNewButton();
		m_pBtn[i]->BoxType(CaObject::BOX_NO);
		m_pBtn[i]->SetTitleAlign(CaObject::ALIGN_LEFT);
	}

	strcpy(title, "定时自检(A)");
	m_pBtn[0]->SetTitle(title, strlen(title));
	m_pBtn[0]->OnObject = S_OnButton0;

	strcpy(title, "连续自检(B)");
	m_pBtn[1]->SetTitle(title, strlen(title));
	m_pBtn[1]->OnObject = S_OnButton1;

	strcpy(title, "FLASH自检(C)");
	m_pBtn[2]->SetTitle(title, strlen(title));
	m_pBtn[2]->OnObject = S_OnButton2;

	strcpy(title, "CPU自检(D)");
	m_pBtn[3]->SetTitle(title, strlen(title));
	m_pBtn[3]->OnObject = S_OnButton3;

	strcpy(title, "COM1自检(E)");
	m_pBtn[4]->SetTitle(title, strlen(title));
	m_pBtn[4]->OnObject = S_OnButton4;

	strcpy(title, "打印机自检(F)");
	m_pBtn[5]->SetTitle(title, strlen(title));
	m_pBtn[5]->OnObject = S_OnButton5;

        	strcpy(title, "打印参数(G)");
	m_pBtn[6]->SetTitle(title, strlen(title));
	m_pBtn[6]->OnObject = S_OnButton6;

	strcpy(title, "显示屏自检(H)");
	m_pBtn[7]->SetTitle(title, strlen(title));
	m_pBtn[7]->OnObject = S_OnButton7;

	strcpy(title, "键盘自检(I)");
	m_pBtn[8]->SetTitle(title, strlen(title));
	m_pBtn[8]->OnObject = S_OnButton8;

	strcpy(title, "钱箱自检(J)");
	m_pBtn[9]->SetTitle(title, strlen(title));
	m_pBtn[9]->OnObject = S_OnButton9;

	strcpy(title, "蜂鸣器自检(0)");
	m_pBtn[10]->SetTitle(title, strlen(title));
	m_pBtn[10]->OnObject = S_OnButton10;

	strcpy(title, "RAM自检(1)");
	m_pBtn[11]->SetTitle(title, strlen(title));
	m_pBtn[11]->OnObject = S_OnButton11;

	strcpy(title, "USB自检(5)");
	m_pBtn[12]->SetTitle(title, strlen(title));
	m_pBtn[12]->OnObject = S_OnButton12;

	strcpy(title, "时  间(7)");
	m_pBtn[13]->SetTitle(title, strlen(title));
	m_pBtn[13]->OnObject = S_OnButton13;
	
#if (POS_TYPE != POS_APE5020R)
	strcpy(title, "COM2自检(9)");
	m_pBtn[14]->SetTitle(title, strlen(title));
	m_pBtn[14]->OnObject = S_OnButton14;
#endif




   m_pBtnList->End();
	this->End();
	return 0;
}

void CSelfTestMenu::S_OnButton0(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton0(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton4(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton5(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton6(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton7(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton8(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton9(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton10(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton10(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton11(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton11(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton12(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton12(iEvent,pEventData,iDataLen);
}

void CSelfTestMenu::S_OnButton13(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton13(iEvent,pEventData,iDataLen);
}
#if (POS_TYPE != POS_APE5020R)
 void CSelfTestMenu::S_OnButton14(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSelfTestMenu *win=(CSelfTestMenu *)obj->GetdWin();
	win->OnButton14(iEvent,pEventData,iDataLen);
}
#endif

void CSelfTestMenu::OnButton0(int iEvent, unsigned char * pEventData, int iDataLen)
{
	MachineAutoCheck();
	this->ReFresh();		
}

void CSelfTestMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ContinuousTestEvent();
	this->ReFresh();		
}

void CSelfTestMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	FlashEvent(1);
	this->ReFresh();
}

void CSelfTestMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
//	if (ifLightOn == 1)
//	{
//#ifndef WIN32
//		WB_LCM_BKL_OFF();//熄灭背光灯
//#endif
//		ifLightOn = 0;
//	}
//	else
//	{
//#ifndef WIN32
//		WB_LCM_BKL_ON();//点亮背光灯
//#endif
//		ifLightOn = 1;
//	}
	CPUEvent(1);
	this->ReFresh();
}

void CSelfTestMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
#if RELEASE_VER == 0
	CaMsgBox::ShowMsg("测试程序不支持自检");
#else
	COM0Event(1);
#endif
	
	this->ReFresh();
}
void CSelfTestMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	PrinterEvent(1);
	this->ReFresh();
}

#if (POS_TYPE != POS_APE5020R)
void CSelfTestMenu::OnButton14(int iEvent, unsigned char * pEventData, int iDataLen)
{
	COM2Event(1);
	this->ReFresh();
}
#endif

void CSelfTestMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
	LCDEvent(1);
	DBG_PRINT(("LCDEvent(1)"));
	this->ReFresh();
	DBG_PRINT(("this->ReFresh()"));
}

void CSelfTestMenu::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
	KeyboardEvent(1);
	DBG_PRINT(("KeyboardEvent() over"));
	this->ReFresh();
	DBG_PRINT(("KeyboardEvent() ReFresh"));

}

void CSelfTestMenu::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{
	BoxEvent(1);
	this->ReFresh();	
}

void CSelfTestMenu::OnButton10(int iEvent, unsigned char * pEventData, int iDataLen)
{
	BeepEvent(1);
	this->ReFresh();		
}

void CSelfTestMenu::OnButton11(int iEvent, unsigned char * pEventData, int iDataLen)
{
	RAMEvent(1);
	this->ReFresh();
}

void CSelfTestMenu::OnButton12(int iEvent, unsigned char * pEventData, int iDataLen)
{
	USBEvent(1);
	this->ReFresh();	
}

void CSelfTestMenu::OnButton13(int iEvent, unsigned char * pEventData, int iDataLen)
{
	TDateTime curDate;
	char chValue[64];
	UINT8 nKeyTime = 1;
	while(1)
	{
		curDate = TDateTime::CurrentDateTime();
		memset(chValue, 0, sizeof(chValue));
		sprintf(chValue, "   %s", curDate.FormatString(YYYYMMDDHHMMSS).c_str());
		
		CaProgressBar proBar(chValue);
		proBar.Show();
		
		if(CheckKey(nKeyTime)==SUCCESS)
		{
			break;
		}
		CommonSleep(1000);
	}
	
	this->ReFresh();
	return;
}

void CSelfTestMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
//	string strErr = "是否调整打印参数？";
	string strErr = "是否放入白纸打印？";	
	if(YesNoMsBox(strErr)==FAILURE)
	{
		this->ReFresh();
		return;
	}

	if (isPaper() != 0)
	{ 
		CaMsgBox::ShowMsg("打印机未检测到纸");
		return;
	}

	char chValue[256];
	
	UINT8 i =0;
	for(i=0;i<10;i++)
	{
		memset(chValue, 0, sizeof(chValue));

		//bidirection = i;
		DBG_PRINT(("i=%u", i));
		DBG_PRINT(("bidirection=%u", bidirection));

		PrinterIni(i);	
		
//		sprintf(chValue, "双向打印参数打印:航天信息蜂鸣器正常异常状态 参数%uHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH",i);
		sprintf(chValue, "双向打印参数打印:航天信息蜂鸣器正常异常状态 参数%u",i);
		
		//PrintChineseLine(chValue, strlen(chValue));
		PrintAsciiLine(chValue, strlen(chValue));
		SetLineSpace(0);
		sprintf(chValue, "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
		//PrintChineseLine(chValue, strlen(chValue));
		PrintAsciiLine(chValue, strlen(chValue));
		sprintf(chValue, "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
		//PrintStr(chValue);
		//PrintChineseLine(chValue, strlen(chValue));
	    PrintAsciiLine(chValue, strlen(chValue));
		CarriageReturn();
	}

	ForwardNLine(FORWARD_LINES);
	PrinterIni(bidirection);


	m_pSysArgEditWin = (CSysArgEditWin *)(m_pFrame->GetWin(SYS_ARG_EDIT_WIN));

	((CSysArgMenu *)m_pFrame->GetWin(SYSTEM_ARG_MENU))->m_EditSysArgID = SYS_BIDIRECTION_PRINT;

//	m_pSysArgEditWin = (CSysArgEditWin *)(m_pFrame->GetWin(SYS_ARG_EDIT_WIN));

	
	m_pSysArgEditWin->ReFresh();
	ChangeWin(SYS_ARG_EDIT_WIN);


//	this->ReFresh();
}

int CSelfTestMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	int IfPress = 0;
	int index = -1;

	 if (RETURN_MAIN_MENU == iEvent)
	 {
		ChangeWin(MAINTENANCE_MENU);
		return SUCCESS;
	 }

	 //更新出厂日期
	 if( iEvent == SUM_INPUT )
	 {
		 ChangeWin(UPDATE_MACHINE_INFO_WIN);
		 return SUCCESS;
	 }

	 switch(iEvent)
	 {
		case LETTER_A:
			index = 0;
			break;
		case LETTER_B:
			index = 1;
			break;
		case LETTER_C:
			index = 2;
			break;
		case LETTER_D:
			index = 3;
			break;
		case LETTER_E:
			index = 4;
			break;			
		case LETTER_F:
			index = 5;
			break;
		case LETTER_G:
			index = 6;
			break;
		case LETTER_H:
			index = 7;
			break;
		case LETTER_I:
			index = 8;
			break;
		case LETTER_J:
			index = 9;
			break;
		case DIGIT0:
			index = 10;
			break;
		case DIGIT1:
			index = 11;
			break;
		case DIGIT5:
			index = 12;
			break;
		case DIGIT7:
			index = 13;
			break;
		case DIGIT9:
			index = 14;
			break;
	 }

	 if (index!=-1)
	 {
		m_pBtnList->SetFocusToObj(m_pBtn[index]);
		this->ReFresh();
		m_pBtnList->CurObj()->OnObject(m_pBtnList->CurObj(), iEvent, pEventData, iDataLen);
		return SUCCESS;
	 }

	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CSelfTestMenu::ReFresh()
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


