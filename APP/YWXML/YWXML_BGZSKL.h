/*! \file    YWXML_BGZSKL.h
   \brief    款机调用的中间件接口 业务: 口令变更
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_BGZSKL_H
#define YWXML_BGZSKL_H

#include "IncludeMe.h"
#include "YWXmlBase.h"

/**
 *@class CBgzskl
 *@brief 变更证书口令
 */
class CBgzskl: public CYWXmlBase
{
public:
	/*!
	@brie证书口令变更
	@param[in] ywlx 业务类型
	@param[in] yzskl 原证书口令 
	@param[in] xzskl 新证书口令
	*/
	CBgzskl(CYWXML_GY &ywxml_gy, string yzskl, string xzskl);
	~CBgzskl();

	/*!
	@brief 证书口令变更：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();

	/*!
	@brief 证书口令变更：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	string m_yzskl;
	string m_xzskl;
};

#endif



