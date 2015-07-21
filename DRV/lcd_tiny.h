
/*! \file  lcd_tiny.h
\brief     显示屏驱动  
\author   zl
*/

#ifndef LCD_TINY
#define LCD_TINY

#define LCD_DRIVER_NAME				"/dev/lcd"

#define LCD_WIDTH				192
#define LCD_HEIGHT				64
#define LCD_DATA_BIT				8	//LCD数据位宽
#ifdef __cplusplus
extern "C"
{ 
#endif
#define DISP_BUFFER_SIZE		(LCD_WIDTH*LCD_HEIGHT/LCD_DATA_BIT)

#define LCD_DISP_UNIT_MODE			1	

#define CHN_FONT_SIZE		12
#define USE_CHNFONT_FILE	1
	

/*
* Common define
*/
#define UINT32		unsigned int

/*IOCTLs define*/
#define IOCTLSETCURSOR						_IOW('v', 1, UINT32)	//set cursor position
#define IOCTLSETSTARTLINE					_IOW('v', 2, UINT32)	//set display-start line in display buffer
#define IOCTLREADSTATUS						_IOW('v', 3, UINT32)	//read lcd module status
#define IOCTLDISPLAYON						_IOW('v', 4, UINT32)	//display on
#define IOCTLDISPLAYOFF						_IOW('v', 5, UINT32)	//display off
#define IOCTLCLEARSCREEN					_IOW('v', 6, UINT32)	//clear screen
#define IOCTLCLEARPARTOFSCREEN					_IOW('v', 7, UINT32)	//clear part of screen
#define IOCTLFILLPARTOFSCREEN					_IOW('v', 8, UINT32)	//display off
#define IOCTLBKLON						_IOW('v', 9, UINT32)	//clear screen
#define IOCTLBKLOFF						_IOW('v', 10, UINT32)	//clear part of screen
#define IOCTLREDRAWSCREEN					_IOW('v', 11, UINT32)	//refresh screen

#ifndef WIN32

/*!
@brief   根据缓冲信息刷新整个LCD屏
*/
void LCDRedraw();
/*!
@brief   初始化LCD
*/
void LCD_Init(void);
/*!
@brief		填充矩形
@param [in] x0 矩形左上点横坐标
@param [in] y0 矩形左上点纵坐标
@param [in] w  矩形宽度
@param [in] h  矩形高度
@param [in] color 1显示，0不显   
*/
void LCDFillRect(int x0, int y0, int w, int h, int color);
/*!
@brief		填充矩形边框
@param [in] x0 矩形左上点横坐标
@param [in] y0 矩形左上点纵坐标
@param [in] w  矩形宽度
@param [in] h  矩形高度
@param [in] color 1显示，0不显  
*/
void LCDFillRectLine(int x0, int y0, int w, int h, int color);

/*!
@brief		填充图形信息（用于输入汉字信息等）
@param [in] x0  矩形左上点横坐标
@param [in] y0  矩形左上点纵坐标
@param [in] w   矩形宽度
@param [in] h   矩形高度
@param [in] buf 图形信息
*/
void LCDPutImage(int x0, int y0, int w, int h, char *buf);
/*!
@brief		反色填充图形信息（用于输入汉字信息等）
@param [in] x0  矩形左上点横坐标
@param [in] y0  矩形左上点纵坐标
@param [in] w   矩形宽度
@param [in] h   矩形高度
@param [in] buf 图形信息 
*/
void LCDPutImageRev(int x0, int y0, int w, int h, char *buf);
/*!
@brief		 根据像素在液晶屏中的坐标(x,y)设置缓冲中对应的位
@param [in]  x  横坐标
@param [in]  y  纵坐标
@param [in]  c 1显示，0不显示 
*/
void putpixel( int x, int y, int c );
/*!
@brief  清屏
*/
void LCDClearDisplay(void);
/*!
@brief 开背光 
*/
void LCDBlkOn();
/*!
@brief 关背光 
*/
void LCDBlkOff();

/*!
@brief		 根据像素在液晶屏中的坐标(x,y)定位缓冲中对应的位
@param [in]  x 横坐标
@param [in]  y 纵坐标
@param [in]  row  对应的行编号
@param [in]  col  对应的列编号
@param [in]  Bit  对应的字节中位编号
*/
static void LocateInBuff( int x, int y, int *row, int *col, int *Bit );
#endif

/****add by nhp****/
/*!
@brief 背光开启 
*/
void WB_LCM_BKL_ON(void);
/*!
@brief 背光关闭  
*/
void WB_LCM_BKL_OFF(void);
/*!
@brief 背光状态
@return  背光状态值				
*/
int WB_LCM_BKL_STATUS(void);
/*!
@brief 背光状态次数
@return  背光状态递增值
*/
void WB_LCM_BKL_ADD(void);
/****add by nhp****/

#ifdef __cplusplus     
}
#endif

#endif  /* LCD_TINY */
