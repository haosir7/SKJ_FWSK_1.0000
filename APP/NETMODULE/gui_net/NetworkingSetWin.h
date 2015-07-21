/**
 @file  NetworkingSetWin.h
 @brief 界面文件，IP信息设置
*/
#ifndef _NETWORKING_SET_WIN_H
#define _NETWORKING_SET_WIN_H

#include "IncludeMe.h"
#include "CaInput.h"
#include "CaButton.h"
#include "MultiInputCommonWin.h"
#include "netHeaderDef.h"

class CNetworkingSetWin : public CMultiInputCommonWin
{
private:
	IPinfo m_ipInfo;

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
	
	CNetworkingSetWin();
	~CNetworkingSetWin();

private:
    UINT8 CheckInputValid(string &strErr);
	void DispNetworkingInfo();
	void ClearNetworkingWin();
	int changeSettingProc(string &strErr);
};


#endif
