/************************************************************************
*文件说明: 词组输入头文件
*使用说明:
    
作  者：  尹春天
日  期：  2007年08月03日
版  本：  1.0

****************************************************************************/
#ifndef		__PHRASE_H
#define		__PHRASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comdatatype.h"

#define MAX_PHRASE_SIZE		8030
#define PHRASE_OFFSET		0xdb30
#define PHRASE_BUFFER_SIZE	64
#define PHRASE_BUFFER_LEN		16

struct Phrase_Index
{
	UINT16 code;
	UINT8 size;
	UINT32 address;
};
#ifdef __cplusplus
extern "C"
{ 
#endif
/********************************************************************************
功能     ：词组输入
格式     ：UINT8 Phrase_Strokes_enter(INT8 *enter_strokes, UINT8 *OutSize);
输入参数 ：
返回值   ：
*********************************************************************************/

UINT8 Phrase_Strokes_enter(INT8 *enter_strokes, UINT8 *OutSize);
INT8 *Phrase_Result(UINT8 index);

#ifdef __cplusplus
}
#endif

#endif

