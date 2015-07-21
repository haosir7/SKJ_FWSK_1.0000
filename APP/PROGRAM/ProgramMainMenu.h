/*! \file  ProgramMainMenu.h
\brief    功能编程主界面
\author   xsr
\version  1.0
\date     2010-7-28
*/

#ifndef PROGRAM_CTRL_MENU_H
#define PROGRAM_CTRL_MENU_H

#include "CMainFrame.h"
#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CUserInfo.h"

using namespace std;

class CProgramCtrlMenu : public CMultiBtnCommonWin
{
public:
	CProgramCtrlMenu();
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
private:
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen); 
	void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen); 
	void OnButton6(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton7(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton8(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton9(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton10(int iEvent, unsigned char * pEventData, int iDataLen);
public :
	CUserInfo m_userInfo;

};


#endif
