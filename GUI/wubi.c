 /****************************************************************************************
*文件说明: 五笔输入查询算法文件
*使用说明:
作  者：  
日  期：  
-----------------------------------------------------------------------------------------
****************************************************************************************/


#ifdef WIN32
#include <io.h>
#include "fcntl.h"
#else
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#endif
#include "wubi.h"

static INT32 wubi_fd = 0;
static INT8 wubi_buffer[WUBI_BUFFER_SIZE][WUBI_BUFFER_LEN];

static UINT8 Read_Wubi(INT8 *pDes, struct WuBi_Index *pIndex);
static UINT8 Wubi_Search(INT8 *enter_strokes, INT32 *offset);
static UINT8 Read_Wubi_Index(UINT32 offset, struct WuBi_Index *pIndex);
static INT32 Wubi_Cmp(INT8 *Code1, INT8 *Code2, UINT8 Len);

UINT8 Wubi_Strokes_enter(INT8 *enter_strokes, UINT8 *OutSize)
{
	struct WuBi_Index Index;
	INT8 enter[8];
	INT32 begin = 0;
	INT32 end = 0;
	INT32 current = 0;
	INT32 i;

	if ((strlen(enter_strokes) > 4) || (strlen(enter_strokes) == 0))
	{
		return FAILURE;
	}

	strcpy(enter, enter_strokes);

	if (Wubi_Search(enter, &current) == SUCCESS)
	{
		memset(wubi_buffer, 0, WUBI_BUFFER_SIZE*WUBI_BUFFER_LEN);

		begin = current;
		while (begin >= 1)
		{
			begin--;
			Read_Wubi_Index(begin * 9, &Index);
		
			if (Wubi_Cmp(enter, Index.code, 4) != 0)
			{
				begin++;
				break;
			}
			if (begin == 0)
			{
				break;
			}
		}

		end = current;
		while (end < MAX_WUBI_SIZE - 1)
		{
			end++;
			Read_Wubi_Index(end * 9, &Index);
		
			if (Wubi_Cmp(enter, Index.code, 4) != 0)
			{
				end--;
				break;
			}
			if (end >= MAX_WUBI_SIZE - 1)
			{
				break;
			}
		}


		*OutSize = end - begin + 1;
		for(i=begin; i<=end; i++)
		{	
			Read_Wubi_Index(i * 9, &Index);
			Read_Wubi(wubi_buffer[i-begin], &Index);
		}

		return SUCCESS;
	}
	
	*OutSize = 0;
	return FAILURE;
}

UINT8 Wubi_Search(INT8 *enter_strokes, INT32 *offset)
{
	INT32 low = 0;
	INT32 high = MAX_WUBI_SIZE - 1;
	INT32 mid;
	INT32 cmp;
	struct WuBi_Index Index;
	struct WuBi_Index *pIndex = &Index;

	UINT8 found = FAILURE;

	while ((low <= high) && (found == FAILURE))
	{
		mid = (low + high) / 2;
		
		Read_Wubi_Index(mid * 9, pIndex);
		
		cmp = Wubi_Cmp(enter_strokes, pIndex->code, 4);

		if (cmp == 0)
		{
			*offset = mid;
			found = SUCCESS;
			break;
		}
		else if (cmp < 0)
		{
			high= mid - 1;
		}
		else if (cmp > 0)
		{
			low = mid + 1;
		}
	}

	return  found;
}

INT32 Wubi_Cmp(INT8 *Code1, INT8 *Code2, UINT8 Len)
{
	INT32 flag;
	INT8 Str1[8];
	INT8 Str2[8];

	memset(Str1, 0, 8);
	memset(Str2, 0, 8);
	memcpy(Str1, Code1, Len);
	memcpy(Str2, Code2, Len);

	flag = Str1[0] - Code2[0];

	if (flag != 0)
	{
		return flag;
	}

	flag = strlen(&Str1[1]) - strlen(&Str2[1]);

	if (flag != 0)
	{
		return flag;
	}

	return strcmp(&Str1[1], &Str2[1]);
	
}
UINT8 Read_Wubi(INT8 *pDes, struct WuBi_Index *pIndex)
{
	if (wubi_fd == 0)
	{
#ifdef WIN32
		wubi_fd = open("wubi_gb18030.bin", O_RDONLY);
#else
		wubi_fd = open("/usr/local/wubi_gb18030.bin", O_RDONLY);
#endif
	}

	lseek(wubi_fd, pIndex->address + WUBI_OFFSET, SEEK_SET);

	read(wubi_fd, pDes, pIndex->size);

	return SUCCESS;
}

UINT8 Read_Wubi_Index(UINT32 offset, struct WuBi_Index *pIndex)
{
	UINT8 size;

	if (wubi_fd == 0)
	{
#ifdef WIN32
		wubi_fd = open("wubi_gb18030.bin", O_RDONLY);
#else
		wubi_fd = open("/usr/local/wubi_gb18030.bin", O_RDONLY);
#endif	
	}
	lseek(wubi_fd, offset, SEEK_SET);

	read(wubi_fd, pIndex->code, 4);

	read(wubi_fd, &size, 1);
	pIndex->size = size;

	read(wubi_fd, &pIndex->address, 4);

//	printf("pIndex->address = %x\n", pIndex->address);

	return SUCCESS;

}


INT8 *Wubi_Result(UINT8 index)
{
	if (index >= WUBI_BUFFER_SIZE)
	{
		return NULL;
	}

	return wubi_buffer[index];
}








