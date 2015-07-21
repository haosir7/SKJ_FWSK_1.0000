/**
 @file  Net3GWlenMenu.h
 @brief 界面文件，3G管理菜单
*/
#ifndef NET_3G_WLEN_MENU_H
#define NET_3G_WLEN_MENU_H


#include "IncludeMe.h"
#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"



class CNet3GWlenMenu : public CMultiBtnCommonWin
{
public:
	CNet3GWlenMenu();

	int Create(int iX,int iY,int iW,int iH);

	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);


private:

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);



};

#endif



