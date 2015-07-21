#ifndef CAMSGBOX_H
#define CAMSGBOX_H

#include "CaWindow.h"
#include "CaButtonList.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "PosFrame.h"
#ifdef WIN32
extern "C"{

	#include <conio.h>
};
#endif
#define MSGBOX_H (SCREEN_H - 26)
#define MSGBOX_W (SCREEN_W - 40)
// #define TEXT_X_OFFSET 0
// #define TEXT_Y_OFFSET 0
#define BUTTON_W 5
#define BUTTON_H 3

#define OK_PRESSED 0x01
#define NO_PRESSED 0X02
#define CANCEL_PRESSED 0X03

class CaMsgBox:public CaWindow
{
public:
	typedef enum MsgBoxType
	{
		MB_OK=0x0001,
		MB_OKCANCEL=0x0002,
		MB_YESNO=0x0003,
		MB_YESNOCANCEL=0x0004,
		MB_INPUT=0x0005,
		MB_REP_INPUT=0x0006
	};
private:
	int m_iLastLineH;
protected:
	bool m_bModel;
	int m_iMsgBoxType;
	CaInput  *m_pInput;

public:
	int m_iStatus;

public:
	CaMsgBox(const char *text, int type, bool model = true);

    virtual ~CaMsgBox();

    virtual int SetFocusIn();
    virtual int SetFocus();
    virtual	int ReFresh();

    virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int OnLeftKey(void);
    virtual int OnRightKey(void);

	void SetText(const char *text);
	void SetModel(bool model){m_bModel = model;}
	bool GetModel(){return m_bModel;}
	void SetInputType(CaInput::_INPUT_TYPE inputType);
	void SetInputText(const char *text);
	void ClearInput();
	void SetMaxInputLen(int len);
	unsigned char * GetInputContent();
	void InputContentClear(){m_pInput->Clear();};

	void ShowBox();

	/*!
	@brief 消息提示框	
	@param[in] strInfo   提示框显示的字符串
	*/
	static void ShowMsg(string strInfo);

private:
	static void s_onCANCEL(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void s_onOK(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void s_onNO(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
// 	static void s_onInput(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	void OnOK(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnNO(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnCANCEL(int iEvent, unsigned char * pEventData, int iDataLen);
// 	void OnInput(int iEvent, unsigned char * pEventData, int iDataLen);

	void InitBtn(int type);	
	virtual bool IsCurWin();
};
#endif





