/**
 @file  WirelessViewWin.h
 @brief 界面文件，无线信息监测
*/
#ifndef _WIRELESS_VIEW_WIN_H
#define _WIRELESS_VIEW_WIN_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CaWindow.h"


class CWirelessViewWin : public CaWindow
{
public:
	CaLabel *m_pTitle;
	CaLabel *m_pLabel1;
	CaLabel *m_pLabel2;
	CaLabel *m_pLabel3;
	CaButton *m_pBtn1;

	int m_iBtnW;
	int m_iColW;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	CWirelessViewWin();
	~CWirelessViewWin();

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 


};


#endif




















