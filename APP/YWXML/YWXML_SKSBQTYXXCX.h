/*! \file    YWXML_SKSBQTYXXCX.h
   \brief    款机调用的中间件接口 业务: 税控设备其他信息查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SKSBQTYXXCX_H
#define YWXML_SKSBQTYXXCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"

#include "CInvVol.h"


/**
 *@class CSksbqtxxcx
 *@brief 税控设备其他信息查询
 */
class CSksbqtxxcx : public CYWXmlBase
{

public:

	/*!
	@brief 税控设备其他信息查询
	@param[in] ywlx 业务类型
	@param[in] xxlx	信息类型
	@param[out] sksbxx 税控设备信息
	*/
	CSksbqtxxcx(CYWXML_GY &ywxml_gy, UINT8 xxlx, string &sksbxx);
	~CSksbqtxxcx();


	/*!
	@brief 税控设备其他信息查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 税控设备其他信息查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	UINT8 m_xxlx;	//信息类型
	string &m_sksbxx; //税控设备信息
};


#endif



