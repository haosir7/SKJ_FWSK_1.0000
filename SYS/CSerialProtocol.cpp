/*! \file    CSerialProtocol.cpp
   \brief    税控款机国标串口通讯协议类实现
   \author   zl
 */

#ifndef WIN32
#include "lcd_tiny.h"
#endif
#include "arithmetic.h"
#include "TDateTime.h"
#include "CSerialProtocol.h"
using namespace std;

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"


#if (USE_SERIAL_PROTOCOL!=0)


Ser_ErrMsg CSerialProtocol::m_errMsg[] = 
{Ser_ErrMsg(0x1f,	"命令CRC校验错"),
Ser_ErrMsg(0x20,	"命令号无法识别"),
Ser_ErrMsg(0x22,	"命令长度错误"),
Ser_ErrMsg(0x30,	"命令接收超时"),
//Ser_ErrMsg(0x01,	"税控卡上电错误"),
//Ser_ErrMsg(0x02,	"用户卡上电错误"),
//Ser_ErrMsg(0x03,	"稽查卡上电错误"),
Ser_ErrMsg(0x04,	"无用户卡"),
Ser_ErrMsg(0x05,	"无税控卡"),
Ser_ErrMsg(0x06,	"无稽查卡")
//..........
};

CSerialProtocol::CSerialProtocol()
{
	m_CmdNO = -1;
	m_RegCmdSum = 0;
	memset (m_revBuf, 0, SERIAL_MAX_BUFFER);
	memset (m_rspBuf, 0, SERIAL_MAX_BUFFER);

	m_revCmd = &m_cRevCmd;
	memset (m_revCmd, 0, sizeof(rev_cmdStruct));
	m_rspCmd = &m_cRspCmd;
	memset (m_rspCmd, 0, sizeof(rsp_cmdStruct));

	m_Serial = &m_cSerial;
	SerialPortContruct(m_Serial);

	m_fill_count = 0;
	m_get_count = 0;

}

CSerialProtocol::~CSerialProtocol()
{
}

INT8 CSerialProtocol::Add_CMD(void *obj, INT16 CmdNO, ONCMD_FUN cmdFUN)
{
	if (m_RegCmdSum >= CMD_MAX_NO)
	{
		return FAILURE;
	}

	m_cmdReg[m_RegCmdSum].obj = obj;
	m_cmdReg[m_RegCmdSum].CmdNO = CmdNO;
	m_cmdReg[m_RegCmdSum].OnCmdFUN = cmdFUN;
	m_RegCmdSum ++;
	
	return SUCCESS;
}

INT8 CSerialProtocol::Rev_Pack(const char *pack_start)
{
	UINT8 rev_Len;
	UINT8 i=0, packstart_flag=0, tmchar;
	UINT16 pack_len;
	INT64 overtime=0;
	
	//判断包的起始标志1B 10，过虑掉无效字节
	rev_Len = 0;
	while(packstart_flag != 1) 
	{
		i += GetReceiveCount();
		while (i>=2)
		{
			tmchar = GetChar();
			if (m_revBuf[0] != pack_start[0])
			{
				if (tmchar == pack_start[0])
					m_revBuf[0] = pack_start[0];
				else
					i --;
			}
			else 
			{
				if (tmchar == pack_start[1])
				{
					m_revBuf[1] = pack_start[1];
					packstart_flag = 1;

					memset(m_revBuf, 0, SERIAL_MAX_BUFFER);	
					m_revBuf[0] = pack_start[0];
					m_revBuf[1] = pack_start[1];

					break;
				}
				else if (tmchar == pack_start[0])
				{
					m_revBuf[0] = pack_start[0];
					i --;
				}
				else
				{
					m_revBuf[0] = 0x00;
					i -= 2;
				}
			}
		}

		overtime ++;
		if (overtime > SERIAL_NODATA_MAX)
			return SERCMD_NO_DATA;
	}

	//接收命令号、包序号、长度3个字节，如果规定时间内未接收完整，返回超时错误
	rev_Len = 2;
	while( GetReceiveCount() < REVPACK_HEAD_LENGTH-2 )
	{
		overtime ++;
		if (overtime > SERIAL_OVERTIME_MAX)
			return SERCMD_OVERTIME_ERR;
	}
	m_revBuf[2] = GetChar();
	m_revBuf[3] = GetChar();
	m_revBuf[4] = GetChar();
	pack_len = m_revBuf[4] + 2;

	//接收参数和校验码，如果规定时间内未接收完整，则返回包长度错误
	rev_Len=5;
	while (1)
	{
		while (GetReceiveCount())
		{
			m_revBuf[rev_Len++] = GetChar();
			if (rev_Len >= pack_len)
			{
				break;
			}
		}

		if (rev_Len >= pack_len)
		{
			break;
		}

		overtime ++;
		if (overtime > SERIAL_OVERTIME_MAX)
			return SERCMD_PACKLEN_ERR;	
	}

	return SERCMD_SUCCESS;	
}

void CSerialProtocol::Send_Pack()
{
	UINT16 send_len, i;

	send_len = m_rspBuf[3]+2;
	for(i=0; i<send_len; i++)
	{
		PutChar(m_rspBuf+i);
	}

}

INT8 CSerialProtocol::Wait_Cmd_Pack()
{
	INT8 res, crc_flag=0;
	UINT8 pack_len, regNo, i;
	UINT16 crc;
	char pack_start[]={0x1b, 0x10};

	if ( (res=Rev_Pack(pack_start)) != SERCMD_SUCCESS )
		return res;
	pack_len = m_revBuf[4] + 2;
	
	//检查校验码
	crc = Cal_CRC(m_revBuf, pack_len-2);
	if (*(m_revBuf+(pack_len-2))==(UINT8)(crc>>8) && 
		*(m_revBuf+(pack_len-1))==(UINT8)(crc&0xff) )
	{
		crc_flag = 1;
	}
	else 
	{
		memset(m_revBuf, 0, SERIAL_MAX_BUFFER);
		return SERCMD_CRC_ERR;	
	}

	//检查命令号
	m_CmdNO = m_revBuf[2];
	for (i=0; i<m_RegCmdSum; i++)
	{
		if (m_cmdReg[i].CmdNO == m_CmdNO)
		{
			regNo = i;
			break;
		}
	}
	if (i == m_RegCmdSum)
	{
		return SERCMD_CMDNO_ERR;	
	}

	//填写包结构体
	m_revCmd->PackNo = m_revBuf[3];
	m_revCmd->Len = m_revBuf[4];
	if (m_revCmd->Len > 5)
	{
		m_get_count = 0;
		memcpy(m_revCmd->Para, m_revBuf+5, m_revCmd->Len-5);
	}

	//回调业务层处理函数
	m_get_count = 0;
	memset (m_rspCmd, 0, sizeof(rsp_cmdStruct));
//	m_rspCmd->Len = RSPPACK_HEAD_LENGTH;
#ifndef WIN32
	WB_LCM_BKL_ON();
#endif
	m_cmdReg[regNo].OnCmdFUN(m_cmdReg[regNo].obj, this);
			
	if (m_revCmd->PackNo != 0)
		return SERCMD_NOT_LASTPACK;
	else
		return SERCMD_SUCCESS;
}

INT8 CSerialProtocol::Wait_NP_Pack()
{
	INT8 res, crc_flag=0;
	UINT8 pack_len;
	UINT16 crc;
	string pack_start="NP";

	if ( (res=Rev_Pack(pack_start.c_str())) != SERCMD_SUCCESS )
		return res;

	pack_len = m_revBuf[4] + 2;
	//检查校验码
	crc = Cal_CRC(m_revBuf, pack_len-2);
	if (*(m_revBuf+(pack_len-2))==(UINT8)(crc>>8) && 
		*(m_revBuf+(pack_len-1))==(UINT8)(crc&0xff) )
	{
		crc_flag = 1;
	}
	else
	{
		memset(m_revBuf, 0, SERIAL_MAX_BUFFER);
		return SERCMD_CRC_ERR;	
	}

	memset (m_rspCmd, 0, sizeof(rsp_cmdStruct));
//	m_rspCmd->Len = RSPPACK_HEAD_LENGTH;
	return SERCMD_SUCCESS;
}

void CSerialProtocol::Rsp_OK()
{
	UINT16 crc;
	
	m_rspBuf[0] = 'O';
	m_rspBuf[1] = 'K';
	m_rspBuf[2] = m_rspCmd->PackNo;
	m_rspCmd->Len += RSPPACK_HEAD_LENGTH;
	m_rspBuf[3] = m_rspCmd->Len;

	crc = Cal_CRC(m_rspBuf, m_rspCmd->Len);	
	m_rspBuf[m_rspCmd->Len] = (UINT8)(crc>>8);
	m_rspBuf[m_rspCmd->Len+1] = (UINT8)(crc&0xff);	
	Send_Pack();

	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);

}

void CSerialProtocol::Rsp_OK(UINT8 packNo, UINT8 packLen)
{
	UINT16 crc;
	
	m_rspBuf[0] = 'O';
	m_rspBuf[1] = 'K';
	m_rspBuf[2] = packNo;
	m_rspBuf[3] = RSPPACK_HEAD_LENGTH;
	m_rspCmd->Len += RSPPACK_HEAD_LENGTH;

	crc = Cal_CRC(m_rspBuf, m_rspCmd->Len);	
	m_rspBuf[m_rspCmd->Len] = (UINT8)(crc>>8);
	m_rspBuf[m_rspCmd->Len+1] = (UINT8)(crc&0xff);	
	Send_Pack();

	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);

}

void CSerialProtocol::Rsp_ERR(UINT8 err)
{
	UINT16 crc;
	
	m_rspBuf[0] = 'E';
	m_rspBuf[1] = 'R';
	m_rspBuf[2] = 0x00;
	m_rspBuf[3] = 0x05;
	m_rspBuf[4] = err;
	crc = Cal_CRC(m_rspBuf, 5);	
	m_rspBuf[5] = (UINT8)(crc>>8);
	m_rspBuf[6] = (UINT8)(crc&0xff);	
	Send_Pack();

	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);
}

void CSerialProtocol::Rsp_ER()
{
	m_rspBuf[0] = 'E';
	m_rspBuf[1] = 'R';
	m_rspBuf[3] = 0;
	Send_Pack();

	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);
}

void CSerialProtocol::Rsp_WT()
{
	UINT16 crc;
	
	m_rspBuf[0] = 'W';
	m_rspBuf[1] = 'T';
	m_rspBuf[2] = 0x00;
	m_rspBuf[3] = RSPPACK_HEAD_LENGTH;
	crc = Cal_CRC(m_rspBuf, 4);	
	m_rspBuf[4] = (UINT8)(crc>>8);
	m_rspBuf[5] = (UINT8)(crc&0xff);	
	Send_Pack();
	
	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);
}

void CSerialProtocol::Rsp_NP()
{
	UINT16 crc;
	
	m_rspBuf[0] = 'N';
	m_rspBuf[1] = 'P';
	m_rspBuf[2] = 0x00;
	m_rspBuf[3] = RSPPACK_HEAD_LENGTH;
	crc = Cal_CRC(m_rspBuf, 4);	
	m_rspBuf[4] = (UINT8)(crc>>8);
	m_rspBuf[5] = (UINT8)(crc&0xff);
	Send_Pack();

	m_fill_count = 0;
	memset(m_rspCmd, 0, sizeof(rev_cmdStruct));
	memset(m_rspBuf, 0, SERIAL_MAX_BUFFER);
}

UINT16 CSerialProtocol::Cal_CRC(UINT8 *ptr, UINT8 len)
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

INT8 CSerialProtocol::FillParament(UINT8 byte)
{
	if (m_fill_count + RSPPACK_HEAD_LENGTH + 1 > SERIAL_MAX_BUFFER)
		return FAILURE;

	m_rspBuf[RSPPACK_HEAD_LENGTH+m_fill_count] = byte;
	m_fill_count++;
	m_rspCmd->Len ++;

	return SUCCESS;
}

INT8 CSerialProtocol::FillParament(INT64 x, UINT8 len, bool order)
{
	INT8 i;
	INT8 *p=(INT8 *)&x;

	if (m_fill_count + RSPPACK_HEAD_LENGTH +len > SERIAL_MAX_BUFFER)
		return FAILURE;
	
	if (len!=2 && len!=4 && len!=8)
		return FAILURE;

	for (i=0; i<len; i++)
	{
		if (order == 1)
			*(m_rspBuf+RSPPACK_HEAD_LENGTH+m_fill_count+i) = *(p+i);
		else
			*(m_rspBuf+RSPPACK_HEAD_LENGTH+m_fill_count+len-i-1) = *(p+i);
	}

	m_fill_count += len;
	m_rspCmd->Len += len;

	return SUCCESS;
}

INT8 CSerialProtocol::FillParament(UINT8 *buf, UINT8 len)
{
	if (m_fill_count + RSPPACK_HEAD_LENGTH +len > SERIAL_MAX_BUFFER)
		return FAILURE;

	memcpy ( m_rspBuf+RSPPACK_HEAD_LENGTH+m_fill_count, buf, len );
	m_fill_count += len;
	m_rspCmd->Len += len;

	return SUCCESS;
}

INT8 CSerialProtocol::FillParament(string str, UINT8 len)
{
	if (m_fill_count + RSPPACK_HEAD_LENGTH +len > SERIAL_MAX_BUFFER)
		return FAILURE;

	memset ( m_rspBuf+RSPPACK_HEAD_LENGTH+m_fill_count, 0, len );
	strcpy ( (char *)m_rspBuf+RSPPACK_HEAD_LENGTH+m_fill_count, str.c_str() );
	m_fill_count += len;
	m_rspCmd->Len += len;

	return SUCCESS;
}

UINT8 CSerialProtocol::GetParament_BYTE()
{
	UINT8 byte;
//	if (m_get_count > m_revCmd->Len)
//		return FAILURE;

	byte = m_revBuf[REVPACK_HEAD_LENGTH+m_get_count];
	m_get_count++;
	return byte;
}

INT64 CSerialProtocol::GetParament_INT(UINT8 len, bool order)
{
	INT64 x=0;
	UINT8 *p;
	
	p = (UINT8 *)(&x);
	for(int i=0; i<len; i++)
	{
		if (order == 1)
			*(p+i) = *(m_revBuf+REVPACK_HEAD_LENGTH+m_get_count+i);
		else
			*(p+i) = *(m_revBuf+REVPACK_HEAD_LENGTH+m_get_count+len-i-1);
	}

	m_get_count += len;
	return x;
}

INT64 CSerialProtocol::GetParament_BCD(UINT8 len)
{
	INT64 res;
	res = bin2int(m_revBuf+REVPACK_HEAD_LENGTH+m_get_count, len);
	m_get_count += len;
	return res;
}

void CSerialProtocol::GetParament_BUF(UINT8 len, UINT8 *buf)
{
	memcpy ( buf, m_revBuf+REVPACK_HEAD_LENGTH+m_get_count, len );
	m_get_count += len;

	return ;
}

void CSerialProtocol::getErrMsg(string &errStr, UINT8 errNo)
{
	int i;
	for(i=0;i<SERIAL_ERRNUM_MAX;i++)
	{
		if (m_errMsg[i].errNo == errNo)
		{
			errStr = m_errMsg[i].errMsg;
			return;
		}

	}	
	errStr = "";
	return;
}

bool CSerialProtocol::InitPort (int portnr, int baud)
{
	int res;
	char name[20];
	switch (portnr)
	{
	case UART0:
		strcpy(name, UART0_DEV_NAME);
		break;
	case UART1:
		strcpy(name, UART1_DEV_NAME);
		break;
	case UART2:
		strcpy(name, UART2_DEV_NAME);
		break;
	case UART3:
		strcpy(name, UART3_DEV_NAME);
		break;
	}
	res = m_Serial->InitPort(m_Serial, name, portnr, baud);
	return (res != FAILURE) ? SUCCESS : FAILURE;
}

INT32 CSerialProtocol::GetReceiveCount()
{
	return m_Serial->GetReceiveCount(m_Serial);
}

UINT8 CSerialProtocol::GetChar()
{
	return m_Serial->GetChar(m_Serial);
}

void CSerialProtocol::PutChar(UINT8 *byte)
{
	m_Serial->PutChar(m_Serial, byte);
}

bool CSerialProtocol::ReleasePort ()
{
	int res;
	res = m_Serial->ClosePort(m_Serial);
	return (res != FAILURE) ? SUCCESS : FAILURE;
}

void CSerialProtocol::ser_delay(UINT32 Msecs)
{
	UINT32  i;

   	for(; Msecs>0; Msecs--) 
      	for(i=0; i<2500; i++);
    return;
}

#endif	//USE_SERIAL_PROTOCOL
