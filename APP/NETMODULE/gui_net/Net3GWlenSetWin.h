/**
 @file  Net3GWlenSetWin.h
 @brief 界面文件，3G帐户设置
*/
#ifndef _NET_3GWLEN_SET_WIN_H
#define _NET_3GWLEN_SET_WIN_H


#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaButton.h"



class CNet3GWlenSetWin : public CaWindow
{
public:
	CaInput *m_pInput1, *m_pInput2;				
	CaButton *m_pBtn1, *m_pBtn2;				
	
	int m_iBtnW;								
	int m_iColW;								 
	

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);

	CNet3GWlenSetWin();
	virtual ~CNet3GWlenSetWin();
	
	static void S_OnActive(CaWindow *obj);    
	void DoActive(); 
	
private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	
	void Disp3GInfo();
	void ClearWin();

	string m_strUsrPwd;
	
};

#endif

