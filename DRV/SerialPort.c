
/*! \file   SerialPort.cpp
   \brief   串口接口驱动
   \author  zl
*/

#include <string.h>
#ifndef WIN32
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <sys/ioctl.h>
#include     <fcntl.h>      /*文件控制*/
#include     <errno.h>      /*错误号定义*/
#endif

#include "comdatatype.h"
#include "SerialPort.h"


#ifdef WIN32
#include <stdio.h>
char testbuf[] = {
0x1b, 0x10, 0xa0, 0x01, 0x0a, 0x55, 0x55, 0x55, 0x55, 0x55, 0x12, 0xD8,
0x1b, 0x10, 0xa0, 0x02, 0x11, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x0d, 0x0a, 0x45, 0x4e, 0x44, 0x0d, 0x0a, 0x2A, 0x7E,
0x1b, 0x10, 0xa0, 0x03, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x38, 0xFA, 
0x1b, 0x10, 0xa0, 0x04, 0x11, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x0d, 0x0a, 0x45, 0x4e, 0x44, 0x0d, 0x0a, 0x2C, 0x78, 
0x1b, 0x10, 0xa0, 0x05, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x3E, 0x9a, 
0x1b, 0x10, 0xa0, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x37,
};
#endif

/*
@brief Contruct SerialPort
*/
void SerialPortContruct(struct _SerialPort *thisport)
{
//	thisport->m_BandRate = 9600;

	// WIN32下调试用
#ifdef WIN32
	thisport->debugii=0;
	memcpy (thisport->debugbuf, testbuf, sizeof(testbuf));
//	thisport->debugbuf[0]=0x1b;	
//	thisport->debugbuf[1]=0x10;	
//	thisport->debugbuf[2]=0xa1;	
//	thisport->debugbuf[3]=0x00;	
//	thisport->debugbuf[4]=0x05;	
//	thisport->debugbuf[5]=0xcc;	
//	thisport->debugbuf[6]=0x47;	
	thisport->debuglen=sizeof(testbuf);
#endif

	thisport->InitPort = InitPort_proc;
	thisport->ClosePort = ClosePort_proc;
	thisport->GetReceiveCount = GetReceiveCount_proc;
	thisport->GetChar = GetChar_proc;
	thisport->PutChar = PutChar_proc;
	thisport->ChangeBaud = ChangeBaud_proc;
	thisport->GetCurBaud = GetCurBaud_proc;
}

/*!
@brief Initialize the port	 
*/
int InitPort_proc(struct _SerialPort *thisport, 
				   const char *name, 
				   int  portnr,		
				   int  baud	
					)	
{
#ifndef WIN32
	thisport->m_nPortNr = portnr;
	strcpy(thisport->m_devFileName, name);
	thisport->m_fd = open(thisport->m_devFileName, O_RDWR);
	
//	switch (thisport->m_nPortNr)
//	{
//		case UART2:
//			thisport->m_fd = open("/dev/uart2", O_RDWR);
//			break;
//		case UART3:
//			thisport->m_fd = open("/dev/uart3", O_RDWR);
//			break;
//		default:
//			break;
//	}

	if (thisport->m_fd == -1)
	{
		return FAILURE; 
	}
	else
	{
		thisport->ChangeBaud(thisport, baud);
		return SUCCESS;
	}
#else

	return SUCCESS;
	
#endif
}

/*!
@brief get curent receive byte count	 
*/
int GetReceiveCount_proc(struct _SerialPort *thisport)
{
#ifndef WIN32
	int count;

	switch (thisport->m_nPortNr)
	{
		case UART0:
			ioctl(thisport->m_fd, UART0_CAN_READ, &count);
			break;
		case UART1:
			ioctl(thisport->m_fd, UART1_CAN_READ, &count);
			break;
		case UART2:
			ioctl(thisport->m_fd, UART2_CAN_READ, &count);
			break;
		case UART3:
			ioctl(thisport->m_fd, UART3_CAN_READ, &count);
			break;
		default:
			break;
	}
	
	return count;

#else
	return 0;
//	return (thisport->debuglen - thisport->debugii);
#endif
}

/*!
@brief get a char from driver, blocking mode.
*/
unsigned char GetChar_proc(struct _SerialPort *thisport)
{
#ifndef WIN32	
	unsigned char byte;
	int res, tmp;
	
	while ( (res = ReadPort(thisport->m_fd, &byte, 1, &tmp)) != SUCCESS ) ; 

	return byte;
	
#else
	return thisport->debugbuf[thisport->debugii++];
#endif
} 

/*!
@brief put a char to driver, blocking mode.
*/
void PutChar_proc(struct _SerialPort *thisport, unsigned char* byte)
{
	int res, tmp;
	
	while ( (res = WritePort(thisport->m_fd, byte, 1, &tmp)) != SUCCESS) ; 

	return;
}
	
/*!
@brief change baudrate
*/
void ChangeBaud_proc(struct _SerialPort *thisport, int BaudRate)
{
#ifndef WIN32	
	thisport->m_BandRate = BaudRate;
	
	switch (thisport->m_nPortNr)
	{
		case UART0:
			ioctl(thisport->m_fd, UART0_SET_BAUDRATE, BaudRate);
			break;
		case UART1:
			ioctl(thisport->m_fd, UART1_SET_BAUDRATE, BaudRate);
			break;
		case UART2:
			ioctl(thisport->m_fd, UART2_SET_BAUDRATE, BaudRate);
			break;
		case UART3:
			ioctl(thisport->m_fd, UART3_SET_BAUDRATE, BaudRate);
			break;
		default:
			break;
	}

#else
		return;
#endif
}
	
/*!
@brief get current baud.
*/
int GetCurBaud_proc(struct _SerialPort *thisport)
{
	return thisport->m_BandRate;
}

/*!
@brief close port.
*/
int ClosePort_proc(struct _SerialPort *thisport)
{
#ifndef WIN32	
	if( close(thisport->m_fd) == -1 )
		return FAILURE;
	return SUCCESS;
#else
	return SUCCESS;
#endif
}

/*!
@brief read port. private
*/
int ReadPort(int fd, unsigned char* byte, int length, int *rCount)
{
#ifndef WIN32	
	int res = 0;

	if (GetReceiveCount_proc(fd) >0)
	{
	res = read(fd, byte, length);
	*rCount = res;
		return SUCCESS;
	}else
	{
		return FAILURE;
	}
	
// 	res = read(fd, byte, length);
// 	*rCount = res;
// 	if (res == 0)
// 	{
// 		return FAILURE;
// 	}
// 	else
// 	{
// 		return SUCCESS;
// 	}

#else
	//	memcpy(byte, testbuf, length);
		return SUCCESS;
#endif
}

/*!
@brief write port. private
*/
int WritePort(int fd, unsigned char* byte, int length, int *wCount)
{
#ifndef WIN32	
	int res = 0;
	res = write(fd, byte, length);
	*wCount = res;
	if (res == 0)
	{
		return FAILURE;
	}
	else
	{
		return SUCCESS;
	}
	
#else
	{
		int i;
		for (i=0;i<length;i++)
			printf("%x ",*(byte+i));
			return SUCCESS;
	}
#endif
}
