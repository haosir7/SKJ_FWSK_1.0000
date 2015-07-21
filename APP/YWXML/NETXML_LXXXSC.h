/*! \file    NETXML_LXXXSC.h
   \brief    款机调用的中间件接口 业务3.5 离线信息上传
   \author   lzh
   \version  1.0
   \date     2015 
*/

#ifndef NETXML_LXXXSC_H
#define NETXML_LXXXSC_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvKind.h"
#include "SysMacDef.h"

/**
 *@class CLxxxsc
 *@brief 离线信息上传
 */
class CLxxxsc : public CYWXmlBase
{
public:
	/*!
	@brief 3.5. 离线信息上传
	@param[in] ywlx 		业务类型
	@param[in]  invkind		纳税人票种表
	@param[in]  strScjzrq	上传截止日期
	@param[in]  strQtxx		其他信息
	@param[out]  strLzkzxx		离线控制信息
	*/
	CLxxxsc(CYWXML_GY &ywxml_gy, CInvKind &invkind, string &strQtxx, string &strLzkzxx);
	~CLxxxsc();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	const CInvKind &m_invkind;
	const string &m_strQtxx;
	string &m_strLxkzxx;
private:
	
};


#endif

