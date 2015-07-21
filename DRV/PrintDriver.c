
/*! \file   PrintDriver.c
   \brief   打印驱动
   \author  Yin Chuntian
 */

#include     <string.h>
#include     <stdio.h>
#include     <stdarg.h>

#ifndef WIN32
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <sys/ioctl.h>
#include     <fcntl.h>      /*文件控制*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#endif

#include "SerialPort.h"
#include "PrintDriver.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


int fd_uart1;

static void SendCmd(int CmdLen, ...)
{
#ifndef WIN32
	UINT8 i;
	UINT8 byte;
	va_list    prt;

    va_start(prt, CmdLen);

	for(i=1; i<=CmdLen; i++) 
	{
		byte = va_arg(prt, int);
		
#if PRINTER_INTERRUPT
		ser_send_char(2, byte); 
#else
//		SC_UART1_SendByte(byte);
		write(fd_uart1, &byte, 1);
#endif

	}

	va_end( prt );
#endif	//WIN32

	return;
}

void  msdelay(UINT32 Msecs)
{
	UINT32  i;

   	for(; Msecs>0; Msecs--) 
      	for(i=0; i<2500; i++);
    return;
}


void OpenPrinterDEV()
{
#ifndef WIN32
	printf("uart1 opened start\n");
	if ((fd_uart1 = open("/dev/uart1", O_RDWR)) == -1)
	{
		printf("open uart1 err\n");
		exit(-1);
	}
	else
		printf("uart1 opened\n");
#endif
	return;
}


void PrinterIni(UINT8 bidirectionalPrint)
{
#ifndef WIN32
	
#if SIDA_PRINTER 
	SetBidirectionalPara(bidirectionalPrint);
#endif

	SendCmd(2, 0x1B, 0x40);

#endif //WIN32

	return;
}

void SetChineseMode(void)
{
#if SIDA_PRINTER
	return;
#endif
	SendCmd(2, 0x1C, 0x26);
}

void SetAsciiMode(void)
{
#if SIDA_PRINTER
	return;
#endif
	SendCmd(2, 0x1C,0x2E);
	
}

void ForwardNPoint(UINT16 N)
{
	if (N == 0)
	{
		return;
	}
	
	while (N > 255)
	{	
		SendCmd(3, 0x1B, 0x4A, 255);
		N -= 255;
	}
	
	SendCmd(3, 0x1B, 0x4A, N);
}


void ForwardNLine(UINT8 N)
{
	UINT8 i;
	
	for(i=0; i<N; i++)
	{
		CarriageReturn();
	}
}


void SetDoubleChineseMode(void)
{
	SendCmd(2, 0x1C, 0x0E); 
}


void CancelDoubleChineseMode(void)
{
	SendCmd(2, 0x1C, 0x14);
}


void SetLineSpace(UINT8 N)
{
	SendCmd(3, 0x1B, 0x31, N);
}


void CarriageReturn(void)
{
	SendCmd(1, 0x0D);
}


void LineFormard(void)
{
	SendCmd(1, 0x0A);
}


void SeekForntBorderBlackMark(void)
{
	SendCmd(1, 0x16);
}

void SeekBackBorderBlackMark(void)
{
	SendCmd(1, 0x17);
}


void SetHT(void)
{
	SendCmd(2, 0x1B,0x44 );
}


void ExecuteHT(void)
{
	SendCmd(1,0x09);
}


void PrintChineseLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32	
	
	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}
#if (XUNPU_PRINTER == 0)
	while ((GetPrinterStatus() & 0x20) == 0x20)
	{
		prn_log("printer is busy.");
	}
#endif

	SetChineseMode();
	for(i=0; i<BuffLen; i++)
	{
		if (*LineBuff == 0x0)
		{
			break;
		}
		
#if PRINTER_INTERRUPT
		ser_send_char(2, (*LineBuff++)); 
#else
//		SC_UART1_SendByte((*LineBuff++));
		write(fd_uart1, LineBuff+i, 1);
#endif

	}
	
	CarriageReturn();

#endif	//WIN32
	
	return;
}


void PrintDoubleChineseLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32
	
	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}

#if XUNPU_PRINTER == 0	
	while ((GetPrinterStatus() & 0x20) == 0x20)
	{
		prn_log("printer is busy.");
	}
#endif

	SetChineseMode();
	SetDoubleChineseMode();
	for(i=0; i<BuffLen; i++)
	{
		if (*LineBuff == 0x0)
		{
			break;
		}
		
#if PRINTER_INTERRUPT
		ser_send_char(2, (*LineBuff++)); 
#else
//		SC_UART1_SendByte((*LineBuff++));
		write(fd_uart1, LineBuff+i, 1);
#endif

	}
	CancelDoubleChineseMode();
	CarriageReturn();

#endif	//WIN32

	return;
}



void PrintAsciiLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32
	
	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}
#if XUNPU_PRINTER == 0
	while ((GetPrinterStatus() & 0x20) == 0x20)
	{
		prn_log("printer is busy.");
	}
#endif
		
	SetAsciiMode();
	for(i=0; i<BuffLen; i++)
	{
		if (*LineBuff == 0x0)
		{
			break;
		}
		
#if PRINTER_INTERRUPT
		ser_send_char(2, (*LineBuff++)); 
#else
//		SC_UART1_SendByte((*LineBuff++));
		write(fd_uart1, LineBuff+i, 1);
#endif

	}
	
	CarriageReturn();
	
#endif	//WIN32
	return;
}


void PrintReportLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32

	UINT8 i;

#if XUNPU_PRINTER == 0	
	while ((GetPrinterStatus() & 0x20) == 0x20)
	{
		prn_log("printer is busy.");
	}
#endif
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}
	
	i = BuffLen;
	do
	{
		if (LineBuff[--i] == ' ')
		{
			LineBuff[i] = 0x0;
		}
		else
		{
			break;
		}
		
	}
	while (i != 0);
		
	SetChineseMode();
	for(i=0; i<BuffLen; i++)
	{
		if (*LineBuff == 0x0)
		{
			break;
		}
		
#if PRINTER_INTERRUPT
		ser_send_char(2, (*LineBuff++)); 
#else
//		SC_UART1_SendByte((*LineBuff++));
		write(fd_uart1, LineBuff+i, 1);
#endif

	}
	CarriageReturn();

#endif	//WIN32
	return;
}

void PrintReportLine(INT8 *LineBuff, UINT8 BuffLen);


void PrintSeparateLine(void)
{
	PrintReportLine("----------------------------------------", 40); 
}


#if SIDA_PRINTER == 1

void BackwardNPoint(UINT16 N)
{
	if (N == 0)
	{
		return;
	}
	
	while (N > 255)
	{	
		SendCmd(3, 0x1B, 0x49, 255);
		N -= 255;
	}
	
	SendCmd(3, 0x1B, 0x49, N);
}


void SetWordSpace(UINT8 N)
{
	SendCmd(3, 0x1B,0x32, N);
}


void SetBidirectionalPara(UINT8 N)
{
	SendCmd(3, 0x1B,0x33, N);
}


UINT8 GetPrinterStatus(void)
{
	UINT8 status = 0;
	UINT32 timeOut = 0, st=0;

#ifndef WIN32
	UINT8 tmp;

#if PRINTER_INTERRUPT	
	
	while (ser_can_read(2))
	{
		ser_get_char(2);
	}

	SendCmd(2, 0x1B,0x23);
	while (ser_can_read(2) == 0);	
	
	status = ser_get_char(2);

#else

//	while ((SC_UART1_LSR&0x01)==1);
	
//	SC_UART1_SendByte(0x1b);
//	SC_UART1_SendByte(0x23);
	tmp = 0x1b;
	write(fd_uart1, &tmp, 1);
	tmp = 0x23;
	write(fd_uart1, &tmp, 1);
	
	while(!st && timeOut < 5000000)
	{
//		printf("st=%x\n", st);
		ioctl(fd_uart1, UART1_CAN_READ, &st);
		timeOut++;
	}
	
	if (timeOut == 5000000)
	{
		status = 0x01; //缺纸、缓冲区不满，这样不影响下面的执行
	//	Beep_On();
	}
	else
	{
		read(fd_uart1, &status, 1);
//		status = SC_UART1_RBR;
	}
	
#endif

#endif	//WIN32

	return status;
}


UINT8 GetUsedLines(void)
{
	UINT8 status = 0;
	INT32 st=0;

#ifndef WIN32
	UINT8 tmp;

#if PRINTER_INTERRUPT	

	while (ser_can_read(2))
	{
		ser_get_char(2);
	}
	
	SendCmd(2, 0x1B,0x21);
	while (ser_can_read(2) == 0);
	
	status = ser_get_char(2);
	
#else

//	while ((SC_UART1_LSR&0x01)==1);
	
//	SC_UART1_SendByte(0x1b);
//      SC_UART1_SendByte(0x21);
	tmp = 0x1b;
	write(fd_uart1, &tmp, 1);
	tmp = 0x21;
	write(fd_uart1, &tmp, 1);	
	
	while(!st)
	{
//		prn_log("No data");
		ioctl(fd_uart1, UART1_CAN_READ, &st);
	}
/*
	while(!(SC_UART1_LSR & 0x01))
	{
		prn_log("No data");
	}
*/	
	read(fd_uart1, &status, 1);
//	status = SC_UART1_RBR;
	
#endif

#endif	//WIN32

	return status;
}


UINT16 GetUsedBytes(void)
{	
	UINT8 num = 0;
	UINT16 bufUsedSize=0;
	INT32 st=0;

#ifndef WIN32
	UINT8 ch[10];
	UINT8 tmp;
	
	memset(ch, 0x00, sizeof(ch));
	
#if PRINTER_INTERRUPT

	while (ser_can_read(2))
	{
		ser_get_char(2);
	}
	
	SendCmd(2, 0x1B,0x24);
	
	while (num < 2)
	{
		if (ser_can_read(2) != 0)
		{
			*(ch+num) = ser_get_char(2);
			num++;
		}
	}
	
	prn_log2("receive %d data.", num);
	bufUsedSize = (UINT16)hex2int(ch, 2);
	
#else
	
	tmp = 0x1b;
	write(fd_uart1, &tmp, 1);
	tmp = 0x24;
	write(fd_uart1, &tmp, 1);	
	msdelay(200);
	
	while(num < 2)
	{
/*		while (SC_UART1_LSR & 0x01)
		{
			*(ch+num) = SC_UART1_RBR;
			num++;
		}
*/
		ioctl(fd_uart1, UART1_CAN_READ, &st);
		while(st)
		{
			st = read(fd_uart1, ch+num, 1);
			//printf("read num=%d\n", st);
			//printf("ch[0]=%x\n", ch[0]);
			num++;
			ioctl(fd_uart1, UART1_CAN_READ, &st);
			//printf("st = %d\n", st);
		}
	}
	
//	prn_log2("receive %d data.", num);
	//printf("ch[0]=%x\n", ch[0]);
	//printf("ch[1]=%x\n", ch[1]);
	bufUsedSize = (UINT16)hex2int(ch, 2);
	//printf("bufUsedSize=%d\n", bufUsedSize);
#endif

#endif //WIN32


	return bufUsedSize;
}
#endif	//SIDA_PRINTER == 1

void Code128Return(int n, int cmd)
{
	SendCmd(n, cmd);
}

void CodeTwo(int n, int cmd1, int cmd2)
{
	SendCmd(n, cmd1,cmd2);
}


int PrintBiznfo(unsigned char *input1,int VerticalNum,int HorizontalNum, int leftMargin)
{
	int i,m,k,w,num=0;
	int n=0;
	long time = 144000;
	unsigned int print_times,line_high,printwild;
	ForwardNPoint(20);
	
	SendCmd(1,0x1B);//强制不复位
	SendCmd(1,0x2E);
	for(m=0;m<VerticalNum/16;m++)			 //32
	{
		for(k=0;k<2;k++)
		{
			SendCmd(1,0x1B);								
			SendCmd(1,0x2A);			   
			SendCmd(1,0x01);
			SendCmd(1,leftMargin);
			SendCmd(1,0x00); 
			for (i=0; i<leftMargin;i++)
				SendCmd(1,0x00);


			SendCmd(1,0x1B);
			SendCmd(1,0x2A);
			SendCmd(1,0x01);
			SendCmd(1,HorizontalNum);
			SendCmd(1,0x00);

			for(i=0;i<HorizontalNum;i++)
			{
				SendCmd(1,input1[i+n]) ;
			} 
			for (time = 1000000;time>0;time--);//延时

			SendCmd(1,0x1B);
			SendCmd(1,0x4A);
			SendCmd(1,0x01);			
			n+=HorizontalNum;
		}

		SendCmd(1,0x1B);		//初始化打印机
		SendCmd(1,0x40);
		SendCmd(1,0x1B);
		SendCmd(1,0x4A);
		SendCmd(1,0x0e);
	}

	SendCmd(1,0x1B);//恢复复位
	SendCmd(1,0x2F);
	return 1;
}

int PrintBiznfo2(unsigned char *input1,unsigned char *input2,int VerticalNum,int HorizontalNum,int index)
{
	int i,m,k,w,num=0;
	int n=0;
	long time = 144000;
	unsigned int print_times,line_high,printwild;
	char tmpchar[52];			
	ForwardNPoint(20);
	memset(tmpchar,0,sizeof(tmpchar));
// 	sprintf(tmpchar,"二维码打印第%d组",index);
// 	PrintChineseLine(tmpchar,strlen(tmpchar));
	
	SendCmd(1,0x1B);//强制不复位
	SendCmd(1,0x2E);
	for(m=0;m<VerticalNum/16;m++)			 //32
	{
		
		for(k=0;k<2;k++)
		{					
			SendCmd(1,0x1B);								
			SendCmd(1,0x2A);			   
			SendCmd(1,0x01);			   
			SendCmd(1,HorizontalNum);		
			SendCmd(1,0x00);
			
			for(i=0;i<HorizontalNum;i++)
				
			{	
				
				SendCmd(1,input1[i+n]) ;
				/*				for (time = 14400;time>0;time--);//延时*/
				
			} 
			for (time = 1000000;time>0;time--);//延时
			
			//SendCmd(1,0x20);
			SendCmd(1,0x1B);								
			SendCmd(1,0x2A);			   
			SendCmd(1,0x01);
			SendCmd(1,0x0E);
			SendCmd(1,0x00); 
			for (i=0; i<0x0E;i++)
				SendCmd(1,0x00);	
			
			SendCmd(1,0x1B);								
			SendCmd(1,0x2A);			   
			SendCmd(1,0x01);			   
			SendCmd(1,HorizontalNum);
			SendCmd(1,0x00);
			
			
			for(i=0;i<HorizontalNum;i++)
				
			{		
				SendCmd(1,input2[i+n]);
				/*				for (time = 14400;time>0;time--);*/
			} 
			for (time = 1000000;time>0;time--);//延时
			
			
			SendCmd(1,0x1B);
			SendCmd(1,0x4A);
			SendCmd(1,0x01);			
			n+=HorizontalNum;
			
			
		}
		// 		time = 1440000;
		// 		for (;time>0;time--)//延时
		// 		{
		// 		}
		
		SendCmd(1,0x1B);		//初始化打印机
		SendCmd(1,0x40);
		SendCmd(1,0x1B);
		SendCmd(1,0x4A);
		SendCmd(1,0x0e);
	}
	
	SendCmd(1,0x1B);//恢复复位								
	SendCmd(1,0x2F);
	return 1;
}


