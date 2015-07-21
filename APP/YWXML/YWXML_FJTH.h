/*! \file    YWXML_FJTH.h
   \brief    款机调用的中间件接口 业务2.24 分机退回
   \author   zfj
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FJTH_H
#define YWXML_FJTH_H


#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CFjth
 *@brief 分机退回
 */
class CYWXMLFjth : public CYWXmlBase
{
public:
	/*!
	@brief 2.24.分机退回
	@param[in] ywlx 		业务类型
	@param[in]  strJzlx	介质类型
	*/
	CYWXMLFjth(CYWXML_GY &ywxml_gy, UINT8 strJzlx);
	~CYWXMLFjth();
	
	INT32 XmlBuild();
	INT32 XmlParse();
	
	UINT8 	m_strJzlx;
private:
	
};


#endif



