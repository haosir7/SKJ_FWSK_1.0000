/************************************************************************
*文件说明: 五笔输入头文件
*使用说明:
    
作  者：  尹春天
日  期：  2007年07月02日
版  本：  1.0

****************************************************************************/
#ifndef		WUBI_H
#define		WUBI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comdatatype.h"


//98版
//#define MAX_WUBI_SIZE		68301
//#define WUBI_OFFSET			0x96200
//86版
#define MAX_WUBI_SIZE		68330
#define WUBI_OFFSET			0x96240

#define WUBI_BUFFER_SIZE	16
#define WUBI_BUFFER_LEN		16

struct WuBi_Index
{
	INT8 code[4];
	UINT8 size;
	UINT32 address;
};


/********************************************************************************
功能     ：清除上次查询中的过程变量
格式     ：void T9_Strokes_Clear(void)
输入参数 ：
返回值   ：
*********************************************************************************/

UINT8 Wubi_Strokes_enter(INT8 *enter_strokes, UINT8 *OutSize);
INT8 *Wubi_Result(UINT8 index);
#endif
