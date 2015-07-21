/*! \file    NETXML_WLLQFPJGQR.h
   \brief    款机调用的中间件接口 业务3.7 网络领取发票结果确认
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef NETXML_WLLQFPJGQR_H
#define NETXML_WLLQFPJGQR_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

/**
 *@class CWllqfpjgqr
 *@brief 网络领取发票结果确认
 */
class CWllqfpjgqr : public CYWXmlBase
{
public:
	/*!
	@brief 3.7. 网络领取发票结果确认
	@param[in] ywlx 		业务类型
	@param[in] invvol		发票领用信息
	@param[in]  strQtxx		其他信息
	*/

	CWllqfpjgqr(CYWXML_GY &ywxml_gy, CInvVol &invvol, string &strQtxx);
	~CWllqfpjgqr();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CInvVol &m_invvol;
	string	&m_strQtxx;
private:
	
};


#endif

