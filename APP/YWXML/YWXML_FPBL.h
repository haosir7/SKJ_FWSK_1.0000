/*! \file    YWXML_FPBL.h
   \brief    款机调用的中间件接口 业务2.28.发票补录
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPBL_H
#define YWXML_FPBL_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CFpbl
 *@brief 2.28.发票补录
 */
class CFpbl : public CYWXmlBase
{
public:
	/*!
	@brief 2.28.发票补录
	@param[in] ywlx 		业务类型
	@param[in]  StartDate	报税起始日期
	@param[in]  EndDate		报税截止日期
	*/
	CFpbl(CYWXML_GY &ywxml_gy, UINT32 StartDate, UINT32 EndDate);
	~CFpbl();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	UINT32 	m_StartDate;
	UINT32 	m_EndDate;	
private:
	
};


#endif



