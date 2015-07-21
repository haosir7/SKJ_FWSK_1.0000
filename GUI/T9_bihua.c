/****************************************************************************************
*文件说明: 笔画输入查询算法文件
*使用说明:
作  者：  
日  期：  
-----------------------------------------------------------------------------------------
****************************************************************************************/
//#define T9_DEBUG	1

#include <fcntl.h>
#include "comdatatype.h"
#include "T9_bihua.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"

#if (T9BIHUA_SWITCH == 1)

//---------------------------------------------------------------------------
#define MAX_HANZI_NUM		20831
#define MAX_INSTROKES_NUM	12
#define MAX_READ_HANZI_NUM	5
#define T9_BH_PAGEDOWN		1
#define T9_BH_PAGEUP		2
//---------------------------------------------------------------------------

static UINT8 instrokes[MAX_INSTROKES_NUM];
static UINT8 instrokes_size;
static struct BiHua_Index BH_index;
static struct HanZi_Info HZ_info;

static INT32 Cur_HanZiIndex;
static UINT8 Cur_strokes_num;			//查询汉字过程中的当前查询笔画数
static UINT8 g_outsize;					//每次输出的字数,仅上翻页时用到

static unsigned char tmp_buf[20];

static struct BiHua_Index BH_index_bak;

struct _BHIndexBlock BH_block[] = 		//每种笔画的索引地址
{
	{ 0,  31 },
	{ 32, 51 },
	{ 52, 78 },
	{ 79, 104 },
	{ 105, 119 },
};

struct _HZoffsetBlock HZ_block[] = 		//每类汉字的首尾偏移
{
	{ 0,  	6191 },
	{ 6192, 9470 },
	{ 9471, 14403 },
	{ 14404, 18891 },
	{ 18892, 20831 },
};

#ifdef WIN32
FILE *bihua_fd = NULL;
FILE *bihua_index_fd = NULL;
#else
static int bihua_fd = 0;
static int bihua_index_fd = 0;
#endif


#ifdef T9_DEBUG
static UINT8 tmp_debug[3];
extern void PrintHexData(UINT8 *data, UINT32 len);
#endif

//**********************************************************************************


static INT32 bihua_pow(INT32 a, INT8 b)
{
	INT32 value;
	INT8 i=1;
	
	if (b == 0)
	{
		return 1;
	}
	
	value = a;
	while(i<b)
	{
		value = a * value;
		i++;
	}
	return value;
}

static INT32 Int32_convert(UINT8 *p)
{
	INT32 value;
	
	value = ((INT32) (*p)) | ((INT32) (*(p + 1))) << 8 | ((INT32) (*(p + 2))) << 16 | (INT32) (*(p + 3) << 24);
	
	return value;
}

/**************************************************************************
功能     ：读取bihua
格式     ：unsigned char Read_bihua()
返回值   ：0-失败，1－成功
**************************************************************************/
static unsigned char Read_bihua(unsigned char *pDes,unsigned int offset)
{
#ifdef WIN32
	if (bihua_fd == NULL)
	{
		bihua_fd = fopen("F:/Debug/bihua.bin", "rb");
	}
	fseek(bihua_fd, IC1_BIHUA_START_ADDR + offset*6, SEEK_SET);
	fread(pDes, 6, 1, bihua_fd);
#else
	if (bihua_fd <= 0)
	{
		bihua_fd = open("/usr/ziku/bihua.bin", O_RDONLY);
	}
	
	lseek(bihua_fd, IC1_BIHUA_START_ADDR + offset*6, SEEK_SET);
	read(bihua_fd, pDes, 6);
#endif

	return 1;
}

/**************************************************************************
功能     ：读取bihua_index
格式     ：unsigned char Read_bihua_index()
返回值   ：0-失败，1－成功
**************************************************************************/
static unsigned char Read_bihua_index(unsigned char *pDes,unsigned int offset)
{
#ifdef WIN32
	if (bihua_index_fd == NULL)
	{
		bihua_index_fd = fopen("F:/Debug/bihua.bin", "rb");
	}
	fseek(bihua_index_fd, IC1_BIHUA_INDEX_START_ADDR + offset*18, SEEK_SET);
	fread(pDes, 18, 1, bihua_index_fd);
#else
	if (bihua_index_fd <= 0)
	{
		bihua_index_fd = open("/usr/ziku/bihua.bin", O_RDONLY);
	}
	
	lseek(bihua_index_fd, IC1_BIHUA_INDEX_START_ADDR + offset*18, SEEK_SET);
	read(bihua_index_fd, pDes, 18);
#endif

	return 1;
}

/********************************************************************************
功能     ：清除上次查询中的过程变量
格式     ：void T9_BH_Clear(void)
输入参数 ：
返回值   ：
*********************************************************************************/
void T9_Strokes_Clear(void)
{
	memset(instrokes, 0, MAX_INSTROKES_NUM);
	instrokes_size = 0;
	memset(&BH_index, 0, sizeof(BH_index));
	memset(&HZ_info, 0, sizeof(HZ_info));
	Cur_strokes_num = 0;
 	Cur_HanZiIndex = 0;
}

/********************************************************************************
功能     ：根据给定笔画判断索引中当前的条目是否符合
格式     ：IsIndexRight(unsigned char *instrokes, unsigned int instrokes_size)
输入参数 ：instrokes：笔画字串
返回值   ：1－符合 0－不符合
*********************************************************************************/
static UINT8 IsIndexRight()
{
	UINT32 strokes_index_cmp;
	INT32 startNo_index_bak, endNo_index_bak;
	UINT8 temp;

	/* 截取输入笔画码的低4位生成"索引比较值" */
	if (instrokes_size > 4)
	{	
		memset (tmp_buf, 0, 10);
		memcpy (tmp_buf, instrokes, 4);
		strokes_index_cmp = atoi((char *)tmp_buf);
	}
	else
	{
		strokes_index_cmp = atoi((char *)instrokes);	
	}
	prn_log2("strokes_index_cmp=%lu", strokes_index_cmp);

	/* 将当前索引条目中的起始号、终止号信息转化成与"索引比较值"位数相同的数值 */
	if (instrokes_size == BH_index.bihua_num)
	{
		startNo_index_bak = BH_index.startNo_index;
		endNo_index_bak = BH_index.endNo_index;
	}
	else
	{
		if (instrokes_size>=4)
		{
			temp = 0;
		}
		else
		{
			if (BH_index.bihua_num < 4)
			{
				temp = BH_index.bihua_num - instrokes_size;
			}
			else
			{
				temp = 4 - instrokes_size;
			}
		}	
		startNo_index_bak = BH_index.startNo_index / bihua_pow(10, temp);
		endNo_index_bak = BH_index.endNo_index / bihua_pow(10, temp);
		
		prn_log4("startNo_index=%lu, endNo_index=%lu, temp=%d", startNo_index_bak, endNo_index_bak, temp);		
	}

	/* 进行比较 */
	if (strokes_index_cmp >= startNo_index_bak && strokes_index_cmp <= endNo_index_bak)
	{
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}

}

/********************************************************************************
功能     ：根据索引中当前条目的信息在主表中找出对应汉字的偏移值
格式     ：cmp_instrokes(unsigned char *instrokes, unsigned int instrokes_size)
输入	 ：instrokes：笔画码
输出	 ：Cur_HanZiIndex：偏移值
返回值   ：1－找到 0－未找到
说明	 ：如果找到符合的汉字Cur_HanZiIndex为该字的偏移，否则Cur_HanZiIndex值不变
*********************************************************************************/
static UINT8 cmp_instrokes(UINT8 mode)
{
	INT32 start, end, middle;
	INT32 instrokes_value, instrokes_cmp;

	prn_log("-----------Start search HANZI----------");
	start = BH_index.offset;
	end = BH_index.offset + BH_index.Hanzi_num;
	
	/* 折半查找汉字 */
	while (1)
	{
		middle = (start+end) / 2;
		
		if (Read_bihua(tmp_buf, middle) != SUCCESS)
		{
			prn_log("read hanzi_info ERR! strange!");
			return FAILURE;
		}
		HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
		HZ_info.GB_code[0] = tmp_buf[4];
		HZ_info.GB_code[1] = tmp_buf[5];
#ifdef T9_DEBUG				
		prn_log2("------Read HANZI_info: %lu-----", middle);
		prn_log2("HZ_info.bihua_code=%lu", HZ_info.bihua_code);
		memcpy(tmp_debug, HZ_info.GB_code, 2);
		prn_log2("GB_code: %s", tmp_debug);
#endif
		
		// 码比较
		instrokes_value = atoi((char *)(instrokes+1));
		instrokes_cmp = HZ_info.bihua_code / bihua_pow(10, BH_index.bihua_num - instrokes_size);
#ifdef T9_DEBUG				
		prn_log3("BH_index.bihua_num=%d, instrokes_size=%d", BH_index.bihua_num, instrokes_size);
		prn_log3("instrokes_value=%lu, instrokes_cmp=%lu", instrokes_value, instrokes_cmp);
#endif
		
		if (instrokes_cmp == instrokes_value)
		{
			Cur_HanZiIndex = middle;
			break;
		}
		else
		{
			if (start == middle && end == middle)
			{
				prn_log("---------search HANZI FAILURE--------");
				return FAILURE;
			}
			else if (instrokes_value > instrokes_cmp)
			{
				start = middle + 1;
			}
			else 
			{
				end = middle;
			}
		}
	}

	/* 找到汉字后，将偏移值回退到符合笔画的第一个汉字位置 */
	while(1)
	{
		if (mode == 1)
		{
			Cur_HanZiIndex --;
			if(Cur_HanZiIndex < BH_index.offset)
			{
				break;
			}
		}
		else
		{
			Cur_HanZiIndex ++;
			if(Cur_HanZiIndex > MAX_HANZI_NUM)
			{
				break;
			}
		}
		
		if (Read_bihua(tmp_buf, Cur_HanZiIndex) != SUCCESS)
		{
			prn_log("read hanzi_info ERR! strange!");
			return FAILURE;
		}
		HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
		HZ_info.GB_code[0] = tmp_buf[4];
		HZ_info.GB_code[1] = tmp_buf[5];
		
		instrokes_cmp = HZ_info.bihua_code / bihua_pow(10, BH_index.bihua_num - instrokes_size);
		prn_log2("instrokes_cmp=%lu", instrokes_cmp);
		
		if (instrokes_cmp != instrokes_value)
		{
			break;
		}
	}
	
	if (mode == 1)
	{
		Cur_HanZiIndex ++;
	}
	else
	{
		Cur_HanZiIndex --;
	}
	
	prn_log("----search HANZI SUCCESS complete----");


	return SUCCESS;
}

/********************************************************************************
功能     ：根据给定的笔画码、限定的笔画数在主表中找出第一（末尾）个汉字的偏移
格式     ：Search_BHindex_By_strokes(UINT8 *instrokes, UINT8 strokes_num)
输入参数 ：strokes_num：限定的查询笔画数
输出	 ：BH_index：索引条目信息（全局）
返回值   ：1－找到 0－无找到
*********************************************************************************/
static UINT8 Search_BHindex_By_strokes(UINT8 strokes_num, UINT8 mode)
{
	UINT8 strokes_index;
	UINT8 BH_oft;
	
	memcpy(&BH_index_bak, &BH_index, sizeof(BH_index));	//当前的索引信息备份
	
	
	prn_log2("-------search BH index:%d-------", strokes_num);
	
	BH_oft = (*instrokes) - 0x31;	
	
	for (strokes_index=BH_block[BH_oft].start; strokes_index<=BH_block[BH_oft].end; strokes_index++)		
	{
		Read_bihua_index(tmp_buf, strokes_index);
		
		BH_index.bihua_num 		= tmp_buf[0];
		BH_index.startNo_index 	= Int32_convert(&tmp_buf[1]);
		BH_index.endNo_index 	= Int32_convert(&tmp_buf[5]);
		BH_index.offset 		= Int32_convert(&tmp_buf[9]);
		BH_index.Hanzi_num 		= Int32_convert(&tmp_buf[13]);
#ifdef T9_DEBUG				
		prn_log2("------Read bihua index %d------", strokes_index);
//			PrintHexData(tmp_buf, 17);
		prn_log2("BH_index.bihua_num=%lu", BH_index.bihua_num);
		prn_log2("BH_index.startNo_index=%lu", BH_index.startNo_index);
		prn_log2("BH_index.endNo_index=%lu", BH_index.endNo_index);
		prn_log2("BH_index.offset=%lu", BH_index.offset);
		prn_log2("BH_index.Hanzi_num=%lu", BH_index.Hanzi_num);
#endif
		
		if (BH_index.bihua_num < strokes_num)
		{
			continue;
		}
		else if (BH_index.bihua_num > strokes_num)
		{
			break;
		}
		
		if (IsIndexRight() == SUCCESS)
		{
			if ( cmp_instrokes(mode) != SUCCESS )
			{
				continue;
			}
			else
			{
				prn_log("------search BH index SUCCESS------");	
				return SUCCESS;
			}
		}
	}
			
	prn_log("------search BH index FAILURE------");	
	memcpy(&BH_index, &BH_index_bak, sizeof(BH_index));	//从备份恢复索引信息
	return FAILURE;

}


/********************************************************************************
功能     ：将当前偏移移动到下一个符合条件的汉字
格式     ：UINT8 Put_CurIndex_to_next()
返回值   ：1－移动成功 
		   0－后面无符合的字
*********************************************************************************/
UINT8 Put_CurIndex_to_next()
{
	INT32 Cur_HanZiIndex_bak, Cur_strokes_num_bak;
	INT32 instrokes_cmp, instrokes_value;
	UINT8 strokes_max;
	
	prn_log("----start make cur offset to next----");
	
	/* 取出笔画码，作为比较基准 */
	instrokes_cmp = atoi( (char *)(instrokes+1) );
	
	/* 首先判断当前偏移加1位置的汉字是否符合 */	
	if (Cur_HanZiIndex == HZ_block[(*instrokes)-0x31].end)
	{
		prn_log("----make cur offset to previous FAILURE----");
		return FAILURE;
	}
	
	Cur_HanZiIndex_bak = Cur_HanZiIndex + 1;
	if (Read_bihua(tmp_buf, Cur_HanZiIndex_bak) != SUCCESS)
	{
		prn_log("read hanzi_info ERR! strange!");
		return FAILURE;
	}
	
	HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
	HZ_info.GB_code[0] = tmp_buf[4];
	HZ_info.GB_code[1] = tmp_buf[5];
#ifdef T9_DEBUG		
	prn_log("------read HZ info---------");
	prn_log2("HZ_info.bihua_code=%lu", HZ_info.bihua_code);
	memcpy(tmp_debug, HZ_info.GB_code, 2);
	prn_log2("GB_code: %s", tmp_debug);
#endif		
	
	/* 码比较 */
	instrokes_value = HZ_info.bihua_code / bihua_pow(10, BH_index.bihua_num - instrokes_size);
	
	prn_log3("instrokes_cmp=%lu, instrokes_value=%lu", instrokes_cmp, instrokes_value);
	if (instrokes_value == instrokes_cmp)
	{
		Cur_HanZiIndex = Cur_HanZiIndex_bak;
		prn_log("----make cur offset to next SUCCESS----");
		return SUCCESS;
	}

	/* 如不符合，继续在后面的索引中寻找 */
	prn_log("----try to find next index----");
	Cur_strokes_num_bak = Cur_strokes_num;
	strokes_max = 10;		//再考虑	
		
	while (Cur_strokes_num <= strokes_max)
	{
		Cur_strokes_num++;
		
		if (Cur_strokes_num > strokes_max)
		{
			break;
		}
		
		if (Search_BHindex_By_strokes(Cur_strokes_num, 1) == SUCCESS)
		{
			prn_log("----make cur offset to next SUCCESS----");
			return SUCCESS;
		}
	}
	
	Cur_strokes_num = Cur_strokes_num_bak;
	
	prn_log("----make cur offset to next FAILURE----");
	return FAILURE;
}

/********************************************************************************
功能     ：将当前偏移移动到上一个符合条件的汉字
格式     ：UINT8 Put_CurIndex_to_previous()
返回值   ：1－移动成功 
		   0－后面无符合的字
*********************************************************************************/
UINT8 Put_CurIndex_to_previous()
{
	INT32 Cur_HanZiIndex_bak, Cur_strokes_num_bak;
	INT32 instrokes_cmp, instrokes_value;
	UINT8 strokes_max;
	
	prn_log("----start make cur offset to previous----");
	
	/* 取出笔画码，作为比较基准 */
	instrokes_cmp = atoi( (char *)(instrokes+1) );
	
	/* 首先判断当前偏移减1位置的汉字是否符合 */	
	prn_log3("Cur_HanZiIndex=%d, BH_block.start=%d", Cur_HanZiIndex, BH_block[(*instrokes)-0x31].start);
	if (Cur_HanZiIndex == HZ_block[(*instrokes)-0x31].start)
	{
		prn_log("----make cur offset to previous FAILURE----");
		return FAILURE;
	}
	
	Cur_HanZiIndex_bak = Cur_HanZiIndex - 1;
	if (Read_bihua(tmp_buf, Cur_HanZiIndex_bak) != SUCCESS)
	{
		prn_log("read hanzi_info ERR! strange!");
		return FAILURE;
	}
	
	HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
	HZ_info.GB_code[0] = tmp_buf[4];
	HZ_info.GB_code[1] = tmp_buf[5];
#ifdef T9_DEBUG		
	prn_log("------read HZ info---------");
	prn_log2("HZ_info.bihua_code=%lu", HZ_info.bihua_code);
	memcpy(tmp_debug, HZ_info.GB_code, 2);
	prn_log2("GB_code: %s", tmp_debug);
#endif		
	
	/* 码比较 */
	instrokes_value = HZ_info.bihua_code / bihua_pow(10, BH_index.bihua_num - instrokes_size);
	
	prn_log3("instrokes_cmp=%lu, instrokes_value=%lu", instrokes_cmp, instrokes_value);
	if (instrokes_value == instrokes_cmp)
	{
		Cur_HanZiIndex = Cur_HanZiIndex_bak;
		prn_log("----make cur offset to previous SUCCESS----");
		return SUCCESS;
	}

	/* 如不符合，继续在前面的索引中寻找 */
	prn_log("----try to find previous index----");
	Cur_strokes_num_bak = Cur_strokes_num;
	strokes_max = 10;		//再考虑	
		
	prn_log3("Cur_strokes_num=%d, instrokes_size=%d", Cur_strokes_num, instrokes_size);
	while (Cur_strokes_num >= instrokes_size)
	{
		Cur_strokes_num--;
		
		if (Cur_strokes_num < instrokes_size)
		{
			break;
		}
		
		if (Search_BHindex_By_strokes(Cur_strokes_num, 0) == SUCCESS)
		{
			prn_log("----make cur offset to previous SUCCESS----");
			return SUCCESS;
		}
	}
	
	Cur_strokes_num = Cur_strokes_num_bak;
	
	prn_log("----make cur offset to previous FAILURE----");
	return FAILURE;
}

/********************************************************************************
功能     ：以当前偏移为准，向后查找5个汉字
格式     ：T9_Strokes_pageDown(UINT8 *outbuff, UINT8 *outsize)
输出参数 ：outbuff：找到的汉字
		   outsize：找到的汉字个数
补充说明 ：每次读六个汉字，向outbuff输出后5个，若outsize=6则说明在5个汉字后面仍有汉字。
*********************************************************************************/
void T9_Strokes_pageDown(UINT8 *outbuff, UINT8 *outsize)
{
	UINT8	i;
	
	prn_log("----------------------------------------------------");
	prn_log("--------Start Output Next Hanzi by HZoffset---------");
	prn_log("----------------------------------------------------");
	
	for (i=0; i<MAX_READ_HANZI_NUM; i++)
	{
		prn_log2("Cur_HanZiIndex=%lu", Cur_HanZiIndex);
		if (Read_bihua(tmp_buf, Cur_HanZiIndex) != SUCCESS)
		{
			prn_log("read hanzi_info ERR! strange!");
			return;
		}
		HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
		HZ_info.GB_code[0] = tmp_buf[4];
		HZ_info.GB_code[1] = tmp_buf[5];
		
		*(outbuff + i*2) 	 = HZ_info.GB_code[0];
		*(outbuff + i*2 + 1) = HZ_info.GB_code[1];
		
		if (Put_CurIndex_to_next() != SUCCESS)
		{
			break;
		}
	}
	
	if (i == MAX_READ_HANZI_NUM)
	{
		*outsize = 6;
	}
	else
	{
		*outsize = i + 1;
	}
	
	g_outsize = *outsize;
	return;
	
}

/********************************************************************************
功能     ：以当前偏移为准，向前查找5个汉字
格式     ：T9_Strokes_pageUP(UINT8 *outbuff, UINT8 *outsize)
输出参数 ：outbuff：找到的汉字
		   outsize：找到的汉字个数
补充说明 ：每次读六个汉字，向outbuff输出后5个，若outsize=6则说明在5个汉字后面仍有汉字。
*********************************************************************************/
void T9_Strokes_pageUP(UINT8 *outbuff, UINT8 *outsize)
{
	UINT8 i; 
	INT32 Cur_HanZiIndex_bak;

	prn_log("----------------------------------------------------");
	prn_log("------Start Output previous Hanzi by HZoffset-------");
	prn_log("----------------------------------------------------");
	
	/* 首先将偏移值向前移动10个符合的汉字 */
	for (i=0; i<MAX_READ_HANZI_NUM + g_outsize - 1; i++)
	{
		prn_log2("Cur_HanZiIndex=%lu", Cur_HanZiIndex);
		if (Put_CurIndex_to_previous() != SUCCESS)
		{
			break;
		}
	}
	
	prn_log3("g_outsize=%d, i=%d", g_outsize, i);
	if (i == 0)
	{
		*outsize = 0;
	}
	else if (i == MAX_READ_HANZI_NUM + g_outsize - 1) 
	{
		Cur_HanZiIndex_bak = Cur_HanZiIndex;
		
		if (Put_CurIndex_to_previous() == SUCCESS)
		{
			*outsize = 6;
		}
		else
		{
			*outsize = 5;		
		}
		Cur_HanZiIndex = Cur_HanZiIndex_bak;
	}
	else
	{
		*outsize = 5;
	}
	

	/* 再将从偏移值开始向后读5个符合的汉字 */
	if (*outsize != 0)
	{
		for (i=0; i<MAX_READ_HANZI_NUM; i++)
		{
			if (Read_bihua(tmp_buf, Cur_HanZiIndex) != SUCCESS)
			{
				prn_log("read hanzi_info ERR! strange!");
				return;
			}
			HZ_info.bihua_code = Int32_convert(&tmp_buf[0]);
			HZ_info.GB_code[0] = tmp_buf[4];
			HZ_info.GB_code[1] = tmp_buf[5];
			
			*(outbuff + i*2) 	 = HZ_info.GB_code[0];
			*(outbuff + i*2 + 1) = HZ_info.GB_code[1];		
			
			if (Put_CurIndex_to_next() != SUCCESS)
			{
				break;
			}
		}
	}
	
	g_outsize = *outsize;
	return;	
}

/********************************************************************************
功能     ：T9笔画输入，根据输入的笔画码输出符合的汉字(1至5个)
格式     ：T9_Strokes_enter(UINT8 *enter_strokes, UINT8 *OutHanZi)
输入参数 ：enter_strokes：笔画码
输出参数 ：OutHanZi：输出的汉字串
		   OutSize：输出的汉字个数
返回值   ：1－找到 0－无找到
补充说明 ：每次读六个汉字，向OutHanZi输出前(后)5个，若outsize=6则说明在5个汉字后(前)面仍有汉字。
*********************************************************************************/
UINT8 T9_Strokes_enter(UINT8 *enter_strokes, UINT8 *OutHanZi, UINT8 *OutSize)
{
	UINT8 strokes_max, i;
	
	instrokes_size = strlen((char *)enter_strokes);
	strcpy((char *)instrokes, (char *)enter_strokes);
	
	prn_log("------Input new strokes, then search BH index.------"); 
	prn_log2("instrokes_size = %d", instrokes_size);
	
	/* 根据输入的比画数生成被查寻笔画数的范围 */
	if (instrokes_size == 1)
	{
		strokes_max = 4;
	}
	else if (instrokes_size >= 2 && instrokes_size <= 8)
	{
		strokes_max = instrokes_size + 2;
	}
	else 
	{
		strokes_max = 10;
	}
	
	/* 寻找符合笔画码的第一个汉字 */
	strokes_max = 10;		//再考虑
	for ( i = instrokes_size; i <= strokes_max; i++ )		
	{
		/* 在当前笔画数内找符合的第一个索引 */
		if ( Search_BHindex_By_strokes(i, 1) != SUCCESS)
		{
			continue;
		}
		else
		{
			Cur_strokes_num = BH_index.bihua_num;
			prn_log("------Read bihua index SUCCESS Complete!------");
			break;
		}
	}
	
	if (i <= strokes_max)
	{
		/* 以偏移值为准，向后再找最多5个汉字 */
		T9_Strokes_pageDown(OutHanZi, OutSize);
		return SUCCESS;
	}
	else
	{
		*OutSize = 0;
		return FAILURE;
	}
	
}

#endif
