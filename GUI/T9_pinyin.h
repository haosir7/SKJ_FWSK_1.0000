/************************************************************************
*文件说明: 中文输入头文件
*使用说明:
     文件是拼音输入法码表数据文件，输入字符集: abcdefghijklmnopqrstuvwxyz(小写)
     对于每个合法的字符串_pInput，函数unsigned char *GetPyIndex(unsigned char *_pInput)
     将返回一个汉字字符串或空串NULL;
     _pInput指向的空间要求最少6个字节空间，当输入不满6字节时，用空格' '补齐
     当输入只有一个字符时，函数返回5个以该字符声母的常用汉字(你也可以任意添加
     你想要的字符，包括其它项亦如此)；符号的输入为"zz",也可改变{_PyMa_zz,"z}   "}中的'z'
     使其满足的要求。
     _cByte可以重新定义。
作  者：  程智锋
日  期：  2004年9月27日
版  本：  0.9

****************************************************************************/
#ifndef		T9_CHNTOOL_H
#define		T9_CHNTOOL_H

#define 	T9PINYIN_SWITCH 		1

#if (T9PINYIN_SWITCH == 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <wchar.h>


#ifndef SUCCESS
#define SUCCESS		1
#endif

#ifndef FAILURE
#define FAILURE		0
#endif

#define	PY_MAX_LENGTH	12

#define	PyIndex_i PyIndex_j
#define	PyIndex_u PyIndex_w
#define	PyIndex_v PyIndex_w
//#define NULL 0

typedef const char _cByte;     //常量字节，用于定义字库信息

//将拼音与对应的韵母组合在一起
struct _PyIndexStru{
//	const char* _PyMa;
	const char* _YunMu;
};

//按键与键码的匹配
struct _KeyBuf    
{
        int KeyNum;           //1~8
        char* KeyStr;  //"abc","def"...
};

typedef struct _PyIndexStru _PyIndexType;


#ifdef __cplusplus
extern "C"
{
#endif

char IsPyStr(unsigned char* _pInput);      //是否为有效拼音
unsigned char T9_Input(unsigned char *key, unsigned char *num, unsigned char (*PYBuf)[PY_MAX_LENGTH]); 	//按键对应的字母经检索组合成有效的拼音

#ifdef __cplusplus
}
#endif


#endif //T9PINYIN_SWITCH

#endif
