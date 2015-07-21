/*! \file    Templatestruct.h
\brief    发票打印相关结构体定义文件
\author   zfj
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "comdatatype.h"

#define NAME_LEN                      30     ///发票头汉字内容字段
#define VALUE_LEN   	 	          200	 ///发票数据内容

#define  ATTRIBUTENAMECOUNT           100    ///抬头数量
#define  ATTRIBUTEVALUECOUNT          100    ///打印数据数量

#define  NORMAL_PRINT                 0     //普通打印
#define  ALL_PRINT                    1     //(全打印)电子存根打印
#define  TEST_PRINT					  2



#ifdef WIN32

PACK_PREFIX struct TmpAttribute 
{
	UINT32		nPaperLen;				//长度	
	UINT32		nPaperWidth;			//宽度
	UINT8		bClockLen;				//定长标志
	UINT32		nTextHeight;			//字高
	UINT32		nTextWith;				//字宽
	UINT32		nDy;					//行间距（Δy方向）
	UINT32		nDx;					//字间距(Δx方向)
	UINT32		nPrntendLen;			//打印后走纸长度
	UINT32		nPrntBegLen;			//打印前走纸长度
	UINT32      nxPos;                  //打印针头横向偏移
	UINT32      nyPos;                  //打印针头纵向偏移
	UINT8       nPrntType;              //打印模式--套打 1 普通打印  0
	UINT8       nAutoPaper;             //是否自动切纸
};

//发票台头结构体,属性的汉字部分
PACK_PREFIX struct TmpAttributeName
{
	UINT16		ID;
	UINT16		TYPE;
	UINT8		bPrnted;				//打印标志,是否已经打印
	UINT32		nX;
	UINT32		nY;
   	UINT32      nRight;
	UINT32      nBottom;
	UINT8		content[NAME_LEN];//存储内容
};

//属性的值
PACK_PREFIX struct TmpAttributeValue
{
	UINT16		ID;
	UINT16		TYPE;
	UINT8		bPrnted;				   //打印标志,是否已经打印
	UINT32		nX;						   //打印左端
	UINT32		nY;						   //打印上端
	UINT32      nRight;					   //打印右端
	UINT32      nBottom;				   //打印框底部位置
	UINT8		content[VALUE_LEN];        //存储内容
	UINT8       curLine;                   //当前打印行
};

//存储模版数据
PACK_PREFIX struct  TmplateData
{
  struct TmpAttribute        tmpattrData;
  struct TmpAttributeName    tmpattrnameData[ATTRIBUTENAMECOUNT];   //打印模版抬头数据
  struct TmpAttributeValue   tmpPrntValueData[ATTRIBUTEVALUECOUNT];  //打印数据
};

#else
//模板整体属性
PACK_PREFIX struct TmpAttribute 
{
	UINT32		nPaperLen;				//长度	
	UINT32		nPaperWidth;			//宽度
	UINT8		bClockLen;				//定长标志
	UINT32		nTextHeight;			//字高
	UINT32		nTextWith;				//字宽
	UINT32		nDy;					//行间距（Δy方向）
	UINT32		nDx;					//字间距(Δx方向)
	UINT32		nPrntendLen;			//打印后走纸长度
	UINT32		nPrntBegLen;			//打印前走纸长度
	UINT32      nxPos;                  //打印针头横向偏移
	UINT32      nyPos;                  //打印针头纵向偏移
	UINT8       nPrntType;              //打印模式--套打 1 普通打印  0
	UINT8       nAutoPaper;             //是否自动切纸
} __attribute__ ((packed));

//发票台头结构体,属性的汉字部分
PACK_PREFIX struct TmpAttributeName
{
	UINT16		ID;
	UINT16		TYPE;
	UINT8		bPrnted;				//打印标志,是否已经打印
	UINT32		nX;
	UINT32		nY;
   	UINT32      nRight;
	UINT32      nBottom;
	UINT8		content[NAME_LEN];//存储内容
} __attribute__ ((packed));

//属性的值
PACK_PREFIX struct TmpAttributeValue
{
	UINT16		ID;
	UINT16		TYPE;
	UINT8		bPrnted;				   //打印标志,是否已经打印
	UINT32		nX;						   //打印左端
	UINT32		nY;						   //打印上端
	UINT32      nRight;					   //打印右端
	UINT32      nBottom;				   //打印框底部位置
	UINT8		content[VALUE_LEN];        //存储内容
	UINT8       curLine;                   //当前打印行
} __attribute__ ((packed));

//存储模版数据
PACK_PREFIX struct  TmplateData
{
  struct TmpAttribute        tmpattrData;
  struct TmpAttributeName    tmpattrnameData[ATTRIBUTENAMECOUNT];   //打印模版抬头数据
  struct TmpAttributeValue   tmpPrntValueData[ATTRIBUTEVALUECOUNT];  //打印数据
} __attribute__ ((packed));

#endif


#endif

