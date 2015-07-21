/**
 @file  NetModeSetMenu.h
 @brief 界面文件，网络模式设置菜单
*/
#ifndef NET_MODE_SET_MENU_H
#define NET_MODE_SET_MENU_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaButton.h"
#include "MultiBtnCommonWin.h"


class CNetModeSetMenu : public CMultiBtnCommonWin
{
public:
	CNetModeSetMenu();
		
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);


private:

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);


};

#endif



