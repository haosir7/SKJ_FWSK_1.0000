#ifndef __IPINFO_PROC_H_
#define __IPINFO_PROC_H_

#include "netHeaderDef.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define	GET_IPINFO_SUCCESS					0
#define	GET_IP_FAIL							-1
#define	GET_MASK_FAIL						-2
#define	GET_ROUTE_FAIL						-3
#define	GET_DNS_FAIL						-4
#define	OPEN_SOCKET_ER						-10
#define	OPEN_FILE_ER						-11



int getIPandMASK(const char *keyword, IPinfo *info);
int getRoute(IPinfo *info);
int getDNS(IPinfo *info);


#ifdef __cplusplus
}
#endif


#endif

