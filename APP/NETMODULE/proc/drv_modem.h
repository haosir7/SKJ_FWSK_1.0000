#ifndef DRV_MODEM_H
#define DRV_MODEM_H

#ifdef __cplusplus
extern "C"
{
#endif

#define DEBUG

#ifdef DEBUG
#define Debug4(fmt,v1,v2,v3,v4)                              \
    {char * trick = (fmt);                                   \
    (void) fprintf (stderr,"%s(%d): ", __FILE__,__LINE__);   \
    (void) fprintf (stderr,(trick),(v1),(v2),(v3),(v4));     \
    (void) fprintf (stderr,"\n");}

#define Debug2(fmt,v1,v2) Debug4((fmt),(v1),(v2),0,0)
#else
#define Debug4(fmt,v1,v2,v3,v4)
#define Debug2(fmt,v1,v2)
#endif

#define UART_TTY
//#define UART_CUSTOMER_ZL
//#define UART_CUSTOMER_YYQ

#ifdef UART_TTY
#define MODEM_DEV 					"/dev/ttyS3"
#define MODEM_INIT_BAUDRATE			115200
#define MODEM_COM_BAUDRATE			115200
#endif

#ifdef UART_CUSTOMER_ZL
#define MODEM_DEV 					"/dev/uart3"
#define MODEM_INIT_BAUDRATE			115200
#define MODEM_COM_BAUDRATE			57600

/* ioctrl define */
#define UART_MAGIC 'u'
#define UART_MAXNR  8

#define UART0_CAN_READ			_IOW(UART_MAGIC, 1, unsigned int)
#define UART1_CAN_READ			_IOW(UART_MAGIC, 2, unsigned int)
#define UART2_CAN_READ			_IOW(UART_MAGIC, 3, unsigned int)
#define UART3_CAN_READ			_IOW(UART_MAGIC, 4, unsigned int)
#define UART0_SET_BAUDRATE		_IOW(UART_MAGIC, 5, unsigned int)
#define UART1_SET_BAUDRATE		_IOW(UART_MAGIC, 6, unsigned int)
#define UART2_SET_BAUDRATE		_IOW(UART_MAGIC, 7, unsigned int)
#define UART3_SET_BAUDRATE		_IOW(UART_MAGIC, 8, unsigned int)

#endif

#ifdef UART_CUSTOMER_YYQ
#define MODEM_DEV 					"/dev/uart3" 
#define MODEM_INIT_BAUDRATE			115200
#define MODEM_COM_BAUDRATE			115200

#define UART_MAGIC 'u'
#define UART_MAXNR  3

#define UART_CAN_READ           _IOW(UART_MAGIC, 1, unsigned int)
#define UART_SET_BAUDRATE       _IOW(UART_MAGIC, 2, unsigned int)
#define UART_GET_TOTAL_RECV     _IOW(UART_MAGIC, 3, unsigned int)

#endif


#define MODEM_AT_STR				"AT"
#define MODEM_RESET_STR				"ATZ"
#define MODEM_INIT_STR				"ATE0"
#define MODEM_DIAL_STR				"ATDT"
#define MODEM_HANGUP_STR			"ATH"
#define MODEM_ESCAPE_STR			"+++"

#define MODEM_OK_STR				"OK"
#define MODEM_ERROR_STR         	"ERROR"
#define MODEM_CONNECT_STR			"CONNECT"
#define MODEM_BUSY_STR          	"BUSY"
#define MODEM_NOCARRIER_STR     	"NO CARRIER"
#define MODEM_NODIALTONE_STR    	"NO DIALTONE"


#define MODEM_TIMEOUT				20			//Timeout 20s

//Device error id
#define MODEM_DEVICE_BUSY           1
#define MODEM_DEVICE_NO_ANSWER		2
#define MODEM_DEVICE_NO_CARRIER     3
#define MODEM_DEVICE_NO_DIALTONE    4
#define MODEM_DEVICE_RING		    5
#define	MODEM_DEVICE_ERROR_SYNTAX	6
#define MODEM_DEVICE_TIMEOUT		7
#define MODEM_DEVICE_RESPONSE		8




///int port_open(char *dev);
int port_open(char *dev,int baudrate);
int modem_poweron(void);
int modem_init(void); 
int modem_read(void *buf, int len);
int modem_write(void *buf ,int len);
int modem_charsInBuffer(void);
int modem_flushInputBuffer(void);
int modem_close(void);
void modem_sendEnter(void);
void modem_sendCmd(char *cmd);
int modem_waitFor(char *str,int expire_time);
int modem_getResponse(char *buf,int *len);

char *findcrlfbw(unsigned char *str, int len);

#ifdef __cplusplus
}
#endif

#endif

