/****************************************************************************
文件名           ：SelfTestFunc.cpp
功能             ：awe4000r销售程序中机器自检的底层函数。
起始日期         ：2008-3-19 
作者             ：Yu Yan     
****************************************************************************/
#ifndef SELF_TEST_FUNC_H
#define SELF_TEST_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "CaMsgBox.h"


/*!
@brief CPU自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 CPUTest();

/*!
@brief Flash自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 FlashTest(void);

/*!
@brief RAM自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 RAMTest(void);

/*!
@brief COM0自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 COM0Test(void);

/*!
@brief COM2自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 COM2Test(void);

/*!
@brief 显示屏自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 LCDTest(void);

/*!
@brief 键盘自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 KeyboardTest(void);

/*!
@brief USB自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 USBTest(void);

/*!
@brief 蜂鸣器自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 BeepTest(void);

/*!
@brief 钱箱自检	
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 MoneyBoxTest(void);


#endif


