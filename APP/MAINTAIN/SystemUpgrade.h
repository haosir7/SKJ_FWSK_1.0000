#ifndef _SYSTEM_UPGRADE_WIN_H
#define _SYSTEM_UPGRADE_WIN_H

#include <string>
#include "CaWindow.h"
#include "CaButton.h"
#include "pos_debug.h"
#include "g_def.h"
#include "CMainFrame.h"
//#include "SerialPort.h"

using namespace std;

#define UPGRADE_OVERTIME 50000

class CSystemUpgradeWin : public CaWindow
{
public:
	CaLabel *m_pLabel;
	CaButton *m_pBtn1;

	int m_iBtnW;
	int m_iColW;

private:

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	CSystemUpgradeWin();
	~CSystemUpgradeWin();

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	void ShowMsg(string strInfo);

	/*!
	@brief 等待接收串口命令	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	static void S_OnActive(CaWindow *obj);
	void DoActive(); 

	void ChangeWinUpgrade();


};


#endif






