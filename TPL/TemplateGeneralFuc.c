/*! \file    TemplateGeneralFuc.c
\brief    打印模块打印平台无关性通用函数实现
\author   Xiao Pengkun   2005-07-20
 */

//#define POS_DEBUG_TO_TTY

#include "TemplateGeneralFuc.h"
#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


UINT8 print_template_buf[PRINT_TEMPLATE_BUF_LEN];

UINT8 template_line_buf[TEMPLATE_LINE_BUF_LEN];

UINT8 print_template_line_field_buf[PRINT_TEMPLATE_LINE_FIELD_BUF_LEN];

UINT8 match_field_label_buf[MATCH_FIELD_LABEL_BUF_LEN];

//----------------------------------------------------
// 以下各全局变量和函数为临时定义，为了使模板部分可以
// 正常使用
// 
//----------------------------------------------------
//商品取消标志，应用于不定长销售中，打印取消商品行需要
UINT8 DetailCancelFlag = 0;
UINT8 g_cungen = 0;

UINT8 is_gb_4byte(unsigned char *str)
{
    if ( (str[0] >= 0x81) && (str[0] <= 0xfe) )
	{
		if ( (str[1] >= 0x40) && (str[1] <= 0xfe) )
		{
			return 1;
		}
		else if ( (str[1] >= 0x30) && (str[1] <= 0x39) ) 
		{
			if ( (str[2] >= 0x81 && str[2] <= 0xFE) &&
			 	 (str[3] >= 0x30 && str[3] <= 0x39) )
			{
				return 2;
			}
			else	//汉字输入错误
			{
				return 1;
			}
		}
    }
    return 0;
}

void HZ_Count_Err()
{
	return;
}

UINT32 HZCount(UINT8 *str, UINT32 len, 
			   UINT8 *num_asc, UINT8 *num_hz2B, UINT8 *num_hz4B)
{
	unsigned int i, disp_len=0;
	
	*num_asc = 0;
	*num_hz2B = 0;
	*num_hz4B = 0;
	
	prn_log3("str: %s; len = %u", str, len);
	prn_log5("%x%x%x%x", str[0], str[1], str[2], str[3]);
	for(i=0; i<len; )
	{
		if ( str[i] <=0x80 )
		{
			prn_log3("i = %u; it's a ascii char: %x", i, str[i]);
			(*num_asc)++;
			i++;
			disp_len++;
		}
		else if ( str[i] <=0xfe )
		{
			if ( (str[i+1] >= 0x40) && (str[i+1] <= 0xfe) )
			{
				prn_log4("i = %u; it's a 2B ch: %x%x", i, str[i], str[i+1]);
				(*num_hz2B)++;
				i += 2;
				disp_len += 2;
			}
			else if ( (str[i+1] >= 0x30) && (str[i+1] <= 0x39) )
			{
				if ( (str[i+2] >= 0x81 && str[i+2] <= 0xFE) &&
				 	 (str[i+3] >= 0x30 && str[i+3] <= 0x39) )
				{
					prn_log4("i = %u; it's a 4B ch: %x%x", i, str[i], str[i+1]);
					(*num_hz4B)++;
					i += 4;
					disp_len += 2;
				}
				else
				{
					prn_log("error3");
					HZ_Count_Err();
//					i += 4;
//					LCDDisplayLine_L(0,0,"汉字识别错误",sizeof("汉字识别错误"));
				}
			}
			else
			{
				prn_log("error2");
				HZ_Count_Err();
//				i += 2;
//				LCDDisplayLine_L(0,0,"汉字识别错误",sizeof("汉字识别错误"));
			}
		}
		else
		{
			prn_log("error1");
			HZ_Count_Err();
//			i++;
//			LCDDisplayLine_L(0,0,"汉字识别错误",sizeof("汉字识别错误"));
		}
	}
		
	return disp_len;
}
//----------------------------------------------------


UINT32 GetHalfHZCount(UINT8 *str,UINT32 len)
{
	int c=0;
	int p=0;
	while (p < len)
	{
		if ((str[p] >= 0x81) && (str[p] <= 0xfe))
		{
			c++;
			p++;
			if (p < len)
			{
				if ((str[p] >= 0x40) && (str[p] <= 0xfe))
				{
					c++;
					p++;
				}
			}
		}
		else
		{
			p++;
		}
	}
	
	return c;

	
}
void InitPrinLineInfo(TPrnLineInfo *prnLineInfo)
{
   prn_log("start InitPrinLineInfo ");
	memset((void*)prnLineInfo,0,sizeof(TPrnLineInfo));
	memset((void *)prnLineInfo->chContent,' ',PRINT_LINE_BUF_LEN-1);
	prnLineInfo->chContent[PRINT_LINE_BUF_LEN-1] = '\0';
	prnLineInfo->chContent[PRINT_LINE_BUF_LEN] = '\0';
   prn_log("end InitPrinLineInfo ");
}

void ScanReplaceSpace(TPrnLineInfo *pPrnLineInfo)
{
	UINT8 i = 0;
        prn_log("start ScanReplaceSpace ");
	while ((i<PRINT_LINE_BUF_LEN-1))
	{
		if (pPrnLineInfo->chContent[i] == '\0')
		{
			pPrnLineInfo->chContent[i] = ' ';
		}
		i++;
	}
        prn_log("end ScanReplaceSpace ");
	
}


void GetPrintTemplateBuffer(void **p)
{
  prn_log(" start GetPrintTemplateBuffer");
	memset((void *)print_template_buf,0,PRINT_TEMPLATE_BUF_LEN);
	*p = (void *) print_template_buf;
  prn_log(" end GetPrintTemplateBuffer");
	
}

void GetTemplateLineBuffer(void **p)
{
  prn_log(" start GetTemplateLineBuffer");
	memset((void *)template_line_buf,0,TEMPLATE_LINE_BUF_LEN);
   	*p = (void *)template_line_buf;	
  prn_log(" end GetTemplateLineBuffer");
	
}
void GetTemplateLineFieldBuffer(void **p)
{
  prn_log(" start GetTemplateLineFieldBuffer");
	memset((void *)print_template_line_field_buf,0,PRINT_TEMPLATE_LINE_FIELD_BUF_LEN);
	*p = (void *)print_template_line_field_buf;
  prn_log(" end GetTemplateLineFieldBuffer");
	
}
void GetMatchFieldLabelBuffer(void ** p)
{
  prn_log(" start GetMatchFieldLabelBuffer");
	memset((void *)match_field_label_buf,0,MATCH_FIELD_LABEL_BUF_LEN);
	*p = (void *)match_field_label_buf;
  prn_log(" end GetMatchFieldLabelBuffer");
}
