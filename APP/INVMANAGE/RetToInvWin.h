/*! \file    InvAssignWin.h
   \brief    awe4000r发票退回（主分机一样）界面
   \author   xsr
   \version  1.0
   \date     2008-2-22
 */
#ifndef _RET_TO_INV_WIN_H
#define _RET_TO_INV_WIN_H

#include"CaWindow.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaInput.h"
#include "CaButton.h"


/**
 *@class CRetToInvWin 
 *@brief 发票退回（主分机一样）指定界面
 */
class CRetToInvWin : public CaWindow
{
public:
	CaInput *m_pInput1;				/**< 指向输入栏的指针 */  
	CaInput *m_pInput2;				/**< 指向输入栏的指针 */
	CaInput *m_pInput3;				/**< 指向输入栏的指针 */  
	CaButton *m_pBtn1;				/**< 指向按钮的指针 */
	CaButton *m_pBtn2;				/**< 指向按钮的指针 */
	
	int m_iBtnW;					/**< 两列时，控件的宽度 */  
	int m_iColW;					/**< 两列时，第二列的横坐标 */  
   
	string m_invCode;               /**< 发票代码*/
	UINT32 m_invStartNo;			/**< 发票起始号码 */ 
	UINT32 m_invNum;				/**< 发票张数 */ 


public:
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
	@brief 构造函数	
	*/
	CRetToInvWin();

	/*!
	@brief 输入栏响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 输入栏响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 输入栏响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive(); 
	
private:
	
	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 按钮响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
   
	/*!
	@brief 检查输入栏接收的值是否有效	
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  输入有效； 0 输入无效
	*/
	UINT8 CheckInputValid();
};


#endif
