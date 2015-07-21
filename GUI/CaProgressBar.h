#ifndef CAPROGRESSBAR_H
#define CAPROGRESSBAR_H

#include "CaWindow.h"
#include "CaLabel.h"
#include "PosFrame.h"
#include "CaLabelBar.h"


#define PROGRESSBAR_H (GRID_LINE_H * 2)
#define PROGRESSBAR_W (SCREEN_W*3/4)
#define PROGRESSBAR_X (SCREEN_W/8)
#define PROGRESSBAR_Y (SCREEN_H/3-2)

class CaProgressBar:public CaWindow
{
protected:
	CaLabelBar  *m_pBar;
	bool m_bWithBar;

public:
	CaProgressBar(const char *text,bool withBar = false);

    virtual ~CaProgressBar();

    virtual int SetFocusIn();
    virtual int SetFocus();
    virtual	int ReFresh();

    virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

	void SetText(const char *text);
	void SetProgress(int pos);
	void Show();

	bool IsCurWin();
};
#endif


