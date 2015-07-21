/*! \file   Scanner.c
   \brief   扫描枪驱动
   \author  
 */
#include <string.h>
#include "SerialPort.h"
#include "Scanner.h"
#include "VersionConfig.h"
#include "DRVModuleHeader.h"


static unsigned char Scanner_Buf[32];

#if (USE_SCANNER_DRV != 0 )

#define MAX_SCANNER_OVERTIME		300000
static SerialPort scanner_dev;
static unsigned char current = 0;
static unsigned char scanner_State = 0;		//扫描枪状态，0－未初始化；1－已初始化

int ScannerPressed_mode1(char length)
{
	char ch;
	unsigned char haveData;
	
	haveData = scanner_dev.GetReceiveCount(&scanner_dev);
	while (haveData)
	{
		ch = scanner_dev.GetChar(&scanner_dev);
		
		if ((ch >= '0') && (ch <= '9')
			|| (ch >= 'a') && (ch <= 'z')
			|| (ch >= 'A') && (ch <= 'Z'))
		{
		}
		else
		{
			//			ser_send_char(0, (ch & 0xf0)>>4 + 0x30);
			//			ser_send_char(0, (ch & 0x0f) + 0x30);
			haveData = scanner_dev.GetReceiveCount(&scanner_dev);
			continue;
		}
		
		//		ser_send_char(0, ch + 0x30);
		
		Scanner_Buf[current++] = ch;
		
		if (current >= length)
		{
			for(;current<32;current++)
			{
				Scanner_Buf[current] = 0;
			}
			
			current = 0;
			
			while (scanner_dev.GetReceiveCount(&scanner_dev))
			{
				ch = scanner_dev.GetChar(&scanner_dev);
			}
			
			//			ser_send_char(0, 0xa);
			//			ser_send_char(0, 0xd);
			return 1;
		}
		
		haveData = scanner_dev.GetReceiveCount(&scanner_dev);
	}

	return 0;
}


int ScannerPressed_mode2(char *str)
{
	char ch;
	unsigned char haveData, len;
	unsigned int overtime=0;
	
	memset(Scanner_Buf, 0, sizeof(Scanner_Buf));
	len = strlen(str);
	haveData = scanner_dev.GetReceiveCount(&scanner_dev);
	if (!haveData)
		return 0;
	
	while(overtime < MAX_SCANNER_OVERTIME)
	{
		if (haveData > 0)
		{
			ch = scanner_dev.GetChar(&scanner_dev);
			//			printf("ch = %x\n", ch);
			
			// 过虑掉包头字节
			if (current == 0)
			{
				if ((ch >= '0') && (ch <= '9')
					|| (ch >= 'a') && (ch <= 'z')
					|| (ch >= 'A') && (ch <= 'Z'))
				{;
				}
				else
				{
					haveData = scanner_dev.GetReceiveCount(&scanner_dev);
					continue;
				}
			}
			
			if (current < sizeof(Scanner_Buf)-1)
			{
				Scanner_Buf[current++] = ch;
			}
			else
			{
				overtime = MAX_SCANNER_OVERTIME+1;
				break;
			}
			
			//判断是否接收到包尾
			if ( current > len )
			{
				//确认收到了包尾的话，再清除掉BUF中的包尾字节，返回
				if( strcmp(Scanner_Buf+current-len, str) == 0)
				{
					*(Scanner_Buf+current-len) = '\0';
					break;
				}
			}
		}
		else
		{
			overtime++;
		}
		
		haveData = scanner_dev.GetReceiveCount(&scanner_dev);
	}
	
	current = 0;
	
	if (overtime >= MAX_SCANNER_OVERTIME)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


unsigned char *ScannerCode()
{
	return Scanner_Buf;
}


void ClearScannerBuf()
{
	unsigned char ch;
	
	while(scanner_dev.GetReceiveCount(&scanner_dev))
	{
		ch = scanner_dev.GetChar(&scanner_dev);
	}
	
	memset (Scanner_Buf, 0, sizeof(Scanner_Buf));
	current = 0;
}

int ScannerStateCheck(void)
{
	//printf("scanner_State = %d",scanner_State);
	return scanner_State;
}


int ScannerInit(void)
{
	int res;
	
	if (scanner_State == 1)	//扫描枪驱动已经初始化
		return 1;
	
	SerialPortContruct(&scanner_dev);
	
#if (POS_TYPE == POS_APE4020R)
	res = scanner_dev.InitPort(&scanner_dev, UART2_DEV_NAME, UART2, 9600);
#else
	res = scanner_dev.InitPort(&scanner_dev, UART3_DEV_NAME, UART3, 9600);
#endif

	if (res == 1)
		scanner_State = 1;
	
	return res;
}


int ScannerRelease(void)
{
	if (scanner_State == 1)	
	{
		scanner_State = 0;
		return scanner_dev.ClosePort(&scanner_dev);
	}
	return 0;
}

#else


int ScannerInit(void)
{
	
	return 0;
}

int ScannerPressed_mode1(char length)
{
	return 0;
}

int ScannerPressed_mode2(char *str)
{
	return 0;
}

int ScannerStateCheck(void)
{
	return 0;
}

unsigned char *ScannerCode()
{
	return Scanner_Buf;
}
	
#endif  //USE_SCANNER_DRV