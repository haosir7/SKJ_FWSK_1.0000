/*! \file    ClearDepotWin.h
   \brief    awe4000r销售程序中系统维护的清库界面
   \author   Yu Yan
   \version  1.0
   \date     2008-2-22
 */


#ifndef CLEAR_DEPOT_WIN_H
#define CLEAR_DEPOT_WIN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#endif

#include "IncludeMe.h"
#include "CaWindow.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaInput.h"
#include "CaButton.h"
#include "CaMsgBox.h"
#include "TDateTime.h"


#include "SysMacDef.h"
#include "CGlobalArg.h"

/**
 *@class CClearDepotWin 
 *@brief 清库界面
 */
class CClearDepotWin : public CaWindow
{
public:
	char title_array[5][OBJ_TITLE_MAX_LEN + 1];  /**< 标签的标题*/
	
	CaInput *m_pInput1;							/**< 指向输入栏的指针 */
	CaButton *m_pBtn1, *m_pBtn2;				/**< 指向按钮的指针 */
	        	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  

	UINT8 m_workState;							/**< 工作状态*/
	

private:

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
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

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
	@brief 输入栏响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 构造函数	
	*/
	CClearDepotWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CClearDepotWin();
    
	  
	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive(); 

	/*!
	@brief 具体清库操作
	@param[in] strInfo   显示的消息
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClearDepot(string &strInfo);

	 
};

#endif
