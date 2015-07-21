#include "CTax.h"

#include "LOGCTRL.h"
#include "pos_debug.h"


CTax::CTax()
{
	m_roll = 1;	

	SET_TABLE_NAME("TAX");
	SET_FIELD_NUM(3);

	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_sl, "SL", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_backup, "BACKUP", 0);
	
	ResetTax();
}

CTax::~CTax()
{
}

void CTax::ResetTax()
{	
	m_no= 0;
	m_sl= 0;        /**< 税率 */
	m_backup= "";    /**< 备用 */
}
