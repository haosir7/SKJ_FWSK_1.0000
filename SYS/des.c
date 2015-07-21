// des.cpp : Defines the entry point for the console application.
//
#include <string.h>
#include "des.h"

#if (USE_DES_FUNC!=0)

/* INITIALIZE THE TABLES */ 
/* Table - s1 */ 
static UINT8 s1[4][16] = 
{ 
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 
}; 

/* Table - s2 */ 
static UINT8 s2[4][16] = 
{ 
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 
}; 

/* Table - s3 */ 
static UINT8 s3[4][16] = 
{ 
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 
}; 

/* Table - s4 */ 
static UINT8 s4[4][16] = 
{ 
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 
}; 

/* Table - s5 */ 
static UINT8 s5[4][16] = 
{ 
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 
}; 

/* Table - s6 */ 
static UINT8 s6[4][16] = 
{ 
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 
}; 

/* Table - s7 */ 
static UINT8 s7[4][16] = 
{ 
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 
}; 

/* Table - s8 */ 
static UINT8 s8[4][16] = 
{ 
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 
}; 

/* Table - Shift */ 
static UINT8 shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; 

/* Table - Binary */ 
static UINT8 binary[64] = 
{ 
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 
	0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 
	1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 
	1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 
}; 

static UINT8 g_des_bufout[64];
static UINT8 g_des_kwork[56];
static UINT8 g_des_worka[48];
static UINT8 g_des_kn[48];
static UINT8 g_des_buffer[64];
static UINT8 g_des_key[64]; 

/********************************************************************************
功能     ：针对源字符串生成加密后或者解密后的字符串
格式     ：INT32 des(UINT8 *source, UINT8 * dest, UINT8 * inkey, INT32 flg)
输入参数 ：UINT8 *source 源字符串
		 ：UINT8 * inkey 密钥
		 ：INT32 flg
输出参数 ：UINT8 * dest 加密或者解密后的字符串
返回值   ：0 FAILURE, 1 SUCCESS
作者     ：
日期     ：
摘要     ：
*********************************************************************************/
INT32 des(UINT8 *source, UINT8 * dest, UINT8 * inkey, INT32 flg) 
{ 
	UINT8 nbrofshift, temp1, temp2; 
	INT32 valindex; 
	INT32 i, j, k, iter; 

	/* MAIN PROCESS */ 
	/* Convert from 64-bit g_des_key into 64-byte g_des_key */ 
	for (i = 0; i < 8; i++) 
	{ 
		g_des_key[8*i]   = (((j = inkey[i]) >> 7 ) &0x01l); 
		g_des_key[8*i+1] = ( (j >> 6) &0x01l); 
		g_des_key[8*i+2] = ( (j >> 5) &0x01l); 
		g_des_key[8*i+3] = ( (j >> 4) &0x01l); 
		g_des_key[8*i+4] = ( (j >> 3) &0x01l); 
		g_des_key[8*i+5] = ( (j >> 2) &0x01l); 
		g_des_key[8*i+6] = ( (j >> 1) &0x01l); 
		g_des_key[8*i+7] = ( (j&0xffl) &0x01l);
	} 

	/* Convert from 64-bit data into 64-byte data */ 
	for (i = 0; i < 8; i++) 
	{ 
		g_des_buffer[8*i]   =  (((j = source[i]) >> 7) &0x01l ); 
		g_des_buffer[8*i+1] =  ((j >> 6) &0x01l); 
		g_des_buffer[8*i+2] =  ((j >> 5) &0x01l); 
		g_des_buffer[8*i+3] =  ((j >> 4) &0x01l); 
		g_des_buffer[8*i+4] =  ((j >> 3) &0x01l); 
		g_des_buffer[8*i+5] =  ((j >> 2) &0x01l); 
		g_des_buffer[8*i+6] =  ((j >> 1) &0x01l); 
		g_des_buffer[8*i+7] =  ((j&0xffl) &0x01l);  	
	} 

	/* Initial Permutation of Data */ 
	g_des_bufout[ 0] = g_des_buffer[57]; 
	g_des_bufout[ 1] = g_des_buffer[49]; 
	g_des_bufout[ 2] = g_des_buffer[41]; 
	g_des_bufout[ 3] = g_des_buffer[33]; 
	g_des_bufout[ 4] = g_des_buffer[25]; 
	g_des_bufout[ 5] = g_des_buffer[17]; 
	g_des_bufout[ 6] = g_des_buffer[ 9]; 
	g_des_bufout[ 7] = g_des_buffer[ 1]; 
	g_des_bufout[ 8] = g_des_buffer[59]; 
	g_des_bufout[ 9] = g_des_buffer[51]; 
	g_des_bufout[10] = g_des_buffer[43]; 
	g_des_bufout[11] = g_des_buffer[35]; 
	g_des_bufout[12] = g_des_buffer[27]; 
	g_des_bufout[13] = g_des_buffer[19]; 
	g_des_bufout[14] = g_des_buffer[11]; 
	g_des_bufout[15] = g_des_buffer[ 3]; 
	g_des_bufout[16] = g_des_buffer[61]; 
	g_des_bufout[17] = g_des_buffer[53]; 
	g_des_bufout[18] = g_des_buffer[45]; 
	g_des_bufout[19] = g_des_buffer[37]; 
	g_des_bufout[20] = g_des_buffer[29]; 
	g_des_bufout[21] = g_des_buffer[21]; 
	g_des_bufout[22] = g_des_buffer[13]; 
	g_des_bufout[23] = g_des_buffer[ 5]; 
	g_des_bufout[24] = g_des_buffer[63]; 
	g_des_bufout[25] = g_des_buffer[55]; 
	g_des_bufout[26] = g_des_buffer[47]; 
	g_des_bufout[27] = g_des_buffer[39]; 
	g_des_bufout[28] = g_des_buffer[31]; 
	g_des_bufout[29] = g_des_buffer[23]; 
	g_des_bufout[30] = g_des_buffer[15]; 
	g_des_bufout[31] = g_des_buffer[ 7]; 
	g_des_bufout[32] = g_des_buffer[56]; 
	g_des_bufout[33] = g_des_buffer[48]; 
	g_des_bufout[34] = g_des_buffer[40]; 
	g_des_bufout[35] = g_des_buffer[32]; 
	g_des_bufout[36] = g_des_buffer[24]; 
	g_des_bufout[37] = g_des_buffer[16]; 
	g_des_bufout[38] = g_des_buffer[ 8]; 
	g_des_bufout[39] = g_des_buffer[ 0]; 
	g_des_bufout[40] = g_des_buffer[58]; 
	g_des_bufout[41] = g_des_buffer[50]; 
	g_des_bufout[42] = g_des_buffer[42]; 
	g_des_bufout[43] = g_des_buffer[34]; 
	g_des_bufout[44] = g_des_buffer[26]; 
	g_des_bufout[45] = g_des_buffer[18]; 
	g_des_bufout[46] = g_des_buffer[10]; 
	g_des_bufout[47] = g_des_buffer[ 2]; 
	g_des_bufout[48] = g_des_buffer[60]; 
	g_des_bufout[49] = g_des_buffer[52]; 
	g_des_bufout[50] = g_des_buffer[44]; 
	g_des_bufout[51] = g_des_buffer[36]; 
	g_des_bufout[52] = g_des_buffer[28]; 
	g_des_bufout[53] = g_des_buffer[20]; 
	g_des_bufout[54] = g_des_buffer[12]; 
	g_des_bufout[55] = g_des_buffer[ 4]; 
	g_des_bufout[56] = g_des_buffer[62]; 
	g_des_bufout[57] = g_des_buffer[54]; 
	g_des_bufout[58] = g_des_buffer[46]; 
	g_des_bufout[59] = g_des_buffer[38]; 
	g_des_bufout[60] = g_des_buffer[30]; 
	g_des_bufout[61] = g_des_buffer[22]; 
	g_des_bufout[62] = g_des_buffer[14]; 
	g_des_bufout[63] = g_des_buffer[ 6]; 

	/* Initial Permutation of g_des_key */ 
	g_des_kwork[ 0] = g_des_key[56]; 
	g_des_kwork[ 1] = g_des_key[48]; 
	g_des_kwork[ 2] = g_des_key[40]; 
	g_des_kwork[ 3] = g_des_key[32]; 
	g_des_kwork[ 4] = g_des_key[24]; 
	g_des_kwork[ 5] = g_des_key[16]; 
	g_des_kwork[ 6] = g_des_key[ 8]; 
	g_des_kwork[ 7] = g_des_key[ 0]; 
	g_des_kwork[ 8] = g_des_key[57]; 
	g_des_kwork[ 9] = g_des_key[49]; 
	g_des_kwork[10] = g_des_key[41]; 
	g_des_kwork[11] = g_des_key[33]; 
	g_des_kwork[12] = g_des_key[25]; 
	g_des_kwork[13] = g_des_key[17]; 
	g_des_kwork[14] = g_des_key[ 9]; 
	g_des_kwork[15] = g_des_key[ 1]; 
	g_des_kwork[16] = g_des_key[58]; 
	g_des_kwork[17] = g_des_key[50]; 
	g_des_kwork[18] = g_des_key[42]; 
	g_des_kwork[19] = g_des_key[34]; 
	g_des_kwork[20] = g_des_key[26]; 
	g_des_kwork[21] = g_des_key[18]; 
	g_des_kwork[22] = g_des_key[10]; 
	g_des_kwork[23] = g_des_key[ 2]; 
	g_des_kwork[24] = g_des_key[59]; 
	g_des_kwork[25] = g_des_key[51]; 
	g_des_kwork[26] = g_des_key[43]; 
	g_des_kwork[27] = g_des_key[35]; 
	g_des_kwork[28] = g_des_key[62]; 
	g_des_kwork[29] = g_des_key[54]; 
	g_des_kwork[30] = g_des_key[46]; 
	g_des_kwork[31] = g_des_key[38]; 
	g_des_kwork[32] = g_des_key[30]; 
	g_des_kwork[33] = g_des_key[22]; 
	g_des_kwork[34] = g_des_key[14]; 
	g_des_kwork[35] = g_des_key[ 6]; 
	g_des_kwork[36] = g_des_key[61]; 
	g_des_kwork[37] = g_des_key[53]; 
	g_des_kwork[38] = g_des_key[45]; 
	g_des_kwork[39] = g_des_key[37]; 
	g_des_kwork[40] = g_des_key[29]; 
	g_des_kwork[41] = g_des_key[21]; 
	g_des_kwork[42] = g_des_key[13]; 
	g_des_kwork[43] = g_des_key[ 5]; 
	g_des_kwork[44] = g_des_key[60]; 
	g_des_kwork[45] = g_des_key[52]; 
	g_des_kwork[46] = g_des_key[44]; 
	g_des_kwork[47] = g_des_key[36]; 
	g_des_kwork[48] = g_des_key[28]; 
	g_des_kwork[49] = g_des_key[20]; 
	g_des_kwork[50] = g_des_key[12]; 
	g_des_kwork[51] = g_des_key[ 4]; 
	g_des_kwork[52] = g_des_key[27]; 
	g_des_kwork[53] = g_des_key[19]; 
	g_des_kwork[54] = g_des_key[11]; 
	g_des_kwork[55] = g_des_key[ 3]; 

	/* 16 Iterations */ 
	for (iter = 1; iter < 17; iter++) 
	{ 
		for (i = 0; i < 32; i++) 
		g_des_buffer[i] = g_des_bufout[32+i]; 

		/* Calculation of F(R, K) */ 
		/* Permute - E */ 
		g_des_worka[ 0] = g_des_buffer[31]; 
		g_des_worka[ 1] = g_des_buffer[ 0]; 
		g_des_worka[ 2] = g_des_buffer[ 1]; 
		g_des_worka[ 3] = g_des_buffer[ 2]; 
		g_des_worka[ 4] = g_des_buffer[ 3]; 
		g_des_worka[ 5] = g_des_buffer[ 4]; 
		g_des_worka[ 6] = g_des_buffer[ 3]; 
		g_des_worka[ 7] = g_des_buffer[ 4]; 
		g_des_worka[ 8] = g_des_buffer[ 5]; 
		g_des_worka[ 9] = g_des_buffer[ 6]; 
		g_des_worka[10] = g_des_buffer[ 7]; 
		g_des_worka[11] = g_des_buffer[ 8]; 
		g_des_worka[12] = g_des_buffer[ 7]; 
		g_des_worka[13] = g_des_buffer[ 8]; 
		g_des_worka[14] = g_des_buffer[ 9]; 
		g_des_worka[15] = g_des_buffer[10]; 
		g_des_worka[16] = g_des_buffer[11]; 
		g_des_worka[17] = g_des_buffer[12]; 
		g_des_worka[18] = g_des_buffer[11]; 
		g_des_worka[19] = g_des_buffer[12]; 
		g_des_worka[20] = g_des_buffer[13]; 
		g_des_worka[21] = g_des_buffer[14]; 
		g_des_worka[22] = g_des_buffer[15]; 
		g_des_worka[23] = g_des_buffer[16]; 
		g_des_worka[24] = g_des_buffer[15]; 
		g_des_worka[25] = g_des_buffer[16]; 
		g_des_worka[26] = g_des_buffer[17]; 
		g_des_worka[27] = g_des_buffer[18]; 
		g_des_worka[28] = g_des_buffer[19]; 
		g_des_worka[29] = g_des_buffer[20]; 
		g_des_worka[30] = g_des_buffer[19]; 
		g_des_worka[31] = g_des_buffer[20]; 
		g_des_worka[32] = g_des_buffer[21]; 
		g_des_worka[33] = g_des_buffer[22]; 
		g_des_worka[34] = g_des_buffer[23]; 
		g_des_worka[35] = g_des_buffer[24]; 
		g_des_worka[36] = g_des_buffer[23]; 
		g_des_worka[37] = g_des_buffer[24]; 
		g_des_worka[38] = g_des_buffer[25]; 
		g_des_worka[39] = g_des_buffer[26]; 
		g_des_worka[40] = g_des_buffer[27]; 
		g_des_worka[41] = g_des_buffer[28]; 
		g_des_worka[42] = g_des_buffer[27]; 
		g_des_worka[43] = g_des_buffer[28]; 
		g_des_worka[44] = g_des_buffer[29]; 
		g_des_worka[45] = g_des_buffer[30]; 
		g_des_worka[46] = g_des_buffer[31]; 
		g_des_worka[47] = g_des_buffer[ 0]; 

		/* KS Function Begin */ 
		if (flg) 
		{ 
			nbrofshift = shift[iter-1]; 
			for (i = 0; i < (INT32) nbrofshift; i++) 
			{ 
				temp1 = g_des_kwork[0]; 
				temp2 = g_des_kwork[28]; 
				for (j = 0; j < 27; j++) 
				{ 
					g_des_kwork[j] = g_des_kwork[j+1]; 
					g_des_kwork[j+28] = g_des_kwork[j+29]; 
				} 
				g_des_kwork[27] = temp1; 
				g_des_kwork[55] = temp2; 
			} 
		} 
		else if (iter > 1) 
		{ 
			nbrofshift = shift[17-iter]; 
			for (i = 0; i < (INT32) nbrofshift; i++) 
			{ 
				temp1 = g_des_kwork[27]; 
				temp2 = g_des_kwork[55]; 
				for (j = 27; j > 0; j--) 
				{ 
					g_des_kwork[j] = g_des_kwork[j-1]; 
					g_des_kwork[j+28] = g_des_kwork[j+27]; 
				} 
				g_des_kwork[0] = temp1; 
				g_des_kwork[28] = temp2; 
			} 
		} 

		/* Permute g_des_kwork - PC2 */ 
		g_des_kn[ 0] = g_des_kwork[13]; 
		g_des_kn[ 1] = g_des_kwork[16]; 
		g_des_kn[ 2] = g_des_kwork[10]; 
		g_des_kn[ 3] = g_des_kwork[23]; 
		g_des_kn[ 4] = g_des_kwork[ 0]; 
		g_des_kn[ 5] = g_des_kwork[ 4]; 
		g_des_kn[ 6] = g_des_kwork[ 2]; 
		g_des_kn[ 7] = g_des_kwork[27]; 
		g_des_kn[ 8] = g_des_kwork[14]; 
		g_des_kn[ 9] = g_des_kwork[ 5]; 
		g_des_kn[10] = g_des_kwork[20]; 
		g_des_kn[11] = g_des_kwork[ 9]; 
		g_des_kn[12] = g_des_kwork[22]; 
		g_des_kn[13] = g_des_kwork[18]; 
		g_des_kn[14] = g_des_kwork[11]; 
		g_des_kn[15] = g_des_kwork[ 3]; 
		g_des_kn[16] = g_des_kwork[25]; 
		g_des_kn[17] = g_des_kwork[ 7]; 
		g_des_kn[18] = g_des_kwork[15]; 
		g_des_kn[19] = g_des_kwork[ 6]; 
		g_des_kn[20] = g_des_kwork[26]; 
		g_des_kn[21] = g_des_kwork[19]; 
		g_des_kn[22] = g_des_kwork[12]; 
		g_des_kn[23] = g_des_kwork[ 1]; 
		g_des_kn[24] = g_des_kwork[40]; 
		g_des_kn[25] = g_des_kwork[51]; 
		g_des_kn[26] = g_des_kwork[30]; 
		g_des_kn[27] = g_des_kwork[36]; 
		g_des_kn[28] = g_des_kwork[46]; 
		g_des_kn[29] = g_des_kwork[54]; 
		g_des_kn[30] = g_des_kwork[29]; 
		g_des_kn[31] = g_des_kwork[39]; 
		g_des_kn[32] = g_des_kwork[50]; 
		g_des_kn[33] = g_des_kwork[44]; 
		g_des_kn[34] = g_des_kwork[32]; 
		g_des_kn[35] = g_des_kwork[47]; 
		g_des_kn[36] = g_des_kwork[43]; 
		g_des_kn[37] = g_des_kwork[48]; 
		g_des_kn[38] = g_des_kwork[38]; 
		g_des_kn[39] = g_des_kwork[55]; 
		g_des_kn[40] = g_des_kwork[33]; 
		g_des_kn[41] = g_des_kwork[52]; 
		g_des_kn[42] = g_des_kwork[45]; 
		g_des_kn[43] = g_des_kwork[41]; 
		g_des_kn[44] = g_des_kwork[49]; 
		g_des_kn[45] = g_des_kwork[35]; 
		g_des_kn[46] = g_des_kwork[28]; 
		g_des_kn[47] = g_des_kwork[31]; 
		/* KS Function End */ 

		/* g_des_worka XOR g_des_kn */ 
		for (i = 0; i < 48; i++) 
			g_des_worka[i] = g_des_worka[i] ^ g_des_kn[i]; 

		/* 8 s-functions */ 
		valindex = s1[2*g_des_worka[ 0]+g_des_worka[ 5]] [2*(2*(2*g_des_worka[ 1]+g_des_worka[ 2])+ g_des_worka[ 3])+g_des_worka[ 4]]; 
		valindex = valindex * 4; 
		g_des_kn[ 0] = binary[0+valindex]; 
		g_des_kn[ 1] = binary[1+valindex]; 
		g_des_kn[ 2] = binary[2+valindex]; 
		g_des_kn[ 3] = binary[3+valindex]; 

		valindex = s2[2*g_des_worka[ 6]+g_des_worka[11]][2*(2*(2*g_des_worka[ 7]+g_des_worka[ 8])+g_des_worka[ 9])+g_des_worka[10]]; 
		valindex = valindex * 4; 

		g_des_kn[ 4] = binary[0+valindex]; 
		g_des_kn[ 5] = binary[1+valindex]; 
		g_des_kn[ 6] = binary[2+valindex]; 
		g_des_kn[ 7] = binary[3+valindex]; 
		valindex = s3[2*g_des_worka[12]+g_des_worka[17]] [2*(2*(2*g_des_worka[13]+g_des_worka[14])+ g_des_worka[15])+g_des_worka[16]]; 
		valindex = valindex * 4; 
		g_des_kn[ 8] = binary[0+valindex]; 
		g_des_kn[ 9] = binary[1+valindex]; 
		g_des_kn[10] = binary[2+valindex]; 
		g_des_kn[11] = binary[3+valindex]; 
		valindex = s4[2*g_des_worka[18]+g_des_worka[23]] [2*(2*(2*g_des_worka[19]+g_des_worka[20])+ g_des_worka[21])+g_des_worka[22]]; 
		valindex = valindex * 4; 
		g_des_kn[12] = binary[0+valindex]; 
		g_des_kn[13] = binary[1+valindex]; 
		g_des_kn[14] = binary[2+valindex]; 
		g_des_kn[15] = binary[3+valindex]; 
		valindex = s5[2*g_des_worka[24]+g_des_worka[29]] [2*(2*(2*g_des_worka[25]+g_des_worka[26])+ g_des_worka[27])+g_des_worka[28]]; 
		valindex = valindex * 4; 
		g_des_kn[16] = binary[0+valindex]; 
		g_des_kn[17] = binary[1+valindex]; 
		g_des_kn[18] = binary[2+valindex]; 
		g_des_kn[19] = binary[3+valindex]; 
		valindex = s6[2*g_des_worka[30]+g_des_worka[35]] [2*(2*(2*g_des_worka[31]+g_des_worka[32])+ g_des_worka[33])+g_des_worka[34]]; 
		valindex = valindex * 4; 
		g_des_kn[20] = binary[0+valindex]; 
		g_des_kn[21] = binary[1+valindex]; 
		g_des_kn[22] = binary[2+valindex]; 
		g_des_kn[23] = binary[3+valindex]; 
		valindex = s7[2*g_des_worka[36]+g_des_worka[41]] [2*(2*(2*g_des_worka[37]+g_des_worka[38])+ g_des_worka[39])+g_des_worka[40]]; 
		valindex = valindex * 4; 
		g_des_kn[24] = binary[0+valindex]; 
		g_des_kn[25] = binary[1+valindex]; 
		g_des_kn[26] = binary[2+valindex]; 
		g_des_kn[27] = binary[3+valindex]; 
		valindex = s8[2*g_des_worka[42]+g_des_worka[47]] [2*(2*(2*g_des_worka[43]+g_des_worka[44])+ g_des_worka[45])+g_des_worka[46]]; 
		valindex = valindex * 4; 
		g_des_kn[28] = binary[0+valindex]; 
		g_des_kn[29] = binary[1+valindex]; 
		g_des_kn[30] = binary[2+valindex]; 
		g_des_kn[31] = binary[3+valindex]; 

		/* Permute - P */ 
		g_des_worka[ 0] = g_des_kn[15]; 
		g_des_worka[ 1] = g_des_kn[ 6]; 
		g_des_worka[ 2] = g_des_kn[19]; 
		g_des_worka[ 3] = g_des_kn[20]; 
		g_des_worka[ 4] = g_des_kn[28]; 
		g_des_worka[ 5] = g_des_kn[11]; 
		g_des_worka[ 6] = g_des_kn[27]; 
		g_des_worka[ 7] = g_des_kn[16]; 
		g_des_worka[ 8] = g_des_kn[ 0]; 
		g_des_worka[ 9] = g_des_kn[14]; 
		g_des_worka[10] = g_des_kn[22]; 
		g_des_worka[11] = g_des_kn[25]; 
		g_des_worka[12] = g_des_kn[ 4]; 
		g_des_worka[13] = g_des_kn[17]; 
		g_des_worka[14] = g_des_kn[30]; 
		g_des_worka[15] = g_des_kn[ 9]; 
		g_des_worka[16] = g_des_kn[ 1]; 
		g_des_worka[17] = g_des_kn[ 7]; 
		g_des_worka[18] = g_des_kn[23]; 
		g_des_worka[19] = g_des_kn[13]; 
		g_des_worka[20] = g_des_kn[31]; 
		g_des_worka[21] = g_des_kn[26]; 
		g_des_worka[22] = g_des_kn[ 2]; 
		g_des_worka[23] = g_des_kn[ 8]; 
		g_des_worka[24] = g_des_kn[18]; 
		g_des_worka[25] = g_des_kn[12]; 
		g_des_worka[26] = g_des_kn[29]; 
		g_des_worka[27] = g_des_kn[ 5]; 
		g_des_worka[28] = g_des_kn[21]; 
		g_des_worka[29] = g_des_kn[10]; 
		g_des_worka[30] = g_des_kn[ 3]; 
		g_des_worka[31] = g_des_kn[24]; 

		/* g_des_bufout XOR g_des_worka */ 
		for (i = 0; i < 32; i++) 
		{ 
			g_des_bufout[i+32] = g_des_bufout[i] ^ g_des_worka[i]; 
			g_des_bufout[i] = g_des_buffer[i]; 
		} 
	} /* End of Iter */ 

	/* Prepare Output */ 
	for (i = 0; i < 32; i++) 
	{ 
		j = g_des_bufout[i]; 
		g_des_bufout[i] = g_des_bufout[32+i]; 
		g_des_bufout[32+i] = j; 
	} 

	/* Inverse Initial Permutation */ 
	g_des_buffer[ 0] = g_des_bufout[39]; 
	g_des_buffer[ 1] = g_des_bufout[ 7]; 
	g_des_buffer[ 2] = g_des_bufout[47]; 
	g_des_buffer[ 3] = g_des_bufout[15]; 
	g_des_buffer[ 4] = g_des_bufout[55]; 
	g_des_buffer[ 5] = g_des_bufout[23]; 
	g_des_buffer[ 6] = g_des_bufout[63]; 
	g_des_buffer[ 7] = g_des_bufout[31]; 
	g_des_buffer[ 8] = g_des_bufout[38]; 
	g_des_buffer[ 9] = g_des_bufout[ 6]; 
	g_des_buffer[10] = g_des_bufout[46]; 
	g_des_buffer[11] = g_des_bufout[14]; 
	g_des_buffer[12] = g_des_bufout[54]; 
	g_des_buffer[13] = g_des_bufout[22]; 
	g_des_buffer[14] = g_des_bufout[62]; 
	g_des_buffer[15] = g_des_bufout[30]; 
	g_des_buffer[16] = g_des_bufout[37]; 
	g_des_buffer[17] = g_des_bufout[ 5]; 
	g_des_buffer[18] = g_des_bufout[45]; 
	g_des_buffer[19] = g_des_bufout[13]; 
	g_des_buffer[20] = g_des_bufout[53]; 
	g_des_buffer[21] = g_des_bufout[21]; 
	g_des_buffer[22] = g_des_bufout[61]; 
	g_des_buffer[23] = g_des_bufout[29]; 
	g_des_buffer[24] = g_des_bufout[36]; 
	g_des_buffer[25] = g_des_bufout[ 4]; 
	g_des_buffer[26] = g_des_bufout[44]; 
	g_des_buffer[27] = g_des_bufout[12]; 
	g_des_buffer[28] = g_des_bufout[52]; 
	g_des_buffer[29] = g_des_bufout[20]; 
	g_des_buffer[30] = g_des_bufout[60]; 
	g_des_buffer[31] = g_des_bufout[28]; 
	g_des_buffer[32] = g_des_bufout[35]; 
	g_des_buffer[33] = g_des_bufout[ 3]; 
	g_des_buffer[34] = g_des_bufout[43]; 
	g_des_buffer[35] = g_des_bufout[11]; 
	g_des_buffer[36] = g_des_bufout[51]; 
	g_des_buffer[37] = g_des_bufout[19]; 
	g_des_buffer[38] = g_des_bufout[59]; 
	g_des_buffer[39] = g_des_bufout[27]; 
	g_des_buffer[40] = g_des_bufout[34]; 
	g_des_buffer[41] = g_des_bufout[ 2]; 
	g_des_buffer[42] = g_des_bufout[42]; 
	g_des_buffer[43] = g_des_bufout[10]; 
	g_des_buffer[44] = g_des_bufout[50]; 
	g_des_buffer[45] = g_des_bufout[18]; 
	g_des_buffer[46] = g_des_bufout[58]; 
	g_des_buffer[47] = g_des_bufout[26]; 
	g_des_buffer[48] = g_des_bufout[33]; 
	g_des_buffer[49] = g_des_bufout[ 1]; 
	g_des_buffer[50] = g_des_bufout[41]; 
	g_des_buffer[51] = g_des_bufout[ 9]; 
	g_des_buffer[52] = g_des_bufout[49]; 
	g_des_buffer[53] = g_des_bufout[17]; 
	g_des_buffer[54] = g_des_bufout[57]; 
	g_des_buffer[55] = g_des_bufout[25]; 
	g_des_buffer[56] = g_des_bufout[32]; 
	g_des_buffer[57] = g_des_bufout[ 0]; 
	g_des_buffer[58] = g_des_bufout[40]; 
	g_des_buffer[59] = g_des_bufout[ 8]; 
	g_des_buffer[60] = g_des_bufout[48]; 
	g_des_buffer[61] = g_des_bufout[16]; 
	g_des_buffer[62] = g_des_bufout[56]; 
	g_des_buffer[63] = g_des_bufout[24]; 

	j = 0; 
	for (i = 0; i < 8; i++) 
	{ 
		*(dest + i) = 0x00; 
		for (k = 0; k < 7; k++) 
			*(dest + i) = ((*(dest + i)) + g_des_buffer[j+k]) * 2; 
		*(dest + i) = *(dest + i) + g_des_buffer[j+7]; 
		j += 8; 
	} 
	
	return SUCCESS;
} 

int data_convert(unsigned char *in,int in_len,unsigned char * out,char flag,unsigned char *key)
{
	int count,i,j;
	int mod=0, fill_num=0;
	
	unsigned char tembuf[9];

	if (flag == 1)
	{
		count=in_len/8;
		mod = in_len%8;
#if (DES_PADDING_MODE==DES_ECB_PADDING)
		fill_num = 8-mod;
#endif
#if (DES_PADDING_MODE==DES_ZERO_PADDING)
		fill_num = 0;
#endif		

		for(i=0;i<count;i++)
		{
			memset(tembuf,fill_num,sizeof(tembuf));
			memcpy(tembuf,in+(i*8),8);
			des(tembuf,out+(i*8),key,1);
		}
		
		memset(tembuf,fill_num,sizeof(tembuf));
		memcpy(tembuf,in+(i*8),mod);
		des(tembuf,out+(i*8),key,1);
		i++;

#if (DES_PADDING_MODE==DES_ZERO_PADDING)
		if (mod==0)
			i--;
#endif		
		
		return i*8;
	}

	else
	{
		count=in_len/8;
		for(i=0;i<count;i++)
		{
			memset(tembuf,fill_num,sizeof(tembuf));
			memcpy(tembuf,in+(i*8),8);
			des(tembuf,out+(i*8),key,0);
		}
		
		j=i-1;	//i必定>0
		fill_num = *(out+(j*8)+7);
		mod = 8-fill_num;
		memset(out+(j*8)+mod, 0, fill_num);

		return i*8-fill_num;
	}

}

#endif	//USE_DES_FUNC





