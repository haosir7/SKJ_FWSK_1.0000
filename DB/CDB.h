#ifndef __CDB_H
#define __CDB_H

#include <pthread.h>
#include "IncludeMe.h"
#include "sqlite3.h"

#define DB_ERRMSG_POS(x) DBG_ASSERT_WARNING(false,("code=%d errmsg=%s",sqlite3_errcode(x),sqlite3_errmsg(x)));

typedef struct sqlite3 DB_CON;

#define PTHREAD_MUTEX_OPEN		1

/**
 *@class CDB 
 *@brief 数据库（基类）
 *@brief 
 */
class CDB
{
public:
   /*!
	@brief 打开数据库	
	@return   true  成功； false 失败
	*/   
	bool Open(void);

	/*!
	@brief 链接数据库？？？	

	@return  
	*/ 
	DB_CON* GetConnect(void);

	/*!
	@brief 关闭数据库	
	@return   true  成功； false 失败
	*/ 
	bool Close(void);

	/*!
	@brief 锁定数据库	
	@return   true  之前已锁定； false 之前未锁定，现在锁定
	*/ 
	bool Lock(void);

	/*!
	@brief 解锁数据库	
	*/ 
	void Unlock(void);

	/*!
	@brief 数据库是否处于锁定的状态	
	@return   true  是； false 否
	*/
	bool IfLock(void);

	/*!
	@brief 获得数据库对象	
	@return   数据库对象指针
	@attention 整个工程只允许有一个数据库对象。若没有数据库对象，该函数会
	           新建一个数据库对象；若数据库对象已存在，该函数返回指向该对象的指针。
	*/ 
	static CDB* GetInstance(void);

	static void Init();

	/*!
	@brief 锁定互斥体	
	@return  
	*/ 
	void MutexLock(void);

	/*!
	@brief 解锁互斥体	
	*/ 
	void MutexUnlock(void);

	pthread_mutex_t     *m_rwcs;     /**< 数据库互斥体*/

private:

    /*!
	@brief 构造函数	
	@attention 数据库默认为锁定状态
	*/ 
	CDB();

    /*!
	@brief 析构函数	
	*/
	~CDB();
	
	static CDB*			m_db;		/**< 数据库对象（静态） */
	static const string	m_name;		/**< 数据库名称（静态） */
	DB_CON				*m_con; 	/**< SQLITE3数据库指针 */
	bool				m_lock;		/**< 数据库锁标志 */
	int					countNum;
};

#endif


