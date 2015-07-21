/*! \file    arithmetic.c
   \brief    类型转换、数学计算一类的函数
   \author   zl
 */
#include <stdio.h> 
#include <math.h>
#include "arithmetic.h"

#include "LOGCTRL.h"
#define NO_POS_DEBUG
#include "pos_debug.h"


#define MAX_DOT_NUM			2
#define MAX_AMOUNT_VALUE	0xFFFFFF




void Hex2Char(unsigned char *bin,char *str,unsigned int length)
{
	bin2char(bin, str, length);
}

void Char2Hex(unsigned char *bin,const char *str,unsigned int length)
{
	char2bin(bin, str, length);
}

unsigned short int cal_crc(UINT8 *ptr, UINT32 len)
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

void crcxdata(unsigned int length, unsigned char * buffer, unsigned char *crc)
{
	unsigned short int crc16=0;

	crc16 = cal_crc(buffer, length);
	crc[1] = *((unsigned char *)&crc16);		//输出字节序为主机字节序
	crc[0] = *(((unsigned char *)&crc16)+1);
}

unsigned char yihuo(UINT32 length,unsigned char *buffer)
{
	return bcc(buffer, length);
}

void get_fiscal_code(unsigned char *hex,unsigned char *asc)
{
     UINT8 i=0;
	
     UINT32 decimal_num1=0;
	 UINT32 decimal_num2=0;

     for (i=0;i<4;i++) 
	 {
         decimal_num1 += (UINT32)((UINT32)(1<<8*(3-i)) * (INT32)hex[i]);
         decimal_num2 += (UINT32)((UINT32)(1<<8*(3-i)) * (INT32)hex[i+4]);
     } 
	 
     for (i=0;i<10;i++) 
	 {
		 asc[9-i] = (UINT8)(decimal_num1%10);
         decimal_num1 = (decimal_num1 - asc[9-i])/10;
     }

     for (i=0;i<10;i++) 
	 {
         asc[19-i] = (UINT8)(decimal_num2%10);
         decimal_num2 = (decimal_num2 - asc[19-i])/10;
     }

     for (i=0;i<20;i++)
          asc[i] += 48;

     return;       	  
}

void fiscal_code_hex(unsigned char *hex,unsigned char *decimal)
{
     UINT8 i=0;
	 UINT8 j=0;
     UINT32 decimal_num1=0;
	 UINT32 decimal_num2=0;
	 UINT32 dec=0;
  
     for (i=0; i<10; i++) 
	 {
		 dec = 1;
		 for(j=0; j < (9-i); j++)
         {
           dec *= 10;
         }
         decimal_num1 += (UINT32)((UINT32)dec * (decimal[i]-48));

         dec=1;
		 for(j=0;j< (9-i) ;j++)
		 {
			 dec *=10;
		 }
         decimal_num2 += (UINT32)((UINT32)dec * (decimal[10+i]-48));
     } 

     int2hex(hex,decimal_num1,4);
     int2hex(hex+4,decimal_num2,4);

     return;       	  
}

void bin2char(unsigned char *bin,char *asc,UINT32 length)
{
    UINT32 i=0;
	UINT32 j=0;
    char buff[4];

    memset((void *)buff, 0, 4);
    for(i = 0; i < length; i++)
    {
        j = 2 * i;
        sprintf(buff, "%02x", bin[i]);
        asc[j] = buff[0];
        asc[j + 1] = buff[1];
    }

    return;
}

void char2bin(unsigned char *bin,const char *asc,UINT32 length)
{
    UINT32 i=0;
    for(i=0;i<length;i++) 
	{
       bin[i] = ((asc[2*i]>'9' ?
                        ((asc[2*i]>='A' && asc[2*i]<='F')
                         ?(asc[2*i]-'A'+10):(asc[2*i]-'a'+10)):
                         (asc[2*i] - '0')) <<4 )
                        +
                  (asc[2*i+1]>'9' ?
                        ((asc[2*i+1]>='A' && asc[2*i+1]<='F')
                         ?(asc[2*i+1]-'A'+10):(asc[2*i+1]-'a'+10)):
                         (asc[2*i+1] - '0'));
    }

    return;	
}

INT32 bin2int(unsigned char *bin,UINT32 length)
{
    UINT8 i=0;
    UINT8 j=0;

    INT32 int_answer = 0;   
	INT32 dec=0;

    for(i=0;i<length;i++) 
	{
		dec=1;
        for( j=0; j<2*(length-1-i) ; j++ )
		{
           dec *=10;
		}
	    int_answer += ( ((INT32)bin[i]%16)+
		          ((INT32)bin[i]-((INT32)bin[i]%16))/16*10)*(dec);
    }

    return int_answer;    
}

void int2bin(unsigned char *bin,INT32 int_value,UINT32 length)
{
	UINT32 i=0;
	unsigned char buffer[32];
        
	memset((void *)buffer,0,sizeof(buffer)); 
	
	for(i=0;i<2*length;i++) 
	{
		buffer[2*length-1-i] = (int_value%10);
		int_value = (int_value - buffer[2*length-1-i])/10;
	}
	 
	for(i=0;i<length;i++)
		bin[i] = (buffer[2*i]<<4)+buffer[2*i+1];
	
	return;
}

INT32 hex2int(unsigned char *hex,UINT32 length)
{
    UINT32 i=0;
    INT32 int_answer = 0;
	
	for(i=0;i<length;i++) 
	{
		int_answer += (INT32)hex[i] * (1<<8*(length-1-i)); 
	}
	
    return int_answer; 	
}

void int2hex(unsigned char *hex,UINT32 int_value,UINT32 length)
{
	UINT32 i=0;
	UINT8 buffer[32];
      
	memset((void *)buffer,0,sizeof(buffer));
	
	for(i=0;i<2*length;i++) 
	{
		buffer[2*length-1-i] = (UINT8)(int_value%16);
		int_value = (int_value - buffer[2*length-1-i])/16;
	}
	for(i=0;i<length;i++)
		hex[i] = (buffer[2*i]<<4)+buffer[2*i+1];
	    
	return;
}


UINT8 bcc(unsigned char *databuff, UINT32 size)
{
	UINT8 t;
	UINT32 i;
	t = 0;
	
	for(i = 0; i<size; i++)
	{
		t = t^(*(databuff+i));
	}
	
	return t;
}

INT32 AmountRound(double *f)
{
    char buf[128];
    INT32 dot;
    INT32 i;
	INT32 dotnum;
	UINT64 f2int = 0;
	UINT8 littleNum = 0;
	
	memset((void *)buf, 0x00, sizeof(buf));
	//判断绝对值是否1.0，小于1.0的double数值对小数位数没有限制
	if (fabs(*f) < 1.0)
		littleNum = 1;
	
	//计算小数点后有效位数
    sprintf(buf, "%.10lf", *f);
    i = strlen(buf) - 1;
    while(buf[i] == '0')
    {
        i--;
    }
	
    dot = 0;
    while(buf[dot] != '.')
    {
        dot++;
    }
	
	//对小数位数做四舍五入处理
    dotnum = ((i - dot) > 0)?(i - dot):0;
	DBG_PRN("info",("--------dotnum= %d--------", dotnum));
		DBG_PRN("info",("--------littleNum= %d--------", littleNum));
	if (dotnum > MAX_DOT_NUM && littleNum == 0)
	{
		(*f) += 5.0/pow(10, MAX_DOT_NUM+1);
		sprintf(buf, "%.10lf", *f);
		dotnum = MAX_DOT_NUM;
	}
	DBG_PRN("info",("--------dotnum= %d--------", dotnum));
	//小数位数截断处理，dotnum = 0时需要把小数点去掉
	if (dotnum != 0)
	{
		buf[dot+dotnum+1] = '\0';
	}
	else
	{
		buf[dot+dotnum] = '\0';
	}
	
		DBG_PRN("info",("--------buf= %s--------", buf))
	//在整数部分大于1时，判断默认保留MAX_DOT_NUM位小数或更少情况下，
	//数量是否超过最大值
	*f = atof(buf);	
	DBG_PRN("info",("--------*f= %f--------", *f));
	f2int = (UINT64)((*f)*(pow(10, dotnum)));
	DBG_PRN("info",("--------f2int= %lld--------", f2int));
	DBG_PRN("info",("--------dotnum= %d--------", dotnum));
	if (dotnum <= MAX_DOT_NUM && f2int > MAX_AMOUNT_VALUE)
		return -1;
	
	//如果浮点数小于1，且无法存储，试着截断小数位数来进一步处理
	while(littleNum && f2int > MAX_AMOUNT_VALUE)
	{
		*f = atof(buf);
		
		f2int = (UINT64)((*f)*(pow(10, dotnum)));
		f2int += 5;
		f2int /= 10;
		
		if (f2int > MAX_AMOUNT_VALUE)
			buf[dot+dotnum] = '\0';
		dotnum--;
	} 
	
	return dotnum;
}

UINT32 FormatAmount(UINT32 dotnum, double f)
{
	UINT32 f2int = (UINT32)(f*(pow(10, dotnum+1)));
	f2int += 5;
	f2int /= 10;

	f2int = (f2int | (dotnum << 24));

	return f2int;
}

INT32 CheckFloatBit(double f)
{
    char buf[256];
    INT32 dot;
    INT32 i;
	INT32 dotnum;

	//计算小数点后有效位数
	DBG_PRN("info",("--------f = %f--------", f));
    sprintf(buf, "%.6f", f);
	DBG_PRN("info",("--------buf = %s--------", buf));
    i = strlen(buf) - 1;

    while(buf[i] == '0')
    {
        i--;
    }
	
    dot = 0;
    while(buf[dot] != '.')
    {
        dot++;
    }

	//对小数位数做四舍五入处理
    dotnum = ((i - dot) > 0)?(i - dot):0;
	DBG_PRN("info",("--------i = %d--------", i));
	DBG_PRN("info",("--------dot = %d--------", dot));
	DBG_PRN("info",("--------dotnum = %d--------", dotnum));

	return dotnum;
}

INT64 double2int(double input)
{
	INT64 tmpNum = (INT64)(input * 10.0);
	
	INT64 tmp = tmpNum;
	if (tmp < 0)
	{
		tmp = 0 - tmp;
	}
	
	if ((tmp % 10) >= 5)
	{
		tmp = tmp / 10 + 1;
	}
	else
	{
		tmp /= 10;
	}
	if(tmpNum < 0)
	{
		tmp = 0-tmp;
	}
	return tmp;
}

void getBytes(short data, UINT8* bytes){
	bytes[1] = (UINT8) (data & 0xff);
	bytes[0] = (UINT8) ((data & 0xff00) >> 8);
	return;
}

short getShort(UINT8* bytes){
	return (short) ((0xff & bytes[1]) | (0xff00 & (bytes[0] << 8)));
}


