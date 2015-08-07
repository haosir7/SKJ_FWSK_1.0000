#include "CInvUpFailInfo.h"
#include "SysMacDef.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"

#include <stdio.h>


CInvUpFailInfo::CInvUpFailInfo()
{
	m_fpdm = "";
	m_fphm = 0;
	m_ErrInfo = "";
}

CInvUpFailInfo::~CInvUpFailInfo()
{
}
