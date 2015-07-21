/**
 @file  NetManageMenu.h
 @brief 界面文件，网络模块主菜单
*/
#ifndef NET_MANAGE_MENU_H
#define NET_MANAGE_MENU_H

#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "netConnection.h"

class CNetManageMenu : public CMultiBtnCommonWin
{
public:

	CNetManageMenu();
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

	void netConnectProc(CNetConnection *pNet);
	void ConnectTest();
};


#endif
