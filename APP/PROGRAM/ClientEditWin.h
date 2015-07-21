#ifndef _CLIENT_EDIT_WIN_H
#define _CLIENT_EDIT_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include "MultiInputCommonWin.h"
#include "CClient.h"

using namespace std;


class CClientEditWin : public CaWindow
{
public:
	CaInput *m_pInput1;
	CaInput *m_pInput2;
	CaInput *m_pInput3;

	CaButton *m_pBtn1;
	CaButton *m_pBtn2;
	CaButton *m_pBtn3;
	
	int m_iBtnW;//Button 的宽度
	int m_iColW;
	
	bool m_isClientExist;

private:
	CClient *m_Client_edit;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 
	
	CClientEditWin();
	~CClientEditWin();

private:
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	void ShowMsg(string strInfo);

    UINT8 CheckInputValid(string &strErr);
	void DispClientInfo(CClient *oper);
	void ClearClientWin();
	//根据客户编码位数生成不合法编码
	string DErrorCode();
	
};


#endif
