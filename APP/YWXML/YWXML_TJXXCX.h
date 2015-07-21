/*! \file    YWXML_TJXXCX.h
   \brief    款机调用的中间件接口 业务2.19统计信息查询
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_TJXXCX_H
#define YWXML_TJXXCX_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CTjxxhz.h"
#include "SysMacDef.h"

/**
 *@class CTjxxcx
 *@brief 统计信息查询
 */
class CTjxxcx : public CYWXmlBase
{
public:
	/*!
	@brief 2.19.统计信息查询
	@param[in] ywlx 		业务类型
	@param[in&out]  tjxxhz	统计信息汇总类
	*/

	CTjxxcx(CYWXML_GY &ywxml_gy, CTjxxhz &tjxxhz);
	~CTjxxcx();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CTjxxhz 	&m_tjxxhz;
private:
	
};


#endif


