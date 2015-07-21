 /*! \file    DataIOFunc.h
   \brief    awe4000r数据导入导出的应用函数。
   \author   Yu Yan
   \version  1.0
   \date     2008-8-13
 */


#ifndef DATA_IO_FUNC_H
#define DATA_IO_FUNC_H

#include "IncludeMe.h"
#include "SysMacDef.h"
#include "CaMsgBox.h"
#include "CClient.h"
#include "CDept.h"



/*!
@brief 部类数据导入过程
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 DeptImportProc();

/*!
@brief 付款人数据导入过程
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ClientImportProc();

/*!
@brief 数据导入核心操作(导入sql脚本文件)
@param[in] nKind   导入的数据表  1：PLU表； 2：DEPT表 3：Client表
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ImportHandle(UINT8 nKind);

/*!
@brief 数据导入核心操作(导入TXT文本文件)
@param[in] nKind   导入的数据表  1：PLU表； 2：DEPT表 3：Client表
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ImportTxtHandle(UINT8 nKind);

/*!
@brief 读TXT文本文件，并逐条记录插入PLU表
@param[in] fp   文本文件指针
@param[in] pchToken   文本文件中隔开各个字段的标记符
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 InsertOnePlu(FILE *fp, const INT8 *pchToken);

/*!
@brief 读TXT文本文件，并逐条记录插入Dept表
@param[in] fp   文本文件指针
@param[in] pchToken   文本文件中隔开各个字段的标记符
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 InsertOneDept(FILE *fp, const INT8 *pchToken);

/*!
@brief 读TXT文本文件，并逐条记录插入Client表
@param[in] fp   文本文件指针
@param[in] pchToken   文本文件中隔开各个字段的标记符
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 InsertOneClient(FILE *fp, const INT8 *pchToken);

/*!
@brief 部类数据导出过程
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 DeptExportProc();

/*!
@brief 付款人数据导出过程
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ClientExportProc();

/*!
@brief 数据导出核心操作(导出sql脚本文件)
@param[in] strCmd   导出PLU表到U盘文件的shell指令
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ExportSqlHandle(string strCmd);

/*!
@brief 数据导入核心操作(导出TXT文本文件)
@param[in] nKind   导出的数据表  1：PLU表； 2：DEPT表 3：Client表
@return  1: SUCCESS; 0: FAILURE
*/
UINT8 ExportTxtHandle(UINT8 nKind);

/*!
@brief 组装添加一条Client记录的TXT语句
@param[in]  pClient    付款人信息
@param[out] strTxt  组装好的文本语句
@return  1： SUCCESS； 0：FAILURE
*/
INT8 ComposeClientTxt(const CClient *pClient, string &strTxt);

/*!
@brief 组装添加一条Dept记录的TXT语句
@param[in]  pDept    部类信息
@param[out] strTxt  组装好的文本语句
@return  1： SUCCESS； 0：FAILURE
*/
INT8 ComposeDeptTxt(const CDept *pDept, string &strTxt);

/*!
@brief 从数据库里读一条部类记录，然后写入文件
@param[in]  fp    文件句柄
@return  1： SUCCESS； 0：FAILURE
*/
UINT8 OutputOneDept(FILE *fp);

/*!
@brief 从数据库里读一条付款人记录，然后写入文件
@param[in]  fp    文件句柄
@return  1： SUCCESS； 0：FAILURE
*/
UINT8 OutputOneClient(FILE *fp);



#endif

