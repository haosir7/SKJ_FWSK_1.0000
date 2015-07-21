#ifndef GUI_ENUMS_H
#define GUI_ENUMS_H
#include "keymap.h"

#define PRE_OBJ_KEY			UP_KEY
#define NEXT_OBJ_KEY		DOWN_KEY
#define KEY_PGUP			PAGEUP_KEY
#define KEY_PGDN			PAGEDOWN_KEY
#define CLEAR_ALL_EVENT		CLEAR_KEY
#define CLEAR_ONE_EVENT		BACKSPACE_KEY
#define ENTER_KEY		    CASH_KEY
#define BTN_ENTER_KEY		ENTER_KEY
#define INPUT_LEFT			LEFT_KEY
#define INPUT_RIGHT			RIGHT_KEY


#define SCREEN_W			192
#define SCREEN_H			64

#define CHANGE_INPUT_METHOD_EVENT  INPUT_METHOD

//for debug on the linux platform
/*
#define ENTER_KEY		13  //数字7
#define BTN_ENTER_KEY	ENTER_KEY	//数字7*/
/*#define NEXT_OBJ_KEY	'6'*/

//#define CHANGE_INPUT_METHOD_EVENT  0x5c //'\'
//#define EXIT_KEY		LOCKED_KEY  //退出主程序

#define BLANK				' '
#define PASSWORD_CHAR		'*'
#define INPUT_EVENT			10	

#define DOUBLE_ZERO_KEY		DIGIT00 //0xFF

#endif



