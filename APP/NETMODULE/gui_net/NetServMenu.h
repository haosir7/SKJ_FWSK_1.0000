/*! \file    NetServMenu.h
   \brief    awe4000r系统维护的网络维护菜单
   \author   Yu Yan
   \version  1.0
   \date     2010-10-9 16:53
 */


#ifndef NET_SERV_MENU_H
#define NET_SERV_MENU_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "MultiBtnCommonWin.h"


/**
 *@class CNetServMenu 
 *@brief 系统维护的网络维护菜单
 */
class CNetServMenu : public CMultiBtnCommonWin
{
public:

	CNetServMenu();

	int Create(int iX,int iY,int iW,int iH);

	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

private:

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);


	INT8 EnableM2M(void);
	INT8 DisableM2M(void);

};

#endif