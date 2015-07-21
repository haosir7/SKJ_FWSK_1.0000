#ifndef __CCLIENT_H
#define __CCLIENT_H

#include "CTable.h"
#include "CDB.h"

/**
 *@class CClient 
 *@brief 客户管理信息
 *@brief 对经常使用的付款人名称进行简称管理
 */
class CClient : public CTable
{
public:

	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CClient();

    /*!
	@brief 析构函数	
	*/
	~CClient();

	string m_khbh;		/**< 本地编号 */
	string m_khdm;		/**< 客户代码 */
	string m_khjm; 		/**< 代码简码 */
	string m_khmc;		/**< 客户名称 */
	string m_khsh;		/**< 税号 */
};

#endif
