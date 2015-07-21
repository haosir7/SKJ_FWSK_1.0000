/*! \file    YWXML_KLBG.h
   \brief    款机调用的中间件接口 业务: 金税盘口令变更
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_KLBG_H
#define YWXML_KLBG_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CKlbg
 *@brief 金税盘口令变更
 */
class CKlbg: public CYWXmlBase
{

public:


	/*!
	@brief金税盘口令变更
	@param[in] ywlx 业务类型
	@param[in] ykl 盘原口令 
	@param[in] xkl 盘新口令
	*/
	CKlbg(CYWXML_GY &ywxml_gy, string ykl, string xkl);
	~CKlbg();


	/*!
	@brief 金税盘口令变更：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();

	/*!
	@brief 金税盘口令变更：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();

	string m_ykl;
	string m_xkl;
};


#endif



