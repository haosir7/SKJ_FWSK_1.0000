/*! \file    YWXML_JKSJCX.h
\brief    款机调用的中间件接口 业务: 监控数据查询
\author   myf
\version  1.0
\date     2015 
*/

#ifndef YWXML_JKSJCX_H
#define YWXML_JKSJCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CUserInfo.h"
#include "CInvKind.h"
//#include "JSKManageProc.h"

/**
 *@class CJksjcx
 *@brief 监控数据查询
 */
class CJksjcx: public CYWXmlBase
{

public:

	/*!
	@brief		监控数据查询
	@param[in] ywlx 业务类型
	@param[out] Userinfo 纳税户信息
	@param[out] InvKind 发票种类信息
	*/
	CJksjcx(CYWXML_GY &ywxml_gy, CInvKind &InvKind);
	~CJksjcx();
	
	/*!
	@brief 监控数据查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 监控数据查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();	

	CInvKind &m_InvKind;

private:
};


#endif



