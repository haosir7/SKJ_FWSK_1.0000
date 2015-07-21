
/*! \file   SerialPort.h
   \brief   串口接口驱动
   \author  zl
*/
#ifndef __AWE_SERIALPORT_H
#define __AWE_SERIALPORT_H


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WIN32
/* ioctrl define */
#define UART_MAGIC 'u'
#define UART_MAXNR  8

#define UART0_CAN_READ		_IOW(UART_MAGIC, 1, unsigned int)
#define UART1_CAN_READ		_IOW(UART_MAGIC, 2, unsigned int)
#define UART2_CAN_READ		_IOW(UART_MAGIC, 3, unsigned int)
#define UART3_CAN_READ		_IOW(UART_MAGIC, 4, unsigned int)
#define UART0_SET_BAUDRATE		_IOW(UART_MAGIC, 5, unsigned int)
#define UART1_SET_BAUDRATE		_IOW(UART_MAGIC, 6, unsigned int)
#define UART2_SET_BAUDRATE		_IOW(UART_MAGIC, 7, unsigned int)
#define UART3_SET_BAUDRATE		_IOW(UART_MAGIC, 8, unsigned int)

#endif

/*  */
#define UART0	0
#define UART1	1
#define UART2	2
#define UART3	3

#define	UART0_DEV_NAME		"/dev/uart0"	
#define	UART1_DEV_NAME		"/dev/uart1"
#define	UART2_DEV_NAME		"/dev/uart2"	
#define	UART3_DEV_NAME		"/dev/uart3"	

typedef struct _SerialPort
{														 
	int m_fd;
	int m_nPortNr;
	int m_BandRate;
	char m_devFileName[20];
	
	// WIN32下调试用
#ifdef WIN32
	char debugii;
	char debugbuf[255];	
	char debuglen;
#endif
	
	// port initialisation											
	int (* InitPort)(struct _SerialPort *thisport, const char *name, int portnr, int baud);
	int (* ClosePort)(struct _SerialPort *thisport);

	// read & write											
	int (* GetReceiveCount)(struct _SerialPort *thisport);
	unsigned char (* GetChar)(struct _SerialPort *thisport);
	void (* PutChar)(struct _SerialPort *thisport, unsigned char* byte);

//	int GetBuff(unsigned char* byte, int length, int *Count);
//	int PutBuff(unsigned char* byte, int length, int *Count);
	
	void (* ChangeBaud)(struct _SerialPort *thisport, int BaudRate);
	int (* GetCurBaud)(struct _SerialPort *thisport);
	
}SerialPort;

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
void SerialPortContruct(struct _SerialPort *thisport);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int InitPort_proc(struct _SerialPort *thisport, const char *name, int portnr, int baud);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int ClosePort_proc(struct _SerialPort *thisport);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int GetReceiveCount_proc(struct _SerialPort *thisport);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
unsigned char GetChar_proc(struct _SerialPort *thisport);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
void PutChar_proc(struct _SerialPort *thisport, unsigned char* byte);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
void ChangeBaud_proc(struct _SerialPort *thisport, int BaudRate);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int GetCurBaud_proc(struct _SerialPort *thisport);

/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int ReadPort(int fd, unsigned char* byte, int length, int *rCount);


/*!
@brief     
@param[in]   
@param[out] 
@return  
*/
int WritePort(int fd, unsigned char* byte, int length, int *wCount);



#ifdef __cplusplus
}
#endif

#endif	//__AWE_SERIALPORT_H


