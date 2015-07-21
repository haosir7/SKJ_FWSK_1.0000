#ifndef	__H_PARA_SET_PROC_
#define	__H_PARA_SET_PROC_

#include <stdio.h>
#include "comdatatype.h"
#include "netHeaderDef.h"

#define FP_POINT_NULL_ER		-1
#define FILE_EOF				1


#ifdef __cplusplus
extern "C"
{
#endif

	
int updateInitFileForDHCP(int dchpMode);
int setIPinfoToFile(IPinfo *info);
int restartLAN(IPinfo *info);
int restartDHCP(int dhcpMode, IPinfo *info);
int isIPValid(const INT8 *ip);
int change_secret_file(INT8 *filename, const INT8 *user, const INT8 *pwd);
int updateScriptFileFor3G(const char *file, const char *name);


//内部使用
int updateInitFileForIP(IPinfo *info);
int updateResolvFileForDNS(IPinfo *info);

int getLinefromFile(FILE *fp, UINT8 *pbuf, INT32 bufLen);


#ifdef __cplusplus
}
#endif

#endif

