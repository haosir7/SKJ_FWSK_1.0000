#include "CSysArg.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CSysArg::CSysArg()
{
	m_said = 0;
	m_name = "";
	m_vInt = 0;
	m_vText = "";
	memset((void *)m_vBlob, 0x00, sizeof(m_vBlob));

	SET_TABLE_NAME("SYSARG");
	SET_FIELD_NUM(5);
	
	SET_FIELD_MEMBER(DB_UINT8, &m_said, "SA_ID", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_name, "NAME", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_vInt, "V_INT", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_vText, "V_TEXT", 0);
	SET_FIELD_MEMBER(DB_BLOB, m_vBlob, "V_BLOB", SYS_ARG_BLOB_LEN);
}

CSysArg::~CSysArg()
{
}

