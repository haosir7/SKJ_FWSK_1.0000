#include "commonFunc.h"
#include "netHeaderDef.h"
#include "setToFileProc.h"
#ifndef _DEBUG_VERSION
#include "powoff_protect.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"


static INT8 s_Operate_Buff[1024];

int updateInitFileForDHCP(int dchpMode)
{
	INT32 res=0;
	INT8 tmpBuf1[16], tmpBuf2[16];
	INT8 lineBuf[128];
	FILE *fps, *fpd;

	system("/rw");

	system("rm /bin/init_new");		
	fps = fopen("/bin/init", "r");
	fpd = fopen("/bin/init_new", "w");	//先将修改后的内容写入init_new文件

	if(dchpMode == LAN_DHCP_ON)
	{
		while (!feof(fps))
		{
			memset(lineBuf, 0, sizeof(lineBuf));
			fgets(lineBuf, sizeof(lineBuf), fps);
			
			//匹配关键行并修改
			res = sscanf(lineBuf, " ifconfig eth0 %s netmask %s", tmpBuf1, tmpBuf2);
			if (res == 2)
			{
				prn_log("IP and Mask matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "#ifconfig eth0 %s netmask %s\n", tmpBuf1, tmpBuf2);
			}
			
			res = sscanf(lineBuf, " route add default gw %s", tmpBuf1);
			if (res == 1)
			{
				prn_log("route matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "#route add default gw %s\n", tmpBuf1);
			}
			
			if( strstr(lineBuf, "ifconfig eth0 up") != NULL)
			{
				prn_log("eth0 up matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "#ifconfig eth0 up\n");
			}

			if( strstr(lineBuf, "dhcpc-new") != NULL)
			{
				prn_log("dhcpc-new matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "/bin/dhcpc-new &\n");
			}

			//将修改后的内容写入新文件
			fputs(lineBuf, fpd);
		}
	}
	else if(dchpMode == LAN_DHCP_OFF)		
	{
		while (!feof(fps))
		{
			memset(lineBuf, 0, sizeof(lineBuf));
			fgets(lineBuf, sizeof(lineBuf), fps);
			
			//匹配关键行并修改
			res = sscanf(lineBuf, " #ifconfig eth0 %s netmask %s", tmpBuf1, tmpBuf2);
			if (res == 2)
			{
				prn_log("#IP and Mask matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "ifconfig eth0 %s netmask %s\n", tmpBuf1, tmpBuf2);
			}
			
			res = sscanf(lineBuf, " #route add default gw %s", tmpBuf1);
			if (res == 1)
			{
				prn_log("#route matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "route add default gw %s\n", tmpBuf1);
			}
			
			if( strstr(lineBuf, "ifconfig eth0 up") != NULL)
			{
				prn_log("eth0 up matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "ifconfig eth0 up\n");
			}
			
			if( strstr(lineBuf, "dhcpc-new") != NULL)
			{
				prn_log("dhcpc-new matched SUCCESS.");
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "#/bin/dhcpc-new &\n");
			}
			
			fputs(lineBuf, fpd);
		}
	}
	fclose(fps);
	fclose(fpd);

	CommonSleep(1000);

	//替换原始文件，需掉电保护
	POWOFF_DISABLE();
	system("cp /bin/init_new /bin/init");
	CommonSleep(500);
	
	system("chmod +x /bin/init");
	CommonSleep(500);
	POWOFF_ENABLE();

	return NET_SUCCESS;

}



int setIPinfoToFile(IPinfo *info)
{
	int ret = 0;
	
	//检查ip输入有效性
	if ((ret=isIPValid(info->IP)) != NET_SUCCESS)
		return ret;
	if ((ret=isIPValid(info->Mask)) != NET_SUCCESS)
		return ret;
	if ((ret=isIPValid(info->GeteWay)) != NET_SUCCESS)
		return ret;
	if ((ret=isIPValid(info->DNS)) != NET_SUCCESS)
		return ret;

	//修改相关文件
	if ((ret=updateInitFileForIP(info)) != NET_SUCCESS)
		return ret;
	if ((ret=updateResolvFileForDNS(info)) != NET_SUCCESS)
		return ret;

	return NET_SUCCESS;
}

int restartLAN(IPinfo *info)
{
	INT8 lineBuf[128];

	system("ifconfig eth0 down");
	sprintf(lineBuf, "ifconfig eth0 %s netmask %s", info->IP, info->Mask);
	prn_log2("lineBuf: %s", lineBuf);
	system(lineBuf);
	sprintf(lineBuf, "route add default gw %s", info->GeteWay);
	prn_log2("lineBuf: %s", lineBuf);
	system(lineBuf);
	system("ifconfig eth0 up");
	CommonSleep(500);

	return NET_SUCCESS;
}

int restartDHCP(int dhcpMode, IPinfo *info)
{
	prn_log2("dhcpMode = %d", dhcpMode);
	if (dhcpMode == LAN_DHCP_OFF)
	{
		system("ifconfig eth0 down");
		system("killall dhcpc-new");
		CommonSleep(1000);
		restartLAN(info);
	}
	else if (dhcpMode == LAN_DHCP_ON)
	{
		system("ifconfig eth0 down");
		system("/bin/dhcpc-new &");
		CommonSleep(2000);
	}

	return NET_SUCCESS;
}


int updateInitFileForIP(IPinfo *info)
{
	INT32 res = 0;
	INT8 tmpBuf1[16], tmpBuf2[16];
	INT8 lineBuf[128];
	FILE *fps, *fpd;

	system("/rw");

	system("rm /bin/init_new");
	fps = fopen("/bin/init", "r");
	fpd = fopen("/bin/init_new", "w");

	while (!feof(fps))
	{
		memset(lineBuf, 0, sizeof(lineBuf));		
		fgets(lineBuf, sizeof(lineBuf), fps);

		//匹配关键行，并修改内容
		res = sscanf(lineBuf, " ifconfig eth0 %s netmask %s", tmpBuf1, tmpBuf2);
		if (res == 2)
		{
			prn_log("IP and Mask matched SUCCESS.");
			sprintf(lineBuf, "ifconfig eth0 %s netmask %s\n", info->IP, info->Mask);
		}
		
		res = sscanf(lineBuf, " route add default gw %s", tmpBuf1);
		if (res == 1)
		{
			prn_log("route matched SUCCESS.");
			sprintf(lineBuf, "route add default gw %s\n", info->GeteWay);
		}
		
		//将修改后的内容写入新文件
		fputs(lineBuf, fpd);
	}

	fclose(fps);
	fclose(fpd);
	CommonSleep(500);

	//替换原始文件，需掉电保护
	POWOFF_DISABLE();
	system("cp /bin/init_new /bin/init");
	CommonSleep(200);
	system("chmod +x /bin/init");
	POWOFF_ENABLE();

	system("rm /bin/init_new");
	CommonSleep(200);

	return NET_SUCCESS;
}


int updateResolvFileForDNS(IPinfo *info)
{
	INT32 res = 0;
	INT8 tmpBuf1[16], tmpBuf2[16];
	INT8 lineBuf[128];
	FILE *fps, *fpd;

	// 生成新的resolv.conf文件
	system("rm /etc/resolv.conf_new");
	fps = fopen("/etc/resolv.conf", "r");
	fpd = fopen("/etc/resolv.conf_new", "w");

	while (!feof(fps))
	{
		memset(lineBuf, 0, sizeof(lineBuf));
		fgets(lineBuf, sizeof(lineBuf), fps);

		//如果是关键行，则修改内容
		res = sscanf(lineBuf, " nameserver %s", tmpBuf1);
		if (res == 1)
		{
			prn_log("DNS matched SUCCESS.");
			sprintf(lineBuf, "nameserver %s\n", info->DNS);
		}
		
		fputs(lineBuf, fpd);
	}
	fclose(fps);
	fclose(fpd);
	CommonSleep(500);	

	//替换原始文件，需掉电保护
	POWOFF_DISABLE();
	system("cp /etc/resolv.conf_new /etc/resolv.conf");
	POWOFF_ENABLE();
	
	system("rm /etc/resolv.conf_new");
	CommonSleep(200);
	return NET_SUCCESS;	
}

int isIPValid(const INT8 *ip)
{
	INT32 res, i;	
	UINT32 x[4];

	if (strlen(ip) > 15)
		return NET_INPUT_IP_ER;

	res = sscanf(ip, "%u.%u.%u.%u", x, x+1, x+2, x+3);
	if (res != 4)
		return NET_INPUT_IP_ER;

	for (i=0; i<4; i++)
	{
		prn_log3("x[%d]=%d", i, x[i]);
		if (x[i] > 255)
			return NET_INPUT_IP_ER;
	}
	prn_log("NET_SUCCESS\n");

	return NET_SUCCESS;
}

int getLinefromFile(FILE *fp, UINT8 *pbuf, INT32 bufLen)
{
	if (fp==NULL)
		return FP_POINT_NULL_ER;
	
	if (fgets((INT8 *)pbuf, bufLen, fp) == NULL)
		return FILE_EOF;
	//	prn_log2("pbuf = %s", pbuf);
	//	prn_log2("len = %d", strlen(pbuf));
	
	return 0;
}

int change_secret_file(INT8 *filename, const INT8 *user, const INT8 *pwd)
{
	INT32 ret=0;
	INT32 flag=0;
	INT32 wrLen=0;
	INT8 tmpBuf[128];
	INT8 tmpUser[64];
	FILE *fp=NULL;
	
	memset(s_Operate_Buff, 0, sizeof(s_Operate_Buff));
	prn_log3("filename= %s, user=%s",filename, user);
	system("/rw");
	//打开文件
	fp = fopen(filename, "r");
	if (fp==NULL)
	{
		return NET_OPERATOR_FILE_ER;
	}
	
	//将user添加双引号字串
	memset(tmpUser, 0, sizeof(tmpUser));
	tmpUser[0] = 0x22; //"
	strncpy(tmpUser+1, user, sizeof(tmpUser)-1);
	tmpUser[strlen(tmpUser)] = 0x22;
	tmpUser[strlen(tmpUser)] = 0x0;

	//逐行读取，直到文件结束
	flag = 0;
	wrLen = 0;
	while (1)
	{
		memset(tmpBuf, 0, sizeof(tmpBuf));
		if ((ret=getLinefromFile(fp, (UINT8 *)tmpBuf, sizeof(tmpBuf))) == FILE_EOF)
		{
			break;
		}
		
		//比较用户名字段，如相等则修改该行
		if (memcmp(tmpBuf, tmpUser, strlen(tmpUser)) == 0)
		{
			flag = 1;
			sprintf(tmpBuf, "\"%s\"			*		\"%s\"\n", user, pwd);
		}
		strncpy(s_Operate_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
		wrLen += strlen(tmpBuf);
		//prn_log2("wrLen = %d", wrLen);
	}
	fclose(fp);
	if (flag == 0)
	{
		sprintf(tmpBuf, "\"%s\"			*		\"%s\"\n", user, pwd);
		strncpy(s_Operate_Buff+wrLen, tmpBuf, sizeof(tmpBuf));
	}
	
	//重写文件
	fp = fopen(filename, "w");
	if (fp==NULL)
	{
		return NET_OPERATOR_FILE_ER;
	}
	fwrite(s_Operate_Buff, strlen(s_Operate_Buff), 1, fp);
	
	fclose(fp);
	
	return NET_SUCCESS;
}
	
int updateScriptFileFor3G(const char *file, const char *name)
{
	INT32 res = 0;
	INT8 tmpBuf1[64];
	INT8 lineBuf[64];
	INT8 tmpFile[] = "/etc/ppp/peers/tmp_script";
	FILE *fps, *fpd;

	system("/rw");

	// 生成新拨号脚本文件
	sprintf(tmpBuf1, "rm %s", tmpFile);
	system(tmpBuf1);
	fps = fopen(file, "r");
	fpd = fopen(tmpFile, "w");

	while (!feof(fps))
	{
		memset(lineBuf, 0, sizeof(lineBuf));
		fgets(lineBuf, sizeof(lineBuf), fps);

		//如果是关键行，则修改内容
		res = sscanf(lineBuf, " name %s", tmpBuf1);
		if (res == 1)
		{
			prn_log("name matched SUCCESS.");
			sprintf(lineBuf, "name \"%s\"\n", name);
		}
		
		fputs(lineBuf, fpd);
	}
	fclose(fps);
	fclose(fpd);
	CommonSleep(500);	

	//替换原始文件，需掉电保护
	POWOFF_DISABLE();
	sprintf(tmpBuf1, "cp %s %s", tmpFile, file);
	system(tmpBuf1);
	POWOFF_ENABLE();
	
	sprintf(tmpBuf1, "rm %s", tmpFile);
	system(tmpBuf1);
	CommonSleep(200);
	return NET_SUCCESS;	
}

