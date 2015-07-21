#ifndef _OPERATOR_EDIT_WIN_H
#define _OPERATOR_EDIT_WIN_H

#include"CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CMainFrame.h"
#include <string>
using namespace std;

class COperatorEditWin : public CaWindow
{
public:
	CaInput *m_pInput1;
	CaInput *m_pInput2;
	CaInput *m_pInput3;
	CaInput *m_pInput4;
	CaButton *m_pBtn1;
	CaButton *m_pBtn2;
	CaButton *m_pBtn3;
	
	int m_iBtnW;//Button µÄ¿í¶È
	int m_iColW;
	
	bool m_isOperExist;

private:
	COperator *m_Oper_edit;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput4(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 

	COperatorEditWin();
	~COperatorEditWin();

private:
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

    UINT8 CheckInputValid(string &strErr);
	void DispOperatorInfo(COperator *oper);
	void ClearOperatorWin();
};


#endif
