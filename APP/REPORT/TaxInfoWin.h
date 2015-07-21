
/*! \file    TaxInfoWin.h
   \brief    awe4000r报表打印的税率信息界面
   \author   zcy
   \version  1.0
   \date     2015-6-6
 */

#ifndef		TAX_INFO_WIN_H
#define		TAX_INFO_WIN_H

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
#include "CaButton.h"
#include "CaMsgBox.h"
#include "TDateTime.h"

#include "SysMacDef.h"
#include "CGlobalArg.h"
#include "CTax.h"

#define SHOWNUM_TAX 3

/**
 *@class CTaxInfoWin 
 *@brief 报表打印的税率信息界面
 */
class CTaxInfoWin : public CaWindow
{
public:
	char title_array[SHOWNUM_TAX+1][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/

	CaLabel *label[SHOWNUM_TAX+1];							/**< 指向标签的指针 */
	
	CaButton *m_pBtn1, *m_pBtn2;				/**< 指向按钮的指针 */
	        	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  
	
	UINT32 m_pageIndex;							/**< 当前页码*/
	UINT32 m_pageNum;							/**< 总页码数*/
	CTax *m_pTax;								/**< 授权税率信息*/
	UINT32 m_recordIndex;						/**< 记录在表中的序号*/

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
	CTaxInfoWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CTaxInfoWin(); 
	  
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
	@brief 获取授权税率信息，并修改屏幕显示
	@param[in] pTax 税率记录的指针
	@param[in] nIndex  税率记录的索引号
	@param[in] nPageIndex  本次屏幕显示的页码号
	@param[in] nPageNum  本次屏幕显示的页码总数
	@param[in] nOrder  查询表的顺序  0：升序；1：降序
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 QueryShow(CTax *pTax, UINT32 nIndex, UINT32 nPageIndex, UINT32 nPageNum);

		/*!
	@brief 错误消息提示框	
	@param[in&out] nNum   显示页数 
	@return  FAILURE
	*/

	UINT8 GetTaxNum(UINT32 &nNum);

	/*!
	@brief 打印税率信息	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PrintTaxInfo();
};

#endif
