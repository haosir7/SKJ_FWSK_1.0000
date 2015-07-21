
/*! \file  keypad.h
\brief     键盘驱动  
\author   zl
*/

/*-----------------------键盘部分的接口函数-------------------------*/
#ifndef KEYPAD_POS_DESIGN_H
#define KEYPAD_POS_DESIGN_H
#include "VersionConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if (POS_TYPE == POS_APE3000R)
#define SINGLE_KEY_MAX_NUMBER	80
#else
#define SINGLE_KEY_MAX_NUMBER	34
#endif

#define DOUBLE_KEY_MAX_NUMBER	20

#define KEYPAD_MAX_ROW		5
#define KEYPAD_MAX_COL		7

#define MAX_KEYMAP_NUM		99

#define	GET_SINGLEKEY_VALUE(x1,y1)			((UINT32)(x1<<8 | y1))
#define	GET_DOUBLEKEY_VALUE(x1,y1,x2,y2)	((UINT32)(x1<<24 | y1<<16 | x2<<8 | y2))


#define KEYPAD_MAGIC 'k'
#define KEYPAD_MAXNR  3
#define KPD_BLOCK					_IOW('k', 1, unsigned int)
#define KPD_NONBLOCK				_IOW('k', 2, unsigned int)
#define KPD_SET_CONTINUOUS_MODE		_IOW('k', 3, unsigned int)


typedef struct _KeyStruct
{
	unsigned char type;		//按键类型：1～3对应单键/双键/三键
	unsigned short row1;
	unsigned short col1;
	unsigned short row2;
	unsigned short col2;
}KeyStruct;


typedef struct _SingleKey
{
	unsigned short row1;
	unsigned short col1;
	unsigned char key;
	unsigned char flag;		//是否可更改键值定义 0－可改，1－不可改
}SingleKey;

typedef struct _DoubleKey
{
	unsigned short row1;
	unsigned short col1;
	unsigned short row2;
	unsigned short col2;
	unsigned char key;
	unsigned char flag;		//是否可更改键值定义 0－可改，1－不可改
}DoubleKey;

/*!
@brief 键盘开启 
*/
void KeyEnable(void);

/*!
@brief 键盘关闭
*/
void KeyDisable(void);

/*!
@brief 获取按键键值
@return 获取键盘值 
*/
unsigned char ReadKeypad(void);

/*!
@brief 设置连续按键响应模式
@param[in] mode 1 - 响应连续按键
                0 - 不响应 
*/
void SetKeypadPressMode(unsigned int mode);

#ifdef __cplusplus
}
#endif

#endif
