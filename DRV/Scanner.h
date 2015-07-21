
/*! \file   Scanner.h
   \brief   扫描枪驱动
   \author  
 */
#ifndef _SCANNER_H
#define _SCANNER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@brief     初始化扫描枪 
@return    SUCCESS 初始化正确；FAILURE	初始化错误；
*/
int ScannerInit(void);

/*!
@brief      释放扫描枪设备
@return     SUCCESS 正确；FAILURE	错误；
*/
int ScannerRelease(void);

/*!
@brief      检查扫描枪设备是否初始化 
@return   	SUCCESS	 已经初始化；FAILURE  未初始化
*/
int ScannerStateCheck(void);

/*!
@brief       生成扫描枪条码值（必须指定条码长度）
@param[in]   length	 条码长度
@return      SUCCESS 有扫描码；FAILURE	无扫描码； 
*/
int ScannerPressed_mode1(char length);

/*!
@brief      生成扫描枪条码值（不必指定条码长度，但需要给定条码字节包的包尾标识）
@param[in]  str	条码包尾标识
@return     SUCCESS	 有扫描码；FAILURE	无扫描码；
*/
int ScannerPressed_mode2(char *str);

/*!
@brief   返回接收BUF指针
@return  扫描的条码buf指针
*/
unsigned char *ScannerCode(void);

/*!
@brief   清除缓冲区
*/
void ClearScannerBuf();

#ifdef __cplusplus
}
#endif

#endif
