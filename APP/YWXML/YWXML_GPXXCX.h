/*! \file    YWXML_GPXXCX.h
   \brief    款机调用的中间件接口 业务: 购票信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_GPXXCX_H
#define YWXML_GPXXCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"

#include "CInvVol.h"


/**
 *@class CGpxxcx
 *@brief 购票信息查询
 */
class CGpxxcx : public CYWXmlBase
{

public:

	/*!
	@brief 购票信息查询
	@param[in] ywlx 业务类型
	@param[out] InvVol 发票信息
	*/
	CGpxxcx(CYWXML_GY &ywxml_gy, CInvVol &InvVol);
	~CGpxxcx();


	/*!
	@brief 购票信息查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 购票信息查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	CInvVol &m_InvVol;
};


#endif



