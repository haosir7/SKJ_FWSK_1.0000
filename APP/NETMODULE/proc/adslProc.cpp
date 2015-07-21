#include <signal.h> 
#include "commonFunc.h"
#include "netHeaderDef.h"
#include "adslProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


int adslConnectProc(string name, string pwd, string &strErr)
{
#if (_CONFIG_ADSL_MODULE != 0)

	int len=0;
	FILE *pppd_fd=NULL;
	struct ppp_info my_pppd_info;	
	time_t now,last;

	//在配置文件写入ADSL用户名与密码
	pppd_fd = fopen(CONFIG_FILE_PPPOE, "wb");
	if(pppd_fd == NULL)
	{
		strErr = "打开配置文件错误";
		return NET_OPERATOR_FILE_ER;
	}

	memset((char *)(&my_pppd_info), 0, sizeof(struct ppp_info));
	memcpy(my_pppd_info.StartParam.StartParam.e.pcUserName, name.c_str(), name.length());
	memcpy(my_pppd_info.StartParam.StartParam.e.pcUserPass, pwd.c_str(), pwd.length());

	len=fwrite((char *)(&my_pppd_info), sizeof(struct ppp_info), 1, pppd_fd);
	DBG_PRINT(("write %d bytes into file.", len));
	fclose(pppd_fd);

	//执行拨号
	system("/bin/pppd pppoe &");
	time(&now);
	last = now;
	CommonSleep(6000);	

	//循环读取配置文件以查询拨号结果
	do {
		pppd_fd = fopen(CONFIG_FILE_PPPOE, "r");
		if(pppd_fd == NULL)
		{
			strErr = "打开配置文件错误";
			return NET_OPERATOR_FILE_ER;
		}
		memset((char *)(&my_pppd_info), 0, sizeof(struct ppp_info));
		fread((char*)(&my_pppd_info), sizeof(struct ppp_info), 1, pppd_fd);
		fclose(pppd_fd);

		DBG_PRINT(("adsl username = %s", my_pppd_info.StartParam.StartParam.e.pcUserName));
		DBG_PRINT(("adsl userpwd = %s", my_pppd_info.StartParam.StartParam.e.pcUserPass));
		DBG_PRINT(("adsl status = %lu", my_pppd_info.status));

		time(&now);
		if ( now > last + ADSL_TIME_OUT)
		{
			DBG_PRINT(("time out"));
			system("killall pppd");
			system("rm -fr /swap/hosts");
			system("rm -fr /swap/.pppd_info");
			system("rm -fr /var/run/ppp*");
			strErr = "连接超时,请重试";
			return NET_CONNECT_FAIL;
		}
		
		CommonSleep(1000);
		
	} while(my_pppd_info.status-ADSL_STATUS_NUM == CONNECTION_TRYING);

	//判断拨号结果
	DBG_PRINT(("adsl loc_addr = %s", my_pppd_info.loc_addr));
	DBG_PRINT(("adsl rem_addr = %s", my_pppd_info.rem_addr));
	DBG_PRINT(("adsl dnsaddr = %s", my_pppd_info.dnsaddr[0]));
	DBG_PRINT(("adsl gateway = %s", my_pppd_info.gateway));
	DBG_PRINT(("adsl netmask = %s", my_pppd_info.netmask));
	if( my_pppd_info.status-ADSL_STATUS_NUM == CONNECTION_ON )
	{
		DBG_PRINT(("ADSL connect SUCCESS!"))
		return NET_SUCCESS;
	}
	else
	{
		DBG_PRINT(("ADSL connect FAILURE!"))
		strErr = "拨号连接失败";
		return NET_CONNECT_FAIL;
	}

#else
	return NET_SUCCESS;
#endif
}


int adslDisConnectProc()
{
#if (_CONFIG_ADSL_MODULE != 0)
	
    FILE *pppd_fd;
    struct ppp_info my_pppd_info;	
	
	pppd_fd = fopen(CONFIG_FILE_PPPOE, "r");
	if(pppd_fd == NULL)
	{
		return NET_OPERATOR_FILE_ER;
	}
	memset((char *)(&my_pppd_info), 0, sizeof(struct ppp_info));
	fread((char*)(&my_pppd_info), sizeof(struct ppp_info), 1, pppd_fd);
	fclose(pppd_fd);

	if( LimitKill(&(my_pppd_info.thttpd_pid), SIGTERM, 5000) != NET_SUCCESS )
	{
		return NET_CONNECT_FAIL;
	}

	system("killall pppd");
	system("rm -fr /swap/hosts");
	system("rm -fr /swap/.pppd_info");
	system("rm -fr /var/run/ppp*");
	
	return NET_SUCCESS;

#else
	return NET_SUCCESS;
#endif
}

UINT8 LimitKill(pid_t *pg_pid, int iSignal, unsigned long ulLimitTime_InMiniSec)
{
#if (_CONFIG_ADSL_MODULE != 0)

	unsigned long i;
	if (*pg_pid == 0) 
		return NET_SUCCESS;

	if (kill(*pg_pid, iSignal) != 0)
	{
		DBG_PRINT(("Can not kill process %d.", *pg_pid));
		return NET_CONNECT_FAIL;
	}

	for (i=0; i<ulLimitTime_InMiniSec/50 && *pg_pid != 0; i++)
		CommonSleep(50);

	if (*pg_pid != 0)
	{
		DBG_PRINT(("Can not kill process %d in %d ms.", *pg_pid, ulLimitTime_InMiniSec));
		return NET_CONNECT_FAIL;
	}
	else 
		return NET_SUCCESS;


#else
	return NET_SUCCESS;
#endif

}

	