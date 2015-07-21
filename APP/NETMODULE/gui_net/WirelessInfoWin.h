/**
 @file  WirelessInfoWin.h
 @brief 界面文件，M2M模块信息
*/
#ifndef _WIRELESS_INFO_WIN_H
#define _WIRELESS_INFO_WIN_H

#include "IncludeMe.h"
#include "CaLabel.h"
#include "CaWindow.h"

using namespace std;


class CWirelessInfoWin : public CaWindow
{
public:
	CaLabel *m_pLabel1;
	CaLabel *m_pLabel2;
	CaLabel *m_pLabel3;
	CaLabel *m_pLabel4;
	CaButton *m_pBtn1;

	int m_iBtnW;
	int m_iColW;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	CWirelessInfoWin();
	~CWirelessInfoWin();

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 


};


#endif




















