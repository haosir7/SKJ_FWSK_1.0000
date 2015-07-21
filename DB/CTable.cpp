#include "powoff_protect.h"
#include "commonFunc.h"
#include "CWrSysLog.h"
#include "arithmetic.h"
#include "CTable.h"
#include "SysMacDef.h"
#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

#define MAX_RETRY_COUNT		3
#define DB_ACCESS_SPEED_TEST	0

#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
#include <sys/timeb.h>
#endif
#endif


#define DB_ERRMSG_STR(x, y) _db_errmsg_(x, y)

char *_db_errmsg_(DB_CON* m_con, char *str)
{
	static char s_errBuf[1024];
	int len;
	len = sprintf(s_errBuf, "%s: code=%d errmsg=%s", str, sqlite3_errcode(m_con),sqlite3_errmsg(m_con));
	if (len>sizeof(s_errBuf))
		return NULL;
	return s_errBuf;
}

string strfmt(const char *fmt,...)
{
	int ret;
	char tmpbuf[256];
	va_list args;
	
	va_start(args, fmt);   
	ret = vsprintf(tmpbuf, fmt, args);
	if (ret >= sizeof(tmpbuf))
	{
		printf("!!!vsprintf overflow\n");
		exit(-1);
	}
	va_end(args);	 
	return tmpbuf;
}

CTable::CTable(void)
{

	m_roll              = 0;
	m_filter			= "";
	
	m_db				= CDB::GetInstance();
	m_con				= m_db->GetConnect();
	m_sqstmt			= NULL;
	
	m_name				= "";
	m_sql				= "";
	m_fieldNum			= 0;
	m_tmpNum			= 0;
	m_rowCount			= 0;
	m_fieldType			= NULL;
	m_fieldTypeSize		= NULL;
	m_fieldPtr			= NULL;
	m_fieldNameDB		= NULL;
	m_fieldKey			= NULL;

}

CTable::~CTable()
{
	m_con		= NULL;
	m_db		= NULL;
	m_sqstmt	= NULL;

	if (m_fieldNameDB != NULL)
	{
		delete [] m_fieldNameDB;
		m_fieldNameDB = NULL;
	}

	if (m_fieldType != NULL)
	{
		delete [] m_fieldType;
		m_fieldType = NULL;
	}

	if (m_fieldTypeSize != NULL)
	{
		delete [] m_fieldTypeSize;
		m_fieldTypeSize = NULL;
	}

	if (m_fieldPtr != NULL)
	{
		delete [] m_fieldPtr;
		m_fieldPtr = NULL;
	}
}

void CTable::SET_TABLE_NAME(const string name)
{
	m_name = name;
}

void CTable::SET_FIELD_NUM(const INT32 num)
{
	DBG_ENTER("CTable::SET_FIELD_NUM");
 	DBG_ASSERT_EXIT(m_name != "", ("Please set the TABLE NAME first!"));

	if(num > MAX_NUM)
	{
		string tmpname = GetTableName();
		printf("%s: Exceed table field number\n", tmpname.c_str());
		exit(1);
	}

	m_fieldNum = num;
	DBG_PRINT((" num = %d, m_fieldNum = %d ", num, m_fieldNum));

	DBG_ASSERT_EXIT(m_fieldNameDB == NULL, ("m_fieldNameDB must be NULL!"));
	m_fieldNameDB = new string[num];

	DBG_ASSERT_EXIT(m_fieldType == NULL, ("m_fieldType must be NULL!"));
	m_fieldType = new DB_TYPE[num];

	DBG_ASSERT_EXIT(m_fieldTypeSize == NULL, ("m_fieldTypeSize must be NULL!"));
	m_fieldTypeSize = new INT32[num];

	DBG_ASSERT_EXIT(m_fieldPtr == NULL, ("m_fieldPtr must be NULL!"));
	m_fieldPtr = new UINT32[num];
}

	
void CTable::SET_FIELD_MEMBER(DB_TYPE type, void* fieldPtr, 
							  string fieldNameDB, INT32 typeSize)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	//DBG_ASSERT_EXIT(m_fieldNum > m_tmpNum, ("Exceed table field number!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= type;
	m_fieldTypeSize[m_tmpNum]	= typeSize;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}

void CTable::SET_FIELD_MEMBER(UINT32* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_UINT32;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(UINT16* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_UINT16;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(UINT8* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_UINT8;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(INT64* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_INT64;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(string* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_TEXT;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(float* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_FLOAT;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(double* fieldPtr, string fieldNameDB)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_DOUBLE;
	m_fieldTypeSize[m_tmpNum]	= 0;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}
void CTable::SET_FIELD_MEMBER(UINT8* fieldPtr, string fieldNameDB, INT32 typeSize)
{
	DBG_ENTER("CTable::SET_FIELD_MEMBER");
	
	DBG_ASSERT_EXIT(m_fieldNum != 0, ("Please set the value of m_fieldNum first!"));
	AbnormalExit();
	
	DBG_ASSERT_EXIT(m_fieldType != NULL, ("m_fieldType must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldPtr != NULL, ("m_fieldPtr must not be NULL!"));
	DBG_ASSERT_EXIT(m_fieldNameDB != NULL, ("m_fieldNameDB must not be NULL!"));
	
	m_fieldType[m_tmpNum]	= DB_BLOB;
	m_fieldTypeSize[m_tmpNum]	= typeSize;
	m_fieldPtr[m_tmpNum]	= (UINT32)fieldPtr;
	m_fieldNameDB[m_tmpNum]	= fieldNameDB;
	m_tmpNum++;
}

void CTable::AbnormalExit()
{
	//DBG_ASSERT_EXIT(m_fieldNum > m_tmpNum, ("Exceed table field number!"));
	if( m_fieldNum <= m_tmpNum )
	{
		string tmpname = GetTableName();
		printf("%s: Exceed table field number\n", tmpname.c_str());
		exit(1);
	}
}

string CTable::GetTableName() const
{
	return this->m_name;
}

void CTable::SetSQL(const string sql)
{
	this->m_sql = sql;
	this->m_filter = "";
}

string CTable::GetSQL() const
{
	return m_sql;
}

void CTable::CreateAddSQL(void)
{
	DBG_ENTER("CTable::CreateAddSQL");

	string tmpstr1 = "INSERT INTO ";
	string tmpstr2 = " VALUES(";
	tmpstr1.append(m_name);
	tmpstr1.append("(");
    
	INT32 i = 0;
	if (m_roll == 1)//添加记录时，排除"NO"字段
	{
		i = 1;
	}
	for (; i<m_fieldNum-1; i++)
	{
		tmpstr1.append(m_fieldNameDB[i]);
		tmpstr1.append(", ");
		tmpstr2.append("?, ");
	}

	tmpstr1.append(m_fieldNameDB[m_fieldNum-1]);
	tmpstr1.append(") ");
	tmpstr2.append("?)");

	m_sql = tmpstr1;
	m_sql.append(tmpstr2);

	m_filter = "";
}

void CTable::CreateRequerySQL(void)
{	
	DBG_ENTER("CTable::CreateRequerySQL");
	
	string tmpstr = "SELECT ";

	m_sql = tmpstr;
	for (INT32 i=0; i<m_fieldNum-1; i++)
	{
		m_sql.append(m_fieldNameDB[i]);
		m_sql.append(", ");
	}

	m_sql.append(m_fieldNameDB[m_fieldNum-1]);
	m_sql.append(" FROM ");
	m_sql.append(m_name);
	m_sql.append(" "); //查询语句和查询条件之间的空格符
	m_sql.append(m_filter);

	m_filter = "";
}

INT32 CTable::AddNew(void)
{
	INT32 errorcode;
	UINT8 nTimes = 0;
	errorcode = AddNewOnce();
		
	while((errorcode!=SQLITE_OK)&&(nTimes<3))//容错代码
	{
		CommonSleep(300);
		errorcode = AddNewOnce();
		nTimes++;
		DBG_PRINT((" sqlite3_step() again, errorcode = %d ", errorcode));
		DBG_PRINT((" sqlite3_step() again, nTimes = %u ", nTimes));		
	}
	nTimes = 0;
//	if (errorcode==SQLITE_OK)
//	{
//		if (sqlite3_changes(m_con) > 0) 
//		{
//			SynFunc();
//		}
//	}
	return errorcode;
}
 
INT32 CTable::AddNewOnce(void)
{
	DBG_ENTER("CTable::ExecuteSql");
	DBG_ASSERT_EXIT((m_con != NULL), (" m_con == NULL "));  

	CreateAddSQL(); //组装插入的SQL语句一部分
	
	DBG_ASSERT_EXIT((!m_sql.empty()), ("m_sql.empty()"));  
	DBG_PRN("info", (m_sql.c_str()));

	INT32 count = 0; 

#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	INT64 fTime;
	struct timeb tp;
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: before 判断Lock(), Time = %d ", fTime));
#endif
#endif

	//若数据库已锁定，则等待，直到数据库解锁
	DBG_PRINT(("CTable::AddNewOnce--start---MutexLock"));
	m_db->MutexLock();
	DBG_PRINT(("CTable::AddNewOnce-------MutexLock"));
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "AddNewOnce");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef WIN32
			CommonSleep(200);
#endif
		}
	}
		
#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: before (begin transaction), Time = %d ", fTime));
#endif
#endif
	
	INT32 errorcode;
/*	errorcode = sqlite3_exec(m_con, "begin transaction", NULL, NULL, NULL);

	if (errorcode != SQLITE_OK)
	{
		DB_ERRMSG_POS(m_con);
		DBG_RETURN(errorcode);
			
				
	}	
*/
#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: before sqlite3_prepare, Time = %d ", fTime));
#endif
#endif
	
	MASK_ALARM_SIGNAL();//阻塞Alarm信号
	//编译SQL语句
	errorcode = sqlite3_prepare(m_con, m_sql.c_str(), -1, &m_sqstmt, NULL);
	if (errorcode != SQLITE_OK)
	{
		UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
		DB_ERRMSG_POS(m_con);
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::AddNewOnce-------MutexUnlock"));
		CWrSysLog::WrSysLog(EVENT_DATABASE_PREPARE_ER, errorcode, DB_ERRMSG_STR(m_con, "AddNewOnce"));
		DBG_RETURN(errorcode);			
	}

#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: after sqlite3_prepare, Time = %d ", fTime));
#endif
#endif

	//加入要添加的记录，组装完整的SQL语句
	INT32 index=0;
	INT32 i;
	if (m_roll == 1)//添加记录时，排除"NO"字段
	{
		index = 1;
	}
	string tmptext;
	for (i = 1; index<m_fieldNum; index++, i++)
	{
		int restate;
		restate = SQLITE_OK;
		switch(m_fieldType[index]) 
		{
			case DB_UINT32:
				restate = sqlite3_bind_int(m_sqstmt, i, 
					*((UINT32 *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_UINT16:
				restate = sqlite3_bind_int(m_sqstmt, i, 
					*((UINT16 *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_UINT8:
				restate = sqlite3_bind_int(m_sqstmt, i, 
					*((UINT8 *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_INT64:
				restate = sqlite3_bind_int64(m_sqstmt, i, 
					*((INT64 *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;
				
			case DB_TEXT:
				tmptext = *((string *)(*(m_fieldPtr+index)));
				//DBG_PRINT(("tmptext = %s", tmptext.c_str()));
				restate = sqlite3_bind_text(m_sqstmt, i, 
					tmptext.c_str(), tmptext.length(), NULL);
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_FLOAT:
				restate = sqlite3_bind_double(m_sqstmt, i, 
					*((float *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_DOUBLE:
				restate = sqlite3_bind_double(m_sqstmt, i, 
					*((double *)(*(m_fieldPtr+index))));
				//DBG_PRINT(("restate = 0x%x", restate));
				break;

			case DB_BLOB:
				DBG_PRINT(("********BLOB REAL SIZE = %d", m_fieldTypeSize[index]));
				DBG_NPRINT_HEX(((UINT8 *)*(m_fieldPtr+index)), (m_fieldTypeSize[index]));
				restate = sqlite3_bind_blob(m_sqstmt, i, 
					(void *)(*(m_fieldPtr+index)), m_fieldTypeSize[index], NULL);
				DBG_PRINT(("restate = 0x%x", restate));
				break;
				
			default:
				CWrSysLog::WrSysLog(EVENT_DATABASE_FILEDTYPE_ER, 1, "AddNewOnce : filed type not matching");
				break;
		}
		if (restate != SQLITE_OK)
		{
			CWrSysLog::WrSysLog(EVENT_DATABASE_FILEDTYPE_ER, restate, DB_ERRMSG_STR(m_con, "AddNewOnce"));
		}
	}

#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: before sqlite3_step(), Time = %d ", fTime));
#endif
#endif

	//evalutate the SQL statement
//	DBG_PRINT(("m_sqstmt = 0x%x", m_sqstmt));
	errorcode = sqlite3_step(m_sqstmt);
	if (errorcode != SQLITE_DONE)
	{
		DBG_PRINT((" errorcode = %d ", errorcode));
		UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
		DB_ERRMSG_POS(m_con);
		DBG_ASSERT_WARNING(false,(" sqlite3_step error"));
		sqlite3_finalize(m_sqstmt);
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::AddNewOnce-------MutexUnlock"));
		CWrSysLog::WrSysLog(EVENT_DATABASE_STEP_ER, errorcode, DB_ERRMSG_STR(m_con, "AddNewOnce"));
		DBG_RETURN(errorcode);
	}

#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: after sqlite3_step(), Time = %d ", fTime));
#endif
#endif

	//destroy a prepared statement object
	errorcode = sqlite3_finalize(m_sqstmt);
	if (errorcode != SQLITE_OK)
	{
		DBG_PRINT((" errorcode = %d ", errorcode));
		UN_MASK_ALARM_SIGNAL();
		DB_ERRMSG_POS(m_con);
		m_db->Unlock();
		CWrSysLog::WrSysLog(EVENT_DATABASE_FINALIZE_ER, errorcode, DB_ERRMSG_STR(m_con, "AddNewOnce"));
		DBG_RETURN(errorcode);
	}

/*	
	errorcode = sqlite3_exec(m_con, "commit transaction", NULL, NULL, NULL);
	if (errorcode != SQLITE_OK)
	{
		DB_ERRMSG_POS(m_con);
		DBG_RETURN(errorcode);
	}
*/
	
	//解锁
	m_db->Unlock(); 
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::AddNewOnce-------MutexUnlock"));
	
#ifndef WIN32
#if DB_ACCESS_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" AddNew: after sqlite3_finalize(), Time = %d ", fTime));
#endif
#endif

	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
	DBG_RETURN(SQLITE_OK);
}

INT32 CTable::Requery(void)
{
	DBG_ENTER("CTable::LoadBySQL");
	DBG_ASSERT_EXIT((m_con != NULL), (" m_con == NULL "));  
	
	CreateRequerySQL(); //组装查询SQL语句
	
	DBG_ASSERT_EXIT((!m_sql.empty()), ("m_sql.empty()"));
	DBG_PRN("info", (m_sql.c_str()));

	INT32 count = 0; 
	
	m_db->MutexLock();
	DBG_PRINT(("CTable::Requery-------MutexLock"));
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "Requery");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(200);
#endif
		}
	} 
	
	const char* errorMsg = NULL;
	INT32 errorcode = sqlite3_prepare(m_con, m_sql.c_str(), -1, &m_sqstmt, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		CWrSysLog::WrSysLog(EVENT_DATABASE_PREPARE_ER, errorcode, DB_ERRMSG_STR(m_con, "Requery"));
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::Requery-------MutexUnlock"));
	}
	m_rowCount = 0;
	DBG_RETURN(errorcode);
}

INT32 CTable::MoveFirst(void)
{
	DBG_ENTER("CTable::MoveFirst");

	m_rowCount = 0;

	DBG_RETURN(MoveNext());
}

//读取数据库的一条记录，按栏目存入表对象成员中
INT32 CTable::MoveNext(void)
{
	DBG_ENTER("CTable::MoveNext");
	const void *tmpBlob = NULL;
	INT32 len;

	MASK_ALARM_SIGNAL();//阻塞Alarm信号
	INT32 errorcode = sqlite3_step(m_sqstmt);    
	if (errorcode == SQLITE_ROW) 
	{  
		for (INT32 index=0; index<m_fieldNum; index++)
		{
			switch(m_fieldType[index]) 
			{
				case DB_UINT32:
					*((UINT32 *)(*(m_fieldPtr+index))) = 
						(UINT32)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_INT64:
					*((INT64 *)(*(m_fieldPtr+index))) = 
						(INT64)sqlite3_column_int64(m_sqstmt, index);
					break;

				case DB_UINT16:
					*((UINT16 *)(*(m_fieldPtr+index))) = 
						(UINT16)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_UINT8:
					*((UINT8 *)(*(m_fieldPtr+index))) = 
						(UINT8)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_TEXT:
					*((string *)(*(m_fieldPtr+index))) = 
						(char *)sqlite3_column_text(m_sqstmt, index);
					break;

				case DB_FLOAT:
					*((float *)(*(m_fieldPtr+index))) = 
						(float)sqlite3_column_double(m_sqstmt, index);
					break;

				case DB_DOUBLE:
					*((double *)(*(m_fieldPtr+index))) = 
						sqlite3_column_double(m_sqstmt, index);
					break;

				case DB_BLOB:
					tmpBlob = sqlite3_column_blob(m_sqstmt, index);
					len = sqlite3_column_bytes(m_sqstmt, index);
					//if (len > m_fieldTypeSize[index])
					//{
						//DBG_PRINT(("****BLOB SIZE READ= %d<===> BLOB REAL SIZE = %d", len, m_fieldTypeSize[index]));
						//DBG_NPRINT_HEX(((UINT8 *)tmpBlob), len);
					//}
					DBG_ASSERT_EXIT((len <= m_fieldTypeSize[index]), (" BLOB SIZE IS OUT OF RANGE!!!! ")); 
					if (len <= m_fieldTypeSize[index])
						memcpy((void *)(*(m_fieldPtr+index)), tmpBlob, len);
					else
						memcpy((void *)(*(m_fieldPtr+index)), tmpBlob, m_fieldTypeSize[index]);
					break;
					
				default:
					CWrSysLog::WrSysLog(EVENT_DATABASE_FILEDTYPE_ER, 1, "MoveNext : filed type not matching");
					break;
			}
		}   

		m_rowCount++;
	}
	else
	{
		DBG_PRINT(("execute MoveNext res = 0x%x", errorcode));
		if (errorcode != SQLITE_DONE)
		{
			DB_ERRMSG_POS(m_con);
			DBG_ASSERT_WARNING(false,(" sqlite3_step error, row_count=%d", m_rowCount));
			CWrSysLog::WrSysLog(EVENT_DATABASE_STEP_ER, errorcode, DB_ERRMSG_STR(m_con, "MoveNext"));
		}
		sqlite3_finalize(m_sqstmt);
	}

	m_db->Unlock();//查询到一个记录就解锁
	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
	DBG_RETURN(errorcode);
}

INT32 CTable::MoveEnd(void)
{
	sqlite3_finalize(m_sqstmt);
	m_rowCount = 0;
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::MoveEnd-------MutexUnlock"));
	DBG_RETURN(SUCCESS);
}


INT32 CTable::LoadOneRecord()
{
	DBG_ENTER("CTable::LoadOneRecord");
	
	INT32 errorcode = MoveFirst();
	if (errorcode == SQLITE_ROW)
	{
		MoveEnd();
		DBG_RETURN(SQLITE_OK);
	}
	
	MoveEnd();
	DBG_RETURN(errorcode);
}

INT32 CTable::LoadOneRecordBySql(string strsql)
{
	m_filter.append(strsql);

	INT32 errorcode = Requery();
	DBG_PRINT(("errorcode = %d", errorcode));
	if( errorcode != SQLITE_OK )
	{
		return errorcode;
	}

	errorcode = LoadOneRecord();
	DBG_PRINT(("errorcode = %d", errorcode));
	DBG_RETURN(errorcode);
}

INT32 CTable::Delete()
{
	DBG_ENTER("CTable::Delete");
	INT32 errorcode=0;

	if (m_filter=="")			//add by zl 140811
		m_filter = "WHERE 1";

	m_sql = "DELETE FROM ";
	m_sql.append(m_name);
	m_sql.append(" ");
	m_sql.append(m_filter);

	m_filter = "";
	
	DBG_PRN("info", (m_sql.c_str()));
//	DBG_RETURN(ExecuteSQL());
	errorcode = SQlite3EXec();//非事物执行

	if (errorcode==SQLITE_OK)
	{
		if (sqlite3_changes(m_con) > 0) 
		{
			SynFunc();
		}
	}
	DBG_RETURN(errorcode);
}

INT32 CTable::Update(string strsql, ...)
{
	string tbName = GetTableName();
	va_list args;
	char tmpbuf[256];
	string tmptext;
	int p;
	int index = 0;
	int i=0;
	INT32 errcode = 0;
	UINT32 tmpLen = 0;

	m_sql = "update ";
	m_sql.append(tbName);
	m_sql.append(" set ");

	va_start(args, strsql);
	p = va_arg(args, int);

	while(p != NULL)
	{
		if(index != 0)
			m_sql.append(", ");

		for (i=0; i<m_fieldNum; i++)
		{
			//DBG_PRINT(("m_fieldPtr[i] = %x", m_fieldPtr[i]));
			if(m_fieldPtr[i] == p)
			{
				//DBG_PRINT(("type = %u", m_fieldType[i]));
				DBG_PRINT(("name = %s", m_fieldNameDB[i].c_str()));
				switch(m_fieldType[i])
				{
				case DB_UINT32:
					sprintf(tmpbuf, "%u", *((UINT32 *)(*(m_fieldPtr+i))));
					break;
				case DB_UINT16:
					sprintf(tmpbuf, "%u", *((UINT16 *)(*(m_fieldPtr+i))));
					break;
				case DB_UINT8:
					sprintf(tmpbuf, "%u", *((UINT8 *)(*(m_fieldPtr+i))));
					break;
				case DB_INT64:
					sprintf(tmpbuf, "%lld", *((INT64 *)(*(m_fieldPtr+i))));
					break;
				case DB_TEXT:
					tmptext = *((string *)(*(m_fieldPtr+i)));
					sprintf(tmpbuf, "'%s'", tmptext.c_str());
					break;
				case DB_BLOB:
					if(m_fieldTypeSize[i]*2+2 > sizeof(tmpbuf))
					{
						printf("Exceed tmpbuf max len!\n");
						exit(-1);
					}
					strcpy(tmpbuf, "X'");
					tmpLen += 2;
					for(int k=0; k<m_fieldTypeSize[i]; k++)
					{
						sprintf(tmpbuf+tmpLen, "%02X", *((UINT8 *)(*(m_fieldPtr+i))+k));
						tmpLen += 2;
					}
					strcpy(tmpbuf+tmpLen, "'");
					break;
				case DB_FLOAT:
					sprintf(tmpbuf, "%f", *((float *)(*(m_fieldPtr+i))));
					break;
				case DB_DOUBLE:
					sprintf(tmpbuf, "%f", *((double *)(*(m_fieldPtr+i))));
					break;
				}

				m_sql.append(m_fieldNameDB[i]);
				m_sql.append("=");
				m_sql.append(tmpbuf);
				break;
			}
		}

		if(i>=m_fieldNum)
		{
			printf("%s: Update exceed table field number!!! Check %d Parameters\n", tbName.c_str(), index+2);
			printf("Check whether the last variable is NULL and the symbol '&' was added at every variable.\n");	
			exit(-1);
		}

		index++;
		p = va_arg(args, int);
	}
	va_end(args);

	m_sql.append(" ");
	m_sql.append(strsql);
	DBG_PRINT(("m_sql = %s", m_sql.c_str()));

	errcode = ExecuteSQL();

	return errcode;
}

INT32 CTable::ExecuteSQL(void)
{
	DBG_ENTER("CTable::ExecuteSQL");

	m_filter = "";
	INT32 count=0;     

	m_db->MutexLock();
	DBG_PRINT(("CTable::ExecuteSQL-------MutexLock"));
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "ExecuteSQL");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(1000);
#endif
		}
	}         

	MASK_ALARM_SIGNAL();//阻塞Alarm信号
	char* errorMsg = NULL;
	INT32 errorcode = sqlite3_exec(m_con, "begin transaction", 
								 NULL, NULL, &errorMsg);

	if (errorcode != SQLITE_OK)
	{
		UN_MASK_ALARM_SIGNAL();//接触对Alarm信号的阻塞
		DB_ERRMSG_POS(m_con);
		sqlite3_free(errorMsg);
		CWrSysLog::WrSysLog(EVENT_DATABASE_EXEC_ER, errorcode, DB_ERRMSG_STR(m_con, "ExecuteSQL begin transaction"));
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::ExecuteSQL-------MutexUnlock"));
		DBG_RETURN(errorcode);
	}	

	errorcode = sqlite3_exec(m_con, m_sql.c_str(), 
							 NULL, NULL, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		UN_MASK_ALARM_SIGNAL();//接触对Alarm信号的阻塞
		DB_ERRMSG_POS(m_con);
		sqlite3_free(errorMsg);
		CWrSysLog::WrSysLog(EVENT_DATABASE_EXEC_ER, errorcode, DB_ERRMSG_STR(m_con, "ExecuteSQL begin transaction"));
		sqlite3_exec(m_con, "commit transaction", NULL, NULL, &errorMsg);		//执行sqlite3_exec错误时，也要注销SQLITE事物操作。张磊
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::ExecuteSQL-------MutexUnlock"));
		DBG_RETURN(errorcode);
	}
	errorcode = sqlite3_exec(m_con, "commit transaction", 
							 NULL, NULL, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		sqlite3_free(errorMsg);
		DB_ERRMSG_POS(m_con);	
		CWrSysLog::WrSysLog(EVENT_DATABASE_EXEC_ER, errorcode, DB_ERRMSG_STR(m_con, "ExecuteSQL commit transaction"));
	}
	
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::ExecuteSQL-------MutexUnlock"));
	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞

	if (errorcode==SQLITE_OK)
	{
		if (sqlite3_changes(m_con) > 0) 
		{
			SynFunc();
		}
	}

	DBG_RETURN(errorcode);
}


INT32 CTable::GetRecordNum(void)
{
	DBG_ENTER("CTable::GetRecordNum");
	DBG_ASSERT_EXIT((m_con != NULL), (" m_con == NULL "));  
	
	//组装统计记录条数的SQL语句
	string strTemp = "select count(*) from ";
    strTemp.append(m_name);
    strTemp.append(" ");
    strTemp.append(m_filter);
	m_filter = "";
	SetSQL(strTemp);
	
	DBG_ASSERT_EXIT((!m_sql.empty()), ("m_sql.empty()"));
	DBG_PRN("info", (m_sql.c_str()));

	//判断数据库是否锁定
	INT32 count = 0; 

	m_db->MutexLock();
	DBG_PRINT(("CTable::GetRecordNum-------MutexLock"));
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			m_filter = "";
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "GetRecordNum");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(200);
#endif
		}
	} 
	
	MASK_ALARM_SIGNAL();//阻塞Alarm信号
	const char* errorMsg = NULL;
	INT32 errorcode = sqlite3_prepare(m_con, m_sql.c_str(), -1, &m_sqstmt, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::GetRecordNum-------MutexUnlock"));
		UN_MASK_ALARM_SIGNAL();
		DB_ERRMSG_POS(m_con);
		CWrSysLog::WrSysLog(EVENT_DATABASE_PREPARE_ER, errorcode, DB_ERRMSG_STR(m_con, "GetRecordNum"));
		DBG_RETURN(-1);
	}

    //执行SQL语句
    INT32 nRecordNum;
	errorcode = sqlite3_step(m_sqstmt);    
	if (errorcode == SQLITE_ROW) 
	{  
		nRecordNum = (UINT32)sqlite3_column_int(m_sqstmt, 0);
	}
	else
	{
		nRecordNum = -1;
        if (errorcode != SQLITE_DONE)
		{
			DB_ERRMSG_POS(m_con);
			DBG_ASSERT_WARNING(false,(" sqlite3_step error, row_count=%d", m_rowCount));
			CWrSysLog::WrSysLog(EVENT_DATABASE_STEP_ER, errorcode, DB_ERRMSG_STR(m_con, "GetRecordNum"));
		}
	}
		
	sqlite3_finalize(m_sqstmt);
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::GetRecordNum-------MutexUnlock"));
	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞

	DBG_RETURN(nRecordNum);
}

INT32 CTable::SQlite3EXec(void)
{
	DBG_ENTER("CTable::SQlite3EXec");

	m_db->MutexLock();
	DBG_PRINT(("CTable::SQlite3EXec-------MutexLock"));
	INT32 count=0;     
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "SQlite3EXec");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(200);
#endif
		}
	}         

	MASK_ALARM_SIGNAL();
	char* errorMsg = NULL;

	INT32 errorcode = sqlite3_exec(m_con, m_sql.c_str(), 
							 NULL, NULL, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		CWrSysLog::WrSysLog(EVENT_DATABASE_EXEC_ER, errorcode, DB_ERRMSG_STR(m_con, "SQlite3EXec"));
	}

	UN_MASK_ALARM_SIGNAL();
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::SQlite3EXec-------MutexUnlock"));
	DBG_RETURN(errorcode);
}

INT32 CTable::GetOneResult(DB_TYPE ntype, void* pValue)
{
	DBG_ENTER("CTable::GetOneResult");
	DBG_ASSERT_EXIT((m_con != NULL), (" m_con == NULL "));  
	
	DBG_ASSERT_EXIT((!m_sql.empty()), ("m_sql.empty()"));
	DBG_PRN("info", (m_sql.c_str()));

	//判断数据库是否锁定
	m_db->MutexLock();
	DBG_PRINT(("CTable::GetOneResult-------MutexLock"));
	INT32 count = 0;
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{	
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "GetOneResult");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(200);
#endif
		}
	} 

	MASK_ALARM_SIGNAL();//阻塞Alarm信号

	const char* errorMsg = NULL;
	INT32 errorcode = sqlite3_prepare(m_con, m_sql.c_str(), -1, &m_sqstmt, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::GetOneResult-------MutexUnlock"));
		UN_MASK_ALARM_SIGNAL();
		CWrSysLog::WrSysLog(EVENT_DATABASE_PREPARE_ER, errorcode, DB_ERRMSG_STR(m_con, "GetOneResult"));
		DBG_RETURN(errorcode);
	}
	 
	const void *tmpBlob = NULL;
	INT32 len;
	errorcode = sqlite3_step(m_sqstmt);
	if (errorcode == SQLITE_ROW) 
	{  
		switch(ntype) 
		{
			case DB_UINT32:
				*(UINT32 *)pValue = (UINT32)sqlite3_column_int(m_sqstmt, 0);
				break;

			case DB_INT64:
				*(INT64 *)pValue = (INT64)sqlite3_column_int64(m_sqstmt, 0);
				break;
			case DB_UINT16:
				*(UINT16 *)pValue = (UINT16)sqlite3_column_int(m_sqstmt, 0);
				break;

			case DB_UINT8:
				*(UINT8 *)pValue = (UINT8)sqlite3_column_int(m_sqstmt, 0);
				break;

			case DB_TEXT:
				*(string *)pValue = (char *)sqlite3_column_text(m_sqstmt, 0);
				break;

			case DB_FLOAT:
				*(float *)pValue = (float)sqlite3_column_double(m_sqstmt, 0);
				break;

			case DB_DOUBLE:
				*(double *)pValue =	sqlite3_column_double(m_sqstmt, 0);
				break;

			case DB_BLOB:
				tmpBlob = sqlite3_column_blob(m_sqstmt, 0);
				len = sqlite3_column_bytes(m_sqstmt, 0);
				memcpy((void *)(pValue), tmpBlob, len);
				break;
				
			default:
				break;
		}
	}
	else
	{
       if (errorcode != SQLITE_DONE)
		{
			DB_ERRMSG_POS(m_con);
			DBG_ASSERT_WARNING(false,(" sqlite3_step error, row_count=%d", m_rowCount));
			CWrSysLog::WrSysLog(EVENT_DATABASE_STEP_ER, errorcode, DB_ERRMSG_STR(m_con, "GetOneResult"));
		}
	}
		
	sqlite3_finalize(m_sqstmt);
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::GetOneResult-------MutexUnlock"));
	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
	DBG_RETURN(errorcode);
}

INT32 CTable::GetMultiResult(DB_TYPE *ntype, void* pValue[], UINT8 nColNum, INT32 &nRowCount)
{
	DBG_ENTER("CTable::GetMultiResult");
	DBG_ASSERT_EXIT((m_con != NULL), (" m_con == NULL "));  
	
	DBG_ASSERT_EXIT((!m_sql.empty()), ("m_sql.empty()"));
	DBG_PRN("info", (m_sql.c_str()));

        nRowCount = 0;
	//判断数据库是否锁定
	m_db->MutexLock();
	DBG_PRINT(("CTable::GetMultiResult-------MutexLock"));
	INT32 count = 0;
	while (m_db->Lock())
	{
		if (count++ > MAX_RETRY_COUNT )
		{
			DBG_ASSERT_WARNING(false,("ExecuteDBSql lock error!"));
			CWrSysLog::WrSysLog(EVENT_DATABASE_LOCKED, 1, "GetMultiResult");
			DBG_RETURN(-3);//can retry
		}
		else
		{
#ifndef	WIN32
			CommonSleep(200);
#endif
		}
	} 
	
	MASK_ALARM_SIGNAL();//阻塞Alarm信号

	const char* errorMsg = NULL;
	INT32 errorcode = sqlite3_prepare(m_con, m_sql.c_str(), -1, &m_sqstmt, &errorMsg);
	if (errorcode != SQLITE_OK)
	{
		m_db->Unlock();
		m_db->MutexUnlock();
		DBG_PRINT(("CTable::GetMultiResult-------MutexUnlock"));
		UN_MASK_ALARM_SIGNAL();
		DB_ERRMSG_POS(m_con);
		CWrSysLog::WrSysLog(EVENT_DATABASE_PREPARE_ER, errorcode, DB_ERRMSG_STR(m_con, "GetMultiResult"));
	//	DBG_RETURN(-1);
                DBG_RETURN(errorcode);
	}
	 
	const void *tmpBlob = NULL;
	INT32 len, i;
	UINT8 index;
	m_rowCount = 0;
	while((errorcode = sqlite3_step(m_sqstmt))==SQLITE_ROW)
	{
		for(index=0; index<nColNum; index++)
		{
			i = m_rowCount*nColNum + index;
			switch(ntype[index]) 
			{
				case DB_UINT32:
					*((UINT32 *)(*(pValue+i))) = 
						(UINT32)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_INT64:
					*((INT64 *)(*(pValue+i))) = 
						(INT64)sqlite3_column_int64(m_sqstmt, index);
					break;

				case DB_UINT16:
					*((UINT16 *)(*(pValue+i))) = 
						(UINT16)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_UINT8:
					*((UINT8 *)(*(pValue+i))) = 
						(UINT8)sqlite3_column_int(m_sqstmt, index);
					break;

				case DB_TEXT:
					*((string *)(*(pValue+i))) = 
						(char *)sqlite3_column_text(m_sqstmt, index);
					break;

				case DB_FLOAT:
					*((float *)(*(pValue+i))) = 
						(float)sqlite3_column_double(m_sqstmt, index);
					break;

				case DB_DOUBLE:
					*((double *)(*(pValue+i))) = 
						sqlite3_column_double(m_sqstmt, index);
					break;

				case DB_BLOB:
					tmpBlob = sqlite3_column_blob(m_sqstmt, index);
					len = sqlite3_column_bytes(m_sqstmt, index);
					memcpy((void *)(*(pValue+i)), tmpBlob, len);
					break;
					
				default:
					break;
			}
		}   

		m_rowCount++;
	}

	sqlite3_finalize(m_sqstmt);
	m_db->Unlock();
	m_db->MutexUnlock();
	DBG_PRINT(("CTable::GetMultiResult-------MutexUnlock"));

	if (errorcode != SQLITE_DONE)
	{
		DB_ERRMSG_POS(m_con);
		DBG_ASSERT_WARNING(false,(" sqlite3_step error, row_count=%d", m_rowCount));
		CWrSysLog::WrSysLog(EVENT_DATABASE_STEP_ER, errorcode, DB_ERRMSG_STR(m_con, "GetMultiResult"));
	}
	else
	{
		errorcode = SQLITE_OK;//执行成功
	}
	
	UN_MASK_ALARM_SIGNAL();//解除对Alarm信号的阻塞
//	DBG_RETURN(m_rowCount);
	nRowCount = m_rowCount;
	DBG_RETURN(errorcode);
}

INT32 CTable::getErrMsg(string &errmsg)
{
	INT32 errcode = sqlite3_errcode(m_con);
	errmsg = sqlite3_errmsg(m_con);
	
	return errcode;
}

INT32 CTable::Foreach(string strsql)
{
	INT32 errorcode = 0;
	if( m_rowCount == 0)
	{
		m_filter.append(strsql);	
		errorcode = Requery();
		if( errorcode != SQLITE_OK )
		{
			return errorcode;
		}
	}
	
	DBG_RETURN(MoveNext());
}
INT32 CTable::ForeachEnd()
{
	DBG_RETURN(MoveEnd());
}

INT32 CTable::BeginTransaction()
{
	SetSQL("BEGIN TRANSACTION");
	
	return ExecuteSQL();
}

INT32 CTable::EndTransaction()
{
	SetSQL("END TRANSACTION");
	
	return ExecuteSQL();
}

