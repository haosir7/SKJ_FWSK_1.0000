#ifndef __CSYSARG_H
#define __CSYSARG_H

#include "CTable.h"
#include "CDB.h"

#define SYS_ARG_BLOB_LEN	20

/**
 *@class CSysArg 
 *@brief 系统参数表
 *@brief 
 */
class CSysArg : public CTable
{
public:

	/*!
	@brief 构造函数	
	*/
	CSysArg();

	/*!
	@brief 析构函数	
	*/
	~CSysArg();

	UINT8 	m_said;		/**< 系统参数号 */
	string	m_name;		/**< 系统参数名称 */
	UINT32	m_vInt;		/**< 系统参数整型值 */
	string	m_vText;	/**< 系统参数字符串值 */
	UINT8	m_vBlob[SYS_ARG_BLOB_LEN];	/**< 系统参数数组值 */
};

#endif

