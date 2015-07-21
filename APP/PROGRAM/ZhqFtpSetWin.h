/**
 @file  FTPServerSetWin.h
 @brief 界面文件，服务器设置
*/
#ifndef _FTP_SERVER_SET_WIN_H
#define _FTP_SERVER_SET_WIN_H

#include "IncludeMe.h"
#include "CaInput.h"
#include "CaButton.h"
#include "CaWindow.h"
#include "CMainFrame.h"

#include "MultiInputCommonWin.h"
#include "CNetPara.h"

class CZhqFtpSetWin : public CMultiInputCommonWin
{
private:
	CNetPara *m_netpara;
	
public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput4(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen);
	
	static void S_OnActive(CaWindow *obj);
	void DoActive(); 
	
	CZhqFtpSetWin();
	~CZhqFtpSetWin();
	
private:
    UINT8 CheckInputValid(string &strErr);
	void DispFTPInfo();
	void ClearFTPWin();
	int changeFTPInfoProc(string &strErr);
};

#endif
