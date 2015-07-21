/*! \file pos_log.c
   \brief 本文件实现了调试信息输出函数
   \author zyk
 */
#include "LOGCTRL.h"
#include"pos_log.h"
#ifndef NO_POS_DEBUG

#include<string.h>

static char filename_buf[128];

const char *GetFileName(const char *fullfilename)

{

#ifdef WIN32

	int i=0;

	int j=0;

	int c=0;

	int len=0;

	const char *fn;

	char ch=0;

	if(fullfilename==NULL)

	{

		return " ";

	}

	len=strlen(fullfilename);

	c=len-1;

	//	memset((void*)fn,0,sizeof(fn)); 



	memset((void *)filename_buf,0,sizeof(filename_buf));

	//	memcpy((void *)fn,(void *)fullfilename,len);

	fn=fullfilename+c;

	while( *fn != '\\' && c > -1 )

	{

		filename_buf[j]=*fn;

		c--;

		j++;

		fn=fn-1;

	}

	if(0==j)

	{

		return " ";

	}

	for(i=0;i<(j/2);i++)

	{

		ch=filename_buf[i];

		filename_buf[i]=filename_buf[j-i-1];

		filename_buf[j-i-1]=ch;

	}

	return filename_buf;

#else

	return fullfilename;

#endif

}

#endif // NO_POS_DEBUG

#ifdef POS_DEBUG_TO_FILE

#include<string.h>

#include<stdarg.h>

//#include<varargs.h>



static unsigned char first_open_zyk_flag='\1';

#ifdef WIN32    
char g_log_file_path_and_file[512]="poslog";
#endif

void write_log(const char * file,int line,const char * fmt , ... )

{



	char buf[1024];

	char buf1[128];

	char buf2[512];

	va_list args;

#ifdef WIN32
	FILE *fp=fopen(g_log_file_path_and_file,"a+"); 
#else
	FILE *fp=fopen("poslog","a+");
#endif

	memset((void *)buf,0,sizeof(buf));

	memset((void *)buf1,0,sizeof(buf1));

	memset((void *)buf2,0,sizeof(buf2));

	sprintf(buf1,"%s %d ",file,line);

	va_start(args,fmt);   

	vsprintf(buf2,fmt,args);

	va_end(args);



	sprintf(buf," %s %s \n",buf1,buf2);   



	if(first_open_zyk_flag)

	{

		fputs("**********************************************************************************\n",fp);

		first_open_zyk_flag='\0';

	}



	fputs(buf,fp);

	fflush(fp);



	if(fp)

	{

		fclose(fp);

	}

}
void write_log2(const char *info )

{


#ifdef WIN32
	FILE *fp=fopen(g_log_file_path_and_file,"a+"); 
#else
	FILE *fp=fopen("poslog","a+");
#endif


	if(first_open_zyk_flag)

	{

		fputs("**********************************************************************************\n",fp);

		first_open_zyk_flag='\0';

	}



	fputs(info,fp);

	fflush(fp);

	if(fp)

	{

		fclose(fp);

	}

}
#endif // POS_DEBUG_TO_FILE









#ifdef POS_DEBUG_TO_SERIAL



#include<string.h>

#include<stdarg.h>

#ifndef WIN32
#include <unistd.h>  /* UNIX standard function definitions */
#include <termios.h> /* POSIX terminal control definitions */

#endif

#include <fcntl.h>   /* File control definitions */

#include <errno.h>   /* Error number definitions */



//static char filename_buf[128];

static char log_buf[1024];

static char log_buf1[128];

static char log_buf2[512];

va_list args;

static int debug_com=0;

static struct  termios options;





int   open_debug_com(void)

{



	debug_com = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);

	//fd = open("/dev/ttyUART1", O_RDWR | O_NOCTTY | O_NDELAY);

	if (debug_com == -1)

	{

		perror("open_port: Unable to open /dev/ttyS0 - ");

		return -1;

	}

	else

	{

		fcntl(debug_com, F_SETFL, 0);

	}



	tcgetattr(debug_com, &options);



	cfsetispeed(&options, B115200);

	cfsetospeed(&options, B115200);



	/*

	* Enable the receiver and set local mode...

	*/



	options.c_cflag |= (CLOCAL | CREAD);

	//--  No parity (8N1):



	options.c_cflag &= ~PARENB;// No parity

	options.c_cflag &= ~CSTOPB;// 1 stop bit;





	options.c_cflag &= ~CSIZE; /* Mask the character size bits */

	options.c_cflag |= CS8;    /* Select 8 data bits */





	options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/

	options.c_oflag     &= ~OPOST;   /*Output*/





	options.c_iflag &= ~(IXON | IXOFF | IXANY);



	options.c_iflag &= ~(INLCR|ICRNL|IUCLC);





	options.c_oflag &= ~(OLCUC|ONLCR|OCRNL);



	options.c_cc[VMIN]  = 0;

	options.c_cc[VTIME] = 10;



	tcsetattr(debug_com, TCSANOW, &options);



}

void  SendStrToSerial(int port, unsigned char *str,int len )      

{

	static int first_flag=0;



	if(first_flag == 0)

	{

		open_debug_com();

		first_flag=1;

	}

	write(port,str,len); 



}

void log2serial(const char * file, int line, const char * fmt , ...)

{

	memset((void *)log_buf, 0, sizeof(log_buf));
	memset((void *)log_buf1, 0, sizeof(log_buf1));
	memset((void *)log_buf2, 0, sizeof(log_buf2));

	sprintf(log_buf1,"%s %4d : ",file, line);

	va_start(args, fmt);   
	vsprintf(log_buf2, fmt, args);
	va_end(args);

	sprintf(log_buf," %s %s \n\r",log_buf1, log_buf2);   
	SendStrToSerial(debug_com, (unsigned char *)log_buf, strlen(log_buf));

}

void log2serial2(const char * fmt , ...)

{

	memset((void *)log_buf, 0, sizeof(log_buf));



	va_start(args, fmt);   

	vsprintf(log_buf, fmt, args);

	va_end(args);



	SendStrToSerial(debug_com, (unsigned char  *)log_buf, strlen(log_buf));

}

void log2serial3(const char *info)
{
	SendStrToSerial(debug_com, (unsigned char  *)info, strlen(info));
}

#endif // POS_DEBUG_TO_SERIAL



