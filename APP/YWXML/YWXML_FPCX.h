/*! \file    YWXML_FPCX.h
   \brief    款机调用的中间件接口 业务: 发票查询
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_FPCX_H
#define YWXML_FPCX_H

#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvHead.h"
#include "CUserInfo.h"

/**
 *@class CFpcx
 *@brief 发票查询
 */
class CFpcx: public CYWXmlBase
{

public:

	/*!
	@brief 发票查询
	@param[in] ywlx 业务类型
	@param[in]  cxfs	  查询方式
	@param[in]  cxtj     查询条件
	@param[out] Userinfo 发票信息
	@param[out] strurCTime 发票张数
	*/
	CFpcx(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, CInvHead &InvInfo, UINT32 &InvCOunt);
	~CFpcx();

	/*!
	@brief 发票查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 发票查询：输出参数解析（解析XML文件）
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();
	
	CInvHead &m_InvInfo;
// 	CUserInfo m_UserInfo;

	UINT32 &m_InvCOunt;
	UINT8 m_cxfs;
	string m_cxtj;
};



#endif



