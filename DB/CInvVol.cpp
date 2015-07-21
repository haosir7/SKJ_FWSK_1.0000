#include "CInvVol.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CInvVol::CInvVol()
{
	m_roll = 1;
	
	SET_TABLE_NAME("INV_VOL");
	SET_FIELD_NUM(13);

	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_invtype, "INV_TYPE", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_code, "CODE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_isno, "IS_NO", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_ieno, "IE_NO", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_remain, "REMAIN", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_fpzfs, "FPZFS", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_usedflag, "USED_FLAG", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_overflag, "OVER_FLAG", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_date, "RQ", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fplxdm, "FPLXDM", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_backup1, "BACKUP1", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_backup2, "BACKUP2", 0);

	ResetVol();

}

CInvVol::~CInvVol()
{
}

void CInvVol::DelUsedVol()
{
	m_filter = "where USED_FLAG = 0 and OVER_FLAG = 0";
	Delete();	
}

void CInvVol::ResetVol()
{
	m_invtype = 0;
	m_code = "";		
	m_isno = 0;			
	m_ieno = 0;			
	m_remain = 0;		
	m_fpzfs = 0;
	m_usedflag = 1;		
	m_overflag = 1;		
	m_kindcode = "";	
	m_date = 0;			/**< 购买日期 */	
	m_backup1 = 0;		/**< 备用字段1 */
	m_backup2 = "";		/**< 备用字段2 */	
	
	m_fplxdm = "";
	m_curInvNo = 0;
	m_buyDate = "";
	m_FlagJSorXZ = 0;
	m_fpzl = 0;
	m_lbdm = "";
	m_flag = "";
	m_gpsq = "";
	m_yssh ="";
	m_dqbh ="";
	m_dcbb ="";
	m_fpjjsmw = "";
	m_fpjmw = "";
}
