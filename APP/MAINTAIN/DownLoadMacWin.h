#ifndef _DOWNLOAD_MAC_WIN_H
#define _DOWNLOAD_MAC_WIN_H

#include <string>
#include "CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include "g_def.h"
#include "CMainFrame.h"
//#include "MultiInputCommonWin.h"
using namespace std;


class CDownLoadMacWin : public CaWindow
{
public:

	CaLabel *m_pLbl1;
	CaInput *m_pInput1;							/**< 指向输入栏的指针 */
	CaButton *m_pBtn1, *m_pBtn2, *m_pBtn3;		/**< 指向按钮的指针 */
	        	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  


private:

	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

public:

	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();

	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);

	CDownLoadMacWin();

	virtual ~CDownLoadMacWin();

	static void S_OnActive(CaWindow *obj);
    
	void DoActive(); 

private:
	UINT8 CheckInputValid(string &strErr);
	void ClearMacWin();
	void DispMacInfo();
	
//	INT8 m_MAC[12];
	INT8 m_MAC[MAC_LEN];//存放mac地址 by yy 20120524
	 
};

#endif

