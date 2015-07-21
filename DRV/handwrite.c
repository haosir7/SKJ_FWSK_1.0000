/*! \file    handwrite.c
   \brief    手写板驱动
   \author   
 */

#include <string.h>
#include "SerialPort.h"
#include "handwrite.h"
#include "DRVModuleHeader.h"

//#define	HW_DEBUG	

#if( USE_HW_DRV !=0)

#define HW_STATUS_OK					1
#define HW_WAITING						0
#define HW_COMMUNICATION_ERR			-1
#define HW_OVERTIME						-2
#define HW_PACK_ERR						-3
#define HW_RSP_PARA_ERR					-4


unsigned char sendBuf[8];

enum RESOLVE_STATUS revStatus;
struct RevCmdData RevData;

unsigned char revParaLen=0;
static int HwPort = 0;

static SerialPort hw_dev;

unsigned char CRCTable[]  =  {     
0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,   
0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,   
0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,   
0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,   
0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,   
0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,   
0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,   
0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,   
0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,   
0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,   
0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,   
0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,   
0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,   
0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,   
0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,   
0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3   
};   
   
unsigned char GenerateCRC(unsigned char *Packet, int size)
{
	int i;
	unsigned char value;
	
	value = 0;
	
	for(i=0; i<size-1; i++)
	{
		value ^= Packet[i];
		value = CRCTable[value];
	}
	
	return value;
}

void  HW_Delay(unsigned int dly)
{  
	int  i;

   	for(; dly>0; dly--) 
    	for(i=0; i<50000; i++);
}


int get_rev_count(int HwPort)
{
	return hw_dev.GetReceiveCount(&hw_dev);
}


unsigned char get_char(int HwPort)
{
	return hw_dev.GetChar(&hw_dev);
}

 
void put_char(int HwPort, unsigned char byte)
{
	hw_dev.PutChar(&hw_dev, &byte);
}


void HwWriteCMD(int HwPort, unsigned char CmdNo, unsigned char* buf)
{
	int i;
	
	while (get_rev_count(HwPort))
	{
		get_char(HwPort);
	}
   	
   	HW_Delay(10);
   	
   	memset (sendBuf, 0, sizeof(sendBuf));
   	*(sendBuf+0) = 'P';
   	*(sendBuf+1) = CmdNo;
   	*(sendBuf+2) = 4;
   	memcpy(sendBuf+3, buf, 4);
   	*(sendBuf+7) = GenerateCRC(sendBuf, sizeof(sendBuf));
   	
	for(i=0; i<8; i++)
	{
		put_char(HwPort, sendBuf[i]);
	}
	HW_Delay(30);
	
	return ;

}


int HwParseCMD(int HwPort, struct RevCmdData *p_rData)
{
	unsigned char data; 
	unsigned char crc_res, bufLen;
	unsigned char tmpbuf[30];
	int i;
	
	bufLen = get_rev_count(HwPort);
	if (bufLen == 0)
	{
		return HW_WAITING;
	}
#ifdef HW_DEBUG
	printf("bufLen = %d\n", bufLen);
#endif	
	
	for(i=0; i<bufLen; i++)
	{
		data = get_char(HwPort);
#ifdef HW_DEBUG
	printf("%x\n", data);
#endif	
		//解析
		switch(revStatus)
		{			
			case STATE_RESET:
				if (data == 'P')
				{
					memset ((void *)p_rData, 0, sizeof(struct RevCmdData));
					revStatus = STATE_PACKHEAD;	
				}
				break;
				
			case STATE_PACKHEAD:		
				p_rData->CmdNo = data;
				revStatus = STATE_CMDNO;
				break;

			case STATE_CMDNO:
				p_rData->CmdParaLen = data;
				revStatus = STATE_LENGTH;
				revParaLen = 0;
				break;
				
			case STATE_LENGTH:
				p_rData->CmdPara[revParaLen] = data;
				revParaLen++;
				if (revParaLen == p_rData->CmdParaLen)
				{
					revStatus = STATE_PARA_END;
				}
				break;
				
			case STATE_PARA_END:
				p_rData->CmdCRC = data;
				
				memset(tmpbuf, 0, sizeof(tmpbuf));
				tmpbuf[0] = 'P';
				tmpbuf[1] = p_rData->CmdNo;
				tmpbuf[2] = p_rData->CmdParaLen;
				memcpy(&(tmpbuf[3]), p_rData->CmdPara, p_rData->CmdParaLen);
				
				crc_res = GenerateCRC( tmpbuf, 4 + p_rData->CmdParaLen );
				if (crc_res == p_rData->CmdCRC)
				{
					revStatus = STATE_CRC_OK;
				}
				else
				{
					revStatus = STATE_RESET;
					return HW_COMMUNICATION_ERR;
				}
				
				break;
			
			default:
				revStatus = STATE_RESET;
				return HW_COMMUNICATION_ERR;
			
		}	
	
		//接受命令正确。
		if (revStatus == STATE_CRC_OK)
		{
			revStatus = STATE_RESET;
			return HW_STATUS_OK;
		}
	}
	
	return HW_WAITING;
}


int HwRevCMD(struct RevCmdData *p_rData)
{
	int res, i=0;
	unsigned char tmpbuf[4];
	
	while ( (res = HwParseCMD(HwPort, p_rData)) == HW_WAITING ) 
	{
		i++;
		if (i<MAX_WAIT_TIME)
			return HW_OVERTIME;
	}

	memset (tmpbuf,0,4);
	if ( p_rData->CmdNo == 0x0 && (memcmp(p_rData->CmdPara, tmpbuf, 4)==0) )
		return HW_PACK_ERR;
	
	return res;	
}



int HwReset(void)
{
	unsigned char parabuf[4];
	int res;
	
	memset(parabuf, 0xff, 4);
	HwWriteCMD(HwPort, 0x42, parabuf);
	
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	memset(parabuf, 0xff, 4);
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
	
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	memset(parabuf, 0x00, 4);
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
		
	return HW_STATUS_OK;
}

 
int HwMode(unsigned char *parabuf)
{
	int res;
	
	HwWriteCMD(HwPort, 0x49, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
	
	return HW_STATUS_OK;
}


int HwResCode(unsigned char *parabuf)
{
	int res;
	
	HwWriteCMD(HwPort, 0x41, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
	
	return HW_STATUS_OK;
}

 
int HwHZType(unsigned char *parabuf)
{
	int res;
	
	HwWriteCMD(HwPort, 0x10, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
	
	return HW_STATUS_OK;
}


int HwBorder(unsigned char *parabuf)
{
	int res;
	
	HwWriteCMD(HwPort, 0x46, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;
	
	return HW_STATUS_OK;
}


int HwLinker(unsigned char *parabuf)
{
	int res;

	HwWriteCMD(HwPort, 0x14, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;

	return HW_STATUS_OK;
}


int HwWaitTime(unsigned char *parabuf)
{
	int res;

	HwWriteCMD(HwPort, 0x1A, parabuf);
	
	memset(parabuf, 0xff, 4);
	res = HwRevCMD(&RevData);
	if (res != 1)
		return res;
	if ( memcmp(RevData.CmdPara, parabuf, 4) != 0 )
		return HW_RSP_PARA_ERR;

	return HW_STATUS_OK;
}

 
int HwSetParament(void)
{
	unsigned char parabuf[4];	

/*	
	//设置可识别全体类型字符
	memset(parabuf, 0xff, 4);
	parabuf[1] = 0x01;
	if ( HwHZType(parabuf) != 1)
		return HW_FAILURE;	
*/	

	//设置不发送笔迹
	memset(parabuf, 0xff, 4);
	parabuf[0] = 0x00;
	if ( HwLinker(parabuf) != 1)
		return HW_FAILURE;	
	
	//设置识别停笔等待时间为400ms
	memset(parabuf, 0xff, 4);
	parabuf[0] = 0x02;
	if ( HwWaitTime(parabuf) != 1)
		return HW_FAILURE;	

	
	//设置手写边界为10,10,f0,f0
	parabuf[0] = 0x10;
//	parabuf[0] = 0x2c;
	parabuf[1] = 0x10;
	parabuf[2] = 0xf0;
	parabuf[3] = 0xf0;
	if ( HwBorder(parabuf) != 1)
		return HW_FAILURE;	
	
	return HW_SUCCESS;
}


int HwInit(void)
{

	int res;
	revStatus = STATE_RESET;
	memset ((void *)(&RevData), 0, sizeof(RevData));
	
	SerialPortContruct(&hw_dev);
	res = hw_dev.InitPort(&hw_dev, UART0_DEV_NAME, UART0, 9600);
	if (res != HW_SUCCESS)
		return res;

	res = HwSetParament();
	return res;
}


int HwRelease(void)
{
	return hw_dev.ClosePort(&hw_dev);
}


int HwRevHANZI(unsigned char *hz_buf, unsigned char *num)
{
#ifndef WIN32
	int res, i;
	unsigned char temp;
	
	while (1)
	{
		res = HwRevCMD(&RevData);
		if (res < 0)
			return HW_FAILURE;
		else if (res == 1)
		{
#ifdef HW_DEBUG
	printf("CmdNo = %x\n", RevData.CmdNo);	
#endif
			if( RevData.CmdNo == 0x18 )
			{
				break;
			}
			else if ( RevData.CmdNo == 0x17 )
			{
#ifdef HW_DEBUG
	printf("CmdPara[0]=%x, CmdPara[1]=%x\n", RevData.CmdPara[0],RevData.CmdPara[1]);	
#endif
				if (RevData.CmdPara[0]!=0xff || RevData.CmdPara[1]!=0xff)
				{
					break;
				}
			}
			else if ( RevData.CmdNo == 0x42 && 
					  RevData.CmdPara[0]==0x0 && RevData.CmdPara[1]==0x0 &&
					  RevData.CmdPara[2]==0x0 && RevData.CmdPara[3]==0x0
					  )
			{
				HwSetParament();
				continue;
			}	
		}
	}
	if (RevData.CmdNo == 0x18)
	{
		*num = RevData.CmdPara[0];
		for (i=0; i<(RevData.CmdPara[0]*2); i=i+2)
		{
			temp = RevData.CmdPara[1+i];
			RevData.CmdPara[1+i] = RevData.CmdPara[1+i+1];
			RevData.CmdPara[1+i+1] = temp;
		}
		memcpy( hz_buf, &(RevData.CmdPara[1]), RevData.CmdPara[0]*2 );
	}
	else if (RevData.CmdNo == 0x17)
	{
		*num = 0xff;
		strcpy( (void *)hz_buf, "select0" );
	}
	
	return HW_SUCCESS;

#else
	static unsigned __int64 i=0;
	while(1)
	{
		if (i%0x20000 == 0)
		{
			if (i%0x40000==0)
				strcpy(hz_buf, "一二三四五六奇八九");
			else
				strcpy(hz_buf, "我爱北京天安门下的人");
			*num = 10;
			i++;
			return HW_SUCCESS;
		}
		else
		{
			i++;
			return HW_FAILURE;
		}
	}
#endif
		
}


int ScreenHwInit(void)
{
	int res;
	revStatus = STATE_RESET;
	memset ((void *)(&RevData), 0, sizeof(RevData));
	
	SerialPortContruct(&hw_dev);
	res = hw_dev.InitPort(&hw_dev, UART0_DEV_NAME, UART0, 9600);
	
	return res;
}

 
int HwDataAdjust(unsigned char *chHWbuf, int nHwBufLen)
{
	int bufLen;
	unsigned char i;
	unsigned char buffer[64];
	unsigned char sendData[2];
	int ret;
	int count = 0;
	int tmpbufLen;
	int outsize = 0;
	memset((void*)sendData, 0, sizeof(sendData));

	bufLen = get_rev_count(UART0);//串口数据字节数
#ifdef HW_DEBUG
	printf("bufLen = %d\n", bufLen);
#endif
	if (bufLen > nHwBufLen-1)//超过63则截断
	{
		bufLen = nHwBufLen-1;
	}
	else if( bufLen <= 0 )
		return 0;
	
	//获取串口字节
//	memset((void *)chHWbuf, 0, sizeof(chHWbuf));
	for(i=0; i<bufLen; i++)
	{
		chHWbuf[count++] = get_char(UART0);
#ifdef HW_DEBUG
		printf("chHWbuf[%d] = %x\n",i, chHWbuf[i]);
#endif
	}

	if( bufLen<4 || (bufLen < chHWbuf[2]+4) )		//读取到的数据不够
	{
		while( bufLen < chHWbuf[2]+4 )
		{
			if ((chHWbuf[1] != 0x91) || (chHWbuf[2] == 0))
			{
				return 0;
			}

			tmpbufLen = get_rev_count(UART0);//串口数据字节数
#ifdef HW_DEBUG
			printf("serial size tmpbufLen = %d\n", tmpbufLen);
#endif
			bufLen += tmpbufLen;
			if (bufLen > nHwBufLen-1)//超过63则截断
			{
				outsize = 1;
				bufLen = nHwBufLen-1;
			}
			
			for(i=count; i<bufLen; i++)
			{
				chHWbuf[count++] = get_char(UART0);
#ifdef HW_DEBUG
				printf("chHWbuf[%d] = %x\n",i, chHWbuf[i]);
#endif
			}

			if( outsize == 1 )
				break;
		}
	}
#ifdef HW_DEBUG
	printf("接收到的数据:%s\n", chHWbuf);
#endif
	
	//获取串口多余字节，即清空串口缓存
	while (get_rev_count(UART0))
	{
		get_char(UART0);
	}
	
	//检查串口数据合法性		
#if SCREEN_HW_TYPE == 0
	if ((chHWbuf[0] != 0x1B) || (chHWbuf[1] == 0))//数据非法
#else
	if ((chHWbuf[1] != 0x91) || (chHWbuf[2] == 0))//数据非法
#endif
	{
		sendData[0] = 0x22;
		sendData[1] = 0xDD;
		ret = 0;
#ifdef HW_DEBUG
		printf("数据非法\n");
#endif
	}
	else //数据合法
	{
		sendData[0] = 0x11;
		sendData[1] = 0xEE;
#if SCREEN_HW_TYPE == 0
 		chHWbuf[chHWbuf[1] + 2] = 0x0;
 		ret = chHWbuf[1];
#else
		chHWbuf[chHWbuf[2] + 3] = 0x0;
		ret = chHWbuf[2];
#endif
#ifdef HW_DEBUG
		printf("数据合法 ret = %d\n", ret);
#endif
	}

	for(i = 0; i<2; i++)
	{
		put_char(UART0, sendData[i]);//应答手写板数据
	}

	return ret;
}
#else

int HwInit(void)
{
	return 0;
}


int ScreenHwInit(void)
{
	return 0;
}

int HwRelease(void)
{
	return 0;
}


int HwRevHANZI(unsigned char *hz_buf, unsigned char *num)
{
	return 0;
}


int get_rev_count(int HwPort)
{
	return 0;
}


int HwDataAdjust(unsigned char *chHWbuf, int nHwBufLen)
{
	return 0;
}

#endif


