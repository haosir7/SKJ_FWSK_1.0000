#ifndef __CSYSLOG_H
#define __CSYSLOG_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CSysLog 
 *@brief 系统日志信息表
 *@brief 
 */
class CSysLog : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CSysLog();

    /*!
	@brief 析构函数	
	*/
	~CSysLog();

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

	/*!
	@brief 统计SYSLOG表存储的发票数
	@return  >=0: 发票数; -1: 统计失败
	*/
	INT32 GetSysLogNum();
  
	
	UINT32 m_no; 		/**< 自增 */
	UINT32 m_idate;		/**< 日期 */
	UINT32 m_itime;		/**< 时间 */
	string m_operator;	/**< 操作员名称 */
	UINT8   m_type;		/**< 操作类型 */
	UINT32 m_result;	/**< 结果代码 */
	string m_backup;	/**< 备用 */
};

#endif
