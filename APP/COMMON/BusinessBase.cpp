

#include "BusinessBase.h"
#include "BusinessJSKProc.h"
#include "BusinessXmlProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


#if BUSINESS_TYPE_MODE == BUSINESS_JSK_MODE
#include "SKJYWXmlBase.h"
static CBusinessJSKProc busJSKProc;
#endif
#if BUSINESS_TYPE_MODE == BUSINESS_XML_MODE
static CBusinessXmlProc busXmlProc;
#endif

CBusinessBase *g_pBusBase = NULL;



void CBusinessBase::InitBusiness()
{
#if BUSINESS_TYPE_MODE == BUSINESS_JSK_MODE
	g_pBusBase = &busJSKProc;
#endif
#if BUSINESS_TYPE_MODE == BUSINESS_XML_MODE
	g_pBusBase = &busXmlProc;
#endif
}

INT32 CBusinessBase::ErrParse(INT32 retCode, string &retMsg)
{
#if BUSINESS_TYPE_MODE == BUSINESS_JSK_MODE
	if(retCode == JSK_SUCCESS)
		return SUCCESS;
#endif
#if BUSINESS_TYPE_MODE == BUSINESS_XML_MODE
	if(retCode == XML_SUCCESS)
		return SUCCESS;
#endif
	
	if( (retCode == JSK_FAILURE) || (retCode == XML_FAILURE) )
	{
		return FAILURE;
	}

	if(retMsg.length() > MAX_ERR_MSG_LEN)
		retMsg.erase(MAX_ERR_MSG_LEN, retMsg.length()-MAX_ERR_MSG_LEN);

	INT8 tmpBuf[512];
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf(tmpBuf, "%d:%s", retCode, retMsg.c_str());
	retMsg = tmpBuf;
	DBG_PRINT(("retMsg = %s", retMsg.c_str()));

	return FAILURE;
}



