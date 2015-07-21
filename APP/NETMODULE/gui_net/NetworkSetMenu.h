/**
 @file  NetworkSetMenu.h
 @brief 界面文件，网络设置菜单
*/
#ifndef NETWORK_SET_MENU_H
#define NETWORK_SET_MENU_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "MultiBtnCommonWin.h"

class CNetworkSetMenu : public CMultiBtnCommonWin
{
public:
	
	CNetworkSetMenu();
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
	
};


#endif
