#ifndef __CUSERMORE_H
#define __CUSERMORE_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CUserMore 
 *@brief 额外用户信息表
 *@brief 
 */
class CUserMore : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CUserMore();

    /*!
	@brief 析构函数	
	*/
	~CUserMore();

    void  ResetMember(); //清空各个变量

	string m_backup1;			/**< 备用 */
	string m_backup2;			/**< 备用 */
	string m_backup3;			/**< 备用 */
	string m_backup4;			/**< 备用 */
	string m_backup5;			/**< 备用 */
	string m_backup6;			/**< 备用 */
	string m_backup7;			/**< 备用 */
};

#endif

