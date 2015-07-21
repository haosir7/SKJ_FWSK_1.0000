/*! \file    barcoder_encode.h
\brief    二维码打印编码函数
\author   gaoyang
 */
#ifndef  __BARCODER_ENCODE_H__   
#define __BARCODER_ENCODE_H__

#define MAIN_VERSION_MAJOR      1
#define MAIN_VERSION_MINOR      2
#define MAIN_VERSION_PATCH      0

//#if (PRINT_BIZNFO!=0)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BRAENCODE_EXTERN extern

//编码类型
#define CS_ENCODE   1
#define QR_ENCODE   2
#define DM_ENCODE   3


/*uclinux下限制
CS码 最大版本 20
QR码 最大版本 12 最大编码数据字节300
DM码 最大版本 15
*/
    

/*barcoder_encode
param
    itype:
        编码类型. 汉信码 1 QR码 2 DataMatrix(DM)码 3
    version:
        二维码版本.版本越高可容数据量越大
    ecl:
        二维码纠错等级. 汉信码和QR码为1-4四个等级. DM码没有纠错设置
    pixel:
        一个模块由pixel x pixel个方形数据矩阵组成
    imgData: 
        为空时,返回当前输入信息条件下生成图像需要的总大小
        不为空时,imgDataSize为imgData大小
    imgDataSize:
        输入imgData的字节大小
    imgWidth:
        输出生成图像的宽
    imgHeight:
        输出生成图像的高
return > 0 成功
      -1 输入参数不正确
      -2 输入版本过低
*/
BRAENCODE_EXTERN int barcoder_encode(int encode_type, int version, int ecl, int pixel, unsigned char* data, int data_len, 
              unsigned char* imgData, int imgDataSize, int* imgWidth, int* imgHeight);

#ifdef __cplusplus
}  /* end of extern "C" */
#endif /* __cplusplus */

//endif		//#if (PRINT_BIZNFO!=0)


#endif
