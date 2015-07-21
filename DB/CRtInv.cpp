#include "CRtInv.h"
#include "SysMacDef.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

#include <stdio.h>


CRtInv::CRtInv()
{
	m_roll = 1;
	m_fpdm = "";
	m_fphm = 0;
	
	SET_TABLE_NAME("RT_INV");
	SET_FIELD_NUM(3);
     	
    SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
    SET_FIELD_MEMBER(DB_TEXT, &m_fpdm, "FPDM", 0);
    SET_FIELD_MEMBER(DB_UINT32, &m_fphm, "FPHM", 0);
    
}

CRtInv::~CRtInv()
{
}


INT32 CRtInv::CheckRoll(INT32 nCount, UINT32 &nDelNo)
{
	DBG_ENTER("CRtInv::CheckRolll");
	
	INT32 nNoStart, nNoEnd;
	nDelNo = 0;

	if (nCount>=RT_INV_MAX) //超过容纳最大记录数
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

		nNoEnd = nNoStart + RT_INV_DEL_MAX;
		nDelNo = nNoEnd;
		DBG_RETURN(SUCCESS);
	}

	DBG_RETURN(SUCCESS);	
}

INT32 CRtInv::Roll(UINT32 nDelNo)
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
