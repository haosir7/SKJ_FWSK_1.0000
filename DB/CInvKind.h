#ifndef __INV_KIND_H
#define __INV_KIND_H

#include "CTable.h"
#include "CDB.h"


/**
 *@class CInvKind
 *@brief 纳税人票种表
 *@brief 
 */
class CInvKind : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvKind();

    /*!
	@brief 析构函数	
	*/
	~CInvKind();

    /*!
	@brief 复位各成员	
	*/
	UINT8 ResetKind();

    /*!
	@brief 判断两个票种的信息是否相同	
	*/
	UINT8 CompareInvKind(CInvKind *p1, CInvKind *p2);


	UINT32 m_No; 			/**< 索引号 */
	string m_fplxdm;		/**< 发票类型代码 */	
	UINT32 m_Lxssr;			/**< 离线锁死日（每月的第几天） */
	UINT32 m_Lxkjsj;		/**< 离线开具时间（单位小时） */
	INT64 m_maxSign;		/**< 单张发票限额 */
	INT64 m_maxSum;			/**< 离线正数发票累计限额 */
	string m_backup;		/**< 备用字段 */


    string m_pzCode;		/**< 发票种类代码 */
	UINT8  m_fplx;			/**< 发票类型 */
	string m_bsqsrq;		/**< 数据报送起始日期YYYYMMDDHHMMSS */
	string m_bszzrq;		/**< 数据报送终止日期 YYYYMMDDHHMMSS*/

};

#endif

