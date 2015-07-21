

#include "APIBase.h"
#include "APIskjProc.h"
#include "APIzhqProc.h"


#if TYPE_MODE == SKJ_MODE
static CAPIskjProc APIskjProc;
#endif
#if TYPE_MODE == ZHQ_MODE
static CAPIzhqProc APIzhqProc;
#endif

CAPIBase *g_pAPIBase = NULL;



void CAPIBase::InitAPI()
{
#if TYPE_MODE == SKJ_MODE
	g_pAPIBase = &APIskjProc;
#endif
#if TYPE_MODE == ZHQ_MODE
	g_pAPIBase = &APIzhqProc;
#endif
}




