/*! \file    TimeSumInfoWin.h
   \brief    awe4000r报表打印的时间段汇总界面
   \author   Yu Yan
   \version  1.0
   \date     2008-02-14
 */

#ifndef TIME_SUM_INFO_WIN_H
#define TIME_SUM_INFO_WIN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#endif

#include "IncludeMe.h"
#include "CaWindow.h"
#include "LOGCTRL.h"
#include "pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CMainFrame.h"
#include "CaInput.h"
#include "CaButton.h"
#include "CaMsgBox.h"
#include "TDateTime.h"

#include "TimeSumDateWin.h"
#include "SysMacDef.h"
#include "printer.h"


/**
 *@class CTimeSumInfoWin 
 *@brief 报表打印的时间段汇总界面
 */
class CTimeSumInfoWin : public CaWindow
{
public:
	char title_array[5][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/

	CaLabel *label[3];							/**< 指向标签的指针 */
	
	CaButton *m_pBtn1, *m_pBtn2;				/**< 指向按钮的指针 */
	CTimeSumDateWin* m_pDateWin;				/**< 指向时间段汇总的时间输入界面的指针 */
	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  

//	UINT8 m_create;								/**< 工作状态*/

	UINT32 m_StartDate;							/**< 查询的起始日期*/
	UINT32 m_EndDate;							/**< 查询的截止日期*/
	UINT32 m_StartTime;							/**< 查询的起始时间*/
	UINT32 m_EndTime;							/**< 查询的截止时间*/
	struct TDateSegSumPrnData m_segSumPrnData;  /**< 时间段汇总数据*/
	struct TDateSegSumPrnData *m_pSegSumPrnData;/**< 时间段汇总数据指针*/

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
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
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
	@brief 构造函数	
	*/
	CTimeSumInfoWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CTimeSumInfoWin();
      
	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive(); 

	
	/*!
	@brief 改变Label所关联的title值	
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ChangeTitle();

	/*!
	@brief 获取时间段汇总信息，并修改屏幕显示
	@param[in] pSegSumPrnData 时间段汇总记录的指针
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 QueryShow(struct TDateSegSumPrnData *pSegSumPrnData);

	/*!
	@brief 打印当前显示的时间段汇总信息
	@param[in] pSegSumPrnData 时间段汇总记录的指针
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PrnTimeSumInfo(struct TDateSegSumPrnData *pSegSumPrnData);
};

#endif

