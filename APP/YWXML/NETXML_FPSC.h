/*! \file    NETXML_FPSC.h
   \brief    款机调用的中间件接口 业务: 发票上传
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef NETXML_FPSC_H
#define NETXML_FPSC_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CFpsc
 *@brief 发票上传
 */
class CFpsc: public CYWXmlBase
{

public:

	/*!
	@brief  发票明细上传
	@param[in] ywlx 业务类型
	@param[in]  Fpmx	发票明细
	@param[in]  FpmxCount	发票明细数
	@param[in]  Fpzs		发票张数
	@param[out] Count
	*/
	CFpsc(CYWXML_GY &ywxml_gy, const string &Fpmx, UINT32 Fpzs, string &Slxlh);
	~CFpsc();

	/*!
	@brief 发票明细上传：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 发票明细上传：输出参数解析（解析XML文件） 
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();	

	const string &m_Fpmx;
	UINT32 m_FpCount;
	UINT32 m_Fpzs;
	UINT32 m_Count;
	string &m_Slxlh;

};


#endif



