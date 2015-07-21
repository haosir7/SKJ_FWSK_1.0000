/**
 @file  LocalNetworkMenu.h
 @brief 界面文件，IP模式菜单
*/
#ifndef LOCAL_NETWORK_MENU_H
#define LOCAL_NETWORK_MENU_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaButton.h"
#include "MultiBtnCommonWin.h"


class CLocalNetworkMenu : public CMultiBtnCommonWin
{
public:
	CLocalNetworkMenu();

	int Create(int iX,int iY,int iW,int iH);

	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);


private:

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);



};

#endif



