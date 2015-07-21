/*! \file    YWXML_FPSSSC.h
   \brief    款机调用的中间件接口 业务2.21.发票实时上传
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPSSSC_H
#define YWXML_FPSSSC_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CFpsssc
 *@brief 2.21.发票实时上传
 */
class CFpsssc : public CYWXmlBase
{
public:
	/*!
	@brief 2.21.发票实时上传
	@param[in] ywlx 		业务类型
	@param[in&out] nFpzs		发票张数
	@param[in&out] strCzlx		操作类型
	@param[out]  strInvBuf		发票明细
	*/

	CFpsssc(CYWXML_GY &ywxml_gy, UINT32 &nFpzs, UINT8 &strCzlx, string &strInvBuf);
	~CFpsssc();
	
	
	INT32 XmlBuild();
	INT32 XmlParse();

	UINT32 &m_nFpzs;
	UINT8 &m_strCzlx;
	string &m_strInvBuf;

private:
		
};


#endif



