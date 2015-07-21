#ifndef _SYS_ARG_MENU_H
#define _SYS_ARG_MENU_H

#include "CMainFrame.h"
#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CSysArg.h"
#include "pos_debug.h"
#include "g_def.h"
#include "SysArgInfo.h"
#include "SysArgEditWin.h"

class CSysArgMenu : public CMultiBtnCommonWin
{
public:
	CSysArg *m_sysArg;
	INT32 m_curSysarg_num;
	INT32 m_EditSysArgID;
	
	CSysArgEditWin *m_pSysArgEditWin;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();

	CSysArgMenu();
	~CSysArgMenu();

private:
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen); 
	void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen); 
	void OnButton6(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton7(int iEvent, unsigned char * pEventData, int iDataLen);
	void ShowMsg(string strInfo);

	int ReadArgByID(int id, string &strErr);
	void SysArg_SetTitle(int num, struct _Sys_Arg_info *sys_arg);

};


#endif
