/**
 @file  3GConnection.cpp
 @brief 本文件实现了3G网络接入的几个派生类。
 @author   zl
*/
#include <fcntl.h>   /* File control definitions */ 
#include <errno.h>   /* Error number definitions */ 
#include <termios.h> /* POSIX terminal control definitions */ 

#include "commonFunc.h"
#include "setToFileProc.h"
#include "CommonCheck.h"
#include "CommonGetIPinfo.h"
#include "netManager.h"
#include "3GConnection.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"


char *s_usbDevName_ex[]={
	"/dev/ttyUSB3",
	"/dev/ttyUSB2",
	"/dev/ttyUSB1",
};

int connect3G::connect(string &strErr)
{
	int i=0;
	const INT32 MAX_RETRY_TIMES=10;	
	INT32 carrierType = 0;
	INT32 sysRet = 0; 
	string userName(""), userPwd("");
	CNetConnection *p = (CNetConnection *)m_pConnection;

	DBG_PRINT(("Connect3G..."));

	if (p->performCheck() == CHECK_OK)
	{
		DBG_PRINT(("3g已连接"))
		return NET_SUCCESS;
	}
	
	if (CNetDBWork::load3gCarrier(&carrierType, strErr) != SQLITE_OK)
	{
		strErr = "未设置3G运营商,无法拨号";
		return NET_CONNECT_FAIL;
	}
	if (CNetDBWork::load3gUserNamePwd(userName, userPwd, strErr) != SQLITE_OK)
	{
		strErr = "未设置3G帐号与密码,无法拨号";
		return NET_CONNECT_FAIL;
	}
	else if (userName=="" && userPwd=="")
	{
		strErr = "3G帐号与密码为空,无法拨号";
		return NET_CONNECT_FAIL;
	}

	system("ifconfig eth0 down"); 

	if(carrierType == CARRIER_CT)		
	{
		system("/bin/pppd_modem call evdo &");	//evdo是电信脚本？
		DBG_PRINT(("/bin/pppd_modem call evdo &"));
	}
	else if (carrierType == CARRIER_CU)
	{
		system("/bin/pppd_modem call unicom &");
		DBG_PRINT(("/bin/pppd_modem call unicom &"));
	}
	else	
	{
		strErr = "未设置3G运营商,无法拨号";
		return NET_CONNECT_FAIL;
	}
	CommonSleep(1000);

	for(i=0; i<MAX_RETRY_TIMES; i++)
	{			
		if (p->performCheck() == CHECK_OK)
		{
			DBG_PRINT(("Connect3G..CHECK_OK."));
			CommonSleep(1000);
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
		strErr = "3G连接失败";
		return NET_CONNECT_FAIL;
	}

	p->m_dialState = HAVE_DIALED;
	return NET_SUCCESS;
}


int disconnect3G::disConnect()
{
	const INT32 MAX_RETRY_TIMES=10;	
	int status=0;
	int i=0;
	CNetConnection *p = m_pConnection;

	status = g_netManager->checkConnect();
	switch (status)
	{
	case CONNECT_ON_L2:
		{
			CNetConnection *pL2 = g_netManager->GetConnectionHandleL2();
			pL2->performDisConnect();
			CommonSleep(1000);
		}
		
	case CONNECT_ON_L1:
		system("killall pppd_modem");
		for(i=0; i<MAX_RETRY_TIMES; i++)
		{			
			if (p->performCheck() != CHECK_OK)
			{
				CommonSleep(1000);
				break;
			}
			CommonSleep(2000);
		}
		break;

	default:
		DBG_PRINT(("3G不在连接状态: %d", p->m_dialState));
		break;
	}
	
	p->m_dialState = NOT_DIALED;
	return NET_SUCCESS;
}




_3GConnection::_3GConnection()
{
	m_connectionMode = NET_3G;
	m_dialState = NOT_DIALED;

	m_pConnect = new connect3G(this);
	m_pDisConnect = new disconnect3G(this);
	m_pCheck = new CommonCheck(this);
	m_pGetIPinfo = new CommonGetIPinfo(this);

	m_pATBuff = new char[_3G_AT_BUFF_LEN];
}

_3GConnection::~_3GConnection()
{
	if (m_pATBuff != NULL)
	{
		delete m_pATBuff;
		m_pATBuff = NULL;
	}
}

int _3GConnection::setUserNamePwd(string name, string pwd, string &strErr)
{
	int ret=0;

	if (performCheck() != CHECK_NOT)
	{
		strErr = "请先断开3G连接";
		return NET_CONDITION_ER;
	}

	if ((ret=set3GScirptFunc(name, pwd, strErr)) != NET_SUCCESS)
	{
		return ret;
	}

	if (CNetDBWork::save3gUserNamePwd(name, pwd, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}
	
	return NET_SUCCESS;
}

int _3GConnection::set3Gcarrier(int type, string &strErr)
{
	int ret=0;

	if (performCheck() != CHECK_NOT)
	{
		strErr = "请先断开3G连接";
		return NET_CONDITION_ER;
	}

	//数据库操作
	if (CNetDBWork::save3gCarrier(type, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}

	//将用户名与密码设为该运营商的默认值
	string name(""), pwd("");
	getDefUserNamePwd(name, pwd);
	if (CNetDBWork::save3gUserNamePwd(name, pwd, strErr) != SQLITE_OK)
	{
		return NET_OPERATOR_DB_ER;
	}
	if ((ret=set3GScirptFunc(name, pwd, strErr)) != NET_SUCCESS)
	{
		return ret;
	}
	
	return NET_SUCCESS;
}


int _3GConnection::set3GScirptFunc(string &name, string &pwd, string &strErr)
{
	INT32 carrierType=0;
	string scriptFile("");
	CNetDBWork::load3gCarrier(&carrierType, strErr);
	
	if (carrierType == CARRIER_CT)
		scriptFile.assign(SCRIPT_FILE_CT);
	else if (carrierType == CARRIER_CU)
		scriptFile.assign(SCRIPT_FILE_CU);
		
	if ( updateScriptFileFor3G(scriptFile.c_str(), name.c_str())!=NET_SUCCESS )
	{
		strErr = "修改3G拨号脚本错";
		return NET_OPERATOR_FILE_ER;
	}
	if ( change_secret_file(SECRETS_FILE, name.c_str(), pwd.c_str())!=NET_SUCCESS )
	{
		strErr = "修改帐号文件错";
		return NET_OPERATOR_FILE_ER;
	}
	
	return NET_SUCCESS;
}

void _3GConnection::getDefUserNamePwd(string &name, string &pwd)
{
	INT32 carrierType=0;
	string strErr("");
	CNetDBWork::load3gCarrier(&carrierType, strErr);

	if (carrierType == CARRIER_CT)
	{
		name.assign(DEF_USER_NAME_CT);
		pwd.assign(DEF_USER_PWD_CT);
	}
	else if (carrierType == CARRIER_CU)
	{
		name.assign(DEF_USER_NAME_CU);
		pwd.assign(DEF_USER_PWD_CU);
	}
}

int _3GConnection::getCSQ(int *rssi, int *ber)
{
	int ret=0;
	char *pStr=NULL;

	int serialFD = open_at_port(); 
	if (serialFD < 0)
	{
		return serialFD;
	}
	DBG_PRINT(("serialFD = %d", serialFD));

    memset(m_pATBuff, 0, _3G_AT_BUFF_LEN);
	write(serialFD, "AT+CSQ\r", strlen("AT+CSQ\r"));
	CommonSleep(500);
	ret = read(serialFD, m_pATBuff, _3G_AT_BUFF_LEN);
	DBG_PRINT(("ret = %d", ret));
	close(serialFD); 
	
	//DBG_PRINT(("%s", m_pATBuff));
	if( (pStr=strstr(m_pATBuff, "+CSQ:")) == NULL)
	{
		return NET_AT_CMD_FAIL; 	
	}
	if( sscanf(pStr, "+CSQ: %d, %d", rssi, ber) != 2)
	{
		return NET_AT_CMD_FAIL; 	
	}

	return NET_SUCCESS; 					
}




int _3GConnection::open_at_port() 
{ 
	struct termios options; 
	int fd=-1; 
	int ret=0;
	int portNum = sizeof(s_usbDevName_ex) / sizeof(char *);

	for (int i=0; i<portNum; i++)
	{
		fd = -1;
		//尝试打开端口
		fd = open(s_usbDevName_ex[i], O_RDWR | O_NOCTTY | O_NDELAY); 
		if (fd == -1) 
		{ 
			DBG_PRINT(("open_port: Unable to open the port - %s", s_usbDevName_ex[i])); 
			continue;
		} 
		else 
		{ 
			DBG_PRINT(("Port %s with file descriptor=%i", s_usbDevName_ex[i], fd)); 
			fcntl(fd, F_SETFL, FNDELAY); 
			tcgetattr( fd, &options ); 
			cfsetispeed( &options, _AT_BAUDRATE_3G ); 
			cfsetospeed( &options, _AT_BAUDRATE_3G ); 
			options.c_cflag |= ( CLOCAL | CREAD); 
			options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CSIZE);  
			options.c_cflag |= CS8; 
			options.c_cflag &= ~CRTSCTS;  
			options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
			options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR); 
			options.c_oflag &= ~OPOST; 
			if ( tcsetattr( fd, TCSANOW, &options ) == -1 ) 
			{
				DBG_PRINT(("Error with tcsetattr = %s", strerror(errno) )); 
			}
			else 
			{
				DBG_PRINT(("%s", "succeed")); 
			}
		} 

		//尝试发送AT指令
		memset(m_pATBuff, 0, _3G_AT_BUFF_LEN); 
		write(fd, "AT\r", strlen("AT\r")); 
		CommonSleep(200); 
		ret = read(fd, m_pATBuff, _3G_AT_BUFF_LEN); 
		DBG_PRINT(("ret = %d", ret));
		if( strstr(m_pATBuff, "OK") == NULL)
		{
			close(fd); 
			fd = -1;
			continue; 	
		}
	
		break;
	}
	
	return (fd); 
} 

