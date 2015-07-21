#ifndef FOUR_LABELS_COMMON_WIN_H
#define FOUR_LABELS_COMMON_WIN_H
#include"CaWindow.h"
#include"LOGCTRL.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
/*!
@class 4个label的窗体基类
@brief 打算废弃此文件，用MultiPageCommonWin代替
*/
class CFourLabelsCommonWin : public CaWindow
{
private:
	CaLabel *m_pLabel1;
	CaLabel *m_pLabel2;
	CaLabel *m_pLabel3;
	CaLabel *m_pLabel4;
public:
	CFourLabelsCommonWin(int numOfLabels);

	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

	void SetTitle(int num, const char *title);

protected:
	int m_iTotalLine;
};

#endif


