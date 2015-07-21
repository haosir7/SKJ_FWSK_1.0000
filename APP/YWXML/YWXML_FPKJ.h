/*! \file    YWXML_FPKJ.h
   \brief    款机调用的中间件接口 业务: 发票开具
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPKJ_H
#define YWXML_FPKJ_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvHead.h"
#include "CInvDet.h"

/**
 *@class CFpkj
 *@brief 发票开具
 */
class CFpkj: public CYWXmlBase
{

public:

	/*!
	@brief 发票
	@param[in] ywlx 业务类型
	@param[out] InvHead 开票返回信息
	*/
	CFpkj(CYWXML_GY &ywxml_gy, CInvHead &InvInfo);
	~CFpkj();

	/*!
	@brief 发票开具：输入参数组装（组装成XML文件）
	@param[in]  InvInfo	  开票信息
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 发票开具：输出参数数解析（解析XML文件）
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();
	
	CInvHead &m_InvInfo;
};


#endif



