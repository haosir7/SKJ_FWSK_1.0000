
/*! \file   vfd.h
   \brief   客显驱动
   \author 
*/
#ifndef LED_VFD_POS_H
#define LED_VFD_POS_H


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WIN32                      

#define VFD_MAGIC 'd'

#define VFD_INIT               _IO(VFD_MAGIC, 0)
#define VFD_SET_SPEED          _IOW(VFD_MAGIC, 1, int)
#define VFD_WRITE_BYTE         _IOW(VFD_MAGIC, 2, int)
#define VFD_WRITE_STR          _IOW(VFD_MAGIC, 3, int)
#define VFD_CLEAR              _IO(VFD_MAGIC, 4)

#define VFD_MAXNR  4
	
#endif	//WIN32

/*!
@brief   初始化SPI  
*/
void SPI_Init(void);


void SPI_SET_SPEED(int spd);

/*!
@brief     点亮客显  
@param[in] 客显字符   
*/
void SPI_WRITE_STR(char *str);


void SPI_WRITE_BYTE(char data);

/*!
@brief  释放客显 
*/
void SPI_Release(void);


#ifdef __cplusplus
}
#endif

#endif
