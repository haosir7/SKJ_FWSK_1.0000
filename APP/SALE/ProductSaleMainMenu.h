/*! \file    ProductSaleMainMenu.cpp
   \brief    awe4000r销售程序中商品销售档主界面
   \author   Yu Yan
   \version  1.0
   \date     2007-11-28
 */


#ifndef PRODUCT_SALE_MAIN_MENU_H
#define PRODUCT_SALE_MAIN_MENU_H


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
#include "SaleData.h"
#include "TDateTime.h"
#include "ProductSaleWin.h"



//#define STUDY_INV_NUM 1000		/**< 假发票卷的发票数 */	

extern char title_arr[7][33];	/**< CProductSaleWin的Label的标题 */	 

/*!
@class CProductSaleMainMenu
@brief 商品销售主菜单
*/
class CProductSaleMainMenu : public CaWindow
{
public:
	char title_array[5][OBJ_TITLE_MAX_LEN + 1];		/**< 标签显示的内容 */
	string m_strSuffix;								/**< 标签显示内容的补充部分 */

	CaLabel *label[7];								/**< 指向标签的指针 */
	
//	CaInput *m_pInput1;								/**< 指向输入栏的指针 */
	CaInput *m_pInput2;								/**< 指向输入栏的指针 */  
       	
	int m_iBtnW;									/**< 两列时，控件的宽度 */  
    int m_iColW;									/**< 两列时，第二列的横坐标 */  
  
	CProductSaleWin *pPSWin;						/**< 商品销售窗口的指针 */
	
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

//	/*!
//	@brief 是否当前窗口	
//	@return true
//	*/
//	bool IsCurWin();

	/*!
	@brief 刷新窗口显示	
	@return 1: SUCCESS
	*/
    virtual int ReFresh();

	/*!
	@brief 构造函数	
	*/
	CProductSaleMainMenu();

	/*!
	@brief 析构函数
	*/
	virtual ~CProductSaleMainMenu();
    
	/*!
	@brief 错误消息提示框	
	@param[in] ret   错误号 
	@return  FAILURE
	*/
    UINT8 ErrMsgBox(UINT8 ret);
    
	/*!
	@brief 让Label显示商品销售内容	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 DeptSaleShow();
    
	/*!
	@brief 更新显示内容静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容函数	 
	*/
	void DoActive(); 

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
	@brief 正常显示	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 NormalShow();

	/*!
	@brief 改变Label所关联的title值	
	@return  1: SUCCESS; 0: FAILURE
	*/
	int ChangeTitle();
	
    /*!
	@brief 进纸	
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ForwardKeyProc();

	/*!
	@brief 部类销售过程
	@param[in] deptNo 部类号
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 DeptSaleProc(UINT32 deptNo);

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

	/*!
	@brief 更新销售类对象中对应全局变量的值
	@return  1: SUCCESS; 0: FAILURE
	@attention 发票代码和发票号会在组装发票信息时更新，所以这里不更新
	*/
	UINT8 UpdateSaleValue(void);
	

};

#endif

