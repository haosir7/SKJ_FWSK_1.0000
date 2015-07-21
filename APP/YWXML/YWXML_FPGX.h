/*! \file    YWXML_FPGX.h
   \brief    款机调用的中间件接口 业务2.22.发票更新
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPGX_H
#define YWXML_FPGX_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CFpgx
 *@brief 2.22.发票更新
 */
class CFpgx : public CYWXmlBase
{
public:
	/*!
	@brief 2.15.发票分发
	@param[in] ywlx 		业务类型
	@param[in] strScqrbw		上传成功确认报文
	*/
	CFpgx(CYWXML_GY &ywxml_gy, string strScqrbw);
	~CFpgx();
	
	
	INT32 XmlBuild();
	INT32 XmlParse();

	string 	m_strScqrbw;


private:
	
};


#endif



