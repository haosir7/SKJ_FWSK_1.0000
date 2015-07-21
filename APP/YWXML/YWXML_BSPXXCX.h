/*! \file    YWXML_BSPXXCX.h
   \brief    款机调用的中间件接口 业务: 报税盘信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_BSPXXCX_H
#define YWXML_BSPXXCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CUserInfo.h"


/**
 *@class CBspxxcx
 *@brief 报税盘信息查询
 */
class CBspxxcx : public CYWXmlBase
{

public:

	/*!
	@brief 报税盘信息查询
	@param[in] ywlx		业务类型
	@param[out] sksbbh  报税盘编号
	*/
	CBspxxcx(CYWXML_GY &ywxml_gy, CUserInfo &userInfo);
	~CBspxxcx();

	/*!
	@brief 报税盘信息查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 报税盘信息查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	CUserInfo &m_UserInfo;


	private:
};


#endif



