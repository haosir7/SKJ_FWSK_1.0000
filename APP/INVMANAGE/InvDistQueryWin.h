/*! \file    InvDistQueryWin.h
   \brief    awe4000r发票管理档的分发查询界面
   \author   Yu Yan
   \version  1.0
   \date     2008-2-22
 */

#ifndef INV_DIST_QUERY_WIN_H
#define INV_DIST_QUERY_WIN_H

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
#include "InvManageFunc.h"

#include "SysMacDef.h"
#include "CGlobalArg.h"
#include "SaleData.h"


/**
 *@class CInvDistQueryWin 
 *@brief 发票管理档的分发查询界面
 */
class CInvDistQueryWin : public CaWindow
{
public:
	char title_array[6][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/

	CaLabel *label[5];							/**< 指向标签的指针 */
	
	CaButton *m_pBtn1, *m_pBtn2;				/**< 指向按钮的指针 */
	        	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  

	UINT8 m_workState;							/**< 工作状态*/
	UINT32 m_pageIndex;							/**< 当前页码*/
	UINT32 m_pageNum;							/**< 总页码数*/
	CInvVol *m_pInvVol;							/**< 发票领用信息*/
	UINT32 m_recordIndex;						/**< 记录在表中的序号*/
	string m_codelink;							/**< 当前卷的发票代码*/

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
	@brief 按钮1响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮2响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 构造函数	
	*/
	CInvDistQueryWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CInvDistQueryWin();
    
	/*!
	@brief 错误消息提示框	
	@param[in] ret   错误号 
	@return  FAILURE
	*/
    UINT8 ErrMsgBox(UINT8 ret);
    
	  
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
	@brief 获取未导入发票卷信息，并修改屏幕显示
	@param[in] pInvVol 发票卷记录的指针
	@param[in] nPageIndex  本次屏幕显示的页码号
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 QueryShow(CInvVol *pInvVol, UINT32 nPageIndex);

};

#endif
