
#ifndef _DEPT_EDIT_WIN_H
#define _DEPT_EDIT_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include "MultiInputCommonWin.h"
#include "CDept.h"
#include "CTax.h"

using namespace std;


class CDeptEditWin : public CMultiInputCommonWin
{
public:
	
	bool m_isDeptExist;

private:
	CDept *m_Dept_edit;		//原本是CPlu
	CDept m_dept;

public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput4(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput5(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput6(int iEvent, unsigned char * pEventData, int iDataLen);//20150507linzihao

	
	void OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnBtn3(int iEvent, unsigned char * pEventData, int iDataLen);

	static void S_OnActive(CaWindow *obj);
	void DoActive(); 
	
	CDeptEditWin();
	~CDeptEditWin();

private:
    UINT8 CheckInputValid(string &strErr);
	void DispDeptInfo(CDept *dept);			//原本是CPlu
	void ClearDeptWin();
	//根据商品编码位数生成不合法编码
	string DErrorCode();
};


#endif
