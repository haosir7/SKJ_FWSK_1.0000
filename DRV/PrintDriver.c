
/*! \file   PrintDriver.c
   \brief   打印驱动
	\brief   打印机设置的最小单位为步长即半点间距,为1/144 英寸(0.176mm),针与针间的点距为2/144 英寸
	\brief   打印头走一行打印完整的一行
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
#include "VersionConfig.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_log.h"


int fd_uart1;

SerialPort Printerserial, *pPrinterSerial;

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

 
#if (POS_TYPE == POS_APE4000RG)

void OpenPrinterDEV()
{

#ifndef WIN32

	//printf("4000RG_uart1 opened start\n");
	INT32 res;

	pPrinterSerial = &Printerserial;
	SerialPortContruct(pPrinterSerial);
	
	res = pPrinterSerial->InitPort(pPrinterSerial, "/dev/uart1", 1, 57600);
	
	if (res == FAILURE) 
	{
		pPrinterSerial->ClosePort(pPrinterSerial);
		//printf("4000RG_open uart1 error\n");
		exit(-1);
	}
	else
	{
		//printf("4000RG_uart1 opened\n");
	}
	fd_uart1 = pPrinterSerial->m_fd;
	//printf("4000RG_uart1: InitPort Success %d\n", fd_uart1);

#endif
	return;
}

void PrinterIni(UINT8 bidirectionalPrint)
{
	//printf("\n*****************************4000RG_PrinterIni_Begin********************\n");

	return;
}

void YMPrinterIni(void)
{
#ifndef WIN32
	
	//printf("\n*****************************PrinterIni_Begin********************\n");

	SendCmd(2, 0x1B, 0x40);//设置初始化命令,该命令不会改变双向打印参数的设置,以及下面两项

	UINT8 str_Blackmarkset[]= {0x1b, 0x10, 0x23, 0x00, 0x07, 0x01, 0x56};//黑标检测设置为有效
	UINT8 str_Printerinitset[]= {0x1b, 0x10, 0x18, 0x00, 0x08, 0x00, 0x8c, 0xd7};//打印初始位置相对于黑标的偏移量,检测到黑标后,继续进纸25mm,约140步
	UINT8 str_Jinzhimodeset[]= {0x1b, 0x10, 0x2a, 0x00, 0x07, 0x00, 0x5c};//进纸模式设置,在缺纸下重新上纸打印机将自动卷纸到打印位,在黑标模式下,进纸时检测黑标

	write(fd_uart1, str_Blackmarkset, sizeof(str_Blackmarkset));
	write(fd_uart1, str_Printerinitset, sizeof(str_Printerinitset));
	write(fd_uart1, str_Jinzhimodeset, sizeof(str_Jinzhimodeset));

	//printf("\n*****************************PrinterIni_End********************\n");
#endif //WIN32

	return;
}

void SetChineseMode(void)
{
#if SIDA_PRINTER
	return;
#endif
	SendCmd(2, 0x1C, 0x26);
	SendCmd(3, 0x1C, 0x21, 0x00);//选择中文方式下的打印模式
}

void SetAsciiMode(void)
{
#if SIDA_PRINTER
	return;
#endif
	SendCmd(3, 0x1B,0x21, 0X01);
	
}

void ForwardNPoint(UINT16 N)
{
	//printf("\n*****************************ForwardNPoint N0 = %u********************\n",N);
	if (N == 0)
	{
		return;
	}
	SendCmd(3, 0x1B, 0x4A, N);
}


void ForwardNLine(UINT8 N)
{
	UINT8 i;
	
	//printf("\n*****************************ForwardNLine N0 = %u********************\n",N);
	SendCmd(3, 0x1B, 0x64, N);
}


void SetDoubleChineseMode(void)
{
	//printf("\n*****************************SetDoubleChineseMode********************\n");

	SendCmd(3, 0x1C, 0x21, 0x04);//选择中文方式下的打印模式
}


void CancelDoubleChineseMode(void)
{
	//printf("\n*****************************CancelDoubleChineseMode********************\n");

	SendCmd(3, 0x1C, 0x21, 0x00);//选择中文方式下的打印模式
}


void SetLineSpace(UINT8 N)
{
	//printf("\n*****************************SetLineSpace N0 = %u********************\n",N);

	if (N < 16)
	{
		N = N + 16;
	}
	//printf("\n*****************************SetLineSpace N1 = %u********************\n",N);
	SendCmd(3, 0x1B, 0x33, N);
}


void CarriageReturn(void)
{
	//printf("\n*****************************CarriageReturn********************\n");

	SendCmd(1, 0x0D);
}


void LineFormard(void)
{
	//printf("\n*****************************LineFormard********************\n");

	SendCmd(1, 0x0A);
}


void SeekForntBorderBlackMark(void)
{
	//printf("\n*****************************SeekForntBorderBlackMark********************\n");

	SendCmd(2, 0x1D, 0x0C);
}

void SeekBackBorderBlackMark(void)
{
	//printf("\n*****************************SeekBackBorderBlackMark********************\n");

	SendCmd(2, 0x1D, 0x0C);
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
	//printf("\n*****************************PrintChineseLine_Begin********************\n");

	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}
#if (XUNPU_PRINTER == 0)
	while ((GetPrinterStatus() & 0x80) == 0x80)
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
	LineFormard();

	//printf("\n*****************************PrintChineseLine_End********************\n");

#endif	//WIN32
	
	return;
}


void PrintDoubleChineseLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32
	//printf("\n*****************************PrintDoubleChineseLine_Begin********************\n");

	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}

#if XUNPU_PRINTER == 0	
	while ((GetPrinterStatus() & 0x80) == 0x80)
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
	LineFormard();

	//printf("\n*****************************PrintDoubleChineseLine_End********************\n");

#endif	//WIN32

	return;
}



void PrintAsciiLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32
	//printf("\n*****************************PrintAsciiLine_Begin********************\n");

	UINT8 i;
	
	if ((BuffLen == 0) || (LineBuff == NULL))
	{
		return;
	}
#if XUNPU_PRINTER == 0
	while ((GetPrinterStatus() & 0x80) == 0x80)
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
	LineFormard();

	//printf("\n*****************************PrintAsciiLine_End********************\n");

#endif	//WIN32
	return;
}


void PrintReportLine(INT8 *LineBuff, UINT8 BuffLen)
{
#ifndef WIN32
	//printf("\n*****************************PrintReportLine_Begin********************\n");

	UINT8 i;

#if XUNPU_PRINTER == 0	
	while ((GetPrinterStatus() & 0x80) == 0x80)
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
	while (i != 0);//在填充LineBuff数组的尾巴?
		
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
	LineFormard();

	//printf("\n*****************************PrintReportLine_End********************\n");

#endif	//WIN32
	return;
}

void PrintSeparateLine(void)
{
	PrintReportLine("----------------------------------------", 40); 
}


#if SIDA_PRINTER == 1

void BackwardNPoint(UINT16 N)
{
	//printf("\n*****************************BackwardNPoint N0 = %u********************\n",N);

	if (N == 0)
	{
		return;
	}
	
	while (N > 32)
	{	
		SendCmd(3, 0x1B, 0x4B, 0x20);
		N -= 32;
	}
	
	SendCmd(3, 0x1B, 0x4B, N);
}


void SetWordSpace(UINT8 N)
{
	//printf("\n*****************************SetWordSpace N0 = %u********************\n",N);

	if (N<=32)
	{
		SendCmd(3, 0x1B,0x20, N);
	}
}


void SetBidirectionalPara(UINT8 N)
{
#if SIDA_PRINTER 
	//printf("\n*****************************SetBidirectionalPara N0 = %u********************\n",N);
	if ((GetPrinterStatus() & 0x04) == 0x00)	//有打印纸时执行,否则阻塞串口
	{
		UINT8 checksum = 0;
		INT32 tempi = 0;
		if (N > 9)
		{
			return;
		}
		UINT8 str_BIDparaset[7]= {0x1b, 0x10, 0x03, 0x00, 0x07, N, 0x00};
		for (tempi = 0; tempi<6; tempi++)
		{
			checksum += str_BIDparaset[tempi];
		}
		str_BIDparaset[6]= checksum;
		//printf("str_BIDparaset[6] = %x",str_BIDparaset[6]);
		write(fd_uart1, str_BIDparaset, 7);
	}
#endif

}

void BidirectionalParaTest(void)
{
	//printf("\n*****************************BidirectionalParaTest********************\n");
	SetLineSpace(24);
	UINT8 checksum = 0;
	INT32 tempi = 0;
	UINT8 str_BIDparaset[7]= {0x1b, 0x10, 0x02, 0x00, 0x07, 0x00, 0x34};
	write(fd_uart1, str_BIDparaset, 7);
}

UINT8 GetPrinterStatus(void)
{
	UINT32 timeOut = 0;
	INT32 tempi=0;
	UINT8 chPut=0, chGet=0;

#ifndef WIN32

#if PRINTER_INTERRUPT		
	while (ser_can_read(2))
	{
		ser_get_char(2);
	}

	SendCmd(2, 0x1B,0x76);//返回打印机状态

	while (ser_can_read(2) == 0);	
	
	status = ser_get_char(2);
#else

	UINT8 str_Getstatus[7] = {0x1b, 0x10, 0x06, 0x00, 0x07, 0x00, 0x38};
	for (tempi=0; tempi<7;tempi++)
	{
		pPrinterSerial->PutChar(pPrinterSerial, &str_Getstatus[tempi]);
	}
	CommonSleep(100);
	INT32 nCount = 0;
	while((nCount<1) && (timeOut < 10))
	{
		nCount = pPrinterSerial->GetReceiveCount(pPrinterSerial);
		CommonSleep(100);
		timeOut++;
		//printf("timeOut = %u\n", timeOut);
	}
	//printf("最终的 nCount = %d\n",nCount);
	if (nCount >= 1)
	{
		//printf("uart1: GetChar Success!\n");
		for (tempi=nCount; tempi>0; tempi--)
		{
			chGet = pPrinterSerial->GetChar(pPrinterSerial);
			nCount = pPrinterSerial->GetReceiveCount(pPrinterSerial);
			//printf("此时的 nCount = %d\n",nCount);
			//printf("此时的 chGet = 0x%x\n", chGet);
		}
	}
	else
	{
		chGet = 0x01;
		//printf("uart1: GetChar Failure!");
	}

#endif

#endif	//WIN32
	//printf("最终的 chGet = 0x%x\n", chGet);
	return chGet;
}


UINT8 GetUsedLines(void)
{
	return 0;
}


UINT16 GetUsedBytes(void)
{	
	return 0;
}
#endif	//SIDA_PRINTER == 1

#else

void OpenPrinterDEV()
{

#ifndef WIN32
	//printf("uart1 opened start\n");
	if ((fd_uart1 = open("/dev/uart1", O_RDWR)) == -1)
	{
		//printf("open uart1 err\n");
		exit(-1);
	}
	else
		//printf("uart1 opened\n");
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


#endif	//POS_TYPE == POS_APE4000RG


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


