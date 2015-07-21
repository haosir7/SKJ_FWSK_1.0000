/*! \file    ProductSaleWin.h
   \brief    awe4000r销售程序中商品销售窗口
   \author   Yu Yan
   \version  1.0
   \date     2007-11-28
 */


#ifndef PRODUCT_SALE_WIN_H
#define PRODUCT_SALE_WIN_H

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
#include "TDateTime.h"
#include "SaleData.h"
#include "CaObject.h"
#include "CaMsgBox.h"


//#define TITLE_MAX_LEN = 32						/**< 标签显示的内容长度 */

class CProductSaleWin : public CaWindow
{
public:
//	char title_array[5][OBJ_TITLE_MAX_LEN + 1];	/**< 标签显示的内容 （暂时未用） */
	CaLabel *label[7];							/**< 指向标签的指针 */
	string m_strSuffix;							/**< 标签显示内容的补充部分 */
	
	CaInput *m_pInput1;							/**< 指向输入栏的指针 */
	CaInput *m_pInput2;							/**< 指向输入栏的指针 */   

	bool m_IfCancel;							/**< 是否按“商品取消键” */ 
	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  
    int m_iColW;								/**< 两列时，第二列的横坐标 */ 
     
//  CProductSaleMainMenu *pPSMWin;


public:

	/*!
	@brief 构造函数	
	*/
	CProductSaleWin();
	
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

//	bool IsCurWin();

	/*!
	@brief 刷新窗口显示	
	@return 1: SUCCESS
	*/
	virtual int ReFresh();
    
	/*!
	@brief 改变Label所关联的title值	
	@return  1: SUCCESS; 0: FAILURE
	*/
	int ChangeTitle();

	/*!
	@brief 错误消息提示框	
	@param[in] ret   错误号 
	@return  FAILURE
	*/
    int ErrMsgBox(UINT8 ret);

	/*!
	@brief 让Label显示找零打印内容	
	@param[in] dCash   实付现金额
	@return  1: SUCCESS; 0: FAILURE
	*/
    UINT8 CashShow(double cash);

    /*!
	@brief 让Label显示商品销售内容	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 DeptSaleShow();

    /*!
	@brief 让Label显示付款人	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClientShow();

	/*!
	@brief 让Label显示付款人税号	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClientCodeShow();

	/*!
	@brief 显示商品取消	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 CancelGoodsShow();

	/*!
	@brief 整单取消的处理函数	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 TotalCancelProc();

	/*!
	@brief 部类销售过程
	@param[in] deptNo 部类号
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 DeptSaleProc(UINT32 deptNo);

	/*!
	@brief 让Label显示卷汇总信息并打印
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 InvSumShowPrn(void);

	/*!
	@brief 付款人输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClientInputProc(void);

	/*!
	@brief 付款人税号输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClientCodeInputProc(void);

	/*!
	@brief 备注输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 RemarksInputProc(void);

	/*!
	@brief 折让过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 GoodesReductProc(void);

	/*!
	@brief 单价输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PriceInputProc(void);

	/*!
	@brief 总价输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 SumInputProc(void);
 
	/*!
	@brief 乘时输入过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PlusProc(void);

	/*!
	@brief 快捷销售过程
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 QuickSaleProc(void);
	
};

#endif

