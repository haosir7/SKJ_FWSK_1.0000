/*! \file    commonFunc.c
   \brief    通用函数的实现文件
   \author   zl
 */
#include <time.h>
#include "commonFunc.h"

#if (USE_ICONV_LIB != 0)
#include "iconv.h"
#endif

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"



#ifdef WIN32
#define MAX_RESULT_NUMBER  64                        /*大写金额缓存长度 */
#define MAX_AMOUNT  99999999999999UL                   /*金额的最大值     */
#else
#define MAX_RESULT_NUMBER  64                        /*大写金额缓存长度 */
#define MAX_AMOUNT  99999999999999ULL                   /*金额的最大值     */
#endif


static char AmountResultBuf[MAX_RESULT_NUMBER];
static const char Unit[] = "分角圆拾佰仟万拾佰仟亿拾佰仟零";
static const char Digit[] = "零壹贰叁肆伍陆柒捌玖";


char *ChineseAmount( INT64 Amount, char *outBuff,UINT8 outBuffLen)
{

	INT8 IsNegative = 0;
	INT8 nPos = 0;   //在这里是金额宽度

	char *pChar;
	char *tmp;
	int j = 0;
    INT8 Number[ 16 ];
	INT64 yuan =0; 
	int i = 0;
    char cStr[3];

	if(outBuffLen < MAX_RESULT_NUMBER )
	{
		prn_log2("retLen less than %d",MAX_RESULT_NUMBER);
		return NULL;
	}

	if( Amount < 0 ) 
	{
		Amount = -Amount;
		IsNegative = 1;
	}

    yuan=Amount;
    if(yuan > MAX_AMOUNT)
	{
       	prn_log("amount is large than MAX_AMOUNT ");
		return NULL;
	}
	

    while( yuan >= 1 )
	{
		Number[i] = (INT8)(yuan % 10);
		yuan /= 10;
		i++;
    }

    nPos=i;
	memset((void *)AmountResultBuf, 0, MAX_RESULT_NUMBER);
    memset((void *)outBuff, 0, MAX_RESULT_NUMBER);
 
    for( i = nPos-1; i >= 0; i--)
	{
		AmountResultBuf[j++] = Digit[Number[i] * 2];
		AmountResultBuf[j++] = Digit[Number[i] * 2 + 1];
		AmountResultBuf[j++] = Unit[i * 2];
		AmountResultBuf[j++] = Unit[i * 2 + 1];
	}    

    while ( 1 )
	{
		pChar = strstr(AmountResultBuf, "零" );

		if ( NULL == pChar )
		{
			strcat(outBuff, AmountResultBuf);
			break;
		}

		cStr[0] = pChar[2];
		cStr[1] = pChar[3];
		cStr[2] = 0x0;
		
      //处理"角分"都为"零"时在末尾角"整"
		if (strlen(pChar) == 2)
		{
			strcpy(pChar, "整");
		}
      //在"亿万圆"单位上存在"零"时将"零"去掉并保留单位并在单位后加"零"
		else 
			if (strstr("亿万圆", cStr) != NULL)
			{
				pChar += 2;
				while (*pChar != 0x0)
				{
					*(pChar - 2) = *pChar;
					pChar++;
				}
				*(pChar -2) = 0x0;
			}
	
      //处理在非"亿万圆"单位上存在"零"时去掉"零"及单位,而且还包括在"亿万圆"
      //单位上是"零",而且下一个单位上存在"零"的情况,去掉后一个"零"及单位
			else 
				if (strstr(Unit, cStr) != NULL)
				{
					pChar += 4;
					while (*pChar != 0x0)
					{
						*(pChar - 2) = *pChar;
						pChar++;
					}
					*(pChar - 2) = 0x0;

				}
      //处理在"亿万圆"单位后的"零"后加数字的情况,并且"零"后的子串拆为新的
      //字符串来继续处理
				else
				{
					tmp = AmountResultBuf;
					pChar += 2;
					j = strlen(outBuff);
					while (tmp < pChar)
						outBuff[j++] = *tmp++;
					outBuff[j] = 0x0;

					j = 0;
					while (*pChar != 0x0)
					{
						AmountResultBuf[j++] = *pChar++;
					}
					AmountResultBuf[j] = 0x0;
				}
	}

	pChar = strstr(outBuff, "亿万");
	if (pChar != NULL)
	{
		pChar += 4;
		while (*pChar != 0x0)
		{
			*(pChar - 2) = *pChar;
			pChar++;
		}
		*(pChar -2) = 0x0;
	}
	
	if( IsNegative )
	{
		strcpy(AmountResultBuf, "(负数)");
		strcat(AmountResultBuf, outBuff);
		strcpy(outBuff, AmountResultBuf);
	}
  
	return outBuff;
}


void CommonSleep(int ms)
{
	struct timespec req, rem;
	int res = 0;

	req.tv_sec	= ms/1000;
	req.tv_nsec	= (ms % 1000) * 1000000;

	res = nanosleep(&req, &rem);
	while (res == -1)
	{
		req.tv_sec	= rem.tv_sec;
		req.tv_nsec	= rem.tv_nsec;
		res = nanosleep(&req, &rem);
	}
}


#if (USE_ICONV_LIB != 0)

static char m_convert_buf[ICONV_CONVERT_BUF_LEN];

int code_convert(char *from_charset, char *to_charset, 
				 const char *inStr, int inlen, char **outStr)
{
	iconv_t cd;
	int rc;
	size_t t_inlen, t_outlen;

	const char *pin = inStr;
	char *pout = m_convert_buf;
	memset(m_convert_buf, 0, ICONV_CONVERT_BUF_LEN);

	cd = iconv_open(to_charset, from_charset);
	if (cd==0)
	{
		return -1;
	}

	t_inlen = inlen;
	t_outlen = ICONV_CONVERT_BUF_LEN;

	if (iconv(cd, (const char **)&pin, &t_inlen, &pout, &t_outlen)==-1) //调用之后pout指针会向后偏移
	{
		return -2;
	}

	iconv_close(cd);

	*outStr = m_convert_buf;
	return 0;				
}

#endif	//USE_ICONV_LIB
