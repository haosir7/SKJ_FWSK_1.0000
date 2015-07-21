/**
 @file  M2M_APN_Connection.cpp
 @brief 本文件实现了与M2M-APN模式有关的几个派生类。
 @author   zl
*/
#include "drv_modem.h"
#include "wmmp.h"
#include "commonFunc.h"
#include "CommonCheck.h"
#include "CommonGetIPinfo.h"
#include "M2M_APN_Connection.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"



int connect_M2M_APN::connect(string &strErr)
{
	INT32 i = 0; 
	INT32 sysRet = 0; 
	const INT32 MAX_RETRY_TIMES=5;	
	CNetConnection *p = m_pConnection;
	
	DBG_PRINT(("init M2M_APN..."));
	if (p->performCheck() == CHECK_OK)
	{
		DBG_PRINT(("M2M_APN已经连接"))
		return NET_SUCCESS;
	}
	
	//apn拨号过程
	system("ifconfig eth0 down"); 
	
	system("/bin/pppd_modem call mobile &");
	CommonSleep(1000);
	
	for(i=0; i<MAX_RETRY_TIMES; i++)
	{			
		if (p->performCheck() == CHECK_OK)
		{
			CommonSleep(3000);
			break;	
		}
		else
		{
			CommonSleep(3000);
		}
	}

	if(i==MAX_RETRY_TIMES)
	{
		system("killall pppd_modem");
		strErr = "APN连接失败";
		return NET_CONNECT_FAIL;
	}

	p->m_dialState = HAVE_DIALED;
	DBG_PRINT(("apn connect successed"));
	return NET_SUCCESS;
}

int disconnect_M2M_APN::disConnect()
{
	CNetConnection *p = m_pConnection;

	if (p->performCheck() == CHECK_OK) 
	{
		system("killall pppd_modem");
		CommonSleep(10000);
	}
	else
	{
		DBG_PRINT(("APN不在连接状态: %d", p->m_dialState));
	}

	p->m_dialState = NOT_DIALED;
	DBG_PRINT(("M2M_APN连接已断开"))
	return NET_SUCCESS;
}



M2M_APN_Connection::M2M_APN_Connection()
{
	m_connectionMode = NET_M2M_APN;
	m_dialState = NOT_DIALED;

	m_pConnect = new connect_M2M_APN(this);
	m_pDisConnect = new disconnect_M2M_APN(this);
	m_pCheck = new CommonCheck(this);
	m_pGetIPinfo = new CommonGetIPinfo(this);
}

M2M_APN_Connection::~M2M_APN_Connection()
{
}



int M2M_APN_Connection::initNetDevice(string &strErr)
{
	int rc=0;
	INT32 actState=0;
	DBG_PRINT(("enter APN initNetDevice"));

	//判断M2M激活选项
	if (CNetDBWork::load_M2M_State(&actState, strErr) != SQLITE_OK)
	{
		actState = 0;
	}
	DBG_PRINT(("actState = %d", actState));

	if (actState == M2M_ACT_ON)
	{
		modem_init();
		at_disableModuleApp();
	}
	DBG_PRINT(("APN init successed"));
	
	return NET_SUCCESS;		
}


int M2M_APN_Connection::setApn_Acc_Num(string &Acc, string &Num, string &strErr)
{
	int ret = 0;
	UINT8 m_apnNameFlag=0;

	//修改脚本文件
//	if(Num.length() > 0)	//禁止APN号码的修改 zl 20140616
//	{
//		m_apnNameFlag = 1;
//	}
	UpdateMobileChat(Acc.c_str(), Num.c_str(), m_apnNameFlag);

	//存数据库
	if (CNetDBWork::save_APN_Acc_Num(Acc, Num, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}

	return NET_SUCCESS;
}


void M2M_APN_Connection::UpdateMobileChat(const INT8 *apnValue, const INT8 *apnNumber, UINT8 nameFlag)
{
	int ret = 0;
	INT32 res=0;
	INT8 tmpBuf1[16];
	INT8 lineBuf[128];
	FILE *fps, *fpd;
	
	DBG_PRINT(("apnValue = %s", apnValue));
	DBG_PRINT(("apnNumber = %s", apnNumber));
	DBG_PRINT(("nameFlag = %u", nameFlag));
	
	system("/rw");
	
	//AT+CGDCONT=1,"IP","CMNET",,0,0
	system("rm /etc/ppp/chat/mobile_chat_new");
	fps = fopen("/etc/ppp/chat/mobile_chat", "r");
	fpd = fopen("/etc/ppp/chat/mobile_chat_new", "w");
	
	while (!feof(fps))
	{
		memset(lineBuf, 0, sizeof(lineBuf));
		fgets(lineBuf, sizeof(lineBuf), fps);
		
		if( nameFlag == 1 )
		{
			//修改关键行
			res = sscanf(lineBuf, " SAY             \"Dialing up...%s\\n\"", tmpBuf1);
			if (res == 1)
			{
				DBG_PRINT(("APN Name 1 SUCCESS."));
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "SAY             \"Dialing up...%s\\n\"\n", apnNumber);
			}
		}
		
		//修改关键行
		res = sscanf(lineBuf, " OK              AT+CGDCONT=1,\"IP\",\"%s\",,0,0", tmpBuf1);
		if (res == 1)
		{
			DBG_PRINT(("APN value SUCCESS."));
			memset(lineBuf, 0, sizeof(lineBuf));
			sprintf(lineBuf, "OK              AT+CGDCONT=1,\"IP\",\"%s\",,0,0\n", apnValue);
		}
		
		if( nameFlag == 1 )
		{
			//修改关键行
			res = sscanf(lineBuf, " OK              ATDT%s", tmpBuf1);
			if (res == 1)
			{
				DBG_PRINT(("APN Name 2 SUCCESS."));
				memset(lineBuf, 0, sizeof(lineBuf));
				sprintf(lineBuf, "OK              ATDT%s\n", apnNumber);
			}
		}
		
		//写入新文件
		fputs(lineBuf, fpd);
	}
	
	fclose(fps);
	fclose(fpd);
	CommonSleep(1000);
	
	//替换原始文件，需掉电保护
	POWOFF_DISABLE();
	system("cp /etc/ppp/chat/mobile_chat_new /etc/ppp/chat/mobile_chat");
	CommonSleep(500);
	system("chmod +x /etc/ppp/chat/mobile_chat");
	POWOFF_ENABLE();
	CommonSleep(500);
	
	return ;
}


