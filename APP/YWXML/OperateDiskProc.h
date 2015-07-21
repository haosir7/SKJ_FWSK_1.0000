#ifndef __MAIN_PROC_H
#define __MAIN_PROC_H

#include <string>

using namespace std;

/*!
@brief 第2章 税控收款机与税控设备接口
@param[in]  sInputInfo   输入XML信息
@param[out] sOutputInfo  返回结果XML字符串
@return 
*/
void OperateDisk(string sInputInfo, string &sOutputInfo);


/*!
@brief 第3章 税控收款机与统一受理平台接口
@param[in]  sInputInfo   输入XML信息
@param[out] sOutputInfo  返回结果XML字符串
@return 
*/
void OperateNet(char *pin, string ip, string port, string serverpath, char *service_id,\
				string  nsrsbh, string sInputInfo, string &sOutputInfo, string &Err);



#endif
