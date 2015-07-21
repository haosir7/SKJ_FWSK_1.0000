#include "commonFunc.h"
#include "netHeaderDef.h"
#include "setToFileProc.h"
#include "vpnProc.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"


static INT8 s_VPN_IP[32];
static INT8 s_VPN_username[64];
static INT8 s_VPN_passwd[16];

static INT8 s_ori_gw_IP[32];
static INT8 s_ori_host_IP[5][32];
static INT8 s_ori_host_type[5][8];

static INT32 s_VPN_protocol=VPN_ENCRYPT_PROTOCOL;

static INT8 s_VPN_Proc_Buff[1024];




#if (_CONFIG_VPN_MODULE != 0)

int setLocalVar(const char *ip, const char *userName, const char* passwd)
{
	if(ip==NULL || userName==NULL || passwd==NULL)
	{
		return VPN_PARA_ER;
	}
	
	memset(s_VPN_IP, 0, sizeof(s_VPN_IP));
	memset(s_VPN_username, 0, sizeof(s_VPN_username));
	memset(s_VPN_passwd, 0, sizeof(s_VPN_passwd));
	
	strncpy(s_VPN_IP, ip, sizeof(s_VPN_IP));
	strncpy(s_VPN_username, userName, sizeof(s_VPN_username));
	strncpy(s_VPN_passwd, passwd, sizeof(s_VPN_passwd));
    
	prn_log2("s_VPN_IP= %s",s_VPN_IP);
	prn_log2("s_VPN_username= %s",s_VPN_username);
	
	return VPN_SUCCESS;
}

int change_xl2tp_script()
{
	INT32 ret=0;
	INT32 wrLen=0;
	INT8 tmpBuf[128];
	FILE *fp=NULL;
	
	memset(s_VPN_Proc_Buff, 0, sizeof(s_VPN_Proc_Buff));
	
	//打开文件
	fp = fopen(VPN_FILE_XL2TPD, "r");
	if (fp==NULL)
	{
		return VPN_CHANGE_XL2TP_ER;
	}
	
	//逐行读取，直到文件结束
	wrLen = 0;
	while (1)
	{
		memset(tmpBuf, 0, sizeof(tmpBuf));
		if ((ret=getLinefromFile(fp, (UINT8*)tmpBuf, sizeof(tmpBuf))) == VPN_FILE_EOF)
		{
			break;
		}
		
		if (memcmp(tmpBuf, "lns = ", strlen("lns = ")) == 0)
		{
			sprintf(tmpBuf, "lns = %s\n", s_VPN_IP);
		}
		if (memcmp(tmpBuf, "name = ", strlen("name = ")) == 0)
		{
			sprintf(tmpBuf, "name = %s\n", s_VPN_username);
		}
		if (memcmp(tmpBuf, "refuse pap", strlen("refuse pap"))==0 ||
			memcmp(tmpBuf, "refuse chap", strlen("refuse chap"))==0 ||
			memcmp(tmpBuf, "require pap", strlen("require pap"))==0 ||
			memcmp(tmpBuf, "require chap", strlen("require chap"))==0 
			)
		{
			continue;
		}
		
		strncpy(s_VPN_Proc_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
		prn_log2("wrLen = %d", wrLen);
	}
	fclose(fp);
	
	if (s_VPN_protocol == VPN_ENCRYPT_PAP)
	{
		sprintf(tmpBuf, "require pap=yes\n");
		strncpy(s_VPN_Proc_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
		sprintf(tmpBuf, "refuse chap=yes\n");
		strncpy(s_VPN_Proc_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
	}
	else if (s_VPN_protocol == VPN_ENCRYPT_CHAP)
	{
		sprintf(tmpBuf, "refuse pap=yes\n");
		strncpy(s_VPN_Proc_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
		sprintf(tmpBuf, "require chap=yes\n");
		strncpy(s_VPN_Proc_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
	}
	
	//重写文件
	fp = fopen(VPN_FILE_XL2TPD, "w");
	if (fp==NULL)
	{
		return VPN_CHANGE_XL2TP_ER;
	}
	fwrite(s_VPN_Proc_Buff, strlen(s_VPN_Proc_Buff), 1, fp);
	
	fclose(fp);
	
	return VPN_SUCCESS;
	
}

#endif	//_CONFIG_VPN_MODULE != 0 



int VPN_getOri_route()
{
#if (_CONFIG_VPN_MODULE != 0)
	prn_log("enter getOri_route");
	INT32 num=0, hostNum=0;
	INT32 ret=0;
	UINT32 tmpdesi=0, tmpgw=0;
	RouteInfo route;
	INT8 tmpBuf[128];
	FILE *fp=NULL;
	
	memset(s_ori_host_IP, 0, sizeof(s_ori_host_IP));
	memset(s_ori_host_type, 0, sizeof(s_ori_host_type));
	
	//打开文件
	fp = fopen("/proc/net/route", "r");
	if (fp==NULL)
	{
		return VPN_READ_ROUTE_ER;
	}
	prn_log("读取s_ori_gw_IP");
	//逐行读取，直到文件结束
	num=0;
	hostNum = 0;
	while (1)
	{
		memset(tmpBuf, 0, sizeof(tmpBuf));
		if ((ret=getLinefromFile(fp, (UINT8*)tmpBuf, sizeof(tmpBuf))) == VPN_FILE_EOF)
		{
			prn_log2("ret= %d",ret);
			break;
		}
		if (num==0 || strlen(tmpBuf)<10)
		{
			num++;
			continue;
		}
		
		sscanf(tmpBuf, "%s %X %X %X", route.Iface, &tmpdesi, &tmpgw, &route.Flags);
		route.Destination[0] = (UINT8)(tmpdesi & 0x000000ff);
		route.Destination[1] = (UINT8)(tmpdesi>>8 & 0x000000ff);
		route.Destination[2] = (UINT8)(tmpdesi>>16 & 0x000000ff);
		route.Destination[3] = (UINT8)(tmpdesi>>24 & 0x000000ff);
		
		route.Gateway[0] = (UINT8)(tmpgw & 0x000000ff);
		route.Gateway[1] = (UINT8)(tmpgw>>8 & 0x000000ff);
		route.Gateway[2] = (UINT8)(tmpgw>>16 & 0x000000ff);
		route.Gateway[3] = (UINT8)(tmpgw>>24 & 0x000000ff);
		
		prn_log2("Iface = %s", route.Iface);
		prn_log2("Flags = %x", route.Flags);
		prn_log5("%d.%d.%d.%d", route.Destination[0], route.Destination[1], route.Destination[2], route.Destination[3]);
		prn_log5("%d.%d.%d.%d", route.Gateway[0], route.Gateway[1], route.Gateway[2], route.Gateway[3]);
		
		if ((route.Destination[0]==0) && (route.Destination[1]==0) && (route.Destination[2]==0) && (route.Destination[3]==0))
		{
			prn_log5("%d.%d.%d.%d", route.Gateway[0], route.Gateway[1], route.Gateway[2], route.Gateway[3]);
			sprintf(s_ori_gw_IP, "%d.%d.%d.%d", route.Gateway[0], route.Gateway[1],route.Gateway[2], route.Gateway[3]);
			prn_log2("s_ori_gw_IP= %s",s_ori_gw_IP);
		}
		else
		{
			if (route.Flags == 0x0001)
			{
				num++;
				continue;
			}
			
			sprintf(s_ori_host_IP[hostNum], "%d.%d.%d.%d", route.Destination[0], route.Destination[1], 
				route.Destination[2], route.Destination[3]);
			prn_log2("s_ori_host_IP[hostNum]= %s",s_ori_host_IP[hostNum]);
			sprintf(s_ori_host_type[hostNum], "%s", route.Iface);
			prn_log2("s_ori_host_type[hostNum]= %s",s_ori_host_type[hostNum]);
			hostNum++;
		}
		
		num++;
	}
	fclose(fp);
	
	return VPN_SUCCESS;
	
#else
	return VPN_SUCCESS;
#endif	
}

int VPN_setVPN_route(int netmode)
{
#if (_CONFIG_VPN_MODULE != 0)
	int hostNum=0;
	INT8 tmpbuf[128];
    memset((void *)tmpbuf,0 ,sizeof(tmpbuf));

	//添加VPN路由节点
	prn_log3("s_VPN_IP=%s, s_ori_gw=%s",s_VPN_IP, s_ori_gw_IP);
	if(strcmp(s_VPN_IP,""))
	{
		prn_log("执行 添加VPN路由节点");
		if (netmode == NET_LAN)
		{
			sprintf(tmpbuf, "route add %s gw %s eth0", s_VPN_IP, s_ori_gw_IP);
		}
		else
		{
			sprintf(tmpbuf, "route add %s gw %s ppp0", s_VPN_IP, s_ori_gw_IP);
		}
		prn_log2("tmpbuf= %s",tmpbuf);
		system(tmpbuf);
	}

	//删除原默认网关
	if(strcmp(s_ori_gw_IP,""))
	{
		prn_log("执行 删除原默认网关");
		if (netmode == NET_LAN)
		{
			sprintf(tmpbuf, "route del default eth0");
		}
		else
		{
			sprintf(tmpbuf, "route del default ppp0");
		}
		prn_log2("tmpbuf= %s",tmpbuf);
		system(tmpbuf);
	}
	
	return VPN_SUCCESS;

#else
	return VPN_SUCCESS;
#endif	
}

int VPN_setOri_route()
{
#if (_CONFIG_VPN_MODULE != 0)
	int hostNum=0;
	INT8 tmpbuf[128];
	
	//恢复原默认网关
	if(strcmp(s_ori_gw_IP, ""))
	{
		sprintf(tmpbuf, "route add default gw %s", s_ori_gw_IP);
		prn_log2("tmpbuf= %s",tmpbuf);
		system(tmpbuf);
	}
		
	//删除VPN路由节点
	prn_log2("s_VPN_IP= %s",s_VPN_IP);
	if(strcmp(s_VPN_IP, ""))
	{
		sprintf(tmpbuf, "route del %s gw %s", s_VPN_IP, s_ori_gw_IP);
		prn_log2("tmpbuf= %s",tmpbuf);
		system(tmpbuf);
	}	
	
	return VPN_SUCCESS;
	
#else
	return VPN_SUCCESS;
#endif	
}

int setVPN_para(const char *ip, const char *userName, const char* passwd)
{
#if (_CONFIG_VPN_MODULE != 0)

	int ret=0;
	system("/rw");
	ret = setLocalVar(ip, userName, passwd);
	if (ret != VPN_SUCCESS)
	{
		prn_log2("ret = %d", ret);
		return ret;
	}
	//修改脚本文件
	ret = change_xl2tp_script();
	prn_log2("ret = %d", ret);
	if (ret != VPN_SUCCESS)
	{
		prn_log2("ret = %d", ret);
		return ret;
	}
	//修改secret文件
	ret = change_secret_file(VPN_FILE_SECRET, userName, passwd);
	prn_log2("ret = %d", ret);
	if (ret != VPN_SUCCESS)
	{
		prn_log2("ret = %d", ret);
		return ret;
	}
	return VPN_SUCCESS;
	
#else
	return VPN_SUCCESS;
#endif	
}


int VPN_connent(const char *ip, const char *userName, const char* passwd)
{
#if (_CONFIG_VPN_MODULE != 0)

	int ret=0;

	prn_log("enter VPN_Connet");
	if((ret=setLocalVar(ip,userName,passwd))!=VPN_SUCCESS)
	{
		prn_log2("ret = %d", ret);
		return ret;
	}
	

	prn_log("execute xl2tpd");
#if (_DBG_PRINT_TRANSDATA == 0)
	ret = system("../xl2tpd &");
#else
	ret = system("../xl2tpd -D &");
#endif
	if(ret != 0)
	{
		return VPN_RUN_XL2TPD_ER;
	}
	CommonSleep(2000);
	
    prn_log("execute l2tp");
	ret = system("../l2tp");
	if(ret != 0)
	{
		return VPN_RUN_L2TP_ER;
	}	
    CommonSleep(2000); //3000
    prn_log("connectProc complate");
	
	return VPN_SUCCESS;
#else
	return VPN_SUCCESS;
#endif	
}

int VPN_close(int netmode)
{
#if (_CONFIG_VPN_MODULE != 0)

	int ret=0;
	system("../l2tp_end");
	
	while(1)
	{
		if (netmode == NET_LAN)				//遗留了一处ifconfig判断，待改进
			ret = system("ifconfig ppp0");
		else
			ret = system("ifconfig ppp1");
		prn_log2("ret = %d", ret);
		if (ret != 0)	
		{
			system("killall xl2tpd");
			CommonSleep(1000);
			break;
		}
		CommonSleep(1000);
	}
	
	return VPN_SUCCESS;

#else
	return VPN_SUCCESS;
#endif	
}



