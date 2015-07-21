/*
 * command_at.c
 *
 *  Created on: 2010-3-10
 *      Author: Administrator
 */
//#include <syslib.h>
#include <string.h>

#include "netHeaderDef.h"
#include "command_at.h"
#include "drv_modem.h"
#include "modem_err.h"

//#define AT_DEBUG

#ifdef AT_DEBUG
#define DEBUG_PRINTF	printf
#else
#define DEBUG_PRINTF
#endif


#if (_CONFIG_M2M_MODULE != 0)

//Get manufacturer identification
int at_getCGMI(char *cgmi)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CGMI);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		strncpy(cgmi,tmpbuffer,len);
		cgmi[len] = '\0';
		DEBUG_PRINTF("RESPONSE: %s\n",cgmi);
	}

	return rc;
}
//Get stored IMEI
int at_getCGSN(char *cgsn)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CGSN);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//ccid response format: <IMEI>(15 digit)
		if( sscanf(tmpbuffer,"%15s",cgsn) == 1)
			rc =0;
		else
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
	}

	return rc;
}

//Get identification number for the SIM (20 digit)
int at_getCCID(char *ccid)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CCID);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//ccid response format: +CCID: "<CCID>(20 digit)"
#if (SIMCOM==1)
		if( sscanf(tmpbuffer,"%21s",ccid) == 1)
		{
			rc =0;
		}
#else 
		if( sscanf(tmpbuffer,"+CCID: \"%20s",ccid) == 1)
		{
			rc =0;
		}
#endif	
		else
		{
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
		}
	}

	return rc;
}

//Get IMSI of the SIM card (15 digit)
int at_getIMSI(char *imsi)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CIMI);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//ccid response format: <IMSI>(15 digit)
		if( sscanf(tmpbuffer,"%15s",imsi) == 1)
			rc =0;
		else
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
	}

	return rc;
}


//Send short message
int at_sendMessage(char *phoneNum, char *message)
{
	char tmpbuffer[128];
	char ctrlz[1]={0x1A};
	int len;
	int rc;

	modem_flushInputBuffer();
	sprintf(tmpbuffer,"AT+CMGS=%s",phoneNum);
	modem_sendCmd(tmpbuffer);
	rc = modem_waitFor(">",&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
		goto ExitProcessing;
	}
	//write message
	DEBUG_PRINTF("write message: %s\n",message);
	modem_write( message, strlen(message));
	//terminate
	modem_write( ctrlz, 1 );

	//wait response
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}

ExitProcessing:
	return rc;
}

//Check the registration status of the product
//<mode>: request operation
//		0 disable network registration unsolicited result code (default value)
//		1 enable network registration code result code +CREG: <stat>
//		2 enable network registration and location information unsolicited result code +CREG:
//<stat>: network registration state
//		0 not registered, ME is not currently searching for a new operator
//		1 registered, home network
//		2 not registered, ME currently searching for a new operator
//		3 registration denied
//		4 unknown
//		5 registered, roaming
int at_getCREG(int *mode, int *stat)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CREG);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//creg response format: +CREG: <mode>,<stat>
		if( sscanf(tmpbuffer,"+CREG: %d,%d",mode,stat) == 2)
			rc =0;
		else
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
	}

	return rc;
}

int at_getFirmwareVer(char *ver)
{
	char tmpbuffer[128];
	int len;
	int rc = 0;
	int major,minor;

#if (SIMCOM==1)
	sprintf(ver,"SIMCOM");
#else 
	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_FIRMVER);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//creg response format: +CREG: <mode>,<stat>
		if( sscanf(tmpbuffer,"R%d.%d",&major,&minor) == 2)
		{
			rc =0;
		}
		else
		{
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
		}
	}

	sprintf(ver,"R%d.%02d",major,minor);
#endif
	return rc;
}

int at_checkGsmNetwork(void)
{
	int rc;
	int rssi,ber;
	int mode,stat;
	int retry;

	//Check the registration status of the product
	rc = at_getCREG(&mode, &stat);
	if(rc != 0)
	{
		DEBUG_PRINTF("Get CREG failed\n");
		goto ExitProcessing;
	}
	//DEBUG_PRINTF("mode = %d; stat=%d\n",mode,stat);
	if( (stat != 1) && (stat != 5) )
	{
		DEBUG_PRINTF("GSM CREG failed\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_AT,ERROR_AT_OUTOF_SERVICE);
		goto ExitProcessing;

	}
	//check Signal Quality
	retry=5;
	while(retry--)
	{
		rc = at_getCSQ(&rssi, &ber);
		if(rc != 0)
		{
			DEBUG_PRINTF("Get CSQ failed\n");
			goto ExitProcessing;
		}
		if( (rssi == 99) || (rssi < 5) || (ber == 99) || ( ber > 5) )
		{
			DEBUG_PRINTF("GSM signal too weak rssi:%d; ber:%d\n",rssi,ber);
			continue;
		}
		else
			break;
	}

	if(retry <=0)
		rc = MODEM_ERROR_NUM(MODEM_ERROR_AT,ERROR_AT_SIGNAL_TOOWEAK);


ExitProcessing:
		return rc;
}

//激活模块
int at_enableModuleApp(void)
{
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_ENABLE_APP);
	rc = modem_waitFor(MODEM_OK_STR,20);
	if(rc != 0)
	{
		DEBUG_PRINTF("can not enable module app\n");
	}

	return rc;
}

#endif //_CONFIG_M2M_MODULE != 0



//关闭模块(激活的逆向操作)
int at_disableModuleApp(void)
{
#if (_CONFIG_M2M_MODULE != 0)
	char tmpbuffer[128];
	int len;
	int rc = 0;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_DISABLE_APP);
/*
	rc = modem_waitFor(MODEM_OK_STR,20);
	if(rc != 0)
	{
		DEBUG_PRINTF("can not enable module app\n");
	}
*/

	return rc;

#else
	return 0;
#endif
}

//Get Signal Quality
//<rssi>: received signal strength
//		0-31 signal strength
//		99   not known or not detectable
//<ber>: channel bit error rate
//		0…7 as RXQUAL values in the table GSM 05.08 [10]
//		99 not known or not detectable
int at_getCSQ(int *rssi, int *ber)
{
#if (_CONFIG_M2M_MODULE != 0)
	
	char tmpbuffer[128];
	int len;
	int rc;

	modem_flushInputBuffer();
	modem_sendCmd(CMD_GET_CSQ);
	rc = modem_getResponse(tmpbuffer,&len);
	if(rc != 0)
	{
		DEBUG_PRINTF("error: %x\n",rc);
	}
	else
	{
		DEBUG_PRINTF("RESPONSE: %s\n",tmpbuffer);
		//ccid response format: +CSQ: <rssi>,<ber>
		if( sscanf(tmpbuffer,"+CSQ: %d,%d",rssi,ber) == 2)
			rc =0;
		else
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
	}

	return rc;

#else
	return 0;
#endif
}

int getM2MInfo(M2Minfo *info)
{
#if (_CONFIG_M2M_MODULE != 0)
	
	int rc=0;
	int flag=0;

	//读取IMEI编号
	memset(info->IMEI, 0, sizeof(info->IMEI));
	rc = at_getCGSN(info->IMEI);
	if (rc != 0)
	{
		flag=-1;
	}

	//读取CCID编号
	memset(info->CCID, 0, sizeof(info->CCID));
	rc = at_getCCID(info->CCID);
	if (rc != 0)
	{
		flag=-1;
	}

	//读取IMSI编号
	memset(info->IMSI, 0, sizeof(info->IMSI));
	rc = at_getIMSI(info->IMSI);
	if (rc != 0)
	{
		flag=-1;
	}

	//读取模块固件版本
	memset(info->FirmVer, 0, sizeof(info->FirmVer));
	rc = at_getFirmwareVer(info->FirmVer);
	if (rc != 0)
	{
		flag=-1;
	}

	//读取模块软件版本
	memset(info->SoftVer, 0, sizeof(info->SoftVer));
//	rc = wmmp_readAppVer(info->SoftVer);
//	if (rc != 0)
//	{
//		flag=-1;
//	}
	
	return flag;	


#else
	return 0;
#endif
}





