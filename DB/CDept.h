#ifndef __CDEPT_H
#define __CDEPT_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CDept 
 *@brief 商品信息表
 *@brief 
 */
class CDept : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CDept();

    /*!
	@brief 析构函数	
	*/
	~CDept();
	
	string m_spbm;		/**< 编码 */
	string m_spmc;		/**< 名称 */
	double m_spdj;		/**< 单价 */
	float  m_spsl;		/**< 税率 */
	string m_spdw;		/**< 单位 */
	UINT32 m_blh;       /**< 对应的部类号，默认为零 */

	string m_spjm;		/**< 简称 */
	string m_spgg;		/**< 规格 */
	string m_zspmdm;	/**< 征收商品代码(税目) */
	float  m_spzsl;		/**< 征收率 */
	float  m_rate;		/**< 商品折扣率 */

};

#endif
