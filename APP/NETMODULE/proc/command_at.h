/*
 * command_at.h
 *
 *  Created on: 2010-3-10
 *      Author: Administrator
 */

#ifndef COMMAND_AT_H_
#define COMMAND_AT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define RESPONSE_START_TAG	"\r\n"
#define RESPONSE_END_TAG	"\r\n"
#define RESPONSE_CME_ERR	"+CME ERROR:"
#define RESPONSE_CMS_ERR	"+CMS ERROR:"

//Model Identifications Commands
#define CMD_GET_CGMI		"AT+CGMI"		//Get manufacturer identification
#define CMD_GET_WHWV		"AT+WHWV"		//Requests hardware version
#define CMD_GET_CGMR		"AT+CGMR"		//Get the revised software version
#define CMD_GET_CGSN		"AT+CGSN"		//Get stored IMEI

//SIM Identification Commands
#define CMD_GET_CCID		"AT+CCID"		//Get identification number for the SIM (20 digit)
#define CMD_GET_CIMI		"AT+CIMI"		//Get IMSI of the SIM card (15 digit)


//GSM Network Commands
#define CMD_GET_CSQ			"AT+CSQ"		//Get Signal Quality
#define CMD_GET_CREG		"AT+CREG?"		//Get the registration status of the product

#define CMD_GET_FIRMVER		"ATI3"			//Get revision identification

#define CMD_ENABLE_APP		"AT+WOPEN=1"	//Enable application of modem
#define CMD_DISABLE_APP		"AT+WOPEN=0"	//Enable application of modem


typedef struct{
    char IMEI[32];
    char CCID[32];
    char IMSI[32];
    char FirmVer[32];
    char SoftVer[32];
}M2Minfo;


int at_getCGMI(char *cgmi);
int at_getCGSN(char *cgsn);
int at_getCCID(char *ccid);
int at_getIMSI(char *imsi);
int at_getCSQ(int *rssi, int *ber);
int at_getCREG(int *mode, int *stat);
int at_getFirmwareVer(char *ver);
int at_sendMessage(char *phoneNum, char *message);
int at_checkGsmNetwork(void);
int at_enableModuleApp(void);
int at_disableModuleApp(void);

int getM2MInfo(M2Minfo *info);	//add by zl

//error code define
#define ERROR_AT_BAD_RESPONSE	1
#define ERROR_AT_OUTOF_SERVICE	2
#define ERROR_AT_SIGNAL_TOOWEAK		3

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_AT_H_ */
