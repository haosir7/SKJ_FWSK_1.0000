/*! \file    YWXML_JKHC.h
   \brief    款机调用的中间件接口 业务2.18监控回传
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_JKHC_H
#define YWXML_JKHC_H


#include "IncludeMe.h"
#include "YWXmlBase.h"

/**
 *@class CJkhc
 *@brief 监控回传
 */
class CJkhc : public CYWXmlBase
{
public:
	/*!
	@brief 2.18.监控回传
	@param[in] ywlx 		业务类型
	@param[in]  strFpjkmw	发票监控密文

	*/
	CJkhc(CYWXML_GY &ywxml_gy, string strFpjkmw);
	~CJkhc();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	string 	m_strFpjkmw;
// 	string 	&m_strSzjkmw;

private:
		
};


#endif































