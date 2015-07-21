#ifndef _SYS_ARG_EDIT_WIN_H
#define _SYS_ARG_EDIT_WIN_H

#include"CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CSysArg.h"
#include "CMainFrame.h"
#include "SysArgInfo.h"

//#include "SysArgMenu.h"
#include <string>
using namespace std;



class CSysArgEditWin : public CaWindow
{
public:

	CaLabel *m_pLabel1;
	CaLabel *m_pLabel2;
	CaLabel *m_pLabel3;
	CaInput *m_pInput1;
	CaButton *m_pBtn1;
	CaButton *m_pBtn2;
	CaButton *m_pBtn3;
	
	int m_iBtnW;	//Button 的宽度
	int m_iColW;
	
	int m_SysArgID;	//在数据库系统参数表里的id号
	int m_nId;		//在sys_arg_info[]中的序号

private:
	CSysArg *m_sysArg;
	struct _Sys_Arg_info *m_sysarg_info; 

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	bool IsCurWin();	
	virtual int ReFresh();
	
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);

	CSysArgEditWin();
	~CSysArgEditWin();

private:
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	void ShowMsg(string strInfo);

    UINT8 CheckInputValid(string &strErr);
	void ClearSysArgEditWin();
	int ReadArgByID(int argID, string &strErr);

	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);

	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive();
};


#endif
