/*! \file    YWXML_GPXXXP.h
   \brief    款机调用的中间件接口 业务: 购票信息写盘
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_GPXXXP_H
#define YWXML_GPXXXP_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CGpxxxp
 *@brief 购票信息写盘
 */
class CGpxxxp: public CYWXmlBase
{

public:

	/*!
	@brief 购票信息写盘
	@param[in] ywlx 业务类型
	@param[in] gpxxmw 购票信息密文
	*/
	CGpxxxp(CYWXML_GY &ywxml_gy, string gpxxmw);
	~CGpxxxp();
	

	/*!
	@brief 购票信息写盘：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 购票信息写盘：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();
	
	string m_gpxxmw;	//购票信息密文
};


#endif



