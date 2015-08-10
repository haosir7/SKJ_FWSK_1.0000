/*! \file    YWXML_ERRUPINV.h
   \brief    款机调用的中间件接口 业务 获取上传出错的发票信息
   \author   myf
   \version  1.0
   \date     2015 
*/

#ifndef YWXML_ERRUPINV_H
#define YWXML_ERRUPINV_H


#include "IncludeMe.h"
#include "YWXmlBase.h"
#include "CInvServ.h"


class CErrInvInfo
{
public:
	CDataInvServ InvUpFailInfo[MAX_ERR_INV_COUNT];
	UINT32 m_ErrUpNum;
	
	CErrInvInfo();
	~CErrInvInfo();
};



/**
 *@class CErrUpInv
 *@brief 获取上传出错的发票信息
 */
class CErrUpInv : public CYWXmlBase
{
public:
	/*!
	@brief 获取上传出错的发票信息
	@param[in] ywlx 		业务类型
	*/

	CErrUpInv(CYWXML_GY &ywxml_gy, CErrInvInfo &ErrInvInfo);
	~CErrUpInv();
	
	INT32 XmlBuild();
	INT32 XmlParse();

	CErrInvInfo &m_ErrInvInfo;


private:
	
};


#endif













































