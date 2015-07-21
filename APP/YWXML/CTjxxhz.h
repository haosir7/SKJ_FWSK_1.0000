#ifndef __CTJXXHZ_H
#define __CTJXXHZ_H

#include "IncludeMe.h"

//#include "CTable.h"
//#include "CDB.h"


/**
 *@class CTjxxhz
 *@brief 统计信息汇总表
 *@brief 
 */
class CTjxxhz
{
public:
    
	/*!
	@brief 构造函数	
	*/ 
	CTjxxhz();

    	/*!
	@brief 析构函数	
	*/
	~CTjxxhz();
		
public:
	UINT32  m_Qsrq;		/**< 起始日期*/
	UINT32  m_Jzrq;		/**< 截止日期*/
	UINT32  m_Qckcfs;	/**< 期初库存份数 */
	UINT32  m_Lgfpfs;	/**< 领购发票份数 */
	UINT32  m_Thfpfs;	/**< 退回发票份数 */
	UINT32  m_Zsfpfs;	/**< 正数发票份数 */
	UINT32  m_Zffpfs;		/**< 正废发票份数 */
	UINT32  m_Fsfpfs;	/**< 负数发票份数 */
	UINT32  m_Fffpfs;		/**< 负废发票份数 */
	UINT32  m_Kffpfs;	/**< 空废发票份数 */
	UINT32  m_Qmkcfs;	/**< 期末库存份数 */
	INT64	m_Zsfpljje;	/**< 正数发票累计金额 */
	INT64	m_Zsfpljse;	/**< 正数发票累计税额 */
	INT64	m_Zffpljje;	/**< 正废发票累计金额 */
	INT64	m_Zffpljse;	/**< 正废发票累计税额 */
	INT64	m_Fsfpljje;	/**< 负数发票累计金额 */
	INT64	m_Fsfpljse;	/**< 负数发票累计税额 */
	INT64	m_Fffpljje;	/**< 负废发票累计金额 */
	INT64	m_Fffpljse;	/**< 负废发票累计税额 */
	
};

#endif
