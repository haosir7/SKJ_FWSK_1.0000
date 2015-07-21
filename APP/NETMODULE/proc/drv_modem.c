#include	 <sys/ioctl.h>
#include     <stdio.h>      
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>  
#include     <string.h>
#include	<sys/select.h>

#include "netHeaderDef.h"
#include	"commonFunc.h"
#include     "drv_modem.h"
#include	 "modem_err.h"
 
#define FALSE  -1
#define TRUE   0


#define MODEMIO_MAGIC		'm'
#define MODEMIO_INIT		_IO(MODEMIO_MAGIC,0)
#define MODEMIO_POWER_ON	_IO(MODEMIO_MAGIC,1)
#define MODEMIO_POWER_OFF	_IO(MODEMIO_MAGIC,2)
#define MODEMIO_RTS_HIGH	_IO(MODEMIO_MAGIC,3)
#define MODEMIO_RTS_LOW		_IO(MODEMIO_MAGIC,4)
#define MODEMIO_MAXNR		5	

//#define MODEM_DEBUG

#ifdef MODEM_DEBUG
#define DEBUG_PRINTF	printf
#else
#define DEBUG_PRINTF
#endif


//global
volatile static  int s_modemfd;
static int debug_level=3;
static int g_verbose = 1;
char show_buf[512];


static int check_io(int fd,int tmout,char *buf,int *buflen);
static void set_speed(int fd, int speed);
static int set_property(int fd,int databits,int stopbits,int parity);
static int strDoesContain(char *str,char *lookfor);



#if (_CONFIG_M2M_MODULE != 0)

/**
*@brief  set uart baudrate
*@param  fd    type int  uart file operater
*@param  speed type int  baudrate
*@return  void
*/
static void set_speed(int fd, int speed)
{
	int   i; 
	int   status; 
	struct termios   Opt;
	int speed_arr[] = {B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300};

	tcgetattr(fd, &Opt); 
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) 
	{ 
		if  (speed == name_arr[i]) 
		{     
			tcflush(fd, TCIOFLUSH);     
			cfsetispeed(&Opt, speed_arr[i]);  
			cfsetospeed(&Opt, speed_arr[i]);   
			status = tcsetattr(fd, TCSANOW, &Opt);  
			if  (status != 0) 
			{        
				perror("tcsetattr fd");  
				return;     
			}    
			tcflush(fd,TCIOFLUSH);   
		}  
	}
}

/**
*@brief   set uart property
*@param  fd      int  uart file operater
*@param  databits   int databits 7/8
*@param  stopbits   int stopbits 1/2
*@param  parity    int  :N,E,O,,S
*/
static int set_property(int fd,int databits,int stopbits,int parity)
{ 
	struct termios options; 
    
	if  ( tcgetattr( fd,&options)  !=  0) 
	{ 
		perror("SetupSerial 1");     
		return(FALSE);  
	}

	/* set databits */
	options.c_cflag &= ~CSIZE; 
	switch (databits)
	{   
		case 7:		
			options.c_cflag  |= CS7; 
			break;
		case 8:     
			options.c_cflag  |= CS8;
			break;   
		default:    
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);  
	}
	switch (parity) 
	{   
	case 'n':
	case 'N':    
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Disable parity checking */ 
		break;  
	case 'o':   
	case 'O':     
		options.c_cflag  |= (PARODD| PARENB); /* Odd parity */  
		options.c_iflag  |= INPCK;             /* Enable parity checking */ 
		break;  
	case 'e':  
	case 'E':   
		options.c_cflag  |= PARENB;     /* Enable parity */    
		options.c_cflag &= ~PARODD;   /* Even parity */     
		options.c_iflag  |= INPCK;       /* Enable parity checking */
		break;
	case 'S': 
	case 's':  /*as no parity*/   
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;  
	default:   
		fprintf(stderr,"Unsupported parity\n");    
		return (FALSE);  
	}  
	/* set stopbits */  
	switch (stopbits)
	{   
	case 1:    
		options.c_cflag &= ~CSTOPB;  
		break;  
	case 2:    
		options.c_cflag  |= CSTOPB;  
	   break;
	default:    
		 fprintf(stderr,"Unsupported stop bits\n");  
		 return (FALSE); 
	} 

	tcflush(fd,TCIFLUSH);

 /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
		perror("SetupSerial 3");   
		return (FALSE);  
	} 
	return (TRUE);  
}

int port_open(char *dev,int baudrate)
{
#ifdef UART_TTY
	int fd;
	struct termios options;
	/* open the port */
	fd = open( dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (-1 == fd)	
	{ 			
		perror("Can''t Open Serial Port");
		return -1;		
	}	
	fcntl(fd, F_SETFL, 0);
	/* get the current options */
	tcgetattr(fd, &options);
	/* set raw input, 1 second timeout */
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;
	/* disable software flow control and CR NL map */
	options.c_iflag &= ~(IXON | IXOFF | INLCR | ICRNL);
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 100;				// timeout 10s
	/* set the options */
	tcsetattr(fd, TCSANOW, &options);
	// Set baudrate
	set_speed(fd,baudrate);
	// Set property
	if (set_property(fd,8,1,'N') == FALSE)
	{
		perror("error: set port property");
		return -1;
	}
	return fd;	
#endif

#ifdef UART_CUSTOMER_ZL
	int fd;
	//open uart3
	fd = open(dev,O_RDWR);
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	//set baudrate
	//Fix me: uart0 uart1 uart2 uart3
	ioctl(fd, UART3_SET_BAUDRATE, baudrate);

	return fd;
#endif

#ifdef UART_CUSTOMER_YYQ
	int fd;
	//open uart3
	fd = open(dev,O_RDWR);
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	//set baudrate
	ioctl(fd, UART_SET_BAUDRATE, baudrate);

	return fd;
#endif
}

/**
*@brief  init io and poweron the modem
*@param  none
*@return  void
*/
int modem_poweron(void)
{
	int fd;
	// power on the modem and issue the rts signal
	DEBUG_PRINTF("power on the modem\n");
	fd = open( "/dev/modemIO", O_RDWR );
	if (-1 == fd)	
	{ 			
		perror("Can't Open Modem IO");
		return -1;		
	}
	ioctl(fd,MODEMIO_RTS_HIGH);
	ioctl(fd,MODEMIO_POWER_ON);
	sleep(1);
	ioctl(fd,MODEMIO_RTS_LOW);
	close(fd);
	return 0;
	
}

int modem_write(void *buf, int len)
{
	int n;
    int tmplen = len;
    int writelen;	
#if 1
    while(tmplen > 0)
    {
		writelen=tmplen>512? 512 : tmplen;
		n=write(s_modemfd,buf+len-tmplen,writelen);
		tmplen -= n; 
		CommonSleep(100);
    }


    return len;
#else
    n=write(s_modemfd,buf,len);
    return n;
#endif
	//return(write(s_modemfd,buf,len));
}

int modem_read(void *buf ,int len)
{
#if 1
	int n;
	n= read(s_modemfd,buf,len);
	return n;
#else
	int n;
	int readlen;
	struct timeval timeout;
	int max_fd;
	fd_set input;
	FD_ZERO(&input);
	FD_SET(s_modemfd, &input);
	max_fd = s_modemfd + 1;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	n = select(max_fd, &input, NULL, NULL, &timeout);
	if(n<0)
		perror("select failed\n");
	else
	{
		if(FD_ISSET(s_modemfd,&input))
		{
			readlen = read(s_modemfd,buf,len);
			return readlen;
		}
		else
			return 0;
	}
#endif
}

int modem_close(void)
{
	char cmdbuf[255];
	if(MODEM_COM_BAUDRATE != MODEM_INIT_BAUDRATE)
	{
		DEBUG_PRINTF("restore baudrate to %d\n ",MODEM_INIT_BAUDRATE);
		sprintf(cmdbuf,"AT+IPR=%d",MODEM_COM_BAUDRATE);
		//DEBUG_PRINTF("%s\n",cmdbuf);
		modem_sendCmd(cmdbuf);
	//		if(0 != modem_waitFor(MODEM_OK_STR,20))
	//		{
	//			printf("Error: change baudrate\n");
	//			return (-1);
	//		}
	}
	close(s_modemfd);
	return 0;
}

void modem_sendEnter(void)
{
	 (void) write(s_modemfd,"\r",1);
	 /*printf("\r\n");*/
}

void modem_sendCmd(char *cmd)
{
	int n;
	n=write(s_modemfd,cmd,strlen(cmd));
	/*printf("%s\n",cmd);*/
	modem_sendEnter();
	usleep(5);
}

/*
 *  strDoesContain () - finds if a specified string/portion is available
 *                      in a string.
 *
 *  Return Values:
 *      1      if found
 *      0      if not found
 *
 *  Parameters:
 *      char *str        scan this string
 *      char *lookfor    look for this substring
 */
static int strDoesContain(char *str,char *lookfor)
{
    int rc=0;
    if (strstr(str,lookfor))
        rc=1;
    else
        rc=0;

    return(rc);
}

/*
 *  checkIO()   - checks if there is any IO pending.
 *
 *  RCS
 *      $Revision: 1.2 $
 *      $Date: 2006/08/14 02:57:59 $
 *  Return Values:
 *      1       if any io pending
 *      0       if not
 *
 *  Parameters:
 *      int fd              open file descriptor
 *      int tmout           timeout (not used at this time)
 *      char *buf           fill this buffer with data (returns)
 *      int  *buflen        lenngth of the buf (returns)
 *
 *  Side Effects:
 *      buf must have space to hold at least 127 bytes.
 *
 *      maximum 127 bytes are filled in buf.
 *
 *      buf is modified
 *      buflen is modified
 *
 *  Limitations and Comments:
 *      adapted from minicom ipc.c
 *
 *  Development History:
 *      who                  when           why
 *      ma_muquit@fccc.edu   Jun-04-1997    first cut
 */


/* Check if there is IO pending. */
static int check_io(int fd,int tmout,char *buf,int *buflen)
{
  int
    n=0,
    x=0;

  n=read(fd,buf,127);

  buf[n > 0 ? n : 0] = 0;
  if (buflen) *buflen = n;
  if (n > 0) x |= 1;

  return(x);
}

/*
 *  waitFor ()  - wait for expected responses from Modem
 *
 *  RCS
 *      $Revision: 1.2 $
 *      $Date: 2006/08/14 02:57:59 $
 *  Return Values:
 *      0                   if found what we expected
 *      MODEM_BUSY          if modem is busy
 *      MODEM_NO_CARRIER    if modem has no carrier
 *      MODEM_NO_DIALTONE   if modem has no dial tone
 *      -1                  if malloc or some other kind of errors
 *
 *  Parameters:
 *      char    *str                wait for this string
 *      int     usleep_time         sleep micro seconds (not used right now)
 *
 *  Side Effects:
 *      none
 *
 *  Limitations and Comments:
 *      n/a
 *
 *  Development History:
 *      who                  when           why
 *      ma_muquit@fccc.edu   May-31-1997    first cut
 */
int modem_waitFor(char *str,int expire_time)
{
    int s_timeout=0;
    int found;
    int rc;

    time_t now, last;

    char *s=(char *) NULL;
    char tmpbuf[128];
    char buf[128];
    char modbuf[128];

    int modidx=0;

    int x=0;

    rc=0;

	DEBUG_PRINTF("modem_waitFor begin\n");

	time(&now);
	last=now;

	 modbuf[0]='\0';
	 modidx=0;
	 s=buf;
	 buf[0]='\0';

	while (1)
	{
		if (*s == '\0')
		{
			x=check_io(s_modemfd,1000,buf,NULL);
			s=buf;
		}
		if (x & 1)      /* found something */
		{
			if (*s == '\r' || *s == '\n')
			{
				modbuf[modidx]=0;
				modidx=0;
				DEBUG_PRINTF("modbuf=%s\n");
			}
			else if (modidx < 127)
			{
				modbuf[modidx++]= *s;
				modbuf[modidx]=0;
			}
			/* Skip to next received char */
			if (*s) s++;
		}
		found=strDoesContain(modbuf,str);
		if (found == 1)
		{
			/*
			** if we'r here we found what we expected
			*/
			DEBUG_PRINTF("Found what we expected=%s\n");
			rc=0;
			goto ExitProcessing;
		}
		else
		{
			if (strcasecmp(modbuf,MODEM_BUSY_STR) == 0)
			{
				DEBUG_PRINTF("[Found] %s\n",modbuf);
				rc=MODEM_DEVICE_BUSY;
				goto ExitProcessing;
			}
			else if (strcasecmp(modbuf,MODEM_NOCARRIER_STR) == 0)
			{
				DEBUG_PRINTF("[Found] %s\n",modbuf);
				rc=MODEM_DEVICE_NO_CARRIER;
				goto ExitProcessing;
			}
			else if (strcasecmp(modbuf,MODEM_NODIALTONE_STR) == 0)
			{
				DEBUG_PRINTF("[Found] %s\n",modbuf);
				rc=MODEM_DEVICE_NO_DIALTONE;
				goto ExitProcessing;
			}
			else if (strcasecmp(modbuf,MODEM_ERROR_STR) == 0)
			{
				DEBUG_PRINTF("[Found] %s\n",modbuf);
				rc=MODEM_DEVICE_ERROR_SYNTAX;
				goto ExitProcessing;
			}
		}
		time(&now);
		if(now > (last + expire_time))
		{
			DEBUG_PRINTF("MODEM_DEVICE_TIMEOUT\n");
			rc=MODEM_DEVICE_TIMEOUT;
			goto ExitProcessing;

		}

		//usleep(250000);
		usleep(100);

	}   /* while */


ExitProcessing:

    return (rc);
}

char *findcrlfbw(unsigned char *str, int len)
{
	while (len-- && (*str != '\n') && (*(str-1) != '\r'))
	{
		//printf("len = %d\n",len);
		str--;
	}
	return len > 0 ? str+1 : NULL;
}

char *findcrlffw(unsigned char *str, int len)
{
	while (len-- && (*str != '\r') && (*(str+1) != '\n'))
		str++;
	return len > 0 ? str+2 : NULL;
}

int modem_getResponse(char *buf,int *len)
{
	char rbuf[128];
	int rbuf_pos = 0;
	int rlen;
	int timeout = MODEM_TIMEOUT;

	time_t last,now;

	int rc;
	int error;

	time(&now);
	last = now;

	int finished = 0;

	//char *p;
	char *result_start;
	char *response_start;
	int response_len;


	do
	{
		//read from modem
		rlen = modem_read( &rbuf[rbuf_pos], 128-rbuf_pos);
		if(rlen != 0)
		{
			//update buffer point
			rbuf_pos += rlen;
			rbuf[rbuf_pos] = '\0';
		}

		/* first check if <cr><lf> is found at end of reply_buf.
		 * the needed length is greater 4 because we don't
		 * need to enter if no result/error will be found. */
		if(rbuf_pos > 4 && !strncmp(rbuf + rbuf_pos - 2, "\r\n", 2))
		{
			/* try to find previous <cr><lf> */
			result_start = findcrlfbw(rbuf + rbuf_pos - 2, rbuf_pos - 1);
			/* if not found, start at buffer beginning */
			if (!result_start)
				result_start = rbuf;
			/* there are certainly more that 2 chars in buffer */
			if (!strncmp(result_start, "OK", 2))
			{
				/* try to find the real response begin point */
				response_start = findcrlffw(rbuf,rbuf_pos+1);
				/* response format sample: <cr><lf>response<cr><lf><cr><lf>OK<cr><lf> */
				response_len = (int)(result_start - response_start);
				/* no response, just OK */
				if( response_len == 0 )
				{
					rc = 0;
					*len = 0;
				}
				/* response */
				else if( response_len > 4 )
				{
					/* ignore the <cr><lf><cr><lf> */
					response_len -= 4;
					strncpy(buf,response_start,response_len);
					*len = response_len;
					rc = 0;
				}
				else
					rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RESPONSE);
				finished = 1;

			}
			else if (sscanf(result_start, "+CME ERROR: %d", &error) == 1)
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_CME,error);
				finished = 1;
			}
			else if (sscanf(result_start, "+CMS ERROR: %d", &error) == 1)
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_CMS,error);
				finished = 1;
			}
			else if (!strncmp(result_start, "RING", 4))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_RING);
				finished = 1;
			}
			else if (!strncmp(result_start, "BUSY", 4))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_BUSY);
				finished = 1;
			}
			else if (!strncmp(result_start, "NO ANSWER", 9))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_ANSWER);
				finished = 1;
			}
			else if (!strncmp(result_start, "NO CARRIER", 10))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_CARRIER);
				finished = 1;
			}
			else if (!strncmp(result_start, "NO DIALTONE", 11))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_NO_DIALTONE);
				finished = 1;
			}
			else if (sscanf(result_start, "ERROR: %d", &error) == 1)
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,error);
				finished = 1;
			}
			else if (!strncmp(result_start, "ERROR", 5))
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_ERROR_SYNTAX);
				finished = 1;
			}

			else if (*result_start == '+')
			{
				//for message issued by wavecom Q2686 module
				//ignor +WIND message
				continue;
			}
			else if (!strncmp(result_start, "$M2MMSG", 7))
			{
				//for message issued by wavecom Q2686 module
				//ignor $M2MMSG message
				continue;
			}
			else
			{
				//
			}
		}


		if(!finished)
		{
			time(&now);
			if ( now > last + timeout)
			{
				rc = MODEM_ERROR_NUM(MODEM_ERROR_DEVICE,MODEM_DEVICE_TIMEOUT);
				finished = 1;
				*len = 0;
			}
		}
	}while(!finished);

	return rc;

}

int modem_charsInBuffer(void)
{
#ifdef UART_TTY
	int bytes = 0;
	ioctl(s_modemfd, FIONREAD, &bytes);
	return bytes;
#endif

#ifdef UART_CUSTOMER_ZL
	int bytes = 0;
	//Fix me
	ioctl(s_modemfd, UART3_CAN_READ, &bytes);
	return bytes;
#endif

#ifdef UART_CUSTOMER_YYQ
	int bytes = 0;
	ioctl(s_modemfd, UART_CAN_READ, &bytes);
	return bytes;
#endif
}

int modem_flushInputBuffer(void)
{
#ifdef UART_TTY
	ioctl(s_modemfd, TCFLSH,0);
#else
	int bytes;
	char buf[1024];
//	while(modem_charsInBuffer())
//		modem_read(buf,1024);
	while(modem_read(buf,1024));
	return bytes;
#endif
}


#endif	//_CONFIG_M2M_MODULE != 0





/* O - 0 = MODEM ok, -1 = MODEM bad */
/* I - Serial port file */
int modem_init(void)
{
#if (_CONFIG_M2M_MODULE != 0)

	int len;
	char buf[1024];
	char cmdbuf[255];

	//Open the serial port used by modem
	s_modemfd = port_open(MODEM_DEV,MODEM_INIT_BAUDRATE);
	if(-1 == s_modemfd )
	{
//		printf("error: can not open the port %s\n",MODEM_DEV);
		return -1;
	}
	//Power on the modem
	if(modem_poweron() == -1 )
	{
//		printf("error: can not power on the modem\n");
		return -1;
	}

	//flush the buffer
	modem_flushInputBuffer();
//	while(1)
//	{
//		len = modem_charsInBuffer();
//		printf("len = %d\n",len);
//		if (len == 0)
//			break;
//		else
//			modem_read(buf,1024);
//	}


	// Send reset string
//	modem_sendCmd(MODEM_ESCAPE_STR);
//	sleep(1);
//	DEBUG_PRINTF("send cmd: MODEM_RESET_STR\n ");
//	modem_sendCmd(MODEM_RESET_STR);
//	if(0 != modem_waitFor(MODEM_OK_STR,20))
//	{
//		printf("Error: modem reset\n");
//		return (-1);
//	}



	if(MODEM_COM_BAUDRATE != MODEM_INIT_BAUDRATE)
	{
		DEBUG_PRINTF("change baudrate to %d\n ",MODEM_COM_BAUDRATE);
		sprintf(cmdbuf,"AT+IPR=%d",MODEM_COM_BAUDRATE);
		//DEBUG_PRINTF("%s\n",cmdbuf);
		modem_sendCmd(cmdbuf);
//		if(0 != modem_waitFor(MODEM_OK_STR,20))
//		{
//			printf("Error: change baudrate\n");
//			return (-1);
//		}
		close(s_modemfd);
		sleep(1);
		DEBUG_PRINTF("reopen the modem port\n");
		s_modemfd = port_open(MODEM_DEV,MODEM_COM_BAUDRATE);
		if(-1 == s_modemfd )
		{
//			printf("error: can not open the port %s\n",MODEM_DEV);
			return -1;
		}
	}


#if 1
	DEBUG_PRINTF("send cmd: MODEM_AT_STR\n ");
	DEBUG_PRINTF("wait response\n ");
	modem_sendCmd(MODEM_AT_STR);
	if(0 != modem_waitFor(MODEM_OK_STR,20))
	{
//		printf("Error: modem test\n");
		return (-1);
	}

	// Send init string
	DEBUG_PRINTF("send cmd: MODEM_INIT_STR\n ");
	modem_sendCmd(MODEM_INIT_STR);
	DEBUG_PRINTF("wait resoponse\n ");
	if(0 != modem_waitFor(MODEM_OK_STR,20))
	{
//		printf("Error: modem init\n");
		return (-1);
	}

	//强制设置波特率
	sprintf(cmdbuf,"AT+IPR=%d",MODEM_COM_BAUDRATE);
	//DEBUG_PRINTF("%s\n",cmdbuf);
	modem_sendCmd(cmdbuf);
	if(0 != modem_waitFor(MODEM_OK_STR,20))
	{
		//		printf("Error: modem init\n");
		return (-1);
	}

#endif
	return 0;


#else	//_CONFIG_M2M_MODULE != 0
	return 0;
#endif
}






		



