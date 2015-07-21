#ifndef __COPERATOR_H
#define __COPERATOR_H

#include "CTable.h"
#include "CDB.h"


#define ADMINISTRATOR_ROLE   1  //系统管理员角色
#define DIRECTOR_ROLE		 2  //主管操作员角色
#define NORMAL_ROLE			 3  //普通操作员角色
#define DEMO_ROLE			 4  //学习模式角色


/**
 *@class COperator 
 *@brief 操作员信息表
 *@brief 
 */
class COperator : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	COperator();

    /*!
	@brief 析构函数	
	*/
	~COperator();
	
	UINT8   m_opid;		/**< 编码 */
	string  m_name;		/**< 名称 */
	string  m_passwd;	/**< 密码 */
	UINT8   m_role;		/**< 角色 */
};

#endif
