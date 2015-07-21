/*! \file    YWXML_BSPFPSH.h
   \brief    款机调用的中间件接口 业务2.16 发票收回
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_BSPFPSH_H
#define YWXML_BSPFPSH_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

/**
 *@class CBspfpsh
 *@brief 发票收回
 */
class CBspfpsh: public CYWXmlBase
{
public:
	/*!
	@brief 2.16.发票收回
	@param[in] ywlx 		业务类型
	@param[in] invvol		发票领用信息
	@param[in]  strJzlx	介质类型
	*/
	CBspfpsh(CYWXML_GY &ywxml_gy, CInvVol &invvol, UINT8 strJzlx);
	~CBspfpsh();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CInvVol &m_invvol;
	UINT8 m_strJzlx;
	

private:
	
};


#endif

