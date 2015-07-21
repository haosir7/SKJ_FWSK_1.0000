/*! \file    YWXML_GPXXXPJS.h
   \brief    款机调用的中间件接口 业务: 购票信息写盘解锁
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_GPXXXPJS_H
#define YWXML_GPXXXPJS_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CGpxxxpjs
 *@brief 购票信息写盘解锁
 */
class CGpxxxpjs: public CYWXmlBase
{

public:

	/*!
	@brief 购票信息写盘解锁
	@param[in] ywlx 业务类型
	@param[in] wslpjsxx 网上领票解锁信息
	*/
	CGpxxxpjs(CYWXML_GY &ywxml_gy, string wslpjsxx);
	~CGpxxxpjs();


	/*!
	@brief 购票信息写盘解锁：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 购票信息写盘解锁：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();	
	
	string m_wslpjsxx;
};


#endif



