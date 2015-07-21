/*! \file    TemplateInterface.h
\brief    模板接口文件
\author   Xiao Pengkun   2005-06-23
 */

#ifndef  TEMPLATE_INTERFACE_H
#define  TEMPLATE_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "TemplateDataDesign.h"


extern UINT8 SKYContinueFlag;//收款员折行标志


/*!
@brief 获取模板行中两个~之间的数据。
@param[in] tempLineBuf
@param[in] tempLineFieldBuf
@note 将指向每一行模板的指针的指针传入，将模板行中的两个~间的数据拷贝到
		 tempLineFieldBuf指向的缓存中，将指针的指针返回，以便记住下一次处理
         的位置。
@return  0 - 成功
*/
UINT8  GetPtField(UINT8 **tempLineBuf,UINT8 *tempLineFieldBuf);


/*!
@brief 获取每一行模板数据。
@param[in] prnTempFile
@param[in] tempLineBuf
@note 将指向整个打印模板数据的指针的指针传入，得到每一行模板数据（以'\n'结尾），
		 将包括'\n'的数据拷贝到tempLineBuf指向的缓存中。将指针的指针返回，以
         便记住下一次处理的位置。
@return  0 - 成功
*/
UINT8  GetPtLine(UINT8 **prnTempFile, UINT8 *tempLineBuf);



/*!
@brief 解析模板行0
@param[in] tempLineBuf
@param[in] tempLine0
@note 通过调用GetPtField，完成模板行0的解析，将对应数据存储到模板行0的
         结构体中。
@return  0 - 成功
*/
UINT8  ParsePtLine0(UINT8 *tempLineBuf, TPrnTempLine0 *tempLine0);



/*!
@brief 解析模板行1
@param[in] tempLineBuf
@param[in] tempLine1
@note 通过调用GetPtField，完成模板行1的解析，将对应数据存储到模板行1的
         结构体中
@return  0 - 成功
*/
UINT8  ParsePtLine1(UINT8 *tempLineBuf, TPrnTempLine1 *tempLine1);



/*!
@brief 解析模板行2
@param[in] tempLineBuf
@param[in] tempLine2
@param[in] prnLineInfo
@param[in] tempLine1
@note 通过调用GetPtField，完成模板行2的解析。模板行2为打印行数据内容，通过解析
         模板，将对应的业务数据填充到最终打印行中。
@return  0 - 成功 
*/
UINT8  ParsePtLine2(UINT8 *tempLineBuf, TPrnTempRow *tempLine2,TPrnLineInfo *prnLineInfo,TPrnTempLine1 *tempLine1);

/*!
@brief 获取最终业务数据，形成打印数据行。
@param[in] tempLine2 模板行2结构指针，tempLineBuf2->chDataBuf存储打印模板中需要打印的内容
@param[in] tempLineBuf2 折行处理时的打印行结构
@param[in] tempLine1 模板行1结构，存储最大字符长度，处理折行。
@param[in] prnLineInfo 输出的打印行结构
@param[in] pInv 发票数据结构
@note tempLine2存储模板行的内容，包括标签的名称与位置和内容的位置，以及标签和内容的个数。
		 通过循环，解析出tempLine2->chDataBuf中的内容，如果是标签就直接打印；如果是内容，通过
		 比较（如FPHM代表发票号码）将pInv的相关发票数据添加到对应的位置，循环处理结束即形成最终
         的打印行数据，此时指针prnLineInfo所指即为送打印机的打印行。
@return  0 - 成功 
*/
UINT8 GetPrnLineData(TPrnTempRow *tempLine2, TPrnLineInfo *tempLineBuf2,TPrnTempLine1 *tempLine1,TPrnLineInfo *prnLineInfo,TPrnInvoiceInfo *pInv);

#ifdef __cplusplus
}
#endif

#endif



