
/*! \file  lcd_tiny_littleLcd.c
\brief     显示屏驱动  
\author   zl
*/

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>

#include <sys/types.h>


#ifndef WIN32
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <signal.h> 
#include "lcd_tiny.h"
#include "VersionConfig.h"
#include "DRVModuleHeader.h"

/**
* @brief  与显存对应的缓冲区
*/
#if (LCD_DISP_UNIT_MODE == 1)
static unsigned char disp_Buf[LCD_HEIGHT/LCD_DATA_BIT][LCD_WIDTH];
#endif

#if (LCD_DISP_UNIT_MODE == 2)
static unsigned char disp_Buf[LCD_HEIGHT][LCD_WIDTH/LCD_DATA_BIT];
#endif

// LCD设备文件标识
static int fd_lcd;


/*************************  add by nhp for bklight test  ************************/

#define outpw(port,value)     (*((UINT32 volatile *) (port))=value)
#define inpw(port)            (*((UINT32 volatile *) (port)))

#define GPIO_BA   0xFFF83000 /* GPIO Control */
#define REG_GPIO_CFG1			(GPIO_BA+0x0010)	/* GPIO port1 configuration Register  */
#define REG_GPIO_DIR1			(GPIO_BA+0x0014)	/* GPIO port1 direction control Register  */
#define REG_GPIO_DATAOUT1		(GPIO_BA+0x0018)	/* GPIO port1 data out Register  */

static unsigned char wb_lcm_blk_status=0;

void WB_LCM_BKL_ON(void)
{
	if (wb_lcm_blk_status == 0)
	{
		outpw(REG_GPIO_CFG1, inpw(REG_GPIO_CFG1)&0xFFFF3FFF);			//GPIO27
		
		outpw(REG_GPIO_DIR1, inpw(REG_GPIO_DIR1)|0x03FF0080);			//GPIO27 output

#if (POS_TYPE == POS_APE3000R)
		outpw(REG_GPIO_DATAOUT1, inpw(REG_GPIO_DATAOUT1)|0x80);
#else
		outpw(REG_GPIO_DATAOUT1, inpw(REG_GPIO_DATAOUT1)&(~0x80));		//GPIO27 low
#endif

	}
	
	wb_lcm_blk_status = 1;
}

void WB_LCM_BKL_OFF(void)
{
	if (wb_lcm_blk_status != 0)
	{
		outpw(REG_GPIO_CFG1, inpw(REG_GPIO_CFG1)&0xFFFF3FFF);			//GPIO27
		
		outpw(REG_GPIO_DIR1, inpw(REG_GPIO_DIR1)|0x03FF0080);			//GPIO27 output

#if (POS_TYPE == POS_APE3000R)
		outpw(REG_GPIO_DATAOUT1, inpw(REG_GPIO_DATAOUT1)&(~0x80)); 
#else
		outpw(REG_GPIO_DATAOUT1, inpw(REG_GPIO_DATAOUT1)|0x80);			//GPIO27 high
#endif

	}

	wb_lcm_blk_status = 0;
}

int WB_LCM_BKL_STATUS(void)
{
	return wb_lcm_blk_status;
}

void WB_LCM_BKL_ADD(void)
{
	wb_lcm_blk_status++;
}

/*************************  add by nhp for bklight test  ************************/


void LCD_Init(void)
{
	/****add by nhp*****/
	WB_LCM_BKL_ON();
    /****add by nhp*****/

	if( ( fd_lcd=open(LCD_DRIVER_NAME, O_RDWR) ) ==-1 )
	{		
		printf("open lcd driver err!\n");
		exit(-1);
	}
	memset (disp_Buf, 0, sizeof(disp_Buf));
}  

void LCDclose(void)
{		   
	if( close(fd_lcd) ==-1 )
	{		
		printf("close lcd driver err!\n");
		exit(-1);
	}
}  


void LCDClearDisplay(void)
{
	memset(disp_Buf, 0, sizeof(disp_Buf));

	LCDRedraw();
}

void LCDRedraw()
{
	int ret;
	if ( (ret = ioctl(fd_lcd, IOCTLREDRAWSCREEN, disp_Buf)) != 0 )
        {
                printf("lcd refresh err! errNo = %d\n", ret);
                exit(-1);               
        }  		
}


static void LocateInBuff( int x, int y, int *row, int *col, int *Bit )
{	
#if (LCD_DISP_UNIT_MODE == 1)
	*col = x;
	
	*row = y/LCD_DATA_BIT;
	
	*Bit = y%LCD_DATA_BIT;
#endif

/*	if ( x < 64 )
		*piBuf = 0;
	else if ( x < 128 )
		*piBuf = 1;
	else
		*piBuf = 2;

	*piByte = y/8*64 + x%64;

	*piBit = y%8;
*/
}

void putpixel( int x, int y, int c )
{
	if ( x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT )
		return;

	int row, col, Bit;
	LocateInBuff( x, y, &row, &col, &Bit );

	if ( c )
		disp_Buf[row][col] |= (1<<Bit);
	else
		disp_Buf[row][col] &= ~(1<<Bit);
}



void LCDFillRect(int x0, int y0, int w, int h, int color)
{
	int i, j;
	for (i=0;i<w ;i++)
		for (j=0;j<h ;j++)
			putpixel(x0  + i, y0 + j, color);
}



void LCDFillRectLine(int x0, int y0, int w, int h, int color)
{
	int i, j;

	for (i=0;i<w;i++)
	{
		for (j=0;j<h;j++)
		{
			if ((i==0) ||(i==(w-1))||(j==0)||(j==(h-1)))
				putpixel(x0 + i, y0 + j, color);
		}
	}
}



void LCDPutImage(int x0, int y0, int w, int h, char *buf)
{
	int i, j, k;
	char abyte;
	for (i=0;i<w ;i++)
	{
		for (j=0;j<h ;j++)
		{
#if CHN_FONT_SIZE == 16
			abyte = buf [(i + w * j) / 8];
	   	 	if (abyte & (0x80 >> (i % 8)))
#else
			abyte = buf [(i + w * j) / 6];
	   	 	if (abyte & (0x80 >> (i % 6)))
#endif
			{
				putpixel(x0  + i, y0  + j, 1);
			}
	   	 	else
			{
				putpixel(x0  + i, y0  + j, 0);
			}
		}
	}

//	LCDRedraw();
}


void LCDPutImageRev(int x0, int y0, int w, int h, char *buf)
{
	int i, j, k;
	char abyte;
	for (i=0;i<w ;i++)
	{
		for (j=0;j<h ;j++)
		{
#if CHN_FONT_SIZE == 16
			abyte = buf [(i + w * j) / 8];
	   	 	if (abyte & (0x80 >> (i % 8)))
#else
			abyte = buf [(i + w * j) / 6];
	   	 	if (abyte & (0x80 >> (i % 6)))
#endif			
			{
				putpixel(x0  + i, y0  + j, 0);
			}
			else
			{
				putpixel(x0  + i, y0  + j, 1);
			}
		}
	}

}


void LCDBlkOn()
{
	int ret;
	if ( (ret = ioctl(fd_lcd, IOCTLBKLON, disp_Buf)) != 0 )
        {
                printf("open blk err! errNo = %d\n", ret);
                exit(-1);               
        }  		
}


void LCDBlkOff()
{
	int ret;
	if ( (ret = ioctl(fd_lcd, IOCTLBKLOFF, disp_Buf)) != 0 )
        {
                printf("close blk err! errNo = %d\n", ret);
                exit(-1);               
        }  		
}
#endif


