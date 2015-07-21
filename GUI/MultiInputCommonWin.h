#ifndef MULTI_INPUT_COMMON_WIN_H
#define MULTI_INPUT_COMMON_WIN_H
#include"CaWindow.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "CaInputList.h"
#include "CaButton.h"

/*!
@class 多个input和btn窗体基类
*/
class CMultiInputCommonWin : public CaWindow
{
//private:
public:
	CaInput *m_pInput1;
	CaInput *m_pInput2;
	CaInput *m_pInput3;
	CaInput *m_pInput4;
	CaInput *m_pInput5;
	CaInput *m_pInput6;
	CaInput *m_pInput7;
	CaInput *m_pInput8;
	CaInput *m_pInput9;

	CaButton *m_pBtn1;
	CaButton *m_pBtn2;
	CaButton *m_pBtn3;
	
	CaInputList *m_pInputList;
public:
	CMultiInputCommonWin(int numOfInput);
	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual ~CMultiInputCommonWin();
//	void SetTitle(int num, const char *title);
	void SetObjPara(int num, const char *title, CaInput::TYPE inputtype=CaInput::aCHAR, int len=20);
	
	virtual void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput4(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput5(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput6(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput7(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput8(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnInput9(int iEvent, unsigned char * pEventData, int iDataLen);

	virtual void OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnBtn3(int iEvent, unsigned char * pEventData, int iDataLen);

	virtual void SetFocusToObj(const CaObject *targetObj);

	int ClearContentBuf(int n);

	void ReSetFocus();

private:
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput6(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput7(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput8(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnInput9(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnBtn1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnBtn2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnBtn3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
protected:
	int m_iNumOfInput;
	int m_iNumOfBtn;
};
#endif


