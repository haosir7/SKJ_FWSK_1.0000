#ifndef __CRT_INV_H
#define __CRT_INV_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CRtInv 
 *@brief 已退发票信息表
 *@brief 
 */
class CRtInv : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CRtInv();

    /*!
	@brief 析构函数	
	*/
	~CRtInv();

	/*!
	@brief 滚动判断，即表内记录太多时，是否删除部分记录
	@param[in] nCount 表实际存储的记录个数
	@param[out] nDelNo 实际要删除记录的截止序号
	@return  0 失败，  1 成功
	*/
	INT32 CheckRoll(INT32 nCount, UINT32 &nDelNo);

	/*!
	@brief 滚动，即表内记录太多时，删除部分记录
	@param[in] nDelNo 实际要删除记录的截止序号
	@return  0 失败，  1 成功
	*/
	INT32 Roll(UINT32 nDelNo);
  
	
	UINT32 m_no; 		/**< 索引号 */	
	string m_fpdm;		/**< 红票代码 */
	UINT32 m_fphm;		/**< 红票号码 */
};

#endif
