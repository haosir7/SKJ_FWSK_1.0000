/*! \file    TemplateInterface.c
\brief    模板接口文件
\author   Xiao Pengkun   2005-06-23
 */

#include "TemplateInterface.h"
#include "TemplateGeneralFuc.h"
#include "TemplateDataDesign.h"
#include "TemplateMacro.h"
#include <string.h>
#include <stdlib.h>
#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


extern UINT8 DetailCancelFlag;
//extern void PrintHexData(UINT8 *data, UINT32 len);
extern INT8 templateNo;
extern TSaveTemplateInfo *pSaveTemplateInfo;

//电子存根和续写行的标志、位置
extern UINT8 stubFlag;
extern UINT8 stubPlace;
extern UINT8 continueWriteFlag;
extern UINT8 continueWriteLocation;
extern UINT8 g_cungen;
//标记循环模板行
extern UINT8 CycleTempFlag;
//标记下一行为循环模板行
extern UINT8 BeginCycleTemp;

//打印右对齐所需中间缓冲区
static INT8  saveRightBuf[32] ;


//存储收款员折行内容
static INT8    saveSkyContent[10];
UINT32 saveRealLocation = 0;//记录折行打印位置
UINT8 SKYContinueFlag = 0;//收款员折行标志

UINT8 CorpNameCounte = 0;//记录企业名称需要打的行数
//UINT8 CorpAddrCounte = 0;//记录企业名称需要打的行数
UINT8 *startPointer = NULL;
UINT8 *endPointer = NULL;

UINT8  GetPtLine(UINT8 **prnTempBuf, UINT8 *tempLineBuf)
{	
    UINT32 len=0;      //模板行的长度，不包括回车;

	UINT8 *pSaveInit = NULL;//每一次提取新的模板行数据时模板缓冲区的起始位置;
	
    //prn_log(" start GetPtLine ");
	if (!(*prnTempBuf) || (!tempLineBuf))
	{
		return 1;
	}	
	
	pSaveInit = *prnTempBuf;
	
	
	//TEMPLATE_LINE_END_CHARACTER为每一模板行的结束符'\n'
	while ( ( *(*prnTempBuf) != TEMPLATE_LINE_END_CHARACTER)
	            && ( *(*prnTempBuf) != '\0' ))
	{
	    if(len >= TEMPLATE_LINE_BUF_LEN)
	    {
	    	return 2;
	    }
		(*prnTempBuf)++;
		
		len++;
	}
	(*prnTempBuf)++;
	
	//len+1：将回车换行符拷贝到tempLineBuf
	len++;
	memcpy(tempLineBuf,pSaveInit,len);
    //prn_log(" end GetPtLine ");
    return 0;
}


UINT8  GetPtField(UINT8 **tempLineBuf,UINT8 *tempLineFieldBuf)
{
	UINT8 *pHead = *tempLineBuf;
	UINT32  len = 0;
	
	if (!(*tempLineBuf) || (!tempLineFieldBuf))
	{
		return 1;
	}
	
	//将模板行缓存中的属性(两个~之间的数据)提取出来，memcpy到tempLineFieldBuf
	
	
	//TEMPLATE_ATTRIBUTE_SEPERATE_CHARACTER为字符'~'
	while ((*(*tempLineBuf) != TEMPLATE_ATTRIBUTE_SEPERATE_CHARACTER)
	          && (*(*tempLineBuf) != '\0' ) )
	{
	    if(len >= PRINT_TEMPLATE_LINE_FIELD_BUF_LEN)
	    {	
	    	return 2;
	    	
	    }
		(*tempLineBuf)++;
		len++;
	}
	(*tempLineBuf)++;
	memcpy(tempLineFieldBuf,pHead,len);
	
	return 0;
	
}



UINT8  ParsePtLine0(UINT8 *tempLineBuf, TPrnTempLine0 *tempLine0)
{
	UINT8 *pMatchTemplateBuffer=NULL;     
	UINT8 *tempLineFieldBuf=NULL;
	UINT8 *pSaveMatchTemplateBuffer=NULL;
	UINT8 *pSavetempLineFieldBuffer=NULL;
	
	
	UINT32 lengthOfField=0;	
        UINT32 l=0;
	UINT8 *pTempLineField = NULL;//保存模板行两个~之间属性的起始位置
        //prn_log("start ParsePtLine0");
	
	if ((!tempLineBuf) || (!tempLine0))
	{
		return 1;
	}
	
	//获取模板行属性缓存，即存储两个~之间的数据
	GetTemplateLineFieldBuffer((void **)&tempLineFieldBuf);
    pSavetempLineFieldBuffer = tempLineFieldBuf;
	
	//获取匹配模板属性缓存，即两个~之间数据#前的值
	GetMatchFieldLabelBuffer((void **)&pMatchTemplateBuffer);
    pSaveMatchTemplateBuffer = pMatchTemplateBuffer;
	
	
	//略去0~
	tempLineBuf+=2;
	


	/*循环处理，直到模板行结束，获取模板行属性GetPtField，通过
	  匹配模板属性，完成相应值的赋值*/
	
	while ( (*tempLineBuf != TEMPLATE_LINE_END_CHARACTER) && 
	         (*tempLineBuf != '\0')  )
	{
		
		if( GetPtField(&tempLineBuf,tempLineFieldBuf) != 0 )
        {
             return 2; 
        }		
		
		lengthOfField = 0;
		pTempLineField = tempLineFieldBuf;
		while ((*tempLineFieldBuf != ATTIBUTE_SEPERATE_FIELD) && 
		         (*tempLineFieldBuf != '\0' ))
		{
			if(lengthOfField >= MATCH_FIELD_LABEL_BUF_LEN)
			{
			    return 2;
			}
			tempLineFieldBuf++;
			lengthOfField++;
		}

		memcpy(pMatchTemplateBuffer,pTempLineField,lengthOfField);
		
		//判断是否为TEMP_Type
		if(memcmp(pMatchTemplateBuffer,"TEMP_TYPE",lengthOfField) == 0)
			
		{
			tempLine0->TempType = atoi((INT8 *)++pTempLineField);
					
		}
        //判断是否为VER
		else if(memcmp(pMatchTemplateBuffer,"VER",lengthOfField) == 0)
		{			
                        l=strlen((INT8 *)++tempLineFieldBuf);
                        l= l > PT_VER_LEN ? PT_VER_LEN : l ; 
                        memcpy((void *)tempLine0->chVer,(void *)++tempLineFieldBuf,l);
		}
		//判断是否为DIS
		else if(memcmp(pMatchTemplateBuffer,"DIS",lengthOfField) == 0)
		{
                        l=strlen((INT8 *)++tempLineFieldBuf);
                        l= l > PT_DIS_LEN ? PT_DIS_LEN : l ; 
                        memcpy((void *)tempLine0->chDis,(void *)++tempLineFieldBuf,l);
		}
		//判断是否为DATE
		else if(memcmp(pMatchTemplateBuffer,"DATE",lengthOfField) == 0)
		{
                        l=strlen((INT8 *)++tempLineFieldBuf);
                        l= l > PT_DATE_LEN ? PT_DATE_LEN : l ; 
                        memcpy((void *)tempLine0->chDate,(void *)++tempLineFieldBuf,l);
		}
/*
		else
		{
			return 0;
		}
*/				
		
		//memset使用的两个缓冲区		
		pMatchTemplateBuffer = pSaveMatchTemplateBuffer;		
		memset(pMatchTemplateBuffer,0,MATCH_FIELD_LABEL_BUF_LEN);
		
		tempLineFieldBuf = pSavetempLineFieldBuffer;
		memset(tempLineFieldBuf,0,PRINT_TEMPLATE_LINE_FIELD_BUF_LEN);
		
	}//end of while
        //prn_log("end ParsePtLine0");
	return 0;
	
}


UINT8  ParsePtLine1(UINT8 *tempLineBuf, TPrnTempLine1 *tempLine1)
{
	UINT8 *pMatchTemplateBuffer=NULL;
	UINT8 *tempLineFieldBuf=NULL;
	UINT8 *pSaveMatchTemplateBuffer=NULL;
	UINT8 *pSavetempLineFieldBuffer=NULL;
	
	
	UINT32 lengthOfField=0;
	UINT8 *pTempLineField=NULL;
	
	// 标记模板行中是否有黑标这一项
	UINT32 MarkInTemplate = 0;
        //prn_log("start ParsePtLine1");	
	if ((!tempLineBuf) || (!tempLine1))
	{	       
            return 1;
	}
	
	
	GetMatchFieldLabelBuffer((void **)&pMatchTemplateBuffer);
	pSaveMatchTemplateBuffer = pMatchTemplateBuffer;
	
	GetTemplateLineFieldBuffer((void **)&tempLineFieldBuf);
	pSavetempLineFieldBuffer = tempLineFieldBuf;
	
	tempLineBuf+=2;

	tempLine1->EndBackwardFlag = 0;	//默认为0，防止有些旧模板没有该参数
	
	while ( (*tempLineBuf != TEMPLATE_LINE_END_CHARACTER) && 
	         (*tempLineBuf != '\0' )) 
	{		
		
		if(GetPtField(&tempLineBuf,tempLineFieldBuf) != 0 )
	    {
		   return 2;
		}
		
		lengthOfField = 0;
		pTempLineField = tempLineFieldBuf;
		while ((*tempLineFieldBuf != ATTIBUTE_SEPERATE_FIELD) && 
		      (*tempLineFieldBuf != '\0'))
		{
			if(lengthOfField >= MATCH_FIELD_LABEL_BUF_LEN)
			{
			   return 2;
			}
			tempLineFieldBuf++;
			lengthOfField++;
		}
		memcpy(pMatchTemplateBuffer,pTempLineField,lengthOfField);
		
		//对每一个field进行处理
		
		if(memcmp(pMatchTemplateBuffer,"TYPE",lengthOfField) == 0)
		   {	
				INT8 *p;
				tempLine1->PaperL = atoi((INT8 *)++tempLineFieldBuf);
				p = strchr((INT8 *)tempLineFieldBuf,'#');
				tempLine1->PaperW = atoi(++p);
				p = NULL;			
			
		    }
		else if(memcmp(pMatchTemplateBuffer,"FONT",lengthOfField) == 0)
			{	
				INT8 *p;
				tempLine1->FontH = atoi((INT8 *)++tempLineFieldBuf);
				p = strchr((INT8 *)tempLineFieldBuf,'#');				
				tempLine1->FontW = atoi(++p);
				p = NULL;				
			}
        else if(memcmp(pMatchTemplateBuffer,"MARK",lengthOfField) == 0)
			{
			    //如果模板行中有黑标的标签，则将MarkInTemplate置1
				tempLine1->MarkFlag = atoi((INT8 *)++tempLineFieldBuf);
				MarkInTemplate = 1;
					
			}
	   else	if (memcmp(pMatchTemplateBuffer,"BEGIN_FORWARDPOINT",lengthOfField) == 0)
			{
				tempLine1->BeginForwardPoint = atoi((INT8 *)++tempLineFieldBuf);
			}
	   else	if(memcmp(pMatchTemplateBuffer,"MLNS",lengthOfField) == 0)
			{
				tempLine1->MaxInvLineCount = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else	if(memcmp(pMatchTemplateBuffer,"REPAIR",lengthOfField) == 0)
			{
				tempLine1->RepairPoint = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else	if(memcmp(pMatchTemplateBuffer,"FKDW_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxFKDWCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else	if(memcmp(pMatchTemplateBuffer,"SKDW_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxSKDWCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
	   else	if(memcmp(pMatchTemplateBuffer,"SKDWDZ_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxSKDWDZCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else	if(memcmp(pMatchTemplateBuffer,"SKY_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxSKYCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else	if(memcmp(pMatchTemplateBuffer,"SPMC_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxSPMCCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
			
	   else if (memcmp(pMatchTemplateBuffer,"END_FORWARDPOINT",lengthOfField) == 0)
		    {
				tempLine1->EndForwardPoint = atoi((INT8 *)++tempLineFieldBuf);
			}
	 
	   else if (memcmp(pMatchTemplateBuffer,"END_BACKWARDFLAG",lengthOfField) == 0)
		   {
			   tempLine1->EndBackwardFlag = atoi((INT8 *)++tempLineFieldBuf);
		   }
	   
	   else if (memcmp(pMatchTemplateBuffer,"BACKWARD_POINT",lengthOfField) == 0)
	        {		
	        	tempLine1->BackwardPoint  = atoi((INT8 *)++tempLineFieldBuf);
	        }
	  
	    else if (memcmp(pMatchTemplateBuffer,"UNFIXED_FORWORD",lengthOfField) == 0)
	        {		
	        	tempLine1->UnfixedForwardPoint  = atoi((INT8 *)++tempLineFieldBuf);
	        } 
	   else if (memcmp(pMatchTemplateBuffer,"JEDX_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxJEDXCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
	   else if (memcmp(pMatchTemplateBuffer,"MAX_GOODS_COUNT",lengthOfField) == 0)
	   		{
	   			tempLine1->MaxGoodsCount = atoi((INT8 *)++tempLineFieldBuf);
	  		}
	   else if(memcmp(pMatchTemplateBuffer,"BZ_MAX",lengthOfField) == 0)
			{
				tempLine1->MaxBZCharacter = atoi((INT8 *)++tempLineFieldBuf);
			}
	   else if(memcmp(pMatchTemplateBuffer,"QR_CODE_LEFT_MARGIN",lengthOfField) == 0)
	   {
		   tempLine1->QRCodeLeftMargin = atoi((INT8 *)++tempLineFieldBuf);
	   }
	   /**********for 57mm***********/
	   else if (memcmp(pMatchTemplateBuffer,"INV_76_MM",lengthOfField) == 0)
	   {
		   tempLine1->InvFlag76 = atoi((INT8 *)++tempLineFieldBuf);
	   }
	   /**********for 57mm***********/
	   /*		
	   else
	       {
	    	 return 0;
	       }	
		*/
		
		
		pMatchTemplateBuffer = pSaveMatchTemplateBuffer;		
		memset(pMatchTemplateBuffer,0,MATCH_FIELD_LABEL_BUF_LEN);
		
		tempLineFieldBuf = pSavetempLineFieldBuffer;
		memset(tempLineFieldBuf,0,PRINT_TEMPLATE_LINE_FIELD_BUF_LEN);
		
		
	}//end of while
	
	//如果模板行中无黑标标签，则默认为不找黑标，将模板行1的MarkFlag设为0
	if(MarkInTemplate == 0)
	{
		tempLine1->MarkFlag = 0;
	}

	return 0;
}


UINT8  ParsePtLine2(UINT8 *tempLineBuf, TPrnTempRow *tempLine2,TPrnLineInfo *prnLineInfo,TPrnTempLine1 *tempLine1)
{

	UINT8 *pMatchTemplateBuffer = NULL;
	UINT8 *tempLineFieldBuf = NULL;
	UINT8 *pSaveMatchTemplateBuffer = NULL;
	UINT8 *pSavetempLineFieldBuffer = NULL;
	
	UINT32 nCount = 0;//记录标签和内容的数目
	UINT32 lengthOfField = 0;
	UINT8 *pTempLineField = NULL;
	
	INT8 *pLB = NULL;
	INT8 *pCO = NULL;
	
	//记录写入模板数据后的chDatabuf缓存的位置（下一个模板数据开始写入的位置）；
	INT8 *pSaveDataBuf = NULL;
//    prn_log("start ParsePtLine2 ");	
	
	if((!tempLineBuf) || (!tempLine2) )
	{
           return 1;
	}
	
	GetMatchFieldLabelBuffer((void **)&pMatchTemplateBuffer);
	pSaveMatchTemplateBuffer = pMatchTemplateBuffer;
	
	GetTemplateLineFieldBuffer((void **)&tempLineFieldBuf);
	pSavetempLineFieldBuffer = tempLineFieldBuf;
	
	tempLineBuf+=2;
	
	//将打印行字体属性设为默认属性，解析模板行1所得；
	prnLineInfo->FontH = tempLine1->FontH;
	prnLineInfo->FontW = tempLine1->FontW;
	
	//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
	
	
	while ((*tempLineBuf != TEMPLATE_LINE_END_CHARACTER) &&
	      (*tempLineBuf != '\0') )
	{
		
	
		if( GetPtField(&tempLineBuf,tempLineFieldBuf) != 0 )
        {
            return 2;
        }
		
		lengthOfField = 0;
		pTempLineField = tempLineFieldBuf;
		while ( (*tempLineFieldBuf != ATTIBUTE_SEPERATE_FIELD) && 
		        (*tempLineFieldBuf != '\0' ))
		{
			if(lengthOfField >= MATCH_FIELD_LABEL_BUF_LEN)
			{
			    return 2;
			}
			tempLineFieldBuf++;
			lengthOfField++;
		}
		memcpy(pMatchTemplateBuffer,pTempLineField,lengthOfField);
		
		//对每一个field进行处理
		
		
		//prn_log2("pMatchTemplateBuff = %s",pMatchTemplateBuffer);
		//prn_log2("lengthOfField = %u",lengthOfField);
		if(memcmp(pMatchTemplateBuffer,"LN",lengthOfField) == 0)
			
		{	
			tempLine2->Index = atoi((INT8 *)++tempLineFieldBuf);
			
		}
		
		if(memcmp(pMatchTemplateBuffer,"HEAD",lengthOfField) == 0)
			
		{			     
			tempLine2->Type = 0;
			prnLineInfo->Property = tempLine2->Type;
			
		}

		if(memcmp(pMatchTemplateBuffer,"DATACYCLE",lengthOfField) == 0)
			
		{			     
			tempLine2->Type = 0;
			prnLineInfo->Property = tempLine2->Type;
			CycleTempFlag = 1;
			
			
		}
		if(memcmp(pMatchTemplateBuffer,"START_TAIL",lengthOfField) == 0)
			
		{			     
			tempLine2->Type = END_INVOICE_DATA_LINE;
			prnLineInfo->Property = tempLine2->Type;
			CycleTempFlag = 0;
			
		}
		
		if(memcmp(pMatchTemplateBuffer,"TAIL",lengthOfField) == 0)
			
		{			     
			tempLine2->Type = 0;
			prnLineInfo->Property = tempLine2->Type;
			
		}
		/*--------标记下一行为循环模板行-------------------------------*/		
		if(memcmp(pMatchTemplateBuffer,"CYCLE_BEGIN",lengthOfField) == 0)
			
		{	 prn_log("here  !!!");
		    tempLine2->Type = END_INVOICE_HEAD_LINE;
			prnLineInfo->Property = tempLine2->Type;		     
			BeginCycleTemp = 1;
									
		}
		
		//模板行的字体设置，如果模板行中有此标签，则将解析出的字体属性赋予打印行结构体
		if(memcmp(pMatchTemplateBuffer,"FONT",lengthOfField) == 0)
		{	
			INT8 *p;
			tempLine2->FontH = atoi((INT8 *)++tempLineFieldBuf);
			p = strchr((INT8 *)tempLineFieldBuf,'#');				
			tempLine2->FontW = atoi(++p);
			p = NULL;
			prnLineInfo->FontH = tempLine2->FontH;
			prnLineInfo->FontW = tempLine2->FontW;	
		
		}		
	
		if(memcmp(pMatchTemplateBuffer,"ROWH",lengthOfField) == 0)
		{
			tempLine2->High = atoi((INT8 *)++tempLineFieldBuf);
			prnLineInfo->High = tempLine2->High;
			
		}
		if(memcmp(pMatchTemplateBuffer,"ROWBH",lengthOfField) == 0)
		{
			INT8 *p;
			tempLine2->BhighOne = atoi((INT8 *)++tempLineFieldBuf);
			p = strchr((INT8 *)tempLineFieldBuf,'#');	
			if (p == NULL)
			{
				//prn_log("p is NULL");
				tempLine2->BhighTwo = 0;
			}
			else
			{
				tempLine2->BhighTwo = atoi(++p);
				//prn_log2("tempLine2->BhighTwo = %u", tempLine2->BhighTwo);
			}
			p = NULL;
			prnLineInfo->Bhigh = tempLine2->BhighOne;
			//prnLineInfo->BhighTwo = tempLine2->BhighTwo;
			
		}
		
		if(memcmp(pMatchTemplateBuffer,"EMPTY_FORWARD",lengthOfField) == 0)
		{
			tempLine2->EmptyForwardHigh = atoi((INT8 *)++tempLineFieldBuf);
			prnLineInfo->EmptyForwardHigh = (INT16)tempLine2->EmptyForwardHigh;
			
		}
		
		if(memcmp(pMatchTemplateBuffer,"LB",lengthOfField) == 0)
			
		{	
		
			tempLineFieldBuf++;
			
            //每一个标签都加上是否打印的标志,0--打印，1--不打印
			if(*tempLineFieldBuf == '0' || g_cungen == 1)		     
			
			{	
                if(nCount >= PT_MAX_ITEM_COUNT) 
                {
                    return 3;
                }
				nCount++;
				tempLine2->TotalCount = (UINT8)nCount;
				
				
				if (nCount == 1)
				{
					memset((void *)tempLine2->chDatabuf,0,PRINT_LINE_BUF_LEN);
					
					tempLine2->Items[nCount-1].dataPtr = tempLine2->chDatabuf;
					
					pSaveDataBuf = tempLine2->chDatabuf;
				}
				else
				{
					tempLine2->Items[nCount-1].dataPtr = pSaveDataBuf;
					
				}
				
				//以如LB#0#发票代码：#2为例
								
				//当前指针指向'0',将指针指到'发'
				tempLineFieldBuf += 2;
				
				//pLB的值为指向'#'
				pLB = strchr((INT8 *)tempLineFieldBuf,'#');
				
				//DataLen的值为'发票代码：'的长度
				tempLine2->Items[nCount-1].DataLen = pLB-(INT8 *)tempLineFieldBuf;
				
				//将'发票代码：'拷贝到dataPtr所指向的缓冲区
				strncpy((INT8 *)tempLine2->Items[nCount-1].dataPtr,
					(INT8 *)tempLineFieldBuf,pLB-(INT8 *)tempLineFieldBuf);
					
				//Left为'发票代码：'在打印行中所处的位置
				tempLine2->Items[nCount-1].Left = atoi(++pLB);
				
				//表示属性为标签				
				tempLine2->Items[nCount-1].Type = 1;
				
				//将pSaveDataBuf指向'发票代码：'后的位置
				pSaveDataBuf +=  strlen((INT8 *)tempLineFieldBuf);
				
				pLB = NULL;					
			}
						
		}
		
		if(memcmp(pMatchTemplateBuffer,"CO",lengthOfField) == 0)
			
		{			
            if(nCount >= PT_MAX_ITEM_COUNT) 
            {
                return 3;
            }
			nCount++;
			tempLine2->TotalCount = (UINT8)nCount;
			if (nCount == 1)
			{
				memset((void *)tempLine2->chDatabuf,0,PRINT_LINE_BUF_LEN);
				tempLine2->Items[nCount-1].dataPtr = tempLine2->chDatabuf;
				pSaveDataBuf = tempLine2->chDatabuf;
			}
			else
			{
				tempLine2->Items[nCount-1].dataPtr = (INT8 *)pSaveDataBuf;				
			}						
			tempLineFieldBuf++;
			pCO = strchr((INT8 *)tempLineFieldBuf,'#');
			
			tempLine2->Items[nCount-1].DataLen = pCO-(INT8 *)tempLineFieldBuf;
			
			strncpy((INT8 *)tempLine2->Items[nCount-1].dataPtr,
				(INT8 *)tempLineFieldBuf,pCO-(INT8 *)tempLineFieldBuf);
			
			tempLine2->Items[nCount-1].Left = atoi(++pCO);
			tempLine2->Items[nCount-1].Type = 2;
			pSaveDataBuf += strlen((INT8 *)tempLineFieldBuf);
			pCO = NULL;		
		}
		
		pMatchTemplateBuffer = pSaveMatchTemplateBuffer;		
		memset(pMatchTemplateBuffer,0,MATCH_FIELD_LABEL_BUF_LEN);
		
		tempLineFieldBuf = pSavetempLineFieldBuffer;
		memset(tempLineFieldBuf,0,PRINT_TEMPLATE_LINE_FIELD_BUF_LEN);
		
	}//end of while
	pSaveDataBuf = NULL;
   // prn_log("end ParsePtLine2 ");	
	//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
		
	return 0;
}


static INT8 tmp[8];
UINT8 GetPrnLineData(TPrnTempRow *tempLine2, TPrnLineInfo *conWriteBuffer, TPrnTempLine1 *tempLine1,TPrnLineInfo *prnLineInfo,TPrnInvoiceInfo *pInv)
{	
	INT32 tmpLocation = 0;
    INT32 location = 0;//记录左起位置
	UINT32 len = 0;
	UINT32 i = 0; //记录模板行中的标签和内容的个数
	UINT8 j;
	UINT8 position = 0;
	UINT8 hzNum = 0;
	UINT8 gb2Num = 0;
	UINT8 gb4Num = 0;
	UINT8 ascNum = 0;
	UINT32 displen = 0;
	UINT32 displen2 =0;
	INT64 multiple;
	INT64 realAmount;

//	prn_log2("================pSaveTemplateInfo->workMode = %u==================", pSaveTemplateInfo->workMode);
	
	//当收款员折行,而付款单位按模板不折行,但打印时必须折行,要与收款员折行位置留有空隙)
	UINT32 saveLocation = 0;//记录位置
	
    //prn_log("start GetPrnLineData ");	
    //prn_log2("TotalCount=%u",tempLine2->TotalCount);	
	//解析Items中的内容,即每一行模板数据，包括标签和打印内容
    for ( i=0; i<tempLine2->TotalCount; i++)
    {
		
		//location为标签和内容打印的位置
		location = tempLine2->Items[i].Left;
        if( abs( location) >  PRINT_LINE_WORD_LEN )
        {
             return 3;
        }
	    
        //内容为LB ，对应的Type=1
		if (tempLine2->Items[i].Type == 1)
		{
			strncpy(&(prnLineInfo->chContent[location-1]),
				tempLine2->Items[i].dataPtr,
				tempLine2->Items[i].DataLen);
		}
		//内容为CO
		else
		{
            //prn_log2("dataPtr=%s",tempLine2->Items[i].dataPtr);
            //prn_log2("dataLen=%u",tempLine2->Items[i].DataLen);
			//发票代码
			if (memcmp(tempLine2->Items[i].dataPtr,"FPDM",
				tempLine2->Items[i].DataLen) == 0)
			{		
				prn_log("here  !!!");
				memcpy((void *)&(prnLineInfo->chContent[location-1]), 
					(void *)pInv->chTypeCode, INV_TYPE_CODE_PRN_LEN);
				
			}
			//发票号码 
            else if (memcmp(tempLine2->Items[i].dataPtr,"FPHM",
				tempLine2->Items[i].DataLen) == 0)
			{
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{
					sprintf((char *)&(prnLineInfo->chContent[location-1]),
						"%08lu",pInv->InvNo);
					//判断是否为打印电子存根
					
					//prn_log2("当前打印类型%u",pInv->PrintType);
//					if (pInv->PrintType == STUB_PRINT_TEMPLATE)
//					{
//						stubFlag = 1;
//						stubPlace = (UINT8)location ;					
//					}	
				}
				else
				{
					memcpy((void *)&(prnLineInfo->chContent[location-1]),"********",8);
					
				}
				
			} 
			 //原发票代码
			else if (memcmp(tempLine2->Items[i].dataPtr,"YFPDM",
				tempLine2->Items[i].DataLen) == 0)
			{	
				prn_log("here  !!!");
				if(pInv->InvType ==RETURN_INV_TEMPLATE)
				{
					prn_log("here  !!!");
					memcpy((void *)&(prnLineInfo->chContent[location-1]), 
						(void *)pInv->chPosCode, INV_TYPE_CODE_PRN_LEN);
				}
				else
				{
						prn_log("hereTHERE!!!");
					prnLineInfo->Property = INVALID_TEMPLATE_LINE;
				}
				
			}
			//原发票号码
            else if (memcmp(tempLine2->Items[i].dataPtr,"YFPHM",
				tempLine2->Items[i].DataLen) == 0)
			{
				if(pInv->InvType ==RETURN_INV_TEMPLATE)
				{
					if (pSaveTemplateInfo->workMode == WORK_MODE)
					{
						sprintf((char *)&(prnLineInfo->chContent[location-1]),
							"%08lu",pInv->PosInvNo);
						
					}
					else
					{
						memcpy((void *)&(prnLineInfo->chContent[location-1]),"********",8);
						
					}
				}
				else
				{
					prnLineInfo->Property = INVALID_TEMPLATE_LINE;
				}
				
			}
			//地址简称
			else if (memcmp(tempLine2->Items[i].dataPtr,"DZJC",
				tempLine2->Items[i].DataLen) == 0)
			{
				prn_log2("here  %s!!!", pInv->chKPDZJC);
				strncpy(&(prnLineInfo->chContent[location-1]),
					pInv->chKPDZJC, strlen(pInv->chKPDZJC));
				
			} 
			//机器编号
			else if (memcmp(tempLine2->Items[i].dataPtr,"JQBH",
				tempLine2->Items[i].DataLen) == 0)
			{
				prn_log2("here  %s!!!", pInv->chMachineNo);
				strncpy(&(prnLineInfo->chContent[location-1]),
					pInv->chMachineNo, strlen(pInv->chMachineNo));
				
			} //税号
			else if (memcmp(tempLine2->Items[i].dataPtr,"SWDJH",
				tempLine2->Items[i].DataLen) == 0)
			{
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{				
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chCorpCode,strlen(pInv->chCorpCode));
			    }
			    else
			    {
			    	memcpy((void *)&(prnLineInfo->chContent[location-1]),"********************",CORP_TAX_CODE_PRN_LEN);
			    }
				
			} //收款单位，涉及到折行处理
			else if (memcmp(tempLine2->Items[i].dataPtr,"SKDW",
				tempLine2->Items[i].DataLen) == 0)
			{				
				if (strlen(pInv->chCorpName) > (tempLine1->MaxSKDWCharacter))
				{
					//折行有打印内容，标志置3
					continueWriteFlag = 3;
					/**********for 57mm***********/
					continueWriteLocation = location;//跟上一行付款单位的起始打印位置对齐
					//continueWriteLocation = 2;//从一行的起始位置开始打印
					/**********for 57mm***********/
					
					prnLineInfo->Bhigh = tempLine2->BhighOne;
					//判断单位名称的前MaxSKDWCharacter中的半个汉字的个数，如果为奇数，
					//则将最后的半个汉字移到下一行来打
					//if(GetHalfHZCount((UINT8 *)pInv->chCorpName,tempLine1->MaxSKDWCharacter)%2 == 0)
					HZCount((UINT8 *)pInv->chCorpName, tempLine1->MaxSKDWCharacter, &ascNum, &gb2Num, &gb4Num);
					position = ascNum + gb2Num * 2 + gb4Num * 4;
					prn_log2("position = %u", position);
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chCorpName,position);
					prn_log2("拷贝的内容  %s", &(prnLineInfo->chContent[location-1]));	
					startPointer = (UINT8 *)&(pInv->chCorpName[position]);
					endPointer = startPointer;
					hzNum = (42 - continueWriteLocation) / 2;
					prn_log2("location = %u", location);
					prn_log2("hzNum = %u", hzNum);
					prn_log2("后续字符串   %s", &(pInv->chCorpName[position]));
					displen = HZCount(&(pInv->chCorpName[position]), strlen((INT8 *) &(pInv->chCorpName[position])), &ascNum, &gb2Num, &gb4Num);
					prn_log2("displen = %u", displen);
					if (displen % 2 == 0)
					{
						displen = displen / 2;
					}
					else
					{
						displen = displen / 2 + 1;
					}
					if (displen <= hzNum)
					{
						CorpNameCounte = 1;
					}
					else
					{
						if (displen % hzNum == 0)
						{
							CorpNameCounte = (UINT8)(displen / hzNum);
						}
						else
						{
							CorpNameCounte = (UINT8)(displen / hzNum + 1);
						}
					}
					conWriteBuffer->Bhigh = tempLine2->BhighTwo;
					conWriteBuffer->Property = NORMAL_TEMPLATE_LINE;
				}
				else
				{
				    //收款单位名称如果没有超过最大字符数，要走一段距离后再打下一行
				      //这时返回走空行的距离conWriteBuffer->Bhigh
				      
					//折行无打印内容，需要走一段距离，标志置2

	                // 更改标志位，为了不折行打印
					continueWriteFlag = 0;
					//continueWriteFlag = 2;

					prnLineInfo->Bhigh = tempLine2->BhighOne;
					
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chCorpName,strlen(pInv->chCorpName));
					
				}
				
			}

			//收款单位地址
			else if (memcmp(tempLine2->Items[i].dataPtr,"SKDWDZ",
				tempLine2->Items[i].DataLen) == 0)
			{
				if (strlen(pInv->chCorpAddr) > (tempLine1->MaxSKDWDZCharacter))
				{
					//折行有打印内容，标志置3
					continueWriteFlag = 3;
					/**********for 57mm***********/
					continueWriteLocation = location;//跟上一行付款单位的起始打印位置对齐
					//continueWriteLocation = 2;//从一行的起始位置开始打印
					/**********for 57mm***********/
					
					prnLineInfo->Bhigh = tempLine2->BhighOne;
					//判断单位名称的前MaxSKDWCharacter中的半个汉字的个数，如果为奇数，
					//则将最后的半个汉字移到下一行来打
					//if(GetHalfHZCount((UINT8 *)pInv->chCorpName,tempLine1->MaxSKDWCharacter)%2 == 0)
					HZCount((UINT8 *)pInv->chCorpAddr, tempLine1->MaxSKDWDZCharacter, &ascNum, &gb2Num, &gb4Num);
					position = ascNum + gb2Num * 2 + gb4Num * 4;
					prn_log2("position = %u", position);
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chCorpAddr,position);
					prn_log2("拷贝的内容  %s", &(prnLineInfo->chContent[location-1]));	
					startPointer = (UINT8 *)&(pInv->chCorpAddr[position]);
					endPointer = startPointer;
					hzNum = (42 - continueWriteLocation) / 2;
					prn_log2("location = %u", location);
					prn_log2("hzNum = %u", hzNum);
					prn_log2("后续字符串   %s", &(pInv->chCorpAddr[position]));
					displen = HZCount(&(pInv->chCorpAddr[position]), strlen((INT8 *) &(pInv->chCorpAddr[position])), &ascNum, &gb2Num, &gb4Num);
					prn_log2("displen = %u", displen);
					if (displen % 2 == 0)
					{
						displen = displen / 2;
					}
					else
					{
						displen = displen / 2 + 1;
					}
					if (displen <= hzNum)
					{
						CorpNameCounte = 1;
					}
					else
					{
						if (displen % hzNum == 0)
						{
							CorpNameCounte = (UINT8)(displen / hzNum);
						}
						else
						{
							CorpNameCounte = (UINT8)(displen / hzNum + 1);
						}
					}
					conWriteBuffer->Bhigh = tempLine2->BhighTwo;
					conWriteBuffer->Property = NORMAL_TEMPLATE_LINE;
				}
				else
				{
				    //收款单位名称如果没有超过最大字符数，要走一段距离后再打下一行
				      //这时返回走空行的距离conWriteBuffer->Bhigh
				      
					//折行无打印内容，需要走一段距离，标志置2

					continueWriteFlag = 2;
					prnLineInfo->Bhigh = tempLine2->BhighOne;
					
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chCorpAddr,strlen(pInv->chCorpAddr));
					
					
					if( prnLineInfo->EmptyForwardHigh > 0)
						
					{
					  	memset((void *)conWriteBuffer->chContent, 0, PRINT_LINE_BUF_LEN+1);
					 	conWriteBuffer->Bhigh = (UINT8)prnLineInfo->EmptyForwardHigh;	
					  	conWriteBuffer->Property = EMPTY_TEMPLATE_LINE;						
					}
					else
				    {
				    	strncpy(conWriteBuffer->chContent," ", 1);
				    	conWriteBuffer->Bhigh = tempLine2->BhighTwo;	
					    conWriteBuffer->Property =NORMAL_TEMPLATE_LINE;		
				    	
				    }				
          
				}				
			}	

			//付款单位需要折行，同上处理
			else if (memcmp(tempLine2->Items[i].dataPtr,"FKDW",
				tempLine2->Items[i].DataLen) == 0)
			{
					j = 0;
					while(1)
					{	
						prn_log2("while  j = %u", j);					
						displen = HZCount((UINT8 *)pInv->chClientName, (tempLine1->MaxFKDWCharacter - j), &ascNum, &gb2Num, &gb4Num);
						position = ascNum + gb2Num * 2 + gb4Num * 4;
						if (j == 0)
							displen2 = displen;
						prn_log2("position = %u", position);
						if (SKYContinueFlag == 1)
						{
							if ((location + displen) > (saveRealLocation - 2))
							{
								j++;
								continue;
							}
							break;
						}
						else
						{
							break;
						}
					}
						
					if (strlen(pInv->chClientName) > (tempLine1->MaxFKDWCharacter)
						|| ( (strlen(pInv->chClientName) <= tempLine1->MaxFKDWCharacter) && ((location + displen2) > (saveRealLocation - 2)) && (SKYContinueFlag == 1)))
					{					
						continueWriteFlag = 3;
						/**********for 57mm***********/
						continueWriteLocation = location;		
					//	continueWriteLocation = 2;	
						/**********for 57mm***********/
						prnLineInfo->Bhigh = tempLine2->BhighOne;
						
						strncpy(&(prnLineInfo->chContent[location-1]),
							pInv->chClientName,position);
						if (SKYContinueFlag == 1)
							strncpy(&(prnLineInfo->chContent[saveRealLocation - 1]),
						    		saveSkyContent,strlen(saveSkyContent));
						prn_log2("拷贝的内容  %s", &(prnLineInfo->chContent[location-1]));	
						startPointer = (UINT8 *)&(pInv->chClientName[position]);
						endPointer = startPointer;
						hzNum = (42 - location) / 2;
						prn_log2("location = %u", location);
						prn_log2("hzNum = %u", hzNum);
						prn_log2("后续字符串   %s", &(pInv->chClientName[position]));
						displen = HZCount(&(pInv->chClientName[position]), strlen((INT8 *) &(pInv->chClientName[position])), &ascNum, &gb2Num, &gb4Num);
						if (displen % 2 == 0)
						{
							displen = displen / 2;
						}
						else
						{
							displen = displen / 2 + 1;
						}
						if (displen <= hzNum)
						{
							CorpNameCounte = 1;
						}
						else
						{
							if (displen % hzNum == 0)
							{
								CorpNameCounte = (UINT8)(displen / hzNum);
							}
							else
							{
								CorpNameCounte = (UINT8)(displen / hzNum + 1);
							}
						}
						conWriteBuffer->Bhigh = tempLine2->BhighTwo;
						conWriteBuffer->Property = NORMAL_TEMPLATE_LINE;
					}
					else
					{
					    //prn_log("付款人名称没有超过最大字符数，呵呵");
						//  更改标志位，为了不折行打印
                        continueWriteFlag = 0;
                       	//continueWriteFlag = 2;
						prnLineInfo->Bhigh = tempLine2->BhighOne;
						
					    //prn_log("收款员没有折行");
					    //prn_log2("pInv->chClientName = %s",pInv->chClientName);
						strncpy(&(prnLineInfo->chContent[location - 1]),
							     pInv->chClientName, strlen(pInv->chClientName));
							     
						if (SKYContinueFlag == 1)
							strncpy(&(prnLineInfo->chContent[saveRealLocation - 1]),
						    		saveSkyContent,strlen(saveSkyContent));
						//注释目的是为了不折行，仅仅显示一行
						 /*   			     
						if( prnLineInfo->EmptyForwardHigh > 0)

						{
							memset((void *)conWriteBuffer->chContent, 0, PRINT_LINE_BUF_LEN+1);
							conWriteBuffer->Bhigh = (UINT8)prnLineInfo->EmptyForwardHigh;	
							conWriteBuffer->Property = EMPTY_TEMPLATE_LINE;						
						}
						else
						{
							strncpy(conWriteBuffer->chContent," ", 1);
							conWriteBuffer->Bhigh = tempLine2->BhighTwo;	
							conWriteBuffer->Property =NORMAL_TEMPLATE_LINE;		
						}
                       */
						/*-------------added end---------------------------------*/									
					}						
			//	}	
				
			}//END FKDW 
			 
			//开票日期
			else if (memcmp(tempLine2->Items[i].dataPtr,"KPRQ",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%04lu-%02lu-%02lu", (pInv->m_Date) / 10000, ( (pInv->m_Date) % 10000 ) / 100 ,
					( (pInv->m_Date) % 10000 ) % 100  );			
				//判断是否为打印电子存根	
				if (pInv->PrintType == STUB_PRINT_TEMPLATE)
				{
					stubFlag = 1;
					stubPlace = location ;					
				}			
				if (pSaveTemplateInfo->workMode != WORK_MODE) //学习模式打印“不可报销”
				{				
					stubFlag = 2;
					stubPlace = (UINT8)location;	
				}
			}
			else if	(memcmp(tempLine2->Items[i].dataPtr,"KPSJ",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%02lu:%02lu:%02lu", (pInv->m_Time) / 10000, ( (pInv->m_Time) % 10000 ) / 100 ,
					( (pInv->m_Time) % 10000 ) % 100  );
				
				//判断是否为打印电子存根	
//				if (pInv->PrintType == STUB_PRINT_TEMPLATE)
//				{
//					stubFlag = 1;
//					stubPlace = location ;					
//				}			
			} 
			//收款员折行处理，但如果没有超过最大字符，则不需要走空行；
			else if (memcmp(tempLine2->Items[i].dataPtr,"SKY",
				tempLine2->Items[i].DataLen) == 0)
			{
				memset((void *)saveSkyContent,0,10);

                //prn_log2("收款员pInv->chOperatorName = %s",pInv->chOperatorName);
                //prn_log2("收款员最大字符数tempLine1->MaxSKYCharacter = %u",tempLine1->MaxSKYCharacter);
				if (strlen(pInv->chOperatorName) > (tempLine1->MaxSKYCharacter))
				{
				    //prn_log("收款员折行，置标志为1");
				
					/*标志收款员折行*/
						SKYContinueFlag = 1;
						strncpy(&(prnLineInfo->chContent[location-1]),
							pInv->chOperatorName,tempLine1->MaxSKYCharacter);
                                            
						saveRealLocation = location; //记录折行位置
						
						strncpy(saveSkyContent,
							&(pInv->chOperatorName[tempLine1->MaxSKYCharacter]),
							strlen(pInv->chOperatorName) - (tempLine1->MaxSKYCharacter)); //将折行内容存储
						
				
				}
				else
				{	
				    //prn_log("收款员未折行，置标志为0");
				    SKYContinueFlag = 0;
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chOperatorName,strlen(pInv->chOperatorName));
				}
				
			} //金额小写
			else if (memcmp(tempLine2->Items[i].dataPtr,"JEXX",
				tempLine2->Items[i].DataLen) == 0)
			{	
				
				
				//prn_log2("开票类型*********%u",pInv->InvType);
				tmpLocation = location;
				
				if (location < 0)
				{
					location = -location;
				}
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{				
				
					memset((void *)saveRightBuf,0,sizeof(saveRightBuf));
					
					
					if ((pInv->InvType == RETURN_INV_TEMPLATE) || (pInv->InvType == RETURN_INV_NOTEXIT_TEMPLATE) ||
						(pInv->InvType == RET_SPECIAL_INV))
					{
						sprintf(saveRightBuf,"￥%.2f",-(double)((double)pInv->InvSumLowcase) / 100);
					}
					else
					{
						sprintf(saveRightBuf,"￥%.2f",(double)((double)pInv->InvSumLowcase) / 100);
					}
					
					len = strlen(saveRightBuf);
					
					if (tmpLocation < 0)
					{
						memcpy((void *)&prnLineInfo->chContent[location-len], (void *)saveRightBuf, len);
					}
					else
					{
						memcpy((void *)&prnLineInfo->chContent[location-1], (void *)saveRightBuf, len);
					}
				}
				else
				{
				  if (tmpLocation < 0)
				  {
					memcpy((void *)&prnLineInfo->chContent[location-4], "****", 4);
				
				  }
				  else
				  {
				  	memcpy((void *)&prnLineInfo->chContent[location-1], "****", 4);
				  }
				}
				
			} //金额大写，需要折行处理
			else if (memcmp(tempLine2->Items[i].dataPtr,"JEDX",
				tempLine2->Items[i].DataLen) == 0)
			{
				//prn_log2("开票类型*********%u",pInv->InvType);
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{
					if (strlen(pInv->chInvSumUppercase) > (tempLine1->MaxJEDXCharacter))
					{
						if(pInv->InvType == WASTE_INV_TEMPLATE)
						{
				    		strncpy(&(prnLineInfo->chContent[location-1]),
									pInv->chInvSumUppercase,strlen(pInv->chInvSumUppercase));
						}
						else
						{
							continueWriteFlag = 1;
							continueWriteLocation = (UINT8)location;
						
							//如果为退票
							//prn_log2("************此时的发票类型%u",pInv->InvType);
							if ((pInv->InvType == RETURN_INV_TEMPLATE) || (pInv->InvType == RETURN_INV_NOTEXIT_TEMPLATE) ||
								(pInv->InvType == RET_SPECIAL_INV))
							{
								strncpy(&(prnLineInfo->chContent[location-1]), "(负)", 4);
							
								strncpy(&(prnLineInfo->chContent[location+3]),
									pInv->chInvSumUppercase,tempLine1->MaxJEDXCharacter);
								
								strncpy(&(conWriteBuffer->chContent[location-1]),
									&(pInv->chInvSumUppercase[tempLine1->MaxJEDXCharacter]),
									strlen(pInv->chInvSumUppercase) - (tempLine1->MaxJEDXCharacter));
							
							}
							else
							{
						    
								strncpy(&(prnLineInfo->chContent[location-1]),
									pInv->chInvSumUppercase,tempLine1->MaxJEDXCharacter);
								
								strncpy(&(conWriteBuffer->chContent[location-1]),
									&(pInv->chInvSumUppercase[tempLine1->MaxJEDXCharacter]),
									strlen(pInv->chInvSumUppercase) - (tempLine1->MaxJEDXCharacter));
							
							}	
							conWriteBuffer->Bhigh = prnLineInfo->Bhigh;
							conWriteBuffer->Property = NORMAL_TEMPLATE_LINE;
						}			
						
					}
					else
					{	
						//prn_log2("此时发票类型为%u",pInv->InvType);
						if(pInv->InvType == WASTE_INV_TEMPLATE)
						{
				    		//prn_log2("pInv->chInvSumUppercase = %s",pInv->chInvSumUppercase);
				    		strncpy(&(prnLineInfo->chContent[location-1]),
									pInv->chInvSumUppercase,strlen(pInv->chInvSumUppercase));
						}
						else
						{
							continueWriteFlag = 1;				
							if ((pInv->InvType == RETURN_INV_TEMPLATE) || (pInv->InvType == RETURN_INV_NOTEXIT_TEMPLATE) ||
								(pInv->InvType == RET_SPECIAL_INV))
							{
								strncpy(&(prnLineInfo->chContent[location-1]), "(负)", 4);
								strncpy(&(prnLineInfo->chContent[location+3]),
									pInv->chInvSumUppercase,strlen(pInv->chInvSumUppercase));
							}
							else
							{
								strncpy(&(prnLineInfo->chContent[location-1]),
									pInv->chInvSumUppercase,strlen(pInv->chInvSumUppercase));
							}
							memset((void *)conWriteBuffer->chContent, 0, PRINT_LINE_BUF_LEN+1);
						
							conWriteBuffer->Bhigh = (UINT8)prnLineInfo->EmptyForwardHigh;	
							conWriteBuffer->Property = EMPTY_TEMPLATE_LINE;	
						}
					
					}
				}
				else
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"****",4);
				}
			}//合计税额
			else if (memcmp(tempLine2->Items[i].dataPtr,"HJSE",
				tempLine2->Items[i].DataLen) == 0)
			{	
				//prn_log2("开票类型*********%u",pInv->InvType);
				tmpLocation = location;
				
				if (location < 0)
				{
					location = -location;
				}
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{				
				
					memset((void *)saveRightBuf,0,sizeof(saveRightBuf));
					
					
					if ((pInv->InvType == RETURN_INV_TEMPLATE) || (pInv->InvType == RETURN_INV_NOTEXIT_TEMPLATE) ||
						(pInv->InvType == RET_SPECIAL_INV))
					{
						sprintf(saveRightBuf,"￥%.2f",-(double)((double)pInv->InvTaxLowcase) / 100);
					}
					else
					{
						sprintf(saveRightBuf,"￥%.2f",(double)((double)pInv->InvTaxLowcase) / 100);
					}
					
					len = strlen(saveRightBuf);
					
					if (tmpLocation < 0)
					{
						memcpy((void *)&prnLineInfo->chContent[location-len], (void *)saveRightBuf, len);
					}
					else
					{
						memcpy((void *)&prnLineInfo->chContent[location-1], (void *)saveRightBuf, len);
					}
				}
				else
				{
				  if (tmpLocation < 0)
				  {
					memcpy((void *)&prnLineInfo->chContent[location-4], "****", 4);
				
				  }
				  else
				  {
				  	memcpy((void *)&prnLineInfo->chContent[location-1], "****", 4);
				  }
				}
				
			}//合计金额
			else if (memcmp(tempLine2->Items[i].dataPtr,"HJJE",
				tempLine2->Items[i].DataLen) == 0)
			{	
				//prn_log2("开票类型*********%u",pInv->InvType);
				tmpLocation = location;
				
				if (location < 0)
				{
					location = -location;
				}
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{				
				
					memset((void *)saveRightBuf,0,sizeof(saveRightBuf));
					
					
					if ((pInv->InvType == RETURN_INV_TEMPLATE) || (pInv->InvType == RETURN_INV_NOTEXIT_TEMPLATE) ||
						(pInv->InvType == RET_SPECIAL_INV))
					{
						sprintf(saveRightBuf,"￥%.2f",-(double)((double)pInv->InvMoneyLowcase) / 100);
					}
					else
					{
						sprintf(saveRightBuf,"￥%.2f",(double)((double)pInv->InvMoneyLowcase) / 100);
					}
					
					len = strlen(saveRightBuf);
					
					if (tmpLocation < 0)
					{
						memcpy((void *)&prnLineInfo->chContent[location-len], (void *)saveRightBuf, len);
					}
					else
					{
						memcpy((void *)&prnLineInfo->chContent[location-1], (void *)saveRightBuf, len);
					}
				}
				else
				{
				  if (tmpLocation < 0)
				  {
					memcpy((void *)&prnLineInfo->chContent[location-4], "****", 4);
				
				  }
				  else
				  {
				  	memcpy((void *)&prnLineInfo->chContent[location-1], "****", 4);
				  }
				}
				
			} //税控码
			else if (memcmp(tempLine2->Items[i].dataPtr,"SKM",
				tempLine2->Items[i].DataLen) == 0)
			{
				/*
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{
					memcpy(&(prnLineInfo->chContent[location-1]),
						pInv->chTaxCtrCode,4);
					prnLineInfo->chContent[location-1+4] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+5],
						&(pInv->chTaxCtrCode[4]),4);
					prnLineInfo->chContent[location-1+9] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+10],
						&(pInv->chTaxCtrCode[8]),4);
					prnLineInfo->chContent[location-1+14] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+15],
						&(pInv->chTaxCtrCode[12]),4);
					prnLineInfo->chContent[location-1+19] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+20],
						&(pInv->chTaxCtrCode[16]),4);
				}
				else
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"**** **** **** **** ****",24);
					stubFlag = 2;
					stubPlace = (UINT8)location;
				}
				*/
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chTaxCtrCode);	
			}
			else if (memcmp(tempLine2->Items[i].dataPtr,"JZLSH",
				tempLine2->Items[i].DataLen) == 0)
			{
				
				if (pSaveTemplateInfo->workMode == WORK_MODE)
				{
					memcpy(&(prnLineInfo->chContent[location-1]),
						pInv->chJzlshCode,4);
					prnLineInfo->chContent[location-1+4] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+5],
						&(pInv->chJzlshCode[4]),4);
					prnLineInfo->chContent[location-1+9] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+10],
						&(pInv->chJzlshCode[8]),4);
					prnLineInfo->chContent[location-1+14] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+15],
						&(pInv->chJzlshCode[12]),4);
					prnLineInfo->chContent[location-1+19] = ' ';
					
					memcpy(&prnLineInfo->chContent[location-1+20],
						&(pInv->chJzlshCode[16]),4);
				}
				else
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"**** **** **** **** ****",24);
					stubFlag = 2;
					stubPlace = (UINT8)location;
				}
			}


			else if(memcmp(tempLine2->Items[i].dataPtr,"SKDWDH",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chPhoneNo);				
			}

			//付款单位税号
			else if(memcmp(tempLine2->Items[i].dataPtr,"FKDWSH",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chClientCode);				
			}
			//行业分类
			else if (memcmp(tempLine2->Items[i].dataPtr,"HYFL",
				tempLine2->Items[i].DataLen) == 0)
			{
				strncpy(&(prnLineInfo->chContent[location-1]),
					pInv->chHYFL, strlen(pInv->chHYFL));
				
			} 
			//自定义标签1
			else if(memcmp(tempLine2->Items[i].dataPtr,"ZDY1",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chSelfDefTab1);				
			}
			//自定义内容1
			else if(memcmp(tempLine2->Items[i].dataPtr,"ZDYZ1",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chSelfDefCont1);				
			}
			//自定义标签2
			else if(memcmp(tempLine2->Items[i].dataPtr,"ZDY2",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chSelfDefTab2);				
			}
			//自定义内容2
			else if(memcmp(tempLine2->Items[i].dataPtr,"ZDYZ2",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%s", pInv->chSelfDefCont2);				
			}
			//BZ
			else if (memcmp(tempLine2->Items[i].dataPtr,"BZ",
				tempLine2->Items[i].DataLen) == 0)
			{
				if (strlen(pInv->chRemarks) > (tempLine1->MaxBZCharacter))
				{
					//折行有打印内容，标志置3
					continueWriteFlag = 3;
					//continueWriteLocation = location;
					continueWriteLocation = 2;
					
					prnLineInfo->Bhigh = tempLine2->BhighOne;
					//判断单位名称的前MaxSKDWCharacter中的半个汉字的个数，如果为奇数，
					//则将最后的半个汉字移到下一行来打
					//if(GetHalfHZCount((UINT8 *)pInv->chCorpName,tempLine1->MaxSKDWCharacter)%2 == 0)
					HZCount((UINT8 *)pInv->chRemarks, tempLine1->MaxBZCharacter, &ascNum, &gb2Num, &gb4Num);
					position = ascNum + gb2Num * 2 + gb4Num * 4;
					prn_log2("position = %u", position);
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chRemarks,position);
					prn_log2("拷贝的内容  %s", &(prnLineInfo->chContent[location-1]));	
					startPointer = (UINT8 *)&(pInv->chRemarks[position]);
					endPointer = startPointer;
					hzNum = (42 - continueWriteLocation) / 2;
					prn_log2("location = %u", location);
					prn_log2("hzNum = %u", hzNum);
					prn_log2("后续字符串   %s", &(pInv->chRemarks[position]));
					displen = HZCount(&(pInv->chRemarks[position]), strlen((INT8 *) &(pInv->chRemarks[position])), &ascNum, &gb2Num, &gb4Num);
					prn_log2("displen = %u", displen);
					if (displen % 2 == 0)
					{
						displen = displen / 2;
					}
					else
					{
						displen = displen / 2 + 1;
					}
					if (displen <= hzNum)
					{
						CorpNameCounte = 1;
					}
					else
					{
						if (displen % hzNum == 0)
						{
							CorpNameCounte = (UINT8)(displen / hzNum);
						}
						else
						{
							CorpNameCounte = (UINT8)(displen / hzNum + 1);
						}
					}
					conWriteBuffer->Bhigh = tempLine2->BhighTwo;
					conWriteBuffer->Property = NORMAL_TEMPLATE_LINE;
				}
				else
				{
				    //收款单位名称如果没有超过最大字符数，要走一段距离后再打下一行
				      //这时返回走空行的距离conWriteBuffer->Bhigh
				      
					//折行无打印内容，需要走一段距离，标志置2
					continueWriteFlag = 2;
					prnLineInfo->Bhigh = tempLine2->BhighOne;
					
					strncpy(&(prnLineInfo->chContent[location-1]),
						pInv->chRemarks,strlen(pInv->chRemarks));
					
					
					if( prnLineInfo->EmptyForwardHigh > 0)
						
					{
					  	memset((void *)conWriteBuffer->chContent, 0, PRINT_LINE_BUF_LEN+1);
					 	conWriteBuffer->Bhigh = (UINT8)prnLineInfo->EmptyForwardHigh;	
					  	conWriteBuffer->Property = EMPTY_TEMPLATE_LINE;						
					}
					else
				    {
				    	strncpy(conWriteBuffer->chContent," ", 1);
				    	conWriteBuffer->Bhigh = tempLine2->BhighTwo;	
					    conWriteBuffer->Property =NORMAL_TEMPLATE_LINE;		
				    	
				    }				
				}				
			}
#if PETROL_STATION_VER
			else if(memcmp(tempLine2->Items[i].dataPtr,"BCH",
				tempLine2->Items[i].DataLen) == 0)
			{
			    //prn_log("进入班次号比较");
			  
				if( (pInv->m_Time >= 70000) &&(pInv->m_Time < 120000))			
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"早",2);
				}
				else if((pInv->m_Time >= 120000) &&(pInv->m_Time < 183000))
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"中",2);
				}
				else
				{
					memcpy(&(prnLineInfo->chContent[location-1]),"晚",2);
				}
			}
			else if(memcmp(tempLine2->Items[i].dataPtr,"SKDWDH",
				tempLine2->Items[i].DataLen) == 0)
			{
				sprintf(&(prnLineInfo->chContent[location-1]),
					"%u", pInv->OilGunID);				
			}
#endif
            else
            {

                //prn_log2("***************pInv->GoodsLineCount = %u",pInv->GoodsLineCount);
                //prn_log2("***************templateNo = %u",templateNo);
               			   		
			    
		        if(pInv->GoodsLineCount == 0)
				{  				
					strncpy(prnLineInfo->chContent,	" ", 1);					
					break; 
					
				}
				
				//prn_log2("当前打印类型%u",pInv->PrintType);	
				//如果是不定长并且打印类型，templatNo一直为0
				if ((pSaveTemplateInfo->FixedModeFlag == 0) &&(pInv->InvType != RETURN_INV)&&
					(pInv->InvType != RET_MANUAL_INV))
				{
					//如果不是存根打印，并且不是商品取消行
					if ((pInv->PrintType != STUB_PRINT) && (DetailCancelFlag == 0))
					{
						templateNo = 0;	
					}
									
				}
				
			    //prn_log2("prnLineInfo->chContent = %s",prnLineInfo->chContent);		   
			    //商品名称，如果商品名称大于最大字符数，将商品名称单行打印
			    
				//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
				if (memcmp(tempLine2->Items[i].dataPtr,"SPMC",
					tempLine2->Items[i].DataLen) == 0)
				{					
					//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
					
                    if (strlen(pInv->GoodsLines[templateNo].chProductName)  
						<= (tempLine1->MaxSPMCCharacter) )
					{
					    //prn_log("商品名称不大于最大字符数");
						strncpy(&(prnLineInfo->chContent[location-1]),
							pInv->GoodsLines[templateNo].chProductName,
							strlen(pInv->GoodsLines[templateNo].chProductName));
						
                    }							
			
                    else
                    {   						 
					   						 
				 		if(GetHalfHZCount((UINT8 *) pInv->GoodsLines[templateNo].chProductName,tempLine1->MaxSPMCCharacter)%2==0 )  
					  	{
					  		//prn_log2("tempLine1->MaxSPMCCharacter = %u",tempLine1->MaxSPMCCharacter);
				  			//prn_log2("pInv->GoodsLines[templateNo].chProductName = %s",pInv->GoodsLines[templateNo].chProductName);
					  		
					  		strncpy(&(prnLineInfo->chContent[location-1]),
								pInv->GoodsLines[templateNo].chProductName,tempLine1->MaxSPMCCharacter);
							//prn_log2("prnLineInfo->chContent = %s",prnLineInfo->chContent);
							
							
							strncpy(&(conWriteBuffer->chContent[location-1]),
								&(pInv->GoodsLines[templateNo].chProductName[tempLine1->MaxSPMCCharacter]),
								strlen(pInv->GoodsLines[templateNo].chProductName) - (tempLine1->MaxSPMCCharacter));
							
							//prn_log2("conWriteBuffer->chContent = %s",conWriteBuffer->chContent);
							conWriteBuffer->Bhigh = prnLineInfo->Bhigh;
							
							
						}
					  	else
					  	{
					  		strncpy(&(prnLineInfo->chContent[location-1]),
								pInv->GoodsLines[templateNo].chProductName,tempLine1->MaxSPMCCharacter-1);
							
							strncpy(&(conWriteBuffer->chContent[location-1]),
								&(pInv->GoodsLines[templateNo].chProductName[tempLine1->MaxSPMCCharacter - 1]),
									strlen(pInv->GoodsLines[templateNo].chProductName) - (tempLine1->MaxSPMCCharacter - 1));
							
							conWriteBuffer->Bhigh = prnLineInfo->Bhigh;
							//prn_log2("conWriteBuffer->Bhigh = %u",conWriteBuffer->Bhigh);
								
						}	
						
						continueWriteFlag = 1;
						
					}
				}//END SPMC 
				//商品单价
				else if (memcmp(tempLine2->Items[i].dataPtr,"SPDJ",
					tempLine2->Items[i].DataLen) == 0)
				{
					
					//打印右对齐的处理，针对模板中打印内容位置为负的情况
					
					//tmpLocation为初始值（无论正负）
					tmpLocation = location;
					
					//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
					//如果为负，将location设为正值
					if (location < 0)
					{
						location = -location;
					}
					
					memset((void *)saveRightBuf,0,sizeof(saveRightBuf));
					
					//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
					//折扣处理
					if ((pInv->GoodsLines[templateNo].Type == DETAIL_DISCOUNT_TEMPLATE))
					{
						sprintf(saveRightBuf, "%.1f%%",
							(double)((double)pInv->GoodsLines[templateNo].Price / RATE_EXTENSION_TEMPLATE));
					}
					else if (pInv->GoodsLines[templateNo].Type == DETAIL_REDUCTION_TEMPLATE || pInv->GoodsLines[templateNo].Type == DETAIL_SERVICE_TEMPLATE)
					{
						;
					}
					else
					{
									sprintf(saveRightBuf, "%.2f",
							((double)pInv->GoodsLines[templateNo].Price)  / PRICE_EXTENSION_TEMPLATE);
					}
					
					len = strlen(saveRightBuf);
					
					//如果初始为负，打印位置为从右边向左算，第len个位置开始打印
					if ( tmpLocation < 0 )
					{
						memcpy((void *)&prnLineInfo->chContent[location-len], (void *)saveRightBuf, len);
						
					}
					else
					{
						memcpy((void *)&prnLineInfo->chContent[location-1], (void *)saveRightBuf, len);
					}
					
				}//END SPDJ  
				//商品数量
				else if (memcmp(tempLine2->Items[i].dataPtr,"SPSL",
					tempLine2->Items[i].DataLen) == 0)
				{		
//					if((pInv->GoodsLines[templateNo].Amount % NUMBER_EXTENSION_TEMPLATE) == 0)
					if (0)
					{
					    /*----------------------added xiaopk 2005111-----------------------*/
					    if(pInv->InvType == RETURN_INV_TEMPLATE)
						{
							sprintf(&prnLineInfo->chContent[location-1],
								"%lu",(pInv->GoodsLines[templateNo].Amount / NUMBER_EXTENSION_TEMPLATE));	
						}
						else
						{
							sprintf(&prnLineInfo->chContent[location-1],
								"%lu",(pInv->GoodsLines[templateNo].Amount / NUMBER_EXTENSION_TEMPLATE));	
						}
						/*-----------------------added end-----------------------------------*/
					
					}
					else
					{
					   
						/*___________________added Yinct 20051115______________________________*/
						/*******************************zs20070418***************************/
						if (pInv->GoodsLines[templateNo].Type == DETAIL_DISCOUNT_TEMPLATE || pInv->GoodsLines[templateNo].Type == DETAIL_REDUCTION_TEMPLATE
							|| pInv->GoodsLines[templateNo].Type == DETAIL_SERVICE_TEMPLATE)
						{
							memset((void *)tmp, 0, 8);
						}
						else
						{
							prn_log2("Amount= %u",pInv->GoodsLines[templateNo].Amount);
							realAmount = pInv->GoodsLines[templateNo].Amount & 0xFFFFFFUL;
							prn_log2("realAmount = %u", realAmount);
							multiple = 1;
							for(j=0; j<((pInv->GoodsLines[templateNo].Amount & 0xFF000000UL) >> 24); j++)
							{
								multiple *= 10;
							}
							prn_log2("multiple = %lld", multiple);
							prn_log2("realAmount = %u", realAmount);
							if (multiple <= 100)
							{
								sprintf(tmp, "%%.2f");
							}
							else
							{
								sprintf(tmp, "%%.%luf", ((pInv->GoodsLines[templateNo].Amount & 0xFF000000UL) >> 24) & (0xFF));
							}
						}
						sprintf(&prnLineInfo->chContent[location-1], tmp, (double)((double)realAmount / multiple));
						
						/*______________________________end____________________________________*/
							
					   
					}
				}
				//商品金额，右对齐，同商品单价处理
				else if (memcmp(tempLine2->Items[i].dataPtr,"SPJE",
					tempLine2->Items[i].DataLen) == 0)
				{	
					tmpLocation = location;
					if (location < 0)
					{
						location = -location;
					}


					if (pSaveTemplateInfo->workMode == WORK_MODE)
					{
						memset(saveRightBuf,0,sizeof(saveRightBuf));
						prn_log2("发票类型pInv->InvType = %u", pInv->InvType);
						prn_log2("DetailCancelFlag = %u", DetailCancelFlag);
						prn_log2("商品行类型pInv->GoodsLines[templateNo].Type = %u",pInv->GoodsLines[templateNo].Type);
						//如果为正常发票且为折扣或者为退票且为折扣
						prn_log("不是折扣属性");
						prn_log2("金额为%u",pInv->GoodsLines[templateNo].Sum / SUM_EXTENSION_TEMPLATE);
						prn_log2("(double)pInv->GoodsLines[templateNo].Sum / SUM_EXTENSION_TEMPLATE = %u",(double)pInv->GoodsLines[templateNo].Sum / SUM_EXTENSION_TEMPLATE);
						sprintf(saveRightBuf, "%.2f",
							(double)pInv->GoodsLines[templateNo].Sum / SUM_EXTENSION_TEMPLATE);

						prn_log2("saveRightBuf = %s********************",saveRightBuf);
						len = strlen(saveRightBuf);
						prn_log2("strlen(saveRightBuf) = %u",len);
						prn_log2("saveRightBuf = %s********************",saveRightBuf);


						if ( tmpLocation < 0 )
						{  
							//prn_log("打印位置为负");
							//prn_log2("saveRightBuf = %s********************",saveRightBuf);
							memcpy((void *)&prnLineInfo->chContent[location-len], (void *)saveRightBuf, len);
							//prn_log2("prnLineInfo->chContent[location - len] = %s*********",&prnLineInfo->chContent[location-len]);

						}
						else
						{
							memcpy((void *)&prnLineInfo->chContent[location-1], (void *)saveRightBuf, len);
							//prn_log2("prnLineInfo->chContent[location - len] = %s*********",prnLineInfo->chContent[location-len]);

						}
					} 
					else
					{
						if(tmpLocation < 0)				
						{
							memcpy((void *)&prnLineInfo->chContent[location-4],"****",4);
							//prn_log2("prnLineInfo->chContent = %s",prnLineInfo->chContent);
						}
						else
						{
							memcpy((void *)&prnLineInfo->chContent[location-1],"****",4);
						}
					}
					
				}//end (SPJE)
				
            } //end else
		}//end (else)
	}//end (for)
	
	//prn_log2("prnLineInfo->FontH = %d", prnLineInfo->FontH);
    //prn_log("end GetPrnLineData ");	
	return 0;	
}
