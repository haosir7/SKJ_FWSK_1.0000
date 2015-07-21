#ifndef __CINV_SUM_H
#define __CINV_SUM_H

#include "CTable.h"
#include "CDB.h"


/**
 *@class CInvSum 
 *@brief 单卷发票信息统计表
 *@brief 
 */
class CInvSum : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvSum();

    /*!
	@brief 析构函数	
	*/
	~CInvSum();

	/*!
	@brief 判断是否滚动，即表内记录太多时，是否删除部分记录
	@param[in] nCount 表实际存储的记录个数
	@param[in] strRefDate 上次申报数据的截止日期
	@param[out] nDelDate 实际要删除记录的截止日期
	@return  0 失败，  1 成功
	*/
	INT32 CheckRoll(INT32 nCount, UINT32 strRefDate, UINT32 &nDelDate);

	/*!
	@brief 滚动，即表内记录太多时，删除部分记录
	@param[in] nDelDate 实际要删除记录的截止日期
	@return  0 失败，  1 成功
	*/
	INT32 Roll(UINT32 nDelDate);
    

	UINT32 m_no; 			/**< 索引号 */
	UINT8 	    m_Type;       			/*发票类型*/

	UINT32		m_Qsrq;			/**< 起始日期*/
	UINT32		m_Jzrq;			/**< 截止日期*/
	UINT32	    m_OpenInventory;		 /*期初库存*/  
	UINT32    	m_NewBuy; 			/*本期新购*/ 
	UINT32    	m_EndInventory; 		/*期末库存*/
	UINT32     	m_ReBack; 				/*本期退回*/
	UINT32     	m_InvNum; 			/*正数发票开具份数*/
	UINT32      m_InvCancelNum; 		/*正数发票作废份数*/
	UINT32      m_NegInvNum; 			/*负数发票开具份数*/
	UINT32      m_NegInvCancelNum; 		/*负数发票作废份数*/
	UINT32		m_Kffpfs;			/**< 空废发票份数 */

	INT64     	m_InvSum; 			/*正数发票累计金额*/
	INT64    	m_InvTax; 			/*正数发票累计税额*/
	INT64		m_Zffpljje;		/**< 正废发票累计金额 */
	INT64		m_Zffpljse;		/**< 正废发票累计税额 */
	INT64     	m_NegInvSum; 		/*负数发票累计金额*/
	INT64     	m_NegInvTax; 		/*负数发票累计税额*/
	INT64		m_Fffpljje;		/**< 负废发票累计金额 */
	INT64		m_Fffpljse;		/**< 负废发票累计税额 */

};

#endif