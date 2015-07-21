#ifndef __CPRN_INFO_H
#define __CPRN_INFO_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CPrnInfo 
 *@brief 打印发票时填充的额外信息表
 *@brief 
 */
class CPrnInfo : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CPrnInfo();

    /*!
	@brief 析构函数	
	*/
	~CPrnInfo();
    
	string m_zdyTab1;			/**< 自定义名称1 */
	string m_zdyCont1;		/**< 自定义内容1 */
	string m_zdyTab2;			/**< 自定义名称2 */
	string m_zdyCont2;		/**< 自定义内容2 */
	string m_remarks;			/**< 备用字段 */
	string m_backup;			/**< 备用 */
};

#endif

