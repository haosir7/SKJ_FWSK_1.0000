/************************************************************************
*文件说明: 笔画输入查询算法头文件
*使用说明:
作  者：  
日  期：  

****************************************************************************/
#ifndef		T9_BIHUA_H
#define		T9_BIHUA_H

#define 	T9BIHUA_SWITCH 		1

#if (T9BIHUA_SWITCH == 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define IC1_BIHUA_START_ADDR              	0		
#define IC1_BIHUA_INDEX_START_ADDR          (IC1_BIHUA_START_ADDR+124*(1<<10))				


struct BiHua_Index
{
	INT8 bihua_num;			//笔画数
	INT32 startNo_index;		//起始码(前4位)
	INT32 endNo_index;		//终止码(后4位)
	INT32 offset;				//索引便宜值
	INT32 Hanzi_num;			//索引包含字数
};

struct HanZi_Info
{
	INT32 bihua_code;
	INT8 GB_code[2];
};

struct _BHIndexBlock
{
	UINT8 start;
	UINT8 end;
};

struct _HZoffsetBlock
{
	UINT32 start;
	UINT32 end;
};

#ifdef __cplusplus
extern "C"
{
#endif
/********************************************************************************
功能     ：清除上次查询中的过程变量
格式     ：void T9_Strokes_Clear(void)
输入参数 ：
返回值   ：
*********************************************************************************/
void T9_Strokes_Clear(void);

/********************************************************************************
功能     ：以当前偏移为准，向后查找5个汉字
格式     ：T9_Strokes_pageDown(UINT8 *outbuff, UINT8 *outsize)
输出参数 ：outbuff：找到的汉字
		   outsize：找到的汉字个数
补充说明 ：每次读六个汉字，向outbuff输出后5个，若outsize=6则说明在5个汉字后面仍有汉字。
*********************************************************************************/
void T9_Strokes_pageDown(UINT8 *outbuff, UINT8 *outsize);

/********************************************************************************
功能     ：以当前偏移为准，向前查找5个汉字
格式     ：T9_Strokes_pageUP(UINT8 *outbuff, UINT8 *outsize)
输出参数 ：outbuff：找到的汉字
		   outsize：找到的汉字个数
补充说明 ：每次读六个汉字，向outbuff输出后5个，若outsize=6则说明在5个汉字后面仍有汉字。
*********************************************************************************/
void T9_Strokes_pageUP(UINT8 *outbuff, UINT8 *outsize);

/********************************************************************************
功能     ：T9笔画输入，根据输入的笔画码输出符合的汉字(1至5个)
格式     ：T9_Strokes_enter(UINT8 *enter_strokes, UINT8 *OutHanZi)
输入参数 ：enter_strokes：笔画码
输出参数 ：OutHanZi：输出的汉字串
		   OutSize：输出的汉字个数
返回值   ：1－找到 0－无找到
补充说明 ：每次读六个汉字，向OutHanZi输出前(后)5个，若outsize=6则说明在5个汉字后(前)面仍有汉字。
*********************************************************************************/
UINT8 T9_Strokes_enter(UINT8 *enter_strokes, UINT8 *OutHanZi, UINT8 *OutSize);

#ifdef __cplusplus
}
#endif


#endif	//T9BIHUA_SWITCH

#endif
