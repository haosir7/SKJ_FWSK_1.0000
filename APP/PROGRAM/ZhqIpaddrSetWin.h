/**
 @file  NetworkingSetWin.h
 @brief 界面文件，本地IP信息设置
 @author  lzh
 @date  20150608
*/
#ifndef ZHQ_IPADDR_SET_WIN_H
#define ZHQ_IPADDR_SET_WIN_H

#include "IncludeMe.h"
#include "CaInput.h"
#include "CaButton.h"
#include "MultiInputCommonWin.h"
#include "CNetPara.h"
#include "ZhqParaSetMenu.h"


class CZhqIpaddrSetWin : public CMultiInputCommonWin
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
	
	CZhqIpaddrSetWin();
	~CZhqIpaddrSetWin();

private:
    UINT8 CheckInputValid(string &strErr);
	void DispIpAddrInfo();
	void ClearIpAddrWin();
	int changeIpAddrProc(string &strErr);
};


#endif
