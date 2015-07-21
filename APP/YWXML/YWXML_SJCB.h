/*! \file    YWXML_SJCB.h
   \brief    款机调用的中间件接口 业务2.17数据抄报
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SJCB_H
#define YWXML_SJCB_H


#include "IncludeMe.h"
#include "YWXmlBase.h"

/**
 *@class CSjcb
 *@brief 数据抄报
 */
class CSjcb : public CYWXmlBase
{
public:
	/*!
	@brief 2.17.数据抄报
	@param[in] ywlx 业务类型
	@param[in] jzlx 介质类型
	@param[out]  strSq			属期
	@param[out]  strFpmx		发票明细数据
	@param[out]  strFpdxx		发票段信息
	@param[out]  strFphz		发票汇总数据
	@param[out]  strSzfphz		时钟发票汇总数据
	@param[out]  strQtxx		其他信息
	*/

//	CSjcb(CYWXML_GY &ywxml_gy, string &strSq, string &strFpmx, string &strFpdxx, string &strFphz, string &strSzfphz, string &strQtxx);
	CSjcb(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strSq, string &strFphz);
	~CSjcb();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	string &m_strSq;
	string &m_strFphz;

	string m_strFpmx;
	string m_strFpdxx;
	string m_strSzfphz;
	string m_strQtxx;

	UINT8 m_jzlx;

private:
	
};


#endif























