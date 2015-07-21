#ifndef __H_ADSL_PROC_
#define __H_ADSL_PROC_

#include "IncludeMe.h"
#include "inter.h"

int adslConnectProc(string name, string pwd, string &strErr);
int adslDisConnectProc();
UINT8 LimitKill(pid_t *pg_pid, int iSignal, unsigned long ulLimitTime_InMiniSec);


#endif
