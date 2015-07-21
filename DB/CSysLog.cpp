#include "CSysLog.h"
#include "SysMacDef.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

CSysLog::CSysLog()
{
	m_roll = 1;
	m_idate = 0;
	m_itime = 0;
	m_operator = "";
	m_type = 0;
	m_result = 0;
	m_backup = "";
	
	SET_TABLE_NAME("SYSLOG");
	SET_FIELD_NUM(7);

	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_idate, "I_DATE", 0);
    SET_FIELD_MEMBER(DB_UINT32, &m_itime, "I_TIME", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_operator, "OPERATOR", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_type, "TYPE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_result, "RESULT", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_backup, "BACKUP", 0);		
}

CSysLog::~CSysLog()
{
}


INT32 CSysLog::CheckRoll(INT32 nCount, UINT32 &nDelNo)
{
	DBG_ENTER("CRtInv::CheckRolll");
	
	INT32 nNoStart, nNoEnd;
//	char value[128];

	if (nCount>=SYSLOG_MAX) //超过容纳最大记录数
	{
		//得到第一条记录的序号
		Requery();
		if (LoadOneRecord() == SQLITE_OK)
		{
			nNoStart = m_no;
		}
		else
		{
			DBG_ASSERT_WARNING(false,("Can not load the first record!"));
			DBG_RETURN(FAILURE);
		}

		nNoEnd = nNoStart + SYSLOG_DEL_MAX;
		nDelNo = nNoEnd;
//		DBG_RETURN(SUCCESS);
	}

	DBG_RETURN(SUCCESS);	
}

INT32 CSysLog::Roll(UINT32 nDelNo)
{
	INT8 value[128];
	if (nDelNo>0) //要滚动
	{
		sprintf(value, "WHERE NO < %lu", nDelNo);
		m_filter = value;
		Delete(); 
	}
	DBG_RETURN(SUCCESS);

}

INT32 CSysLog::GetSysLogNum()
{
	INT32 nNoStart = 0;
	INT32 nNoEnd = 0;
	INT32 nCount = 0;
	INT32 errcode;

	m_filter = "limit 1";
	Requery();
	errcode = LoadOneRecord();

	if (errcode == SQLITE_OK)
	{
		nNoStart = m_no; //起始记录的序号
	}
	else if (errcode == SQLITE_DONE) 
	{
		nCount = 0;
		DBG_RETURN(nCount);	//空表，无记录		
	}
	else 
	{
		DBG_PRINT(("Can not load the first record!"));
		DBG_RETURN(-1);
	}

	m_filter = "order by NO desc limit 1";
	Requery();
	if (LoadOneRecord() == SQLITE_OK)
	{
		nNoEnd = m_no; //末尾记录的序号
	}
	else
	{
		DBG_PRINT(("Can not load the last record!"));
		DBG_RETURN(-1);
	}

	nCount = nNoEnd - nNoStart + 1; //表的记录数
	if (nCount<0) 
	{
		DBG_PRINT(("INV_HEAD的nCount<0!"));
		DBG_RETURN(-1);
	}

	DBG_RETURN(nCount);


}
