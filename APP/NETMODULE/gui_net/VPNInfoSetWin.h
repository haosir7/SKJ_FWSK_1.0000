/**
 @file  VPNInfoSetWin.h
 @brief 界面文件，VPN帐户设置
*/
#ifndef _VPNINFO_SET_WIN_H
#define _VPNINFO_SET_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaButton.h"


class CVPNInfoSetWin : public CaWindow
{
public:
	CaInput *m_pInput1, *m_pInput2, *m_pInput3;	
	CaButton *m_pBtn1, *m_pBtn2;				
	
	int m_iBtnW;								
	int m_iColW;								  

private:
	string m_strUsrPwd;
	
public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	
	static void S_OnActive(CaWindow *obj);  
	void DoActive(); 
	
	CVPNInfoSetWin();
	virtual ~CVPNInfoSetWin();
	
private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	void DispVPNInfo();
	void ClearWin();

	
};

#endif

