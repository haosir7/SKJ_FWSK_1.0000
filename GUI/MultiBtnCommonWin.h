#ifndef MULTI_BTN_COMMON_WIN_H
#define MULTI_BTN_COMMON_WIN_H
#include"CaWindow.h"
#include"LOGCTRL.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CaButtonList.h"

#define MAX_BUTTON_NUM	20

/*!
@class 少于7个button的窗体基类  //待修改
*/
class CMultiBtnCommonWin : public CaWindow
{
private:
	CaButton *m_pBtn1;
	CaButton *m_pBtn2;
	CaButton *m_pBtn3;
	CaButton *m_pBtn4;
	CaButton *m_pBtn5;
	CaButton *m_pBtn6;
	CaButton *m_pBtn7;
	CaButton *m_pBtn8;
	CaButton *m_pBtn9;
	CaButton *m_pBtn10;

	CaButton *m_pBtn[MAX_BUTTON_NUM];

	CaButtonList *m_pBtnList;
public:
	CMultiBtnCommonWin(int numOfBtn);
	CMultiBtnCommonWin(int numOfBtn, int colCount);

	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	
	void SetTitle(int num, const char *title);
	
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
protected:
	int m_iNumOfBtn;
};

#endif


