
#include "YWXMLGY.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CYWXML_GY* CYWXML_GY::m_YwXmlArg = NULL;
CYWXML_GY *g_YwXmlArg = NULL;

CYWXML_GY::CYWXML_GY()
{
	m_strID = "";
	m_jqbh  = "";
	m_nsrsbh = "";
	m_nsrmc = "";
	m_sksbbh = "";
	m_sksbkl = "";
	m_fplxdm = "";
	m_bspbh = "";
	m_bspkl = "";
	m_kpjh = "";
	m_zskl = "";
}
CYWXML_GY::~CYWXML_GY()
{
	if (m_YwXmlArg != NULL)
	{
		delete m_YwXmlArg;
		m_YwXmlArg = NULL;
	}
}

CYWXML_GY* CYWXML_GY::GetInstance(void)
{
	if (m_YwXmlArg == NULL)
	{
		m_YwXmlArg = new CYWXML_GY();
	}
	
	DBG_RETURN(m_YwXmlArg);
}


