#include <stdio.h>      
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "comdatatype.h"
#include "ipInfoProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"



int getIPandMASK(const char *keyword, IPinfo *info)
{
	int sock_get_ip;  
	struct   sockaddr_in *sin;  
	struct   ifreq ifr_ip;     

	if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{  
		prn_log("create socket failse! (GetLocalIp)");  
		return OPEN_SOCKET_ER;  
	}  

	memset(&ifr_ip, 0, sizeof(ifr_ip));     
	strncpy(ifr_ip.ifr_name, keyword, sizeof(ifr_ip.ifr_name) - 1);     

	if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )     
	{     
		prn_log("ioctl SIOCGIFADDR fail!");  
		close( sock_get_ip );  
		return GET_IP_FAIL;     
	}       
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
	strncpy(info->IP, inet_ntoa(sin->sin_addr), sizeof(info->IP));         

	if( ioctl( sock_get_ip, SIOCGIFNETMASK, &ifr_ip) < 0 )     
	{     
		prn_log("ioctl SIOCGIFNETMASK fail!");  
		close( sock_get_ip );  
		return GET_MASK_FAIL;     
	}       
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
	strncpy(info->Mask, inet_ntoa(sin->sin_addr), sizeof(info->Mask));         

	
	close( sock_get_ip );  

	return GET_IPINFO_SUCCESS;  	
}


int getRoute(IPinfo *info)
{
	FILE *fp=NULL;
	INT8 *chTemp=NULL;
	INT8 *pchToken = " ";//¿Õ¸ñ
	INT8 lineBuf[128];
	
	system("rm -fr /swap/gateway");
	system("route > /swap/gateway");

	fp = fopen("/swap/gateway", "r");
	if( fp == NULL )
	{
		return GET_ROUTE_FAIL;
	}
	memset((void*)lineBuf, 0, sizeof(lineBuf));	
	fgets(lineBuf, sizeof(lineBuf), fp);
	while (!feof(fp)) 
	{
		fgets(lineBuf, sizeof(lineBuf), fp);
		if( (chTemp = strtok(lineBuf, pchToken)) == NULL ) 
		{
			continue;
		}
		else
		{
			if( memcmp(chTemp, "default", 7) == 0 )
			{
				if( (chTemp = strtok(NULL, pchToken)) != NULL )
				{
					strncpy(info->GeteWay, chTemp, sizeof(info->GeteWay));
					break;
				}
			}
		}
	}
	fclose(fp);	
	
	return GET_IPINFO_SUCCESS;  	
}

int getDNS(IPinfo *info)
{
	INT32 res=0;
	FILE *fp=NULL;
	INT8 lineBuf[128];
	INT8 tmpBuf1[32];
	
	fp = fopen("/etc/resolv.conf", "r");
	if( fp == NULL )
	{
		return GET_DNS_FAIL;
	}
	while (!feof(fp))
	{
		fgets(lineBuf, sizeof(lineBuf), fp);
		
		res = sscanf(lineBuf, " nameserver %s", tmpBuf1);
		if (res == 1)
		{
			strncpy(info->DNS, tmpBuf1, sizeof(info->DNS));
			break;
		}
	}
	fclose(fp);
	
	return GET_IPINFO_SUCCESS;  	
}

