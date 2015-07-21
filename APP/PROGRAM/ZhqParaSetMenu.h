/**
 @file  ZhqParaSetMenu.cpp
 @brief 界面文件，转换器参数设置主菜单
 @author  lzh
 @date  20150608
*/

#ifndef ZHQ_PARA_SET_MENU_H
#define ZHQ_PARA_SET_MENU_H

#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CNetPara.h"

class CZhqParaSetMenu : public CMultiBtnCommonWin
{
public:
	CNetPara m_Netpara;
	CNetPara *m_ComNetpara;

	CZhqParaSetMenu();
	~CZhqParaSetMenu();
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);


private:

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
};


#endif
