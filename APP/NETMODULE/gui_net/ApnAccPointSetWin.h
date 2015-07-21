/**
 @file  ApnAccPointSetWin.h
 @brief 界面文件，设置APN接入点
*/
#ifndef _APN_ACCPOINT_SET_WIN_H
#define _APN_ACCPOINT_SET_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaButton.h"


class CApnAccPointSetWin : public CaWindow
{
public:	
	CaInput *m_pInput1;				
	CaLabel *m_pLabel1;
	CaButton *m_pBtn1, *m_pBtn2;				
	
	int m_iBtnW;								 
	int m_iColW;							
	
	bool m_isInfoExist;
	
public:	
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	
	static void S_OnActive(CaWindow *obj);  
	void DoActive(); 
	
	CApnAccPointSetWin();
	virtual ~CApnAccPointSetWin();
	

private:	
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	void DispApnAccPointInfo();
	void ClearWin();
	
};

#endif

