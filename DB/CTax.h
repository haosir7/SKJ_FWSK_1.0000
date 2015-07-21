#ifndef __CTax_H
#define __CTax_H

#include "CTable.h"
#include "CDB.h"




/**
 *@class CTax 
 *@brief 税率信息表
 *@brief 税率信息
 */
class CTax : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CTax();

    /*!
	@brief 析构函数	
	*/
	~CTax();

	 /*!
	@brief 析构函数	
	*/
	void ResetTax();

    UINT32 m_no;		/**< 序号 */
	UINT32 m_sl;        /**< 税率 */
	string m_backup;    /**< 备用 */	
};

#endif
