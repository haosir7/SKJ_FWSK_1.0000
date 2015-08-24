/*! \file    YWXML_HQLXSJ.h
   \brief    款机调用的中间件接口 业务 2.28.获取离线相关数据
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_HQLXSJ_H
#define YWXML_HQLXSJ_H


#include "IncludeMe.h"
#include "YWXmlBase.h"



/**
 *@class CGetOffLineData
 *@brief 获取离线相关数据
 */
class CGetOffLineData : public CYWXmlBase
{
public:
	/*!
	@brief 获取离线相关数据
	@param[in] ywlx 		业务类型
	*/

	CGetOffLineData(CYWXML_GY &ywxml_gy, UINT32 &wscfpzs, string &wscfpsj, INT64 &wscfpljje);
	~CGetOffLineData();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	UINT32 &m_wscfpzs;
	string &m_wscfpsj;
	INT64 &m_wscfpljje;

private:
	
};


#endif













































