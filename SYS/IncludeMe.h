#ifndef __INCLUDE_ME_H
#define __INCLUDE_ME_H

#include <string>
#include "comdatatype.h"


using namespace std;

//#define	OVERFLOW_AWE		3
#define FOR_DELAY(y) {for(UINT32 x = 0; x<y; x++);}

#define DBG_PRINT(x) DBG_PRN("info", x)

#define DBG_NPRINT_HEX(buf, n)		\
{\
	int row = n/8;\
	int remain = n%8;\
	for (int i=0; i<row; i++)\
		DBG_PRINT(("%02x %02x %02x %02x %02x %02x %02x %02x", \
				buf[i*8], buf[i*8+1], buf[i*8+2], buf[i*8+3], buf[i*8+4], buf[i*8+5], buf[i*8+6], buf[i*8+7]));\
	switch(remain) {\
		case 1:\
			DBG_PRINT(("%02x", buf[row*8]));\
			break;\
		case 2:\
			DBG_PRINT(("%02x %02x", buf[row*8], buf[row*8+1]));\
			break;\
		case 3:\
			DBG_PRINT(("%02x %02x %02x", \
				buf[row*8], buf[row*8+1], buf[row*8+2]));\
			break;\
		case 4:\
			DBG_PRINT(("%02x %02x %02x %02x", \
				buf[row*8], buf[row*8+1], buf[row*8+2], buf[row*8+3]));\
			break;\
		case 5:\
			DBG_PRINT(("%02x %02x %02x %02x %02x", \
				buf[row*8], buf[row*8+1], buf[row*8+2], buf[row*8+3], buf[row*8+4]));\
			break;\
		case 6:\
			DBG_PRINT(("%02x %02x %02x %02x %02x %02x", \
				buf[row*8], buf[row*8+1], buf[row*8+2], buf[row*8+3], buf[row*8+4], buf[row*8+5]));\
			break;\
		case 7:\
			DBG_PRINT(("%02x %02x %02x %02x %02x %02x %02x", \
				buf[row*8], buf[row*8+1], buf[row*8+2], buf[row*8+3], buf[row*8+4], buf[row*8+5], buf[row*8+6]));\
			break;\
		default:\
			break;\
	}\
}

#endif
