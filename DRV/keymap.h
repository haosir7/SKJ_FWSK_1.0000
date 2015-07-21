
/*! \file   keymap.h 
   \brief   AWE4000键盘定义
   \author  zl 
 */

#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include "VersionConfig.h"

#define	ESC_KEY				0x80		//返回键
#define FORWARD_KEY			0x81		//进纸
#define BACKWARD_KEY		0x82		//退纸

#define PAGEUP_KEY			0x83		//上翻
#define PAGEDOWN_KEY		0x84		//下翻

#if (POS_TYPE == POS_APE3000R)
#define UP_KEY		    	0x85		//上
#define DOWN_KEY			0x86		//下
#define LEFT_KEY			0x87		//左
#define RIGHT_KEY			0x88		//右
#else 
#define UP_KEY				DIGIT8		//上
#define DOWN_KEY			DIGIT2		//下
#define LEFT_KEY			DIGIT4		//左
#define RIGHT_KEY			DIGIT6		//右
#endif

#define SHITF_KEY			0x89		//shift
#define BACKSPACE_KEY		0x8A		//退格
#define CLEAR_KEY			0x8B		//清除


//部类与plu键
#define DEPT1_KEY			0x90
#define DEPT2_KEY			0x91
#define DEPT3_KEY			0x92
#define DEPT4_KEY			0x93
#define DEPT5_KEY			0x94

#define CHANGE_DEPT_KEY		0x95        //部类
#define	PLU_KEY				0x96        //PLU
#define CANCEL_GOODS		0x97		//商品取消	
#define CLIENT_NAME_INPUT	0x98		//付款人
#define INPUT_METHOD		0x99		//输入法


//销售
#define PLUS_KEY			0xA0		//乘/时
#define SUM_INPUT			0xA1	    //总价
#define PRICE_INPUT			0xA2	    //单价
#define TOTAL_CANCEL		0xA4		//整单取消
#define FIND_KEY			0xA5		//查询(帮助)
#define HELP_KEY			0xA6		//帮助
#define CASH_KEY			0xA7		//现金


//顶端功能键
#define	LOCKED_KEY			0xB0		//锁机
#define OPEN_CASH_BOX		0xB1		//开钱箱
#define PAYMENT_MODE_KEY	0xB2		//查询
#define CHARGE_KEY			0xB3		//服务费
#define GOODSREDUCT_KEY		0xB4		//折让
#define DISCOUNT_KEY		0xB5		//折扣


#define F1_FUNCTION_KEY		0xBA		//F1功能键
#define F2_FUNCTION_KEY		0xBB		//F2功能键
#define F3_FUNCTION_KEY		0xBC		//F3功能键
#define F4_FUNCTION_KEY		0xBD		//F4功能键
#define F5_FUNCTION_KEY		0xBE		//F5功能键

//数字键
#define DIGIT0			'0'
#define DIGIT1			'1'
#define DIGIT2			'2'
#define DIGIT3			'3'
#define DIGIT4			'4'
#define DIGIT5			'5'
#define DIGIT6			'6'
#define DIGIT7			'7'
#define DIGIT8			'8'
#define DIGIT9			'9'

#define DIGIT00			0xD0

#define DOT_KEY			'.'             /*小数点*/
#define POUND_KEY		'#'             /*井号*/


//其他功能键
#define	SCANNER_EVENT_KEY	0xF0		//扫描枪事件
#define EXIT_APP_KEY		0xF1		//退出应用（调试用）
#define	SHIFT_PLU_KEY	    0xF2      //临时录入商品名称的

#if (POS_TYPE == POS_APE3000R)
#define TAB_KEY				DOWN_KEY
#else
#define TAB_KEY				0xF3		//TAB_KEY兼容旧程序
#endif

//字符键
#define	LETTER_A		DEPT1_KEY
#define	LETTER_B		DEPT2_KEY
#define	LETTER_C		DEPT3_KEY
#define	LETTER_D		DEPT4_KEY
#define	LETTER_E		DEPT5_KEY

#if (POS_TYPE == POS_APE3000R)
#define	LETTER_F			OPEN_CASH_BOX
#define	LETTER_G			PAYMENT_MODE_KEY
#define	LETTER_H			CHARGE_KEY
#define	LETTER_I			GOODSREDUCT_KEY
#define	LETTER_J			DISCOUNT_KEY
#else //POS_TYPE
#define	LETTER_F		CHANGE_DEPT_KEY
#define	LETTER_G		PLU_KEY
#define	LETTER_H		CANCEL_GOODS
#define	LETTER_I		CLIENT_NAME_INPUT
#define	LETTER_J		INPUT_METHOD
#endif //POS_TYPE

typedef struct TKeyMap
{
    int  keyValue;                //键盘值
	char  *keyStr;          	    //键盘宏字符串            
	
};

#endif /*  __KEYMAP_H__  */
