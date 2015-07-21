/*! \file    YWXML_FPZF.h
   \brief    款机调用的中间件接口 业务: 发票作废
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPZF_H
#define YWXML_FPZF_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvHead.h"

/**
 *@class CFpzf
 *@brief 发票作废
 */
class CFpzf: public CYWXmlBase
{

public:

	/*!
	@brief 发票作废
	@param[in] ywlx 业务类型
	@param[out] InvInfo 发票信息
	*/
	CFpzf(CYWXML_GY &ywxml_gy, UINT8 zflx, CInvHead &InvInfo);
	~CFpzf();
	
	/*!
	@brief 发票作废：输入参数组装（组装成XML文件）
	@param[in]  pInvInfo  发票内容
	@param[in] zflx  作废类型
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 发票作废：输出参数解析（解析XML文件）
	@param[in] pInvVol 指向发票信息表的指针 
	@param[in] retInfo 返回信息指针息类对象
	@param[out] construct  XML构造类
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();	

	CInvHead &m_InvInfo;
	UINT8 m_zflx;
};


#endif



