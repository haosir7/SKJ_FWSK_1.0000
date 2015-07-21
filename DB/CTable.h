#ifndef __CTABLE_H
#define __CTABLE_H

#include "IncludeMe.h"
#include "CDB.h"

#define PRIMARY 1
#define UN_PRIMARY 0
#define MAX_NUM 20
 
/**
 *@enum DB_TYPE 
 *@brief 数据类型
 */
typedef enum
{
	DB_UINT32 = 1,
	DB_INT64,
	DB_UINT16,
	DB_UINT8,
	DB_TEXT,
	DB_BLOB,
	DB_FLOAT,
	DB_DOUBLE,
	DB_DATETIME
}DB_TYPE;


/**
 *@class CTable 
 *@brief 表（基类）
 *@brief 
 */
class CTable
{
public:
	
    /*!
	@brief 构造函数	
	@attention 数据库默认为锁定状态
	*/ 
	CTable();

	/*!
	@brief 析构函数	
	@attention 数据库默认为锁定状态
	*/ 
	~CTable();

//------------------
    /*!
	@brief 设置表名	
    @param[in] name: 表名
	*/
	void SetTableName(const string name);

	/*!
	@brief 设置SQL语句	
	@param[in] sql: SQL语句
	*/
	void SetSQL(const string sql);

	/*!
	@brief 获得SQL语句	
	@return  SQL语句
	*/
	string GetSQL(void) const;

    /*!
	@brief 获取表名	
	@return 表名
	*/
	string GetTableName(void) const;

//-----------------
	/*!
	@brief 插入记录（若插入不成功，则自动重试3次）	
	@return sqlite函数执行结果代码
	*/
	INT32 AddNew(void);

	/*!
	@brief 插入记录一次	
	@return sqlite函数执行结果代码
	*/
	INT32 AddNewOnce(void);

	/*!
	@brief 查询记录
	@return sqlite函数执行结果代码
	*/
	INT32 Requery(void);

	/*!
	@brief 删除记录	
	@return sqlite函数执行结果代码
	*/
	INT32 Delete(void);

	/*!
	@brief 更新记录	
	@return strsql 更新操作的条件语句（如where NO = 1）
	@warning 不定参数传入变量地址，最有一个参数为NULL，且不可缺少。
			 如果第二个参数传入的是string类型，没有传入其地址，会造成程序中断(Abort)。
	@note 例如：
	CInvHead invhead;
	char sqlbuf[128];
	sprintf(sqlbuf,"where NO = %u ", 1);
	invhead.m_payer = "管理";
	invhead.m_opId = 5;
	invhead.m_moneySum = 100000;
	invhead.Update(sqlbuf, &invhead.m_payer, &invhead.m_opId, &invhead.m_moneySum, NULL);
	*/
	INT32 Update(string strsql, ...);

	/*!
	@brief 执行SQL语句（按事务执行）	
	@return sqlite函数执行结果代码
	*/
	INT32 ExecuteSQL(void);
    
	/*!
	@brief 执行SQL语句（不按事务执行）	
	@return sqlite函数执行结果代码
	*/
	INT32 SQlite3EXec(void);

    /*!
	@brief 统计表符合某限定条件的记录数目	
	@return >=0: 记录数目; -1: 统计失败
	@attention 调用该函数前只需设置好限定条件（m_filter）
	*/
    INT32 GetRecordNum(void);


	/*!
	@brief 执行有一个返回值的sql语句
	@param[in]  ntype  数据类型
	@param[out] pValue 结果
	@return 错误代码
	@attention 用于sql语句执行后只有一个结果；
	           调用此函数前，必须先设置好完整的sql语句（m_sql）。
	*/
	INT32 GetOneResult(DB_TYPE ntype, void* pValue);
    
	/*!
	@brief 执行有多个返回值的sql语句
	@param[in]  ntype  数据类型数组
	@param[in]  nColNum  结果的列数
	@param[out] pValue 结果
	@param[out] nRowCount 结果的行数
	@return     SQLITE_OK 执行成功，其他：错误代码
	@attention 用于sql语句执行后有多个结果,
	           调用此函数前，必须先设置好完整的sql语句（m_sql）。
	*/
	INT32 GetMultiResult(DB_TYPE *ntype, void* pValue[], UINT8 nColNum, INT32 &nRowCount);

	/*!
	@brief 取第一条记录	
	@return 
	*/
	INT32 MoveFirst(void);

	/*!
	@brief 取下一条记录	
	@return 
	*/
	INT32 MoveNext(void);

	/*!
	@brief 终止查询	
	@return 1 SUCCESS；0 FAILURE
	@attention 用途：调用MoveFirst()或MoveNext()返回SQLITE_ROW时却想停止查询。
	*/
	INT32 MoveEnd(void);

	/*!
	@brief 取下一条记录	
	@param[in]  strsql  需要查询数据的条件语句
	@note 替代来的MoveFirst()和MoveNext()两个函数。
	@return 
	*/
	INT32 Foreach(string strsql="");
	/*!
	@brief 终止查询	
	@return 1 SUCCESS；0 FAILURE
	@attention 用途：Foreach()返回SQLITE_ROW时却想停止查询。
	*/
	INT32 ForeachEnd();

	/*!
	@brief 读取一条记录	
	@return 
	*/
	INT32 LoadOneRecord(void);

	/*!
	@brief 读取一条记录	
	@param[in]  strsql 查询的条件语句（如where NO = 1）
	@return 
	*/
	INT32 LoadOneRecordBySql(string strsql="");

	/*!
	@brief 获取数据库操作的错误代码及描述
	@return 
	*/
	INT32 getErrMsg(string &errmsg);

	string  m_filter;

	/*!
	@brief 事务处理开始	
	@return 
	*/
	INT32 BeginTransaction();
	/*!
	@brief 事务处理结束
	@return 
	*/
	INT32 EndTransaction();

protected:
    
	/*!
	@brief 注册表名	
	@param[in] name  表名
	*/
	void SET_TABLE_NAME(const string name);

	/*!
	@brief 注册栏目个数	
	@param[in] num  个数 
	*/
	void SET_FIELD_NUM(const INT32 num);

	/*!
	@brief 注册栏目相关信息	
	@param[in] type         数据类型 
	@param[in] fieldPtr     具体内容存放的地址
	@param[in] fieldNameDB  该栏目在数据库中的名字
	@param[in] typeSize     类型长度
	*/
	void SET_FIELD_MEMBER(DB_TYPE type, void* fieldPtr, 
						  string fieldNameDB, INT32 typeSize);

	void SET_FIELD_MEMBER(UINT32* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(UINT16* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(UINT8* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(INT64* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(string* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(float* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(double* fieldPtr, string fieldNameDB);
	void SET_FIELD_MEMBER(UINT8* fieldPtr, string fieldNameDB, INT32 typeSize);

	virtual void SynFunc() {}
	
private:

	string		m_name;
	string		m_sql;
	INT32		m_fieldNum;
	INT32		m_tmpNum;
	INT32		m_rowCount;
	DB_TYPE		*m_fieldType;
	INT32		*m_fieldTypeSize;
	UINT32		*m_fieldPtr;
	string		*m_fieldNameDB;
	UINT8       *m_fieldKey;
	DB_CON		*m_con;
	CDB			*m_db;
	sqlite3_stmt *m_sqstmt;

	/*!
	@brief 组装查询表的SQL语句	
	*/

	void CreateRequerySQL(void);

	/*!
	@brief 组装插入表的SQL语句	
	*/
	void CreateAddSQL(void);

	/*!
	@brief 条件判断函数：如果在表中添加的字段超过建表的实际字段数，则退出。
	*/
	void AbnormalExit();

protected:
	/*!
	@表是否具有滚动的属性，1=滚动，0=不滚动	
	*/
	INT32        m_roll;
};

string strfmt(const char *fmt,...);

#endif


