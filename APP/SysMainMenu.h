#ifndef SYS_MAIN_MENU_H
#define SYS_MAIN_MENU_H
#include "MultiBtnCommonWin.h"
#include"LOGCTRL.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaInput.h"
#include "CaButton.h"
#include <string>

using namespace std;

class CSysMainMenu : public CMultiBtnCommonWin
{
public:
	CSysMainMenu();
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
private:
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen); 
	void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen); 
};

#endif


