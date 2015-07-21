/**
 @file  NetAutoDialSetWin.h
 @brief 界面文件，开机自动拨号设置
*/
#ifndef _NET_AUTO_DIAL_SET_WIN_H
#define _NET_AUTO_DIAL_SET_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaButton.h"


class CNetAutoDialSetWin : public CaWindow
{
public:

	CaLabel *m_pLbl1, *m_pLbl2, *m_pLbl3;
	CaInput *m_pInput1;							
	CaButton *m_pBtn1, *m_pBtn2;				
	        	
	int m_iBtnW;								
	int m_iColW;								 

	UINT8 m_pageFlag;

private:

	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

public:

	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 

	CNetAutoDialSetWin();
	virtual ~CNetAutoDialSetWin();

private:
	UINT8 CheckInputValid(string &strErr);
	void DispParalInfo();
	void ChangePara();
};

#endif

