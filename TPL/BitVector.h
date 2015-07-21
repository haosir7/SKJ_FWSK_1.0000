/*! \file    BitVector.h
\brief    二维码打印底层函数
\author   gaoyang
 */

#ifndef BITVECTOR_H
#define BITVECTOR_H

#include "TPLModuleHeader.h"

#if (PRINT_BIZNFO!=0)

#ifdef __cplusplus
extern "C"
{
#endif
	typedef unsigned char BitWord;
	enum { BITWORD_SIZE = (unsigned)sizeof(BitWord) * 8 };
    extern unsigned char bitVectorBuffer[10*1024];
	//bitvector
	typedef struct BITVECTOR
	{
		unsigned char *m_bits;
		int m_size;
		int m_idProjector[2];
		int m_maxCoordinates[2];

		//根据id读取位值
		int (* readByID)(void *pHandle,int id);
		//根据坐标读取位值
		int (* read)(void *pHandle,int row,int col);
		//根据id设置位值
		void (* writeByID)(void *pHandle, int id, int value);
		//根据坐标设置位值
		void (* write)(void *pHandle, int row,int col, int value);
		//设置位所有位
		void (* setAll)(void *pHandle,int value);
		//初始化所有位为值0
		void (* init)(void *pHandle,int row,int col);
		//检测位图是否包含id
		int (* containsByID)(void *pHandle,int id);
		//检测位图是否包含坐标
		int (* contains)(void *pHandle,int row,int col);
		//从坐标得到ID
		int (* getIdFromVector)(void *pHandle, int row,int col);
		//从ID得到坐标
		void (* setVectorFromId)(void *pHandle, int id, int row,int col);

	}BitVector;


#define BITVECTOR_DEFAULT {bitVectorBuffer,\
	0,\
	{0,0},\
	{0,0},\
	readByID_Fun,\
	read_Fun,\
		writeByID_Fun,\
		write_Fun,\
		setAll_Fun,\
		init_Fun,\
		containsByID_Fun,\
		contains_Fun,\
		getIdFromVector_Fun,\
		setVectorFromId_Fun}

		//根据id读取位值
		int readByID_Fun(void *pHandle,int id);
		//根据坐标读取位值
		int read_Fun(void *pHandle,int row,int col);
		//根据id设置位值
		void writeByID_Fun(void *pHandle, int id, int value);
		//根据坐标设置位值
		void write_Fun(void *pHandle, int row,int col, int value);
		//设置位所有位
		void setAll_Fun(void *pHandle,int value);
		//初始化所有位为值0
		void init_Fun(void *pHandle,int row,int col);
		//检测位图是否包含id
		int containsByID_Fun(void *pHandle,int id);
		//检测位图是否包含坐标
		int contains_Fun(void *pHandle,int row,int col);
		//获取id位置的dimension坐标
		int getCoord_Fun(void *pHandle,int id, int dimenson);
		//从坐标得到ID
		int getIdFromVector_Fun(void *pHandle, int row,int col);
		//从ID得到坐标
		void setVectorFromId_Fun(void *pHandle, int id, int row,int col);


		int BitmapTranse_old(unsigned char * imgData, int imgWidth, int imgHeight, int pixelNum, unsigned char * output, int *outWidth,int *outHeight);
		int BitmapTranse(unsigned char * imgData, int imgWidth, int imgHeight, unsigned char * output, int *outWidth,int *outHeight, double length);			
			
#ifdef __cplusplus
};
#endif

#endif	//PRINT_BIZNFO

#endif