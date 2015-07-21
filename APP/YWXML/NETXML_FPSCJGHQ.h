/*! \file    NETXML_FPSCJGHQ.h
   \brief    款机调用的中间件接口 业务: 发票明细上传结果获取
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef NETXML_FPSCJGHQ_H
#define NETXML_FPSCJGHQ_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CFpscjghq
 *@brief 发票明细上传结果获取
 */
class CFpscjghq: public CYWXmlBase
{

public:


	/*!
	@brief 发票明细上传结果获取
	@param[in] ywlx 业务类型
	@param[in]  FpmxCount	发票明细数
	@param[out] Mxjgmw 加密后的明细上传结果密文 
	@param[out] Count 上传发票明细个数
	*/
	CFpscjghq(CYWXML_GY &ywxml_gy, string qtxx, string &Mxjgmw);
	~CFpscjghq();
	
	/*!
	@brief 发票明细上传结果获取：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 发票明细上传结果获取：输出参数解析（解析XML文件） 
	@param[out] Mxjgmw 加密后的明细上传结果密文 
	@param[out] Count 上传发票明细个数
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();
	
	string &m_Mxjgmw;
	UINT32 m_Count;
	UINT32 m_FpCount;
	string m_Slxlh;
	string m_Qtxx;
};


#endif



