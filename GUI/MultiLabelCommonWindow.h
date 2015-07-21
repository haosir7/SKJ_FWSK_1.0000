#ifndef MULTI_LABEL_COMMON_WIN_H
#define MULTI_LABEL_COMMON_WIN_H
#include"CaWindow.h"
#include"LOGCTRL.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CaButtonList.h"

/*!
@class 少于10个button的窗体基类 
一个ButtonList（最多有8个Button），两个Button。利用‘P’键，使焦点在这三个部分间移动。
*/
class CMultiLabelCommonWin : public CaWindow
{
private:
	CaButtonList *m_pBtnList;
    CaButton *m_pBtn[8];

	CaButton *m_pBtn1;
	CaButton *m_pBtn2;


public:
	CMultiLabelCommonWin(int numOfBtn);

	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	
	void SetTitle(int num, const char *title);
	
	virtual void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);

	void SetBtnFocus();

	//修改窗口高度m_iH，以保证满额度的填充窗口。
	//h 增加的量
	inline void Adjustm_iH(int h)
	{
		 m_pBtnList->m_iH += h ; 
	}

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

protected:
	int m_iNumOfBtn;
};

#endif


