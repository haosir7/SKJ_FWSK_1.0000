/*! \file    YWXML_FPDR.h
   \brief    款机调用的中间件接口 业务2.23.发票读入
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPDR_H
#define YWXML_FPDR_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CFpdr
 *@brief 2.23.发票读入
 */
class CFpdr : public CYWXmlBase
{
public:
	/*!
	@brief 2.23.发票读入
	@param[in] ywlx 		业务类型
	@param[in]  strJzlx	介质类型
	*/
	CFpdr(CYWXML_GY &ywxml_gy, UINT8 strJzlx);
	~CFpdr();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	UINT8 	m_strJzlx;
	
private:
	
};


#endif



