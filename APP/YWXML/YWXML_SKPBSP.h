/*! \file    YWXML_SKPBSP.h
   \brief    款机调用的中间件接口 业务: 税控设备报税盘组合操作
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SKPBSP_H
#define YWXML_SKPBSP_H

#include "IncludeMe.h"
#include "YWXmlBase.h"


/**
 *@class CSkpBsp
 *@brief 税控设备报税盘组合操作
 */
class CSkpBsp: public CYWXmlBase
{

public:

	
	/*!
	@brief 税控设备报税盘组合操作
	@param[in] ywlx 业务类型
	@param[in]  Czlx 操作类型
	@param[in]  Hzxx 汇总信息
	*/
	CSkpBsp(CYWXML_GY &ywxml_gy, UINT8 Czlx, string Hzxx);
	~CSkpBsp();

private:

	/*!
	@brief 税控设备报税盘组合操作：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*! 
	@brief 税控设备报税盘组合操作：输出解析（解析XML文件）
	@return 1 SUCCESS， 0 FAILURE 
	*/
	INT32 XmlParse();	

	UINT8 m_Czlx;
	string m_Hzxx;
};


#endif



