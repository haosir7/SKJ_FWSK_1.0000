/**
 @file  ADSLInfoSetWin.h
 @brief 界面文件，设置ADSL帐户信息
*/
#ifndef _ADSLINFO_SET_WIN_H
#define _ADSLINFO_SET_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaButton.h"


class CADSLInfoSetWin : public CaWindow
{
public:
	CaInput *m_pInput1, *m_pInput2;				
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
	
	static void S_OnActive(CaWindow *obj);  
	void DoActive(); 
	
	CADSLInfoSetWin();
	virtual ~CADSLInfoSetWin();
	
private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	void DispADSLInfo();
	void ClearWin();

	
};

#endif

