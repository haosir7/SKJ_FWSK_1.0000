/*! \file    InvStubPrnWin.h
   \brief    awe4000r发票管理档的发票存根打印界面
   \author   Yu Yan 
   \version  1.0
   \date     2008-2-14 
 */


#ifndef INV_STUB_PRN_WIN_H
#define INV_STUB_PRN_WIN_H

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
#include "InvStubWin.h" 

#include "SysMacDef.h"
#include "CGlobalArg.h"
#include "SaleData.h"


/**
 *@class CInvStubPrnWin 
 *@brief 发票管理档的发票存根打印界面
 */
class CInvStubPrnWin : public CaWindow
{
public:
	char title_array[5][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/


	CaLabel *label[5];							/**< 指向标签的指针 */
	
	CaButton *m_pBtn1, *m_pBtn2, *m_pBtn3;		/**< 指向按钮的指针 */
	CInvStubWin* m_pPSWin;						/**< 发票存根查询界面的指针 */
	//CInvInfoDownloadWin *m_pPIWin;
	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */ 

	UINT32 m_pageIndex;							/**< 当前页码*/
	UINT32 m_pageNum;							/**< 总页码数*/
	CInvHead *m_pInvHead;						/**< 发票领用信息指针*/
	CInvHead m_invHead;							/**< 发票领用信息*/
	UINT32 m_recordIndex;						/**< 记录在表中的序号*/


	string m_code;								/**< 所退发票代码*/
	UINT32 m_invStartNo;						/**< 所退发票号码*/
	UINT32 m_invEndNo;							/**< 所退发票号码*/

	UINT8 m_pageflag;

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
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

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
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 构造函数	
	*/
	CInvStubPrnWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CInvStubPrnWin();
    
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
	@brief 获取发票存根信息，并修改屏幕显示
	@param[in] pInvHead 发票记录的指针
	@param[in] nIndex  发票记录的索引号
	@param[in] nPageIndex  本次屏幕显示的页码号
	@param[in] nPageNum  本次屏幕显示的页码总数
	@param[in] nOrder  查询表的顺序  0：升序；1：降序
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 QueryShow(CInvHead *pInvHead, UINT32 nIndex, UINT32 nPageIndex, UINT32 nPageNum, UINT8 nOrder);

	/*!
	@brief 连续打印起始发票号到截止发票号的电子存根
	*/
	void ContinuousIssue();

	/*!
	@brief 连续打印起始发票号到截止发票号的电子存根
	*/
	void ContinuousIssueOne();

	/*!
	@brief 检查按键，判断是否退出自动开票程序	
	@param[out] nKeyTime      连续按确认键的次数 
	@return  1 是(SUCCESS)； 0 否(FAILURE)
	*/
    UINT8 CheckKey(UINT8 &nKeyTime);

	void PrintInv(UINT8 printType);
};

#endif
