/*! \file    NETXML_WLLQFP.h
   \brief    款机调用的中间件接口 业务3.6 网络领取发票
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef NETXML_WLLQFP_H
#define NETXML_WLLQFP_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

/**
 *@class CWllqfp
 *@brief 网络领取发票
 */
class CWllqfp : public CYWXmlBase
{
public:
	/*!
	@brief 3.6. 网络领取发票
	@param[in] ywlx 		业务类型
	@param[in&out] invvol		发票领用信息
	@param[in]  strQtxx		其他信息
	*/
	CWllqfp(CYWXML_GY &ywxml_gy, CInvVol &invvol, const string &strQtxx);
	~CWllqfp();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CInvVol &m_invvol;
	const string &m_strQtxx;
	
private:
	
};


#endif


