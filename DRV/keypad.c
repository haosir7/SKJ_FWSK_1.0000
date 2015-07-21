
/*! \file  keypad.c
\brief     键盘驱动  
\author   zl
*/

#include  <sys/ioctl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/ioctl.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <termios.h>
#include  <errno.h>

#include "keypad.h"
#include "keymap.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


#if (POS_TYPE == POS_APE3000R)

SingleKey singleKey_TAB[SINGLE_KEY_MAX_NUMBER] = 
{
	//顶端功能键
	{5, 7, LOCKED_KEY, 1},
	{4, 7, OPEN_CASH_BOX, 0},
	{3, 7, PAYMENT_MODE_KEY, 1},
	{2, 7, CHARGE_KEY, 1},
	{1, 7, GOODSREDUCT_KEY, 1},
	{0, 7, DISCOUNT_KEY, 1},
	
	//第1列
	{7, 6, ESC_KEY, 1},
	{7, 5, CHANGE_DEPT_KEY, 0},
	{7, 4, PAGEUP_KEY, 1},
	{6, 2, LEFT_KEY, 1},
	{6, 3, SHITF_KEY, 1},
	
	//第2列
	{6, 6, FORWARD_KEY, 1},
	{6, 5, CANCEL_GOODS, 0},
	{6, 4, UP_KEY, 1},
	{5, 3, DOWN_KEY, 1},
	{5, 2, BACKSPACE_KEY, 1},
	
	//第3列
	{5, 6, BACKWARD_KEY, 1},
	{5, 5, TOTAL_CANCEL, 0},
	{5, 4, PAGEDOWN_KEY, 1},
	{4, 3, RIGHT_KEY, 1},
	{4, 2, CLEAR_KEY, 1},
	
	//第4列
	{4, 6, DEPT1_KEY, 1},
	{4, 5, DEPT2_KEY, 0},
	{4, 4, DEPT3_KEY, 1},
	{3, 3, DEPT4_KEY, 1},
	{3, 2, DEPT5_KEY, 1},
	
	//第5列
	{3, 6, PLU_KEY, 1},
	{3, 5, DIGIT7, 1},
	{3, 4, DIGIT4, 1},
	{2, 3, DIGIT1, 1},
	{2, 2, DIGIT0, 1},
	
	//第6列
	{2, 6, PLUS_KEY, 1},
	{2, 5, DIGIT8, 1},
	{2, 4, DIGIT5, 1},
	{1, 3, DIGIT2 , 1},
	{1, 2, DIGIT00 , 1},
	
	//第7列
	{1, 6, SUM_INPUT, 1},
	{1, 5, DIGIT9, 1},
	{8, 2, DIGIT6, 1},
	{0, 3, DIGIT3, 1},
	{1, 4, DOT_KEY, 1},
	
	//第8列
	{0, 6, PRICE_INPUT, 1},
	{0, 5, CLIENT_NAME_INPUT, 1},
	{0, 4, INPUT_METHOD, 1},
	{8, 3, CASH_KEY, 1},
	{0, 2, CASH_KEY, 1},
	
}; 

DoubleKey doubleKey_TAB[DOUBLE_KEY_MAX_NUMBER] = 
{
	{4,7,6,3, F1_FUNCTION_KEY, 1},	
	{3,7,6,3, F2_FUNCTION_KEY, 1},	
	{2,7,6,3, F3_FUNCTION_KEY, 0},	
	{1,7,6,3, F4_FUNCTION_KEY, 0},	
	{0,7,6,3, F5_FUNCTION_KEY, 1},
	{1,4,6,3, POUND_KEY, 1},			//井号键			
	{6,3,7,5, EXIT_APP_KEY, 1},			//退出键
	{0,2,8,3, CASH_KEY, 1},
};

#else		// 4000R 4020R和5020R

#if (KEYPAD_TYPE == 1)
SingleKey singleKey_TAB[SINGLE_KEY_MAX_NUMBER] = 
{
	//第1列
	{4,1,ESC_KEY,1},
	{3,1,OPEN_CASH_BOX,1},
	{2,1,FORWARD_KEY,1},
	{1,1,BACKSPACE_KEY,1},
	{0,2,SHITF_KEY,0},
	
	//第2列
	{4,2,DEPT1_KEY,0},
	{3,2,DEPT2_KEY,0},
	{2,2,DEPT3_KEY,0},
	{1,2,DEPT4_KEY,0},
	{0,1,DEPT5_KEY,1},
	
	//第3列
	{4,3,CHANGE_DEPT_KEY,1},
	{3,3,PLU_KEY,1},
	{2,3,CANCEL_GOODS,1},
	{1,3,CLIENT_NAME_INPUT,1},
	{0,3,INPUT_METHOD,1},
	
	//第4列
	{4,4,PLUS_KEY,0},
	{3,4,DIGIT7,1}, 
	{2,4,DIGIT4,1}, 
	{1,4,DIGIT1,1}, 
	{0,4,DIGIT0,1}, 
	
	//第5列
	{4,5,PRICE_INPUT,0},
	{3,5,DIGIT8,1}, 
	{2,5,DIGIT5,1}, 
	{0,5,DIGIT2,1}, 
	{1,5,DOT_KEY,1}, 
	
	//第6列
	{4,6,SUM_INPUT,0},
	{3,6,DIGIT9,1}, 
	{2,6,DIGIT6,1}, 
	{1,6,DIGIT3,1}, 
	{0,6,TAB_KEY,1},
	
	//第7列
	{4,7,DISCOUNT_KEY,0},
	{3,7,GOODSREDUCT_KEY,1},
	{2,7,FIND_KEY,1},
	{0,7,CASH_KEY,1},
	
}; 

DoubleKey doubleKey_TAB[DOUBLE_KEY_MAX_NUMBER] = 
{
	{0,2,2,1, BACKWARD_KEY, 1},	//退纸
	{0,2,1,1, CLEAR_KEY, 1},	//清除
	{0,2,2,3, TOTAL_CANCEL, 0},	//整单取消
	{0,2,2,7, HELP_KEY, 0},	//帮助
	{0,2,1,5, POUND_KEY, 1},			//井号键
	{0,2,3,6, PAGEUP_KEY, 1},			//PAGEUP
	{0,2,1,6, PAGEDOWN_KEY, 1},			//PAGEDOWN
	{0,2,4,1, LOCKED_KEY, 1},	//锁机
	{0,2,3,1, EXIT_APP_KEY, 0},	//退出应用（临时）
	{0,2,3,3, SHIFT_PLU_KEY, 1},	//临时输入商品名称 （shift+PLU）
};

#else  //KEYPAD_TYPE为0
SingleKey singleKey_TAB[SINGLE_KEY_MAX_NUMBER] = 
{
	//第1列
	{4,1,ESC_KEY,1},
	{3,1,OPEN_CASH_BOX,1},
	{2,1,FORWARD_KEY,1},
	{1,1,BACKSPACE_KEY,1},
	{0,1,SHITF_KEY,0},
	
	//第2列
	{4,2,DEPT1_KEY,0},
	{3,2,DEPT2_KEY,0},
	{2,2,DEPT3_KEY,0},
	{1,2,DEPT4_KEY,0},
	{0,2,DEPT5_KEY,1},
	
	//第3列
    {4,3,CHANGE_DEPT_KEY,1},
	{3,3,PLU_KEY,1},
	{2,3,CANCEL_GOODS,1},
	{1,3,CLIENT_NAME_INPUT,1},
	{0,3,INPUT_METHOD,1},
	
	//第4列
	{4,4,PLUS_KEY,0},
	{3,4,DIGIT7,1}, 
	{2,4,DIGIT4,1}, 
	{1,4,DIGIT1,1}, 
	{0,4,DIGIT0,1}, 
	
	//第5列
	{4,5,PRICE_INPUT,0},
	{3,5,DIGIT8,1}, 
	{2,5,DIGIT5,1}, 
	{0,5,DIGIT2,1}, 
	{1,5,DOT_KEY,1}, 
	
	//第6列
	{4,6,SUM_INPUT,0},
	{3,6,DIGIT9,1}, 
	{2,6,DIGIT6,1}, 
	{1,6,DIGIT3,1}, 
	{0,6,TAB_KEY,1},
	
	//第7列
	{4,7,DISCOUNT_KEY,0},
	{3,7,GOODSREDUCT_KEY,1},
	{2,7,FIND_KEY,1},
	{0,7,CASH_KEY,1},
	
}; 

DoubleKey doubleKey_TAB[DOUBLE_KEY_MAX_NUMBER] = 
{
	{0,1,2,1, BACKWARD_KEY, 1},	//退纸
	{0,1,1,1, CLEAR_KEY, 1},	//清除
	{0,1,2,3, TOTAL_CANCEL, 0},	//整单取消
	{0,1,3,7, HELP_KEY, 0},	//帮助
	{0,1,1,5, POUND_KEY, 1},			//井号键
	{0,1,3,6, PAGEUP_KEY, 1},			//PAGEUP
	{0,1,1,6, PAGEDOWN_KEY, 1},			//PAGEDOWN
	{0,1,4,1, LOCKED_KEY, 1},	//锁机
	{0,1,3,1, EXIT_APP_KEY, 0},	//退出应用（临时）
};

#endif

#endif


//INT32 g_keymapList[MAX_KEYMAP_NUM];

volatile  static int fd_kpi;		//设备文件号



void KeyEnable(void)
{
	
	//打开键盘设备文件
	if( ( fd_kpi = open("/dev/keypad",O_RDONLY) ) == -1 )
	{
		prn_log("open keypad err\n");
	}
	
	SetKeypadPressMode(0);
}


void KeyDisable(void)
{
	if( close(fd_kpi) ==-1 )
	{		
		prn_log("close keypad err!\n");
		//		exit(-1);
	}
}


unsigned char GetKey_Value(KeyStruct *key)
{
	int i;
	
	if (key->type == 1)
	{
		prn_log3("row = %d, col = %d", key->row1, key->col1);
		for (i=0; i<SINGLE_KEY_MAX_NUMBER; i++)
		{
			prn_log2("singleKey_TAB[i].row1 = %x", singleKey_TAB[i].row1);
			if (key->row1 == singleKey_TAB[i].row1)
			{
				prn_log2("singleKey_TAB[i].col1 = %x", singleKey_TAB[i].col1);
				
				if (key->col1 == singleKey_TAB[i].col1)
				{
					prn_log2("singleKey_TAB[i].key = %x", singleKey_TAB[i].key);
					return singleKey_TAB[i].key;
				}
				
			}
		}
	}
	else if (key->type == 2)
	{
		prn_log5("row1 = %d, col1 = %d, row2 = %d, col2 = %d", 
			key->row1, key->col1, key->row2, key->col2);
		for (i=0; i<DOUBLE_KEY_MAX_NUMBER; i++)
		{
			if ( key->row1 == doubleKey_TAB[i].row1 &&
				key->row2 == doubleKey_TAB[i].row2 &&
				key->col1 == doubleKey_TAB[i].col1 &&
				key->col2 == doubleKey_TAB[i].col2 
				)
			{
				return doubleKey_TAB[i].key;
			}
		}
	}
	
	return 0;
}


unsigned char ReadKeypad(void)
{
	KeyStruct key;
	
	if ( read(fd_kpi, &key, sizeof(key)) == 1 )
    {
		return (GetKey_Value(&key));
    }  
	
	return 0;
}





void SetKeypadPressMode(unsigned int mode)
{
	ioctl(fd_kpi, KPD_SET_CONTINUOUS_MODE, mode);
}


