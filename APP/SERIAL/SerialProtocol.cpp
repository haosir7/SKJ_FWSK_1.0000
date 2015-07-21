#include "IncludeMe.h"
#include <termios.h>
#include <string>
#include <stdlib.h>
#include "arithmetic.h"
#include <unistd.h>
#include "TDateTime.h"
#include "SerialProtocol.h"
#include "VersionConfig.h"
#include "commonFunc.h"
#include "CGlobalArg.h"
#include "DataDesign.h"
using namespace std;


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

SerialProtocol::SerialProtocol()
{
	resetAll();
}

SerialProtocol::~SerialProtocol()
{
	ReleasePort();
}

void SerialProtocol::resetSendStruct(){
	m_sendCmd = &m_cSendCmd;
	memset (m_sendCmd, 0, sizeof(send_cmdStruct));
	m_sendCmd->sendData = m_sendBuf;
}

void SerialProtocol::resetRspStruct() {
	m_rspCmd = &m_cRspCmd;
	memset (m_rspCmd, 0, sizeof(rsp_cmdStruct));
	m_rspCmd->rspData = businessData;
}

void SerialProtocol::resetAll() {
	memset (m_sendBuf, 0, SERIAL_BUFFER_MAX_LEN);
	memset (m_revBuf, 0, SERIAL_BUFFER_MAX_LEN);
	memset (businessData, 0, BUSINESS_DATA_BUF_MAX_LEN);
	resetSendStruct();
	resetRspStruct();
	m_fill_count = 0;
	m_get_count = 0;
	packageNo = 0;
	headLen = 0;
	packageLen = 0;

	newCmd = true;
}

/*!
@brief init serial port. private.
*/
bool SerialProtocol::InitPort(INT8 *devpath, int baud)
{
		INT32 res;
		pSerial = &serial;
		SerialPortContruct(pSerial);

 		res = pSerial->InitPort(pSerial, devpath, UART, baud);

		if (res == FAILURE) 
		{
		 	pSerial->ClosePort(pSerial);
		 	DBG_PRINT(("COM3: InitPort Failure!"));
		 	return FAILURE;
		}
		//m_devfd = pSerial->m_fd;
		//DBG_PRINT(("COM3: InitPort Success %d!!!!!!!", m_devfd));
		return SUCCESS;
}

bool SerialProtocol::ReleasePort()
{
	pSerial->ClosePort(pSerial);
	return SUCCESS;
}


UINT8 SerialProtocol::sendData(UINT8 type, UINT8 cmd, string &strErr)
{
	UINT16 templen;
	UINT8 headLen=0;
	UINT16 bufLen=0;
	UINT8 ret=0;
	
	m_sendCmd->cmdType = type;
	m_sendCmd->cmdNo = cmd;
	DBG_PRINT(("m_sendCmd->cmdNo : 0x%x", m_sendCmd->cmdNo));

	headLen = ZC_PROTOCOL==type? SENDPACK_ZC_HEAD_LENGTH: SENDPACK_AISINO_HEAD_LENGTH;
	bufLen = ZC_PROTOCOL==type? SERIAL_PACKAGE_ZC_MAX_LEN: SERIAL_PACKAGE_AISINO_MAX_LEN;
	DBG_PRINT(("cmdType = %x, cmdNo = %x, m_fill_count = %u, headLen = %u, bufLen = %u", type, cmd, m_fill_count, headLen, bufLen));
	DBG_PRINT(("!!!!!!!!!!是否超过255字节!!!!!!!!!!m_fill_count+headLen+CRC_LEN  = %u", m_fill_count+headLen+CRC_LEN ));
	if (m_fill_count+headLen+CRC_LEN <= bufLen)
	{
		m_sendCmd->PackNo = 0x00;
		m_sendCmd->cmdLen = m_fill_count+headLen;	
		memcpy((void *)(m_sendCmd->sendData+headLen), businessData, m_fill_count);

		ret = Send_Pack();
 		if (SUCCESS != ret)
 		{
			strErr = SERAL_SEND_DATA_ERR;
 			return FAILURE;
 		}		
	} 
	else
	{
		packageNo = 1;
		m_get_count = 0;
		DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!templen = %u, m_fill_count = %u", m_get_count, m_fill_count));
		while(m_get_count < m_fill_count)
		{
			DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!m_fill_count-m_get_count + headLen + CRC_LEN  = %u", m_fill_count-m_get_count + headLen + CRC_LEN ));
			if (m_fill_count-m_get_count + headLen + CRC_LEN > bufLen)
			{
				templen = bufLen - headLen - CRC_LEN;
				DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!templen = %u", templen));
			} 
			else
			{
				templen = m_fill_count-m_get_count;
				packageNo = 0;
				DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!templen = %u", templen));
			}
			m_sendCmd->PackNo = packageNo;
			m_sendCmd->cmdLen = templen+headLen;
			DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!templen = %u", templen));
			memcpy((void *)(m_sendCmd->sendData+headLen), businessData+m_get_count, templen);
			m_get_count += templen;

			ret = Send_Pack();
			if (SUCCESS != ret)
 			{
				strErr = SERAL_SEND_DATA_ERR;
 				return FAILURE;
			}				
	
			if (0 == packageNo)
			{
				break;
			}

			ret = Wait_Next_Cmd_Pack(strErr);
			DBG_PRINT(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!接到的ret = %u", ret));
			if (ER_PACKAGE_TYPE == ret || SERCMD_HEAD_PARA_ERR == ret)
			{
				strErr = WAIT_NEXT_DATA_ERR;
				return FAILURE;
			}
			else if (NP_PACKAGE_TYPE == ret)
			{
				packageNo++;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return SUCCESS;
}

UINT8 SerialProtocol::Wait_Next_Cmd_Pack(string &strErr)
{
	UINT8 ret;
	UINT8 tempPakageNo=0;

	ret = Rev_Pack(strErr);
	if (SERCMD_SUCCESS != ret)
	{
		if (SERCMD_OVERTIME_ERR != ret)
		return ret;
	}

	tempPakageNo = m_revBuf[2];
	if (m_revBuf[0]=='W' && m_revBuf[1]=='T')
	{
		DBG_PRINT(("WT package!!!"));
		return WT_PACKAGE_TYPE;
	}
	if (m_revBuf[0]=='E' && m_revBuf[1]=='R')
	{
		DBG_PRINT(("ER package!!!"));
		memcpy(m_rspCmd->rspData, m_revBuf+headLen, packageLen-headLen);
		m_rspCmd->rspData[packageLen-headLen] = 0x00;
		return ER_PACKAGE_TYPE;
	}

	if (m_revBuf[0]=='N' && m_revBuf[1]=='P')
	{
		DBG_PRINT(("NP package!!!"));
		if (0x00 != tempPakageNo)
		{
			DBG_PRINT(("wait next package head error 1"));
			strErr = HEAD_PARA_ERR;
			return SERCMD_HEAD_PARA_ERR;
		}
		if (m_sendCmd->PackNo != m_revBuf[packageLen-1])
		{
			DBG_PRINT(("wait next package head error 2"));
			strErr = HEAD_PARA_ERR;
			return SERCMD_HEAD_PARA_ERR;
		}

		return NP_PACKAGE_TYPE;
	}

	if (m_revBuf[0]=='O' && m_revBuf[1]=='K')
	{
		DBG_PRINT(("OK package!!!"));
		DBG_PRINT(("Should NOT rev OK package here!!!"));
		//该函数接收到的OK包肯定是第一个包
/*		if (0x01 != tempPakageNo && 0x00 != tempPakageNo)
		{
			DBG_PRINT(("wait next package head error 3"));
			return SERCMD_HEAD_PARA_ERR;
		}

		m_rspCmd->dataLen = packageLen-headLen;
		memcpy(m_rspCmd->rspData, m_revBuf+headLen, m_rspCmd->dataLen);*/

		return OK_PACKAGE_TYPE;
	}

	return SERCMD_SUCCESS;
}

UINT8 SerialProtocol::revData(string &strErr)
{
	UINT8 ret=0;
	DBG_PRINT(("进入 SerialProtocol::revData()中"));
	m_rspCmd->dataLen = 0;
	headLen = ZC_PROTOCOL==m_sendCmd->cmdType? RSPPACK_ZC_HEAD_LENGTH: RSPPACK_AISINO_HEAD_LENGTH;
	do 
	{
		ret = Rev_Pack(strErr);
		DBG_PRINT(("Rev_Pack ret = %u", ret));
		if (SERCMD_SUCCESS != ret)
		{
			DBG_PRINT(("ret = 0x%x", ret));
			resetRspStruct();
			clearSerial();	

			break;
		}
		else 
		{
			DBG_PRINT(("respone head = %c%c", m_rspCmd->head[0], m_rspCmd->head[1]));
			if (m_rspCmd->head[0]=='O' && m_rspCmd->head[1]=='K')
			{
				DBG_PRINT(("get OK reponse"));
				DBG_PRINT(("packageLen=%u, headLen=%u", packageLen, headLen));
				memcpy((void *)(m_rspCmd->rspData+m_rspCmd->dataLen), (void *)(m_revBuf+headLen), packageLen-headLen);
				m_rspCmd->dataLen += (packageLen-headLen);
				DBG_PRINT(("m_rspCmd->PackNo = %u", m_rspCmd->PackNo));
				if (0 == m_rspCmd->PackNo)
				{
					if (0x01 == m_rspCmd->multiPackages)
					{
						Rsp_OK();
					}
					
					break;
				}
				else
				{
					m_rspCmd->multiPackages = 0x01;
					Rsp_NP(m_rspCmd->PackNo);
				}
			}
			else if((m_rspCmd->head[0]=='E' && m_rspCmd->head[1]=='R'))
			{
				DBG_PRINT(("get ER reponse packageLen-headLen = %u", packageLen-headLen));
				memcpy(m_rspCmd->rspData, m_revBuf+headLen, packageLen-headLen);
				m_rspCmd->rspData[packageLen-headLen] = 0x00;
				break;
			} 
			else if (m_rspCmd->head[0]=='W' && m_rspCmd->head[1]=='T')
			{
				DBG_PRINT(("get WT reponse"));
			}
		}
		DBG_PRINT(("ret = 0x%x", ret));
	} while (1);

	DBG_PRINT(("ret = 0x%x", ret));
	return ret;
}

/*!
@brief receive a pack. private	 
*/
UINT8 SerialProtocol::Rev_Pack(string &strErr)
{
	UINT8 headLen=0;
	UINT16 crc = 0;
	UINT8 offset=0;
	int readLen = 0;
	UINT32 times=0;

	memset(m_revBuf, 0x00, sizeof(m_revBuf));
	//款机放读取响应数据超时为30s
	while(times < WAIT_TIME) 
	{
		ReadPort(pSerial->m_fd, m_revBuf+offset, 2-offset, &readLen);
		offset += readLen;
		if (offset >= 2)
		{
			break;
		}
		else
		{
			CommonSleep(100);
			times++;
		}
	}
	DBG_PRINT(("!!!!!!!!!!!!!!!offset = %u", offset));
	if (offset < 2)
	{
		DBG_PRINT(("wait next package time out"));
		strErr = WAIT_TIMEOUT_ERR;
		return SERCMD_OVERTIME_ERR;
	}

	headLen = ZC_PROTOCOL==m_sendCmd->cmdType? RSPPACK_ZC_HEAD_LENGTH: RSPPACK_AISINO_HEAD_LENGTH;
	DBG_PRINT(("package head lenght is %u", headLen));
	
	times = 0;
	while(times<REV_TIME)
	{
		ReadPort(pSerial->m_fd, m_revBuf+offset, headLen-offset, &readLen);
		offset += readLen;
		if (offset >= headLen)
		{
			break;
		}
		else
		{
			CommonSleep(TIME_MS);
			times++;
		}
	}
	DBG_PRINT(("!!!!!!!!!!!!!!!offset = %u", offset));
	if (offset < headLen)
	{
		DBG_PRINT(("wait next package time out"));
		strErr = WAIT_TIMEOUT_ERR;
		return SERCMD_OVERTIME_ERR;
	}

	packageLen = ZC_PROTOCOL==m_sendCmd->cmdType? m_revBuf[3]:getShort(m_revBuf+3);
	DBG_PRINT(("package lenght is %u", packageLen));

	times = 0;
	while(times<REV_TIME)
	{
		ReadPort(pSerial->m_fd, m_revBuf+offset, packageLen+CRC_LEN-offset, &readLen);
		offset += readLen;
		if (offset >= packageLen+CRC_LEN)
		{
			break;
		}
		else 
		{
			CommonSleep(TIME_MS);
			times++;
		}
	}
	DBG_NPRINT_HEX(m_revBuf, offset);
	DBG_PRINT(("!!!!!!!!!!!!!!!offset = %u", offset));
	if (offset < packageLen+CRC_LEN)
	{
		DBG_PRINT(("wait next package time out"));
		strErr = WAIT_TIMEOUT_ERR;
		return SERCMD_OVERTIME_ERR;
	}

	//检查校验码
	crc = Cal_CRC(m_revBuf, packageLen);
	if (!(m_revBuf[packageLen]==(UINT8)(crc>>8) && m_revBuf[packageLen+1]==(UINT8)(crc&0xff) ))
	{
		DBG_PRINT(("wait next package crc error"));
		strErr = CRC_ERR;
		return SERCMD_CRC_ERR;	
	}

	m_rspCmd->head[0] = m_revBuf[0];
	m_rspCmd->head[1] = m_revBuf[1];
	m_rspCmd->PackNo = m_revBuf[2];
	m_rspCmd->cmdLen = packageLen;

	return SERCMD_SUCCESS;	
}

//void SerialProtocol::Send_Pack(){
UINT8 SerialProtocol::Send_Pack()
{
	UINT16 crc;
	UINT8 tempbytes[2];
	int writeLen=0;

	m_sendCmd->sendData[0] = 0x1B;
	m_sendCmd->sendData[1] = m_sendCmd->cmdType;
	m_sendCmd->sendData[2] = m_sendCmd->cmdNo;
	m_sendCmd->sendData[3] = m_sendCmd->PackNo;
	if (ZC_PROTOCOL==m_sendCmd->cmdType)
	{
		m_sendCmd->sendData[4] = m_sendCmd->cmdLen;
	} 
	else
	{
		getBytes(m_sendCmd->cmdLen, tempbytes);
		m_sendCmd->sendData[4] = tempbytes[0];
		m_sendCmd->sendData[5] = tempbytes[1];
	}

	crc = Cal_CRC(m_sendCmd->sendData, m_sendCmd->cmdLen);	
	m_sendCmd->sendData[m_sendCmd->cmdLen] = (UINT8)(crc>>8);
	m_sendCmd->sendData[m_sendCmd->cmdLen+1] = (UINT8)(crc&0xff);
	m_sendCmd->cmdLen += CRC_LEN;
	DBG_PRINT(("m_sendCmd->cmdLen=%u", m_sendCmd->cmdLen));
	DBG_NPRINT_HEX(m_sendCmd->sendData, m_sendCmd->cmdLen);

	INT32 ret = WritePort(pSerial->m_fd, m_sendCmd->sendData, m_sendCmd->cmdLen, &writeLen);
	DBG_PRINT(("发送数据后的返回结果 ret=%u", ret));	//ret = 0 发送失败； ret = 1 发送成功
	
	return ret;
}

// void SerialProtocol::Send_Pack(UINT16 len){
UINT8  SerialProtocol::Send_Pack(UINT16 len)
{
	int writeLen=0;

	WritePort(pSerial->m_fd, m_sendBuf, len, &writeLen);
}

/*!
@brief respond a normal "OK cmd"	 
*/
void SerialProtocol::Rsp_OK()
{
	UINT16 crc;

	m_sendBuf[0] = 'O';
	m_sendBuf[1] = 'K';
	m_sendBuf[2] = 0x00;
	if (ZC_PROTOCOL==m_sendCmd->cmdType)
	{
		m_sendBuf[3] = 0x04;
		crc = Cal_CRC(m_sendBuf, 4);	
		m_sendBuf[4] = (UINT8)(crc>>8);
		m_sendBuf[5] = (UINT8)(crc&0xff);	
		Send_Pack(6);
	} 
	else
	{
		m_sendBuf[3] = 0x00;
		m_sendBuf[4] = 0x05;
		crc = Cal_CRC(m_sendBuf, 5);	
		m_sendBuf[5] = (UINT8)(crc>>8);
		m_sendBuf[6] = (UINT8)(crc&0xff);	
		Send_Pack(7);
	}
}

void SerialProtocol::clearSerial()
{
	UINT8 temp[8];
	int readLen=1;
	while(readLen>0)
	{
		ReadPort(pSerial->m_fd, temp, 1, &readLen);
		DBG_PRINT(("调用清串口函数后的返回结果：readLen = %d", readLen));
	}

//	int  ret = tcflush(pSerial->m_fd, TCIOFLUSH);
//	DBG_PRINT(("调用清串口函数后的返回结果：ret = %d", ret));
//	sleep(2); 
}

/*!
@brief respond a "ER cmd"	 
*/
// void SerialProtocol::Rsp_ERR(UINT8 err)
// {
// 	UINT16 crc;
// 	
// 	m_sendBuf[0] = 'E';
// 	m_sendBuf[1] = 'R';
// 	m_sendBuf[2] = 0x00;
// 	if (ZC_PROTOCOL==m_sendCmd->cmdType)
// 	{
// 		m_sendBuf[3] = 0x05;

// 		m_sendBuf[4] = err;
// 		crc = Cal_CRC(m_sendBuf, 5);	
// 		m_sendBuf[5] = (UINT8)(crc>>8);
// 		m_sendBuf[6] = (UINT8)(crc&0xff);	
// 		Send_Pack(7);
// 	} 
// 	else
// 	{
// 		m_sendBuf[3] = 0x00;
// 		m_sendBuf[4] = 0x06;
// 		m_sendBuf[5] = err;
// 		crc = Cal_CRC(m_sendBuf, 6);	
// 		m_sendBuf[6] = (UINT8)(crc>>8);
// 		m_sendBuf[7] = (UINT8)(crc&0xff);	
// 		Send_Pack(8);
// 	}
// 
// 	resetAll();
// //	clearSerial();
// }

/*!
@brief respond a "ER cmd"	 
*/
// void SerialProtocol::Rsp_ERR(string errstr)
// {
// 	UINT16 crc;
// 	UINT16 errlen=0;
// 
// 	m_sendBuf[0] = 'E';
// 	m_sendBuf[1] = 'R';
// 	m_sendBuf[2] = 0x00;
// 	
// 	if (ZC_PROTOCOL==m_sendCmd->cmdType)
// 	{
// 		if (errstr.length()+4+CRC_LEN > SERIAL_PACKAGE_ZC_MAX_LEN)
// 		{
// 			errlen = SERIAL_PACKAGE_ZC_MAX_LEN - 4 - CRC_LEN;
// 		} 
// 		else
// 		{
// 			errlen = errstr.length();
// 		}
// 		m_sendBuf[3] = errlen+4;
// 		memcpy(m_sendBuf+4, errstr.c_str(), errlen);
// 		
// 		crc = Cal_CRC(m_sendBuf, errlen+4);	
// 		m_sendBuf[errlen+4] = (UINT8)(crc>>8);
// 		m_sendBuf[errlen+5] = (UINT8)(crc&0xff);	
// 		Send_Pack(errlen+6);
// 	} 
// 	else
// 	{
// 		if (errstr.length()+5+CRC_LEN > SERIAL_PACKAGE_AISINO_MAX_LEN)
// 		{
// 			errlen = SERIAL_PACKAGE_AISINO_MAX_LEN - 5 - CRC_LEN;
// 		} 
// 		else
// 		{
// 			errlen = errstr.length();
// 		}
// 		UINT8 lenarray[2];
// 		memset(lenarray, 0x00, sizeof(lenarray));
// 		getBytes(errlen+4, lenarray);
// 		m_sendBuf[3] = lenarray[0];
// 		m_sendBuf[4] = lenarray[1];
// 		memcpy(m_sendBuf+5, errstr.c_str(), errlen);
// 		crc = Cal_CRC(m_sendBuf, errlen+5);	
// 		m_sendBuf[errlen+5] = (UINT8)(crc>>8);
// 		m_sendBuf[errlen+6] = (UINT8)(crc&0xff);	
// 		Send_Pack(errlen+7);
// 	}
// 
// 	resetAll();
// 	//	clearSerial();
// }

/*!
@brief respond a "WT cmd"	 
*/
void SerialProtocol::Rsp_WT()
{
	UINT16 crc;
	
	m_sendBuf[0] = 'W';
	m_sendBuf[1] = 'T';
	m_sendBuf[2] = 0x00;
	if (ZC_PROTOCOL==m_sendCmd->cmdType)
	{
		m_sendBuf[3] = 0x04;
		crc = Cal_CRC(m_sendBuf, 4);	
		m_sendBuf[4] = (UINT8)(crc>>8);
		m_sendBuf[5] = (UINT8)(crc&0xff);	
		Send_Pack(6);
	} 
	else
	{
		m_sendBuf[3] = 0x00;
		m_sendBuf[4] = 0x05;
		crc = Cal_CRC(m_sendBuf, 5);	
		m_sendBuf[5] = (UINT8)(crc>>8);
		m_sendBuf[6] = (UINT8)(crc&0xff);	
		Send_Pack(7);
	}
}

/*!
@brief respond a "NP cmd"	 
*/
void SerialProtocol::Rsp_NP(UINT8 packageNo)
{
	UINT16 crc;
	
	m_sendBuf[0] = 'N';
	m_sendBuf[1] = 'P';
	m_sendBuf[2] = 0x00;
	if (ZC_PROTOCOL==m_sendCmd->cmdType)
	{
		m_sendBuf[3] = 0x05;
		m_sendBuf[4] = packageNo;
		crc = Cal_CRC(m_sendBuf, 5);	
		m_sendBuf[5] = (UINT8)(crc>>8);
		m_sendBuf[6] = (UINT8)(crc&0xff);
		Send_Pack(7);
	} 
	else
	{
		m_sendBuf[3] = 0x00;
		m_sendBuf[4] = 0x06;
		m_sendBuf[5] = packageNo;
		crc = Cal_CRC(m_sendBuf, 6);	
		m_sendBuf[6] = (UINT8)(crc>>8);
		m_sendBuf[7] = (UINT8)(crc&0xff);
		Send_Pack(8);
	}
}

/*!
@brief calculate CRC code	 
*/

UINT16 SerialProtocol::Cal_CRC(UINT8 *ptr, UINT16 len)
{
	unsigned char i;
	unsigned int crc=0;
	while(len--!=0)
	{
    	for(i=0x80; i!=0; i/=2)
		{
      		if((crc&0x8000)!=0) 
			{
				crc*=2;
				crc^=0x18005;
			}
        	else
			{
				crc*=2;
			}
			if((*ptr&i)!=0) crc^=0x18005; 
    	}
    	ptr++;
  	}
  	return(crc);	
}

/*!
@brief fill a byte parament to rsp buffer	 
*/
INT8 SerialProtocol::FillParament(UINT8 byte)
{
	if (m_fill_count + 1 > BUSINESS_DATA_BUF_MAX_LEN)
		return FAILURE;

	businessData[m_fill_count] = byte;
	m_fill_count++;

	return SUCCESS;
}

/*!
@brief fill a value parament to rsp buffer. 0-web order 
*/
INT8 SerialProtocol::FillParament(INT64 x, UINT8 len, bool order)
{
	INT8 i;
	INT8 *p=(INT8 *)&x;

	if (m_fill_count +len > BUSINESS_DATA_BUF_MAX_LEN)
		return FAILURE;
	
	if (len!=2 && len!=4 && len!=8)
		return FAILURE;

	for (i=0; i<len; i++)
	{
		if (order == 1)
			*(businessData+m_fill_count+i) = *(p+i);
		else
			*(businessData+m_fill_count+len-i-1) = *(p+i);
	}

	m_fill_count += len;

	return SUCCESS;
}

/*!
@brief fill buffer parament to rsp buffer	 
*/
INT8 SerialProtocol::FillParament(UINT8 *buf, UINT8 len)
{
	DBG_PRINT(("进入FillParament(UINT8 *buf, UINT8 len)"));
	if (m_fill_count +len > BUSINESS_DATA_BUF_MAX_LEN)
		return FAILURE;

	memcpy(businessData+m_fill_count, buf, len );
	m_fill_count += len;
	DBG_PRINT(("m_fill_count = %u", m_fill_count));

	return SUCCESS;
}

/*!
@brief fill a "string" parament to rsp buffer	 
*/
INT8 SerialProtocol::FillParament(string str, UINT32 maxlen)
{
	DBG_PRINT(("进入FillParament(string str, UINT32 maxlen)"));
	UINT32 len=0;

	len = str.length()>maxlen? maxlen : str.length();
	if (m_fill_count + maxlen > BUSINESS_DATA_BUF_MAX_LEN)
		return FAILURE;

	memset(businessData+m_fill_count, 0, maxlen );
	strncpy ( (char *)businessData+m_fill_count, str.c_str(), len);
	m_fill_count += maxlen;

	DBG_PRINT(("m_fill_count = %u", m_fill_count));

	return SUCCESS;
}

/*!
@brief delay function for serial. private.
*/
void SerialProtocol::ser_delay(UINT32 Msecs)
{
	UINT32  i;

   	for(; Msecs>0; Msecs--) 
      	for(i=0; i<2500; i++);
    return;
}

