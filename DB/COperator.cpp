#include "COperator.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

COperator::COperator()
{
	SET_TABLE_NAME("OPERATOR");
	SET_FIELD_NUM(4);

	SET_FIELD_MEMBER(DB_UINT8, &m_opid, "OP_ID", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_name, "NAME", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_passwd, "PASSWD", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_role, "ROLE", 0);		

	m_opid = 0;
	m_name = "";
	m_passwd = "";
	m_role = 0;
}

COperator::~COperator()
{
}


