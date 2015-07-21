#include "CCbhz.h"

#include "LOGCTRL.h"
#include "pos_debug.h"


CCbhz::CCbhz()
{		
	m_roll = 1;

	SET_TABLE_NAME("CB_HZ");
	SET_FIELD_NUM(20);
	
	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Ssq, "SSQ", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Ssqq, "SSQ_Q", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Ssqz, "SSQ_Z", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Zpfs, "ZPFS", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_Zpkpje, "ZPKPJE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Hpfs, "HPFS", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_Hpkpje, "HPKPJE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Zffs, "ZFFS", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_Zfkpje, "ZFKPJE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Hffs, "HFFS", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_Hfkpje, "HFKPJE", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Kbfpfs, "KBFPFS", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Bsrq, "BSRQ", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Bscgrq, "BSCGRQ", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Bssbyy, "BSSBYY", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_Bslx, "BSLX", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_Bscgbz, "BSCGBZ", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_Bsjzbz, "BSJZBZ", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Wscfps, "WSCFPS", 0);

	clear();
}

CCbhz::~CCbhz()
{
}

void CCbhz::clear()
{
	m_Ssq = 0;		
	m_Ssqq = 0;		
	m_Ssqz = 0;		
	m_Zpfs = 0;		
	m_Zpkpje = 0;	
	m_Hpfs = 0;		
	m_Hpkpje = 0;	
	m_Zffs = 0;		
	m_Zfkpje = 0;	
	m_Hffs = 0;		
	m_Hfkpje = 0;	
	m_Kbfpfs = 0;	
	m_Bsrq = 0;		
	m_Bscgrq = 0;	
	m_Bssbyy.assign("");	
	m_Bslx = 0;		
	m_Bscgbz = 0;	
	m_Bsjzbz = 0;	
	m_Wscfps = 0;
}
