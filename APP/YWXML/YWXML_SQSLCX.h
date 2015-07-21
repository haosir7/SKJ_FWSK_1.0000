/*! \file    YWXML_SQSLCX.h
   \brief    款机调用的中间件接口 业务:授权税率查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SQSLCX_H
#define YWXML_SQSLCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"

#include "CTax.h"

class CTaxInfo
{
public:
		CTax m_Tax[MAX_TAX_NUM];
		UINT8 m_TaxCoun;

		CTaxInfo();
		~CTaxInfo();
};

/**
 *@class CSqslcx
 *@brief 授权税率查询
 */
class CSqslcx : public CYWXmlBase
{

public:

	/*!
	@brief	授权税率查询
	@param[in] ywlx 业务类型
	@param[out] Tax 税率信息
	*/
	CSqslcx(CYWXML_GY &ywxml_gy, CTaxInfo &TaxInfo);
	~CSqslcx();

	/*!
	@brief 授权税率查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 授权税率查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	CTaxInfo &m_TaxInfo;

};


#endif



