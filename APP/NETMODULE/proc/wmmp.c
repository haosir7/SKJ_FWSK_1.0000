/*
 * wmmp.c
 *
 *  Created on: 2010-3-10
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>

#include "netHeaderDef.h"
#include "drv_modem.h"
#include "command_at.h"
#include "modem_err.h"
#include "wmmp.h"
#include "decode.h"

//#define WMMP_READ_DEBUG
//#define WMMP_DEBUG

#ifdef WMMP_DEBUG
#define DEBUG_PRINTF	printf
#else
#define DEBUG_PRINTF
#endif

//set timeout value as 10s
#define WMMP_TIMEOUT	20


#if (_CONFIG_M2M_MODULE != 0)


//Global variable
t_wmmp_rx_buf wmmp_rx_buf;

static int wmmp_initRxBuffer(void)
{
	wmmp_rx_buf.buf_head = wmmp_rx_buf.buf_tail = wmmp_rx_buf.buf;
	wmmp_rx_buf.rxlen = 0;
	wmmp_rx_buf.lines = 0;
	wmmp_rx_buf.enumReceiveState = RECV_STATE_IDLE;
	wmmp_rx_buf.enumResponseType = RESPONSE_UNKOWN;
}


static int strDoesContain(char *str,char *lookfor)
{
    int rc=0;
    if (strstr(str,lookfor))
        rc=1;
    else
        rc=0;

    return(rc);
}

static int wmmp_responsePostProcess(void)
{
	char *p;
	wmmp_rx_buf.lines = 0;

	DEBUG_PRINTF("response post process\n");

	//decode the response with <cr><lf> divided format;
	for(p=wmmp_rx_buf.buf_head;p<wmmp_rx_buf.buf_tail-2;p++)
	{
		if( (*p == '\r') && (*(p+1) == '\n') && (*(p+2)!= '\r'))
		{
			wmmp_rx_buf.line[wmmp_rx_buf.lines] = p+2;
			wmmp_rx_buf.lines++;
		}

	}
	return 0;
}

static void wmmp_sendCmd(char *cmdstr)
{
	char prefix_AT[] = "AT";
	char enter[]="\r";
	modem_write(prefix_AT,2);
	modem_write(cmdstr,strlen(cmdstr));
	modem_write(enter,1);
}

static int wmmp_waitResponse(char *cmdstr,int timeout)
{
//	char cmd_response[256];
//	int finished = 0;
	char *result_start;
	int error;

	char *seek_start;
	char *seek_end;
	char seek_tag1[64];

	time_t now,last;

	int rc=0;

	//resopnse format: $M2M<CMD>:
	if (cmdstr != NULL )
	{
		//sprintf(cmd_response,"%s",cmdstr);
		DEBUG_PRINTF("keyword:%s\n",cmdstr);
	}

	DEBUG_PRINTF("init response rx buffer\n");
	wmmp_initRxBuffer();

	//init timeout value
	time(&now);
	last = now;

	do
	{
		//DEBUG_PRINTF("state: %d\n",wmmp_rx_buf.enumReceiveState);
		switch(wmmp_rx_buf.enumReceiveState)
		{
		case RECV_STATE_IDLE:
			//read from modem
			//if(modem_charsInBuffer())
			if(modem_read( wmmp_rx_buf.buf_tail, 1) == 1)
			{

				//modem_read( wmmp_rx_buf.buf_tail, 1);
				wmmp_rx_buf.buf_tail++;
				*(wmmp_rx_buf.buf_tail) = '\0';
				wmmp_rx_buf.rxlen++;
				//show rx buff info
				//DEBUG_PRINTF("rx:%s\n",wmmp_rx_buf.buf_head);
			}
			else
			{
				//break switch case
				break;
			}

			/* first check if <cr><lf> is found at end of reply_buf.
			 * the needed length is greater 4 because we don't
			 * need to enter if no result/error will be found. */
			if(wmmp_rx_buf.rxlen >= 6 && !strncmp(wmmp_rx_buf.buf_tail - 2, "\r\n", 2))
			{
				//DEBUG_PRINTF("%s",wmmp_rx_buf.buf);
				DEBUG_PRINTF("find <cr><lf> \n");
				/* try to find previous <cr><lf> */
					result_start = findcrlfbw(wmmp_rx_buf.buf_tail - 2, wmmp_rx_buf.rxlen);
				/* if not found, start at buffer beginning */
				if (!result_start)
				{
					DEBUG_PRINTF("first <cr><lf> missed!!!\n");
					//ignore the words before <cr><lf>
					//update buf_head
					wmmp_rx_buf.buf_head=wmmp_rx_buf.buf_tail -2;
					wmmp_rx_buf.rxlen = 2;
					//to do ...
					//continue the loop;
					break;

				}

				//update buf_head
					wmmp_rx_buf.buf_head = result_start -2;				
				/* there are certainly more that 2 chars in buffer */
				// <cr><lf>OK<cr><lf>
				if (!strncmp(result_start, "OK", 2))
				{
					//OK response
					DEBUG_PRINTF("OK found\n");
					wmmp_rx_buf.enumResponseType = RESPONSE_OK;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;

				}
				//cmd response with output
				else if( (cmdstr != NULL ) && (!strncmp(result_start, cmdstr,strlen(cmdstr)) ) )
				{
					//
					DEBUG_PRINTF("%s found\n",cmdstr);
					wmmp_rx_buf.enumResponseType = RESPONSE_WMMP_CMD_OK;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_WAIT_END_TAG;
					seek_start = wmmp_rx_buf.buf_tail + 1 ;
				}

				//async ack
				else if( !strncmp(result_start, "$M2MACK",7 ) )
				{
					//
					DEBUG_PRINTF("$M2MACK found\n");
					wmmp_rx_buf.enumResponseType = RESPONSE_WMMP_ACK;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_WAIT_END_TAG;
					seek_start = wmmp_rx_buf.buf_tail + 1 ;
				}
				//message
				else if( !strncmp(result_start, "$M2MMSG",7 ))
				{
					//
					DEBUG_PRINTF("$M2MMSG found\n");
					wmmp_rx_buf.enumResponseType = RESPONSE_WMMP_MSG;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_POST_PROCESS;
					//seek_start = wmmp_rx_buf.buf_tail + 1 ;
					//sprintf(seek_tag,"\r\n");
				}
#if (SIMCOM==1)
				else if( !strncmp(result_start, "$M2M$MSG",8 )) //yy 20110818
				{					
					DEBUG_PRINTF("$M2M$MSG found\n");
					wmmp_rx_buf.enumResponseType = RESPONSE_WMMP_SEND_MSG;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_POST_PROCESS;
					//seek_start = wmmp_rx_buf.buf_tail + 1 ;
					//sprintf(seek_tag,"\r\n");
				}
#endif
				else if (sscanf(result_start, "+CME ERROR: %d", &error) == 1)
				{
					DEBUG_PRINTF("+CME ERROR found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_CME,error);
					wmmp_rx_buf.enumResponseType = RESPONSE_ERROR_CME;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (sscanf(result_start, "+CMS ERROR: %d", &error) == 1)
				{
					DEBUG_PRINTF("+CMS ERROR found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_CMS,error);
					wmmp_rx_buf.enumResponseType = RESPONSE_ERROR_CMS;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "RING", 4))
				{
					DEBUG_PRINTF("RING found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RING);
					wmmp_rx_buf.enumResponseType = RESPONSE_DEVICE_RING;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "BUSY", 4))
				{
					DEBUG_PRINTF("BUSY found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_BUSY);
					wmmp_rx_buf.enumResponseType = RESPONSE_DEVICE_BUSY;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "NO ANSWER", 9))
				{
					DEBUG_PRINTF("NO ANSWER found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_ANSWER);
					wmmp_rx_buf.enumResponseType = RESPONSE_DEVICE_NO_ANSWER;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "NO CARRIER", 10))
				{
					DEBUG_PRINTF("NO CARRIER found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_CARRIER);
					wmmp_rx_buf.enumResponseType = RESPONSE_DEVICE_NO_CARRIER;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "NO DIALTONE", 11))
				{
					DEBUG_PRINTF("NO DIALTONE found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_DIALTONE);
					wmmp_rx_buf.enumResponseType = RESPONSE_DEVICE_NO_DIALTONE;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (sscanf(result_start, "ERROR: %d", &error) == 1)
				{
					DEBUG_PRINTF("ERROR: found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,error);
					wmmp_rx_buf.enumResponseType = RESPONSE_ERROR_NUM;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else if (!strncmp(result_start, "ERROR", 5))
				{
					DEBUG_PRINTF("ERROR found\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_ERROR_SYNTAX);
					wmmp_rx_buf.enumResponseType = RESPONSE_ERROR;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
				else
				{
					DEBUG_PRINTF("no keyword found!!!\n");
					rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_UNKOWN);
					wmmp_rx_buf.enumResponseType = RESPONSE_UNKOWN;
					wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
				}
			}
			else if( !strncmp(wmmp_rx_buf.buf_head, "\r\n>", 3))
			{
				DEBUG_PRINTF("prompt: > found \n");
				wmmp_rx_buf.enumResponseType = RESPONSE_PROMPT;
				wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;

			}
			else
			{

			}
			break;
		case RECV_STATE_WAIT_END_TAG:
			//read from modem
			//if(modem_charsInBuffer())
			if(modem_read( wmmp_rx_buf.buf_tail, 1) == 1)
			{
				//modem_read( wmmp_rx_buf.buf_tail, 1);
				wmmp_rx_buf.buf_tail++;
				*(wmmp_rx_buf.buf_tail) = '\0';
				wmmp_rx_buf.rxlen++;

			}
			else
			{
				//break switch case
				break;
			}

			//parse response
			seek_end = wmmp_rx_buf.buf_tail;

			//$M2MMSG...<cr><lf>
			if(wmmp_rx_buf.enumResponseType == RESPONSE_WMMP_MSG)
			{
				if( !strncmp(wmmp_rx_buf.buf_tail - 2, "\r\n", 2) )
				{
					DEBUG_PRINTF("$M2MMSG received\n");
					wmmp_rx_buf.enumReceiveState = RECV_STATE_POST_PROCESS;
				}

			}
			else
			{
				if(!strncmp(wmmp_rx_buf.buf_tail - 2, "\r\n", 2))
				{
					/* try to find previous <cr><lf> */
					result_start = findcrlfbw(wmmp_rx_buf.buf_tail - 2, wmmp_rx_buf.rxlen);
					/* if not found, start at buffer beginning */
					if (!result_start)
					{
						//result_start = rbuf;
						//ignore the words before <cr><lf>
						//to do ...
						//continue the loop;
						DEBUG_PRINTF("Where am I?!!!\n");

					}

					/* there are certainly more that 2 chars in buffer */

					// <cr><lf>OK<cr><lf>
					if (!strncmp(result_start, "OK", 2))
					{
						DEBUG_PRINTF("OK received\n");
						wmmp_rx_buf.enumReceiveState = RECV_STATE_POST_PROCESS;

					}
					else if (!strncmp(result_start, "ERROR", 5))
					{
						DEBUG_PRINTF("ERROR received\n");
						wmmp_rx_buf.enumResponseType = RESPONSE_WMMP_ERROR;
						wmmp_rx_buf.enumReceiveState = RECV_STATE_POST_PROCESS;
					}

				}
			}

			break;

		case RECV_STATE_POST_PROCESS:
			wmmp_responsePostProcess();
			wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
			break;

		}

		//time out process.
		time(&now);
		if ( now > last + timeout)
		{
			DEBUG_PRINTF("time out!\n");
			rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_TIMEOUT);
			wmmp_rx_buf.enumReceiveState = RECV_STATE_END_TAG_DETECTED;
		}

	}while(wmmp_rx_buf.enumReceiveState != RECV_STATE_END_TAG_DETECTED);

	return rc;
}

static int wmmp_checkWmmpStatus(int *status)
{
	int rc;
	char cmdstr[] = "$M2MSTATUS";

	DEBUG_PRINTF("wmmp check status\n");
	modem_flushInputBuffer();
	wmmp_sendCmd(cmdstr);
	rc = wmmp_waitResponse(cmdstr,20);

	if(rc != 0)
	{
		DEBUG_PRINTF("WMMP get response error: %x\n",rc);
		goto ExitProcessing;
	}
	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	if(sscanf(wmmp_rx_buf.line[0],"$M2MSTATUS: %d",status)!=1)
	{
		DEBUG_PRINTF("status not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;
}


void wmmp_apiTest(void)
{
	int rc;
	char cmdstr[] = "$M2MSTATUS";

//	printf("wmmp api test program\n");
	modem_flushInputBuffer();
	wmmp_sendCmd(cmdstr);
	rc = wmmp_waitResponse(cmdstr,20);

	if(rc == 0)
	{
		DEBUG_PRINTF("RESPONSE type	: %d\n",wmmp_rx_buf.enumResponseType);
		DEBUG_PRINTF("RESPONSE lines	: %d\n",wmmp_rx_buf.lines);
		DEBUG_PRINTF("RESPONSE content: \n%s\n",wmmp_rx_buf.buf_head);
		switch(wmmp_rx_buf.enumResponseType)
		{
		case RESPONSE_WMMP_MSG:
			break;
		case RESPONSE_WMMP_ACK:
			break;
		case RESPONSE_WMMP_CMD_OK:
			break;
		case RESPONSE_PROMPT:
			break;

		}

	}
	else
	{
		DEBUG_PRINTF("error num = %x\n",rc);

	}
}



static int wmmp_checkAppBearerStatus(int flowID,int *open_flag)
{
	int rc;
	int app_id,bearer_type,bearer_status;
	int transID2;
	char cmdbuf[256];
	int i;

	modem_flushInputBuffer();
	//open app flow
	DEBUG_PRINTF("Open app flow %d\n",flowID);
	sprintf( cmdbuf, "AT$M2MAPPBRSTATUS=%d", flowID);
	modem_sendCmd( cmdbuf );

	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse("$M2MAPPBRSTATUS",20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}
	if(sscanf(wmmp_rx_buf.line[0],"$M2MAPPBRSTATUS: %d,%d,%d",&app_id,&bearer_type,&bearer_status)!=3)
	{
		DEBUG_PRINTF("app flow bearer status not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	if( (bearer_status == WMMP_BEARER_ST_OPENING) || (bearer_status == WMMP_BEARER_ST_REOPENING))
	{
		*open_flag = 1;
	}
	else
	{
		*open_flag = 0;
	}

ExitProcessing:
			return rc;
}

static int wmmp_openAppBearer(int flowID)
{
	int rc;
	char cmdbuf[256];

	//flush input buffer
	modem_flushInputBuffer();
	//open app flow bearer
	DEBUG_PRINTF("Open app flow bearer %d\n",flowID);
	sprintf( cmdbuf, "AT$M2MAPPBROPEN=%d", flowID);
	modem_sendCmd( cmdbuf );

	//wait response
	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse(NULL,20);

	if( rc != 0 )
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;
}

static int wmmp_checkMonitorPlatform(void)
{
	int rc;
	int status;
	int retries;
	int maxretries=5;

	//check login status
	for(retries=0;retries<maxretries;retries++)
	{
		DEBUG_PRINTF("retries: %d\n",retries);
		rc = wmmp_checkWmmpStatus(&status);
		if(rc != 0)
		{
			DEBUG_PRINTF("WMMP get response error: %x\n",rc);
			goto ExitProcessing;
		}
		if(status == 500)
		{
			DEBUG_PRINTF("WMMP ready for operation \n");
			break;
		}
		else if(status == 732)
		{
			DEBUG_PRINTF("Alarmed!!!,reset the alarm status\n");
			modem_sendCmd("AT$M2MALMRST=1234");
			rc = modem_waitFor(MODEM_OK_STR,20);
			if( rc != 0 )
			{
				DEBUG_PRINTF("Error: modem init\n");
				goto ExitProcessing;
			}
			CommonSleep(10000);
		}
		else
		{
			DEBUG_PRINTF("Status = %d\n,sleep and retries",status);
		//	CommonSleep (5000);
			CommonSleep (15000);
		}
	}
	//exceed max retries ,reset the modem application
	if (retries == maxretries)
	{
		DEBUG_PRINTF(("Failed,reset the module\n"));
		modem_sendCmd("AT+CFUN=1");
		rc = modem_waitFor("OK",10);
		if(rc != 0)
		{
			DEBUG_PRINTF("WMMP get response error: %x\n",rc);
			goto ExitProcessing;
		}
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_NOT_READY);
		goto ExitProcessing;
	}
ExitProcessing:
		return rc;
}

static int wmmp_trap(unsigned short tag,char *value)
{
	char buf[256];
	char ctrlz[1]={0x1A};
	int transID1,transID2;
	int rc;

	//flush input buffer
	modem_flushInputBuffer();
	//Enable parsing of TLV data.
	DEBUG_PRINTF("Send cmd...\n");
	modem_sendCmd("AT$M2MTRAP=1");

	DEBUG_PRINTF(("WMMP wait for prompt \n"));
	rc = wmmp_waitResponse(NULL,20);
	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_PROMPT)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	//write tag value
	sprintf(buf,"%X,%s",(tag&0xffff),value);
	DEBUG_PRINTF("trap TLV : %s\n",buf);
	modem_write( buf, strlen(buf) );
	modem_write( ctrlz, 1 );

	//wait response
	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse("$M2MTRAP",20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	if(sscanf(wmmp_rx_buf.line[0],"$M2MTRAP: %d",&transID1) != 1)
	{
		DEBUG_PRINTF("transID not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	DEBUG_PRINTF("transID1 = %d\n",transID1);

	//wait async acks
	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse(NULL,20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_ACK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	if(sscanf(wmmp_rx_buf.line[0],"$M2MACK: %d",&transID2) != 1)
	{
		DEBUG_PRINTF("transID not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	DEBUG_PRINTF("transID2 = %d\n",transID2);
	if(transID1 != transID2)
	{
		DEBUG_PRINTF("transID1 != transID2. transID1=%d, transID2=%d\n",transID1,transID2);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;

}

int wmmp_setAppServer(int flowID, const char *server_ip, const char *port)
{
	char buf[256];
	char ctrlz[1]={0x1A};
	int rc;

	// 锁定中国移动网络，防止信号不稳定或与其他运营商有干扰时，
	//中国移动网络的ID号被添加在SIM卡的FPLMN（禁止访问的网络）文件中,造成自动选择网络时被屏蔽。
	//该命令与设置M2M模块IP无关，放这只是想省事，款机使用过程中只执行一次。
	DEBUG_PRINTF("Send cmd...\n");
#if (SIMCOM == 1)
	modem_sendCmd("at+cops=1,2,\"46000\""); //by yy 20110819
#else
	modem_sendCmd("at+cops=1,2,46000;&w");
#endif
	DEBUG_PRINTF("Wait response...\n");
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("锁定中国移动网络 WMMP get response error: %x\n",rc);
		//printf("wmmp_setAppServer, rc = %d\n", rc);
		goto ExitProcessing;
	}
#if (SIMCOM == 1)
	//add by yy 20110819
	DEBUG_PRINTF("Send cmd at&w\n");
//	printf("Send cmd at&w\n");
	modem_sendCmd("at&w");
	DEBUG_PRINTF("Wait response...\n");
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("锁定中国移动网络 WMMP get response error: %x\n",rc);
		//printf("wmmp_setAppServer 1, rc = %d\n", rc);
		goto ExitProcessing;
	}
#endif

	//Enable parsing of TLV data.
	DEBUG_PRINTF("Send cmd...\n");
	modem_sendCmd("AT$M2MBHV=2,1");
	DEBUG_PRINTF("Wait response...\n");
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_setAppServer-1 WMMP get response error: %x\n",rc);
//		printf("wmmp_setAppServer 1, rc = %d\n", rc);
		goto ExitProcessing;
	}

	//确认已登录M2M平台
	DEBUG_PRINTF("Send cmd...\n");
	modem_sendCmd("AT$M2MALMRST=1234");	
	CommonSleep(20);//20ms
//	printf("wmmp_setAppServer 1.1\n");

	//write TLV
	DEBUG_PRINTF("Send cmd...\n");
	modem_sendCmd("AT$M2MCFG=1");
	DEBUG_PRINTF("Wait response...\n");
	rc = modem_waitFor(">",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_setAppServer-2 WMMP get response error: %x\n",rc);
//		printf("wmmp_setAppServer 2, rc = %d\n", rc);
		goto ExitProcessing;
	}
	//write tag value
	sprintf(buf,"D00%X,4,\"tcp://%s:%s\"",(flowID+1),server_ip,port);
	DEBUG_PRINTF("Set Value: %s\n",buf);
	modem_write( buf, strlen(buf) );
	modem_write( ctrlz, 1 );
	
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_setAppServer-3 WMMP get response error: %x\n",rc);
//		printf("wmmp_setAppServer 3, rc = %d\n", rc);
		goto ExitProcessing;
	}
//	printf("wmmp_setAppServer SUCCESS\n", rc);
ExitProcessing:
	return rc;
}

int wmmp_openAppFlow(int flowID)
{
	int rc, i;
	int transID1;
	int transID2;
	char cmdbuf[256];
	int flag;

	//check gsm network status
	rc = at_checkGsmNetwork();
	if(rc != 0)
	{
		DEBUG_PRINTF("check gsm network failed\n");
		goto ExitProcessing;
	}

	//check app flow bearer status
	rc = wmmp_checkAppBearerStatus(flowID,&flag);
	if(rc != 0)
	{
		DEBUG_PRINTF("check gsm network failed\n");
		goto ExitProcessing;
	}
	//app flow bearer not open
	if(flag == 0)
	{
		//open bearer
		wmmp_openAppBearer(flowID);
	}

	modem_flushInputBuffer();
	//open app flow
	DEBUG_PRINTF("Open app flow %d\n",flowID);
	sprintf( cmdbuf, "AT$M2MAPPOPEN=%d", flowID);
	modem_sendCmd( cmdbuf );

	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse("$M2MAPPOPEN",20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

	if(sscanf(wmmp_rx_buf.line[0],"$M2MAPPOPEN: %d",&transID1) != 1)
	{
		DEBUG_PRINTF("transID not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}
	DEBUG_PRINTF("transID1 = %d\n",transID1);
	//wait async acks
	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse(NULL,20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_ACK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}
#ifdef WMMP_DEBUG
	for(i=0;i<wmmp_rx_buf.lines;i++)
		DEBUG_PRINTF("LINE[%d]:%s\n",i,wmmp_rx_buf.line[i]);
#endif
	if(sscanf(wmmp_rx_buf.line[0],"$M2MACK: %d",&transID2) != 1)
	{
		DEBUG_PRINTF("transID not found\n");
		goto ExitProcessing;
	}

	DEBUG_PRINTF("transID2 = %d\n",transID2);
	if(transID1 != transID2)
	{
		DEBUG_PRINTF("transID1 != transID2. transID1=%d, transID2=%d\n",transID1,transID2);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;
}

/**
	@func wmmp_readAppData 
	@brief 读取wmmp接收数据
	@param[in] flowID wmmp流通道
	@param[out] buf 输出BUFF指针
	@param[in-out] len 输入BUFF的最大长度，输出接收数据的实际长度
	@return   0-成功；	其他值-失败 
 */
int wmmp_readAppData(int flowID, char *buf, int *len)
{
	int rc;
	int i;
	int tmpflowID;
	int tmplen;
	int readlen;
	int timeout;
	time_t now,last;
	int retry;

	//char testbuf[4096];
	//char *p;
	char ch;

#ifdef WMMP_READ_DEBUG
	int fp;
#endif



#ifdef WMMP_READ_DEBUG
	fp = fopen("/temp.txt","ab+");
	if(fp == -1)
	{
		DEBUG_PRINTF("open file error\n");
		exit -1;
	}
	fwrite("start", 5, 1, fp);
#endif

	//int fd_ttys2;
	//fd_ttys2 = port_open("/dev/ttyS2");

	//char recbuf[APP_RECV_PACKET_MAXLEN*2+1]="";

	DEBUG_PRINTF("wmmp read data\n");

        retry = 2;
	while(retry --)
	{
		//modem_flushInputBuffer();
		rc = wmmp_waitResponse(NULL,60);
		if(rc != 0)
		{
			DEBUG_PRINTF("WMMP get response error: %x\n",rc);
			goto ExitProcessing;
		}
//	printf("rev4 = %s\n", wmmp_rx_buf.line[0]);
//	printf("*retry = %d\n", retry);

		if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_MSG)
		{
			DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
				//rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
				//goto ExitProcessing;
				continue;
		}
	

		if(sscanf(wmmp_rx_buf.line[0],"$M2MMSG: 7,%d,%d",&tmpflowID,&readlen) != 2)
		{
			DEBUG_PRINTF("expected msg not found\n");
			//rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			//goto ExitProcessing;
			continue;
		}

		/*printf("flowID = %d \nlen = %d\n",tmpflowID, readlen);*/
		if(tmpflowID != flowID)
		{
			DEBUG_PRINTF("unexpected flowID\n");
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		if((readlen > APP_RECV_PACKET_MAXLEN) || (readlen > *len))
		{
			DEBUG_PRINTF("readlen overflow!!!readlen=%d\n",readlen);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_APPREAD_OVERFLOW);
			goto ExitProcessing;
		}
		break;
	}

	if( retry < 0 )
	{
		DEBUG_PRINTF("expected msg not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

#if 0
	//wait data ?
	timeout = 5000;
	do
	{
		tmplen = modem_charsInBuffer();
		//DEBUG_PRINTF("read data len = %d\n",tmplen);
		timeout --;
	}while( (tmplen < (readlen*2)) && (timeout >0));

	if(timeout <= 0)
	{
		DEBUG_PRINTF("read data len = %d\n",tmplen);
		DEBUG_PRINTF("read data timeout\n");
//		rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_TIMEOUT);
//		goto ExitProcessing;
	}
	//read data into wmmp_rx_buf.buf
	modem_read(wmmp_rx_buf.buf,readlen*2);
#endif

#if 0
	//DEBUG_PRINTF("init response rx buffer\n");
	sleep(3);
	wmmp_initRxBuffer();
	time(&last);
	do
	{
#if 0
		tmplen = modem_charsInBuffer();
		if(tmplen != 0)
		{
			modem_read(wmmp_rx_buf.buf_tail,tmplen);
			wmmp_rx_buf.buf_tail += tmplen;
			*(wmmp_rx_buf.buf_tail) = '\0';
			wmmp_rx_buf.rxlen += tmplen;
			DEBUG_PRINTF("read data len = %d\n",wmmp_rx_buf.rxlen);
		}
#else
		//if(modem_charsInBuffer()){
		if(modem_read( wmmp_rx_buf.buf_tail, 1) == 1)
		//if(modem_read( &ch, 1) == 1)
		{
			//fwrite(&ch, 1, 1, fp);
			//modem_read( wmmp_rx_buf.buf_tail, 1);
			//DEBUG_PRINTF("%c",*wmmp_rx_buf.buf_tail);
			//write(fd_ttys2,wmmp_rx_buf.buf_tail,1);
			fwrite(wmmp_rx_buf.buf_tail, 1, 1, fp);
			wmmp_rx_buf.buf_tail++;
//			*(wmmp_rx_buf.buf_tail) = '\0';
			wmmp_rx_buf.rxlen++;
		}
#endif
		if(wmmp_rx_buf.rxlen < readlen*2)
		{
			time(&now);
			if(now > last + 60)			//timeout 60s
			{
				DEBUG_PRINTF("read data len = %d\n",wmmp_rx_buf.rxlen);
				DEBUG_PRINTF("read data timeout\n");
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_TIMEOUT);
				goto ExitProcessing;
			}
		}
		else
			break;
	}while(1);


#endif

	//20100417 for uart overrun error@115200bps
#if 1
	timeout=35000;
	int recLen = 0;

	/*printf("star receive!\n");*/	
	wmmp_initRxBuffer();
	do
	{
		recLen = modem_read( wmmp_rx_buf.buf_tail, readlen*2-wmmp_rx_buf.rxlen);
		if(recLen != 0)
		{
#ifdef WMMP_READ_DEBUG
			fwrite(wmmp_rx_buf.buf_tail, 1, recLen, fp);
#endif
			wmmp_rx_buf.buf_tail += recLen;
			//			*(wmmp_rx_buf.buf_tail) = '\0';
			wmmp_rx_buf.rxlen += recLen;
			DEBUG_PRINTF("wmmp_rx_buf.rxlen = %d\n", wmmp_rx_buf.rxlen);
		}

	}while((wmmp_rx_buf.rxlen < readlen*2) && timeout-- );

// 	printf("readlen*2 = %d\n",readlen*2);
// 	printf("wmmp_rx_buf.rxlen = %d\n",wmmp_rx_buf.rxlen);

	if(timeout <= 0)
	{
		rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_TIMEOUT);
		goto ExitProcessing;
	}


#endif

	//Decode hex into str
	tmplen = str2hex(wmmp_rx_buf.buf,readlen*2,buf,*len);
	/*printf("%s",buf);*/
	//normal return
#ifdef WMMP_READ_DEBUG
	fwrite("end", 3, 1, fp);
	fclose(fp);
#endif
	*len = tmplen;
	return 0;
	//error process
ExitProcessing:
#ifdef WMMP_READ_DEBUG
	fwrite("end", 3, 1, fp);
	fclose(fp);
#endif
	*len = 0;
	return rc;
}

int wmmp_sendAppData(int flowID, char *buf, int len)
{
	char cmdbuf[128];
	char ctrlz[1]={0x1A};
	int tmplen;
	int transID1,transID2;
	int rc;
	int wlen;
	int readlen;

	char sendbuf[APP_SEND_PACKET_MAXLEN*2+1]="";

	char *rp;

	rp = buf;

	int i = 0;

	do
	{
		tmplen = len > APP_SEND_PACKET_MAXLEN ? APP_SEND_PACKET_MAXLEN : len;
//		check status
//		DEBUG_PRINTF(("WMMP get status \n"));
//		rc = wmmp_checkStatus();
//		if( rc != 0 )
//		{
//			DEBUG_PRINTF(("WMMP ready for operation \n"));
//			goto ExitProcessing;
//		}

		//flush input buffer
		modem_flushInputBuffer();
		//send cmd
		DEBUG_PRINTF(("WMMP send cmd \n"));
		sprintf( cmdbuf, "AT$M2MAPPSEND=%d", flowID);
		modem_sendCmd( cmdbuf );
		
// 		printf("**i = %d tmplen = %d", i++, tmplen);
// 		printf("**send cmdbuf = %s", cmdbuf);
		

		DEBUG_PRINTF(("WMMP wait for prompt \n"));
//		rc = modem_waitFor(">",10);
//		if(rc != 0)
//		{
//			DEBUG_PRINTF(("WMMP get prompt error: %x\n",rc));
//			goto ExitProcessing;
//		}
		rc = wmmp_waitResponse(NULL,20);
		if (rc != 0)
		{
			DEBUG_PRINTF("error: rc = %x\n",rc);
			goto ExitProcessing;
		}
		

		if(wmmp_rx_buf.enumResponseType != RESPONSE_PROMPT)
		{
			DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}
#if 0
		//RAW data mode
		//write data
		DEBUG_PRINTF("WMMP send data,len =%d\n",tmplen);
		modem_write( rp, tmplen );
		//terminate
		modem_write( ctrlz, 1 );
#else
		//HEX encoded data mode
		//HEX encode the output data
		wlen = hex2str(rp,tmplen,sendbuf,(APP_SEND_PACKET_MAXLEN*2+1));

		//write data
		DEBUG_PRINTF("WMMP send data,len =%d\n",wlen);

		modem_write( sendbuf, wlen );
		//terminate
		modem_write( ctrlz, 1 );
#endif
		//wait response
		DEBUG_PRINTF("WMMP wait response \n");
		rc = wmmp_waitResponse("$M2MAPPSEND",20);

		if (rc != 0)
		{
			DEBUG_PRINTF("error: rc = %x\n",rc);
			goto ExitProcessing;
		}
//		printf("rev1 = %s\n", wmmp_rx_buf.line[0]);
		if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
		{
			DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		if(sscanf(wmmp_rx_buf.line[0],"$M2MAPPSEND: %d",&transID1) != 1)
		{
			DEBUG_PRINTF("transID not found\n");
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		DEBUG_PRINTF("transID1 = %d\n",transID1);
#if 1
		//wait async acks
		DEBUG_PRINTF("WMMP wait response \n");
		rc = wmmp_waitResponse(NULL,20);

		if (rc != 0)
		{
			DEBUG_PRINTF("error: rc = %x\n",rc);
			goto ExitProcessing;
		}
//		printf("rev2 = %s\n", wmmp_rx_buf.line[0]);
		if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_ACK)
		{
			DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		if(sscanf(wmmp_rx_buf.line[0],"$M2MACK: %d",&transID2) != 1)
		{
			DEBUG_PRINTF("transID not found\n");
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		DEBUG_PRINTF("transID2 = %d\n",transID2);
		if(transID1 != transID2)
		{
			DEBUG_PRINTF("transID1 != transID2. transID1=%d, transID2=%d\n",transID1,transID2);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}
#endif
/*
#if 1	//add by yy 20110818
		//wait async $MSG
		DEBUG_PRINTF("WMMP wait response \n");
		printf("**WMMP wait response $MSG \n"); //for debug
		rc = wmmp_waitResponse(NULL,20);

		if (rc != 0)
		{
			DEBUG_PRINTF("**error: rc = %x\n",rc);
			goto ExitProcessing;
		}
		printf("rev3 = %s", wmmp_rx_buf.line[0]);
		
		if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_SEND_MSG)
		{
			DEBUG_PRINTF("**unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		DEBUG_PRINTF("here\n");
	//	if(sscanf(wmmp_rx_buf.line[0],"$M2M$MSG: %d, %",&transID2) != 1)
		if(sscanf(wmmp_rx_buf.line[0],"$M2M$MSG: 7,%d,%d",&transID2,&readlen) != 2)
		{
			DEBUG_PRINTF("**transID not found\n");
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}

		DEBUG_PRINTF("transID2 = %d\n",transID2);
		printf("transID2 = %d\n",transID2); //for debug
		if(transID1 != transID2)
		{
			DEBUG_PRINTF("**transID2 = %d\n",transID2);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}
		DEBUG_PRINTF("readlen = %d\n",readlen);
		printf("readlen = %d\n",readlen); //for debug
		//比长度
		if(readlen != tmplen)
		{
			DEBUG_PRINTF("**readlen = %d, tmplen = %d\n",readlen, tmplen);
			rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
			goto ExitProcessing;
		}
		
#endif*/

		//update len and buf pointer
		len -= tmplen;
		rp = buf +tmplen;

	}while( len > 0 );
ExitProcessing:
	return rc;

}

int wmmp_closeAppFlow(int flowID)
{
	int rc;
	int transID1;
	int transID2;
	char cmdbuf[256];

	//flush input buffer
	modem_flushInputBuffer();
	//close app flow
	DEBUG_PRINTF("Close app flow %d\n",flowID);
	sprintf( cmdbuf, "AT$M2MAPPCLOSE=%d", flowID);
	modem_sendCmd( cmdbuf );

	//wait response
	DEBUG_PRINTF("WMMP wait response \n");
	rc = wmmp_waitResponse(NULL,20);

	if (rc != 0)
	{
		DEBUG_PRINTF("error: rc = %x\n",rc);
		goto ExitProcessing;
	}

	if(wmmp_rx_buf.enumResponseType != RESPONSE_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;
}



int wmmp_trapAlarmCode(unsigned short usAlarmCode)
{
	int rc;
	unsigned short tag=0x300D;
	char value[64];

	//check monitor platform
	rc = wmmp_checkMonitorPlatform();
	//report alarm code
	if(rc == 0)
	{
		sprintf(value,"%04X",usAlarmCode);
		rc = wmmp_trap(tag,value);
	}

	return rc;
}

int wmmp_readAppVer(char *appver)
{
	int rc;
//	char cmdstr[] = "$M2MVER?";
	char cmdstr[] = "$M2MVER";

	DEBUG_PRINTF("wmmp read m2mver\n");
	modem_flushInputBuffer();
	wmmp_sendCmd(cmdstr);
	rc = wmmp_waitResponse("",20);

//	printf("****rc*** = %d", rc);
	if (rc != 0)
	{
		DEBUG_PRINTF("WMMP get response error: %x\n",rc);
		goto ExitProcessing;
	}
//	printf("***rc*** = %d", rc);
	if(wmmp_rx_buf.enumResponseType != RESPONSE_WMMP_CMD_OK)
	{
		DEBUG_PRINTF("unexpected response: %d\n",wmmp_rx_buf.enumResponseType);
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}
//	printf("****%s***", wmmp_rx_buf.line[0]);
#if (SIMCOM==1)
	if(sscanf(wmmp_rx_buf.line[0],"APP:%s",appver)!=1)
#else
	if(sscanf(wmmp_rx_buf.line[0],"WMMP-LIB: %s",appver)!=1)
#endif
	{
		DEBUG_PRINTF("app ver not found\n");
		rc = MODEM_ERROR_NUM(MODEM_ERROR_WMMP,ERROR_WMMP_BAD_RESPONSE);
		goto ExitProcessing;
	}

ExitProcessing:
	return rc;
}

#endif		//_CONFIG_M2M_MODULE != 0



int wmmp_init(void)
{
#if (_CONFIG_M2M_MODULE != 0)
	
	int rc;
	int transID1;
	int transID2;
	char tmpbuf[1024];
	int len;

	//flush input buffer
	modem_flushInputBuffer();

	// Send init string;disable echo
	DEBUG_PRINTF("send cmd: MODEM_INIT_STR\n ");
	modem_sendCmd(MODEM_INIT_STR);
	rc = modem_waitFor(MODEM_OK_STR,20);
	if( rc != 0 )
	{
		DEBUG_PRINTF("Error: modem init\n");
		goto ExitProcessing;
	}

	//Enable module app
	rc = at_enableModuleApp();
	if(rc != 0)
	{
		DEBUG_PRINTF("Error: start module app\n");
		goto ExitProcessing;
	}

	//Enable/disable interactive mode
	//disable echo
	modem_sendCmd("AT$M2MBHV=0,0");
	rc = modem_waitFor("OK",10);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_init-1 WMMP get response error: %x\n",rc);
		goto ExitProcessing;
	}

	//Enable/disable interactive mode for DATA in data command
	//disable echo
	modem_sendCmd("AT$M2MBHV=3,0");
	rc = modem_waitFor("OK",10);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_init-2 WMMP get response error: %x\n",rc);
		goto ExitProcessing;
	}

	//Enable/disalbe raw data mode for DATA input in data command
	//AT$M2MBHV=4,1: <DATA> raw data mode
	//AT$M2MBHV=4,0: <DATA> HEX format
	//modem_sendCmd("AT$M2MBHV=4,1");
	modem_sendCmd("AT$M2MBHV=4,0");
	rc = modem_waitFor("OK",10);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_init-3 WMMP get response error: %x\n",rc);
		goto ExitProcessing;
	}
ExitProcessing:
	return rc;

#else
	return 0;
#endif
}

/**!
@parem s_APNtag : 0002
@parem s_APNvalue : APN接入点
@ 此函数应该没用，APN模式下的接入点设置是修改的标准拨号脚本。zl
*/
int wmmp_setAPN(char *s_APNtag, const char *s_APNvalue)
{
#if (_CONFIG_M2M_MODULE != 0)
	
	char buf[256];
	char ctrlz[1]={0x1A};
	int rc;

	//确认已登录M2M平台
	DEBUG_PRINTF("Send cmd...\n");
	modem_sendCmd("AT$M2MALMRST=1234");
	CommonSleep(20);//20ms
	printf("wmmp_setAPN\n");

	//Enable parsing of TLV data.
	modem_sendCmd("AT$M2MBHV=2,1");
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_setAPN 1 WMMP get response error: %x\n",rc);
		DEBUG_PRINTF("wmmp_setAPN 1, rc = %d\n", rc);
		goto ExitProcessing;
	}
	//write TLV
	modem_sendCmd("AT$M2MCFG=1");
	sleep(1);       
	rc  = modem_waitFor(">",WMMP_TIMEOUT);
	if(rc !=  0)
	{
		DEBUG_PRINTF("wmmp_setAPN 2 WMMP get response error: %x\n",rc);
		DEBUG_PRINTF("wmmp_setAPN 2,  rc  =  %d\n",  rc);
		goto ExitProcessing;
	}
	//write tag value
	sprintf(buf,"%s,%s",s_APNtag,s_APNvalue);
	DEBUG_PRINTF("Set   Value:   %s\n",buf);
	modem_write(   buf,   strlen(buf)  );
	modem_write( ctrlz, 1 );
	rc = modem_waitFor("OK",WMMP_TIMEOUT);
	if(rc != 0)
	{
		DEBUG_PRINTF("wmmp_setAppServer-3 WMMP get response error: %x\n",rc);
		DEBUG_PRINTF("wmmp_setAppServer 3, rc = %d\n", rc);
		goto ExitProcessing;
	}
	// printf("wmmp_setAppServer SUCCESS\n", rc);
ExitProcessing:
	return rc;

#else
	return 0;
#endif
}




