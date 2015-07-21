/*! \file    YWXML_SKSBBHCX.h
   \brief    款机调用的中间件接口 业务2.1 税控设备编号查询
   \author   YY
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_SKSBBHCX_H
#define YWXML_SKSBBHCX_H


#include "IncludeMe.h"
//#include "xmlbase.h"

#include "YWXmlBase.h"


/**
 *@class CSksbbhcx
 *@brief 2.1.税控设备编号查询
 */
class CSksbbhcx : public CYWXmlBase
{


public:

	/*!
	@brief 2.1.税控设备编号查询
	@param[in] ywlx 业务类型
	@param[out] sksbbh 税控设备编号
	@param[out] sksbbh 报税盘编号
	*/
	CSksbbhcx(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx);
	~CSksbbhcx();
	
	/*!
	@brief 税控设备编号查询：输入参数组装（组装成XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlBuild();
	
	/*!
	@brief 税控设备编号查询：输出参数解析（解析XML文件）
	@return 1  SUCCESS， 0  FAILURE
	*/
	INT32 XmlParse();
	string &m_sksbbh;
	string &m_bspbh;
	string &m_qtxx;

private:
	
		
};


#endif



