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
#include "phrase.h"

#define PHRASE_HEAD_LEN		16
static UINT8 Read_Phrase(INT8 *pDes, struct Phrase_Index *pIndex);
static UINT8 Phrase_Search(UINT16 enter_strokes, INT32 *offset);
static UINT8 Read_Phrase_Index(UINT32 offset, struct Phrase_Index *pIndex);

static INT32 phrase_fd = 0;
static INT8 phrase_buffer[PHRASE_BUFFER_SIZE][PHRASE_BUFFER_LEN];

static int s_Max_Phrase_Size = 0;
static int s_Phrase_Offset = 0;

UINT8 Phrase_Strokes_enter(INT8 *enter_strokes, UINT8 *OutSize)
{
	struct Phrase_Index Index;
	UINT16 enter;
	INT32 begin = 0;
	INT32 end = 0;
	INT32 current = 0;
	INT32 i;

	if (phrase_fd == 0)
	{
#ifdef WIN32
		phrase_fd = open("phrase.bin", O_RDONLY);
#else
		phrase_fd = open("/usr/ziku/phrase.bin", O_RDONLY);
#endif	
		if (phrase_fd > 0)
		{
			char buff[16];
			char *p;

			read(phrase_fd, buff, 16);

			p = (char *)&s_Max_Phrase_Size;
			memcpy(p, buff, 4);

			p = (char *)&s_Phrase_Offset;
			memcpy(p, &buff[4], 4);
		}
	}

	if (strlen(enter_strokes) != 2)
	{
		return FAILURE;
	}

	memcpy(&enter, enter_strokes, 2);
	
	if (Phrase_Search(enter, &current) == SUCCESS)
	{
		memset(phrase_buffer, 0, PHRASE_BUFFER_SIZE*PHRASE_BUFFER_LEN);

		begin = current;
		while (begin >= 1)
		{
			begin--;
			Read_Phrase_Index(begin * 7, &Index);
		
			if (enter != Index.code)
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
		while (end < s_Max_Phrase_Size - 1)
		{
			end++;
			Read_Phrase_Index(end * 7, &Index);
		
			if (enter != Index.code)
			{
				end--;
				break;
			}
			if (end >= s_Max_Phrase_Size - 1)
			{
				break;
			}

			if ((end - begin) >= PHRASE_BUFFER_SIZE)
			{
				break;
			}
		}

		if ((end - begin) >= PHRASE_BUFFER_SIZE)
		{
			end = begin + PHRASE_BUFFER_SIZE - 1;
		}

		*OutSize = end - begin + 1;
		for(i=begin; i<=end; i++)
		{	
			Read_Phrase_Index(i * 7, &Index);
			Read_Phrase(phrase_buffer[i-begin], &Index);
		}

		return SUCCESS;
	}
	
	*OutSize = 0;
	return FAILURE;
}

UINT8 Phrase_Search(UINT16 enter_strokes, INT32 *offset)
{
	INT32 low = 0;
	INT32 high = s_Max_Phrase_Size - 1;
	INT32 mid;
	INT32 cmp;
	struct Phrase_Index Index;
	struct Phrase_Index *pIndex = &Index;
	UINT8 found = FAILURE;

	while ((low <= high) && (found == FAILURE))
	{
		mid = (low + high) / 2;
		
		Read_Phrase_Index(mid * 7, pIndex);

		cmp = enter_strokes - pIndex->code;

//		printf("enter_strokes = %s, pIndex->code = %s, cmp = %d\n",enter_strokes,pIndex->code, cmp);

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


static UINT8 Read_Phrase(INT8 *pDes, struct Phrase_Index *pIndex)
{
	lseek(phrase_fd, pIndex->address + s_Phrase_Offset, SEEK_SET);

	read(phrase_fd, pDes, pIndex->size);

	return SUCCESS;
}

static UINT8 Read_Phrase_Index(UINT32 offset, struct Phrase_Index *pIndex)
{
	UINT8 size;
//	static int phrase_fd;
	
	lseek(phrase_fd, offset+PHRASE_HEAD_LEN, SEEK_SET);

	read(phrase_fd, &pIndex->code, 2);
	read(phrase_fd, &size, 1);

	read(phrase_fd, &pIndex->address, 4);

	
	pIndex->size = size;

//	printf("pIndex->address = %x\n", pIndex->address);

	return SUCCESS;

}


INT8 *Phrase_Result(UINT8 index)
{
	if (index >= PHRASE_BUFFER_SIZE)
	{
		return NULL;
	}

	return phrase_buffer[index];
}








