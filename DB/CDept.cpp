#include "CDept.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CDept::CDept()
{
	SET_TABLE_NAME("DEPT");
	SET_FIELD_NUM(11);

    SET_FIELD_MEMBER(DB_TEXT, &m_spbm, "SP_BM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spmc, "SP_MC", 0);
	SET_FIELD_MEMBER(DB_DOUBLE, &m_spdj, "SP_DJ", 0);
	SET_FIELD_MEMBER(DB_FLOAT, &m_spsl, "SP_SL", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spdw, "SP_DW", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_blh, "BLH", 0);

	SET_FIELD_MEMBER(DB_TEXT, &m_spjm, "SP_JM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spgg, "SP_GG", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_zspmdm, "ZSPM_DM", 0);
	SET_FIELD_MEMBER(DB_FLOAT, &m_spzsl, "SP_ZSL", 0);
	SET_FIELD_MEMBER(DB_FLOAT, &m_rate, "D_RATE", 0);

	m_spbm="";
	m_spmc="";
	m_spdj=0.0;
	m_spsl=0.0;
	m_spdw = "¼þ";
	m_blh = 0;
	
	m_spjm = "";
	m_spgg = "";
	m_zspmdm = "";
	m_spzsl = 0.0;
	m_rate = 100.0;
}

CDept::~CDept()
{
}


