#include "display.h"
#include "lcd_tiny.h"
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <fcntl.h>


extern unsigned char latinfont[4096];
extern unsigned char chnfont[];

static int chnfont_fd = 0;

#if USE_CHNFONT_FILE == 1

static int read_chnfont(unsigned char *buff, int offset, int size)
{
	if (chnfont_fd == 0)
	{
		chnfont_fd = open("/usr/ziku/gb18030.zk", O_RDONLY);
		if (chnfont_fd <= 0)
		{
//			chnfont_fd = open("/opt/chnfont11X12", O_RDONLY);
//			if (chnfont_fd <= 0)
//			{
//				chnfont_fd = open("/usr/local/chnfont11X12", O_RDONLY);
//			}
		}
	}

	lseek(chnfont_fd, offset, SEEK_SET);

	return read(chnfont_fd, buff, size);
}

#endif

int is_hz(unsigned char *str)
{
	 if ((str[0] >= 0x81) && (str[0] <= 0xfe) &&
		(str[1] >= 0x40) && (str[1] <= 0xfe))
	{
		return 1;
    }
    return 0;
	
}

int hz_index(unsigned char *str)
{
 int addr;
 char buff[3];

 buff[0] = str[0];
 buff[1] = str[1];
 buff[2] = 0;

 /*根据GBK码值向字库中不同的区域映射*/
 if ( ( str[0] >= 0xA1 && str[0] <= 0xF7 ) &&    //双字节1区和2区，共846字符和6768汉字，
      ( str[1] >= 0xA1 && str[1] <= 0xFE ) )    //对应了GB2312中的全部内容。
 {
     addr = (str[0] - 0xA1) * 94 + (str[1] - 0xA1);
 }
 
 else if ( ( str[0] >= 0x81 && str[0] <= 0xA0 ) &&   
         ( str[1] >= 0x40 && str[1] <= 0xFE ) )  //双字节3区，共6080汉字。
 {
     addr = (str[0] - 0x81) * 190 + (str[1] - 0x40) - (str[1] / 128) + 6768;
     addr = addr + 1410;
 }
 
 else if ( ( str[0] >= 0xAA && str[0] <= 0xFE ) &&   
         ( str[1] >= 0x40 && str[1] <= 0xA0 ) )  //双字节4区，共8160汉字。
 {
     addr = (str[0] - 0xAA) * 96 + (str[1] - 0x40) - (str[1] / 128) + 12848;
     addr = addr + 1410;
 }
 
 else if ( ( str[0] >= 0xA8 && str[0] <= 0xA9 ) &&   
         ( str[1] >= 0x40 && str[1] <= 0xA0 ) )  //双字节5区，共168字符。
 {
     addr = (str[0] - 0xA8) * 96 + (str[1] - 0x40) - (str[1] / 128) + 846;
 }

    return addr;
}

#if CHN_FONT_SIZE == 12

void str2image(unsigned char *str, int len, unsigned char *buffer)
{
//	char tmp[256];
//	memset(tmp, 0, 256);
//	memcpy(tmp, str, len);
//	printf("str = %s\n", tmp);
//	
	unsigned char *px, *py, *p;
    unsigned char *hz, *latin;
	char ch1;
	char ch2;
    int i, j;
    px = buffer;
    py = buffer + 1;
    p = buffer + 2;
    *px = len*6;
    *py = 2*6;		
    for (i=0;i<len;) 
	{
		if (is_hz(&str[i])) 
		{
#if USE_CHNFONT_FILE == 1
			unsigned char hzbuff[32];
			read_chnfont(hzbuff, 24 * hz_index(&str[i]), 24);
			hz = hzbuff;
#else
			hz = chnfont + 24 * hz_index(&str[i]);
#endif
			for (j=0;j<12;j++)
			{
				ch1 = *hz++;
				ch2 = *hz++;
				*(p + i + j*len) = ch1 & 0xFC;
				*(p + i + 1 + j*len) = ((ch1 & 0x03) << 6) | ((ch2 &0xF0) >> 2);

				ch1 = *(p + i + j*len);
				ch2 = *(p + i + 1 + j*len);
			}
			i += 2;
		} 
		else 
		{
			latin = latinfont + 12 * str[i];

			for (j=0;j<12;j++)
			{
				*(p + i + j*len) = *latin++;
			}

			i ++;
		}
    }

//	printf("OK\n");
}

#else

void str2image(unsigned char *str, int len, unsigned char *buffer)
{
	
	unsigned char *px, *py, *p;
    unsigned char *hz, *latin;
    int i, j;
    px = buffer;
    py = buffer + 1;
    p = buffer + 2;
    *px = len*8;
    *py = 2*8;
    for (i=0;i<len;) 
	{
		if (is_hz(&str[i])) 
		{
			hz = chnfont + 32 * hz_index(&str[i]);
			for (j=0;j<16;j++)
			{
				*(p + i + j*len) = *hz++;
				*(p + i + 1 + j*len) = *hz++;
			}
			i += 2;
		} 
		else 
		{
			latin = latinfont + 16 * str[i];
			for (j=0;j<16;j++)
			{
			*(p + i + j*len) = *latin++;
			}
			i ++;
		}
    }
}

#endif
unsigned char getch_pos(void)
{
  return 0;
}
unsigned char getch_usb(void)
{
  return 0;
}
unsigned char clear_key_buffer(void)
{
  return 0;
}
unsigned char clear_key_buffer_usb(void)
{
  return 0;
}
unsigned char HaveKey(void)
{
  return 0;
}

