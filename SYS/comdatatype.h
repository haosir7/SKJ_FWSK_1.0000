#ifndef COMDATATYPE_ZYK_H
#define COMDATATYPE_ZYK_H


#define PACK_PREFIX


#ifdef WIN32

#ifndef INT64 
#define INT64	__int64
#endif

#ifndef INT32 
#define INT32	int
#endif

#ifndef INT16 
#define INT16	short
#endif

#ifndef INT8 
#define INT8	char
#endif

#ifndef UINT64 
#define UINT64	unsigned __int64
#endif

#ifndef UINT32 
#define UINT32	unsigned int
#endif

#ifndef UINT16 
#define UINT16	unsigned short
#endif

#ifndef UINT8 
#define UINT8	unsigned char
#endif

#else

#ifndef	BOOL
#define BOOL	char
#endif

#ifndef INT8 
#define INT8	char
#endif

#ifndef INT16 
#define INT16	short int
#endif

#ifndef INT32 
#define INT32	long  int
#endif

#ifndef INT64 
#define INT64	long long int
#endif

#ifndef UINT8 
#define UINT8	unsigned char
#endif

#ifndef UINT16 
#define UINT16	unsigned short int
#endif

#ifndef UINT32 
#define UINT32	unsigned int
#endif

#ifndef UINT64 
#define UINT64	unsigned long long int 
#endif

#ifndef FP32
#define FP32	float
#endif

#ifndef FP64
#define FP64	double
#endif

#endif

#ifndef SUCCESS
#define SUCCESS      					0x01	//成功
#endif

#ifndef FAILURE
#define FAILURE      					0x00	//失败
#endif

#ifndef OVERFLOW
#define OVERFLOW     					0x02	//越界
#endif

#endif  //COMDATATYPE_ZYK_H
