#include "CClient.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CClient::CClient()
{
	SET_TABLE_NAME("CLIENT");
	SET_FIELD_NUM(5);

	SET_FIELD_MEMBER(DB_TEXT, &m_khbh, "KH_BH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_khdm, "KH_DM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_khjm, "KH_JM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_khmc, "KH_MC", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_khsh, "KH_SH", 0);

	m_khbh = "";
	m_khdm = "";
	m_khjm = "";
	m_khmc = "";
	m_khsh = "";
}

CClient::~CClient()
{
}
