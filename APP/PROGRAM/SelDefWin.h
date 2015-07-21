// SelDefWin.h: interface for the CSelDefWin class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ___SELDEF_WIN
#define ___SELDEF_WIN


#include "CaWindow.h"
#include "CaInput.h"
#include "MultiInputCommonWin.h"
#include "CPrnInfo.h"

/**
 *@class CSelDefWin 
 *@brief 录入票面打印的“自定义”内容的界面
 */
class CSelDefWin : public CMultiInputCommonWin 
{
private:
	CPrnInfo m_prnInfo;

public:
	CSelDefWin();

	~CSelDefWin();

	/*!
	@brief 创建窗体	
	@param iX 左上角横坐标
	@param iY 左上角纵坐标
	@param iW 宽度
	@param iH 高度
	@return 1 ：成功；其它：失败
	*/
	int Create(int iX,int iY,int iW,int iH);

	/*!
	@brief 处理事件	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

	/*!
	@brief 刷新窗口显示	
	@return 1: SUCCESS
	*/
	virtual int ReFresh();
	
	/*!
	@brief Input控件的按键响应	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput4(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnInput5(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief button控件的按键响应	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	void OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen);
	
	static void S_OnActive(CaWindow *obj);
	void DoActive(); 

	/*!
	@brief 显示input对应的内容	
	@return   
	*/
	void DispPrnInfo();

	UINT8 CheckInputValid(string &strErr);

	/*!
	@brief 清空Input显示的内容	
	@return   
	*/
	void ClearPrnInfoWin();
};

#endif

