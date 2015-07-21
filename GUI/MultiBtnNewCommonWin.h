#ifndef MULTI_BTN_NEW_COMMON_WIN_H
#define MULTI_BTN_NEW_COMMON_WIN_H

#include"CaWindow.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CaButtonList.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

#define MAX_BUTTON_NUM	10
#define MAX_TITLE_NUM	20
/*!
@class 少于7个button的窗体基类  //待修改
*/
class CMultiBtnNewCommonWin : public CaWindow
{

private:
	CaButton *m_pBtn[MAX_BUTTON_NUM];

	CaButtonList *m_pBtnList;
	unsigned int m_nNum;
	unsigned int m_iNumOfCol; //强制设置菜单列数(1,或者2) 

    char *m_pTitle[MAX_BUTTON_NUM];

public:
	CMultiBtnNewCommonWin(unsigned int colCount=0); //colCount强制设置菜单列数 默认根据菜单项确定
    ~CMultiBtnNewCommonWin();
	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	
	void SetTitle(const char *title);
	void SetButton();

	virtual void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton6(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton7(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton8(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton9(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton10(int iEvent, unsigned char * pEventData, int iDataLen);

	virtual void OnButton(int iEvent, unsigned char * pEventData, int iDataLen);

	//修改窗口高度m_iH，以保证满额度的填充窗口。
	//h 增加的量
	inline void Adjustm_iH(int h)
	{
		 m_pBtnList->m_iH += h ; 
	}

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton10(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnButton(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	int ProcShortcut(int iEvent,unsigned char *pEventData, int iDataLen);

};

#endif


