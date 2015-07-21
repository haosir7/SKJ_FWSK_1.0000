#include <errno.h>
#include "commonFunc.h"
#include "CDB.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

#ifndef WIN32
 #define DB_NAME			"./AWE"
#else
#define DB_NAME			"F:/KPJ_Debug/app/AWE"
#endif

const string	CDB::m_name(DB_NAME);
CDB*			CDB::m_db = NULL;

#if PTHREAD_MUTEX_OPEN == 1
pthread_mutex_t	g_skjmutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif

CDB::CDB()
{
	m_lock = false;
	countNum = 0;
	m_con = NULL;
	m_rwcs = NULL;
}

CDB::~CDB()
{
	if (m_con != NULL)
	{
		sqlite3_close(m_con);
		m_con = NULL;
	}
	if (m_db != NULL)
	{
		delete m_db;
		m_db = NULL;
	}
}

CDB* CDB::GetInstance(void)
{
	if (m_db == NULL)
	{
		m_db = new CDB();
		DBG_PRINT(("m_db = %X ", m_db));
	}
	
	return m_db;
}

void CDB::Init()
{
#if PTHREAD_MUTEX_OPEN == 1
	m_db->m_rwcs = &g_skjmutex;
	DBG_ASSERT_EXIT((m_db->m_rwcs != NULL), (" m_db->m_rwcs == NULL!"));
#endif
}

bool CDB::Lock(void)
{
	DBG_PRINT(("===DB LOCK===="));
	if(m_lock)
	{
		return true;
	}
	m_lock = true;

	return false;
}

void CDB::Unlock(void)
{
	DBG_PRINT(("===DB UNLOCK===="));
	m_lock = false;
}

bool CDB::IfLock(void)
{
	DBG_PRINT(("IfLock?"));
	
	return m_lock;

}

bool CDB::Open(void)
{
	DBG_ENTER("CDB::Open");
	if (m_con == NULL)
	{
		if (sqlite3_open(m_name.c_str(), &m_con) == SQLITE_OK)
		{	 
			DBG_PRINT(("m_db = 0x%X ", m_db));
			DBG_RETURN(true);
		}
		else
		{
			DB_ERRMSG_POS(m_con);
			m_con = NULL;
			DBG_RETURN(false);
		}
	}
	else
	{
		DBG_RETURN(true);
	}
}

DB_CON* CDB::GetConnect(void)
{
	DBG_ENTER("CDB::GetConnect");
	if (m_con == NULL)
	{
		if (Open() == false)
		{
			DBG_PRINT(("Open() database Err!"));
			exit(-1);
		}
	}
	DBG_RETURN(m_con);
}

bool CDB::Close(void)
{
	DBG_PRINT(("m_db = %X ", m_db));
	DBG_ENTER("CDB::Close");
	if (sqlite3_close(m_con) == SQLITE_OK)
	{
		m_con = NULL;
		DBG_RETURN(true);
	}
	else
	{
		DB_ERRMSG_POS(m_con);
		DBG_RETURN(false);
	}
}


void CDB::MutexLock(void)
{
#if PTHREAD_MUTEX_OPEN == 1
	DBG_ASSERT_EXIT((m_rwcs != NULL), (" m_rwcs == NULL!"));
	DBG_PRINT(("^^^^^^^^^^^countNum = %d^^^^^^^^", countNum));
	while (pthread_mutex_trylock(m_rwcs) == EBUSY)
	{
		DBG_PRINT(("EBUSY"));
		CommonSleep(1000);
	}

	countNum++;

	DBG_PRINT(("^^^^^^^^^^^countNum = %d^^^^^^^^", countNum));
#endif
}


void CDB::MutexUnlock(void)
{
#if PTHREAD_MUTEX_OPEN == 1
	int ret = 0;
	ret = pthread_mutex_unlock(m_rwcs);
	if (ret != 0)
	{
		DBG_PRINT(("===MUTEX UNLOCK %d====", ret));
	}
	countNum--;

	DBG_PRINT(("^^^^^^^^^^^countNum = %d^^^^^^^^", countNum));
#endif
}