/*! \file    YWXML_BSPFPCX.h
   \brief    款机调用的中间件接口 业务2.14 报税盘发票查询
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_BSPFPCX_H
#define YWXML_BSPFPCX_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvVol.h"

class CInvVolInfo
{
public:
	CInvVol m_InvVol[INVVOL_MAX_NUM];
	UINT8 m_InvvolCount;
	
	CInvVolInfo();
	~CInvVolInfo();
};

/**
 *@class CBspfpcx
 *@brief 报税盘发票查询
 */
class CBspfpcx : public CYWXmlBase
{
public:
	/*!
	@brief 2.14.报税盘发票查询
	@param[in] ywlx 		业务类型
	@param[out] InvCount	发票领用卷数
	@param[out] invvol		发票领用信息
	@param[out] strLgry		领购人员
	*/

	CBspfpcx(CYWXML_GY &ywxml_gy, CInvVolInfo &invvolinfo);
	~CBspfpcx();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CInvVolInfo &m_invvolinfo;
	// string 	&m_strLgry;

private:
	
};


#endif













































