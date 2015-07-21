#include "CMachine.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CMachine::CMachine()
{
	SET_TABLE_NAME("MACHINE");
	SET_FIELD_NUM(4);

	SET_FIELD_MEMBER(DB_TEXT, &m_machineNo, "M_NO", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_pDate, "P_DATE", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_hVer, "H_VER", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_sVer, "S_VER", 0);

	m_machineNo = "";
	m_pDate = "";
	m_hVer = "";
	m_sVer = "";
	m_IMEI = "";

}

CMachine::~CMachine()
{
}




