/*! \file    YWXML_BSPFPFF.h
   \brief    款机调用的中间件接口 业务2.15 发票分发
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_BSPFPFF_H
#define YWXML_BSPFPFF_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

/**
 *@class CBspfpff
 *@brief 发票分发
 */
class CBspfpff : public CYWXmlBase
{
public:
	/*!
	@brief 2.15.发票分发
	@param[in] ywlx 		业务类型
	@param[in] invvol		发票领用信息
	@param[in]  strJzlx	介质类型
	*/
	CBspfpff(CYWXML_GY &ywxml_gy, CInvVol &invvol, UINT8 strJzlx);
	~CBspfpff();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CInvVol &m_invvol;
	UINT8 	m_strJzlx;
	
private:

};


#endif


