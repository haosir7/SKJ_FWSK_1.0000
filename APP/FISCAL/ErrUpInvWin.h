/*! \file    ErrUpInvWin.h
   \brief    awe4000r获取上传出错发票信息界面
   \author   Yu Yan
   \version  1.0
   \date     2008-2-22
 */

#ifndef ERR_UP_INV_WIN_H
#define ERR_UP_INV_WIN_H

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
#include "YWXmlBase.h"
#include "CInvServ.h"

#include "SysMacDef.h"
#include "CGlobalArg.h"
#include "SaleData.h"


/**
 *@class CInvDistQueryWin 
 *@brief 获取上传出错发票信息界面
 */
class CErrInvUpWin : public CaWindow
{
public:
	char title_array[6][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/

	CaLabel *label[5];							/**< 指向标签的指针 */
	        	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
	int m_iColW;								/**< 两列时，第二列的横坐标 */  

	UINT32 m_pageIndex;							/**< 当前页码*/
	UINT32 m_pageNum;							/**< 总页码数*/
	CDataInvServ m_InvUpFailInfo[MAX_ERR_INV_COUNT];	/**< 发票领用信息*/
	UINT32 m_recordIndex;						/**< 记录在表中的序号*/

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
	CErrInvUpWin();

	/*!
	@brief 析构函数	
	*/
	virtual ~CErrInvUpWin();
    
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
	UINT8 QueryShow(CDataInvServ *pInvUpFailInfo, UINT32 nPageIndex);

};

#endif
