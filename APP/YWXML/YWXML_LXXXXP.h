/*! \file    YWXML_LXXXXP.h
   \brief    款机调用的中间件接口 业务2.20.离线信息写盘
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_LXXXXP_H
#define YWXML_LXXXXP_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CLxxxxp
 *@brief 2.20.离线信息写盘
 */
class CLxxxxp : public CYWXmlBase
{
public:
	/*!
	@brief 2.20.离线信息写盘
	@param[in] ywlx 		业务类型
	@param[in]  strLxxxmw 离线信息密文
	*/
	CLxxxxp(CYWXML_GY &ywxml_gy, string strLxxxmw);
	~CLxxxxp();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	string 	m_strLxxxmw;
	
private:
	
};


#endif



