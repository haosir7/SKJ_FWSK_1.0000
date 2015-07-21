/*! \file    YWXML_SKPXXCX.h
   \brief    款机调用的中间件接口 业务2.2.税控设备信息查询
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SKPXXCX_H
#define YWXML_SKPXXCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CUserInfo.h"

/**
 *@class CSkpxxcx
 *@brief 2.2.税控设备信息查询
 */
class CSkpxxcx:  public CYWXmlBase
{

public:

	/*!
	@brief 2.2.税控设备信息查询
	@param[in] ywlx 业务类型
	@param[out] Userinfo 纳税户信息
	@param[out] strCurTime 当前时间YYYYMMDDHHMMSS
	*/
	CSkpxxcx(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime);
	~CSkpxxcx();

	/*!
	@brief 税控设备信息查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 税控设备信息查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();
	
	CUserInfo &m_UserInfo;	//纳税户信息
	string	  &m_strCurTime;	//当前时间YYYYMMDDHHMMSS

	private:
		
};


#endif



