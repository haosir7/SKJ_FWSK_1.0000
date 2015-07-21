#ifndef __SERIAL_CONFIG_H
#define __SERIAL_CONFIG_H

//#include "error_code.h"

#define SERIAL_DEVICE						"/dev/uart3"
#define SERIAL_BAUDRATE						9600

#define SERIAL_BUFFER_MAX_LEN				65535UL							//串口Buf最大长度	
#define BUSINESS_DATA_BUF_MAX_LEN			10*1024						//业务缓冲区长度

#define SERIAL_PACKAGE_ZC_MAX_LEN			255								//总参协议数据包最大长度
#define SERIAL_PACKAGE_AISINO_MAX_LEN		SERIAL_BUFFER_MAX_LEN			//航信自定义数据包最大长度

#define SERIAL_OVERTIME_MAX					30000000UL
#define SERIAL_NODATA_MAX					50000UL
#define SERIAL_ERRNUM_MAX					0x40

#define CRC_LEN								2								//CRC校验位长度
#define SENDPACK_ZC_HEAD_LENGTH				5								//总参协议接收数据包包头长度
#define RSPPACK_ZC_HEAD_LENGTH				4								//总参协议响应数据包包头长度
#define SENDPACK_AISINO_HEAD_LENGTH			6								//航信协议接收数据包包头长度
#define RSPPACK_AISINO_HEAD_LENGTH			5								//航信协议响应数据包包头长度

enum ProtocolType
{
	ZC_PROTOCOL			= 0x10,				//总参协议包头类型
	XML_PROTOCOL		= 0x20,				//XML数据包头类型
	CA_PROTOCOL			= 0x30,				//CA业务数据包头类型
	PRN_PROTOCOL		= 0x40				//打印数据包头类型
};

typedef struct
{
	UINT8 cmdType;		//命令类型
	UINT8 cmdNo;		//命令号
	UINT8 PackNo;		//包序号
	UINT16 cmdLen;		//整包长度
	UINT32 dataLen;		//业务数据长度，累加
	UINT8 *sendData;
} send_cmdStruct;

typedef struct
{
	UINT8 head[2];
	UINT8 PackNo;
	UINT8 multiPackages;
	UINT16 cmdLen;
	UINT32 dataLen;		//业务数据长度，累加
	UINT8 *rspData;
} rsp_cmdStruct;

enum ByteOrder
{
	WEB_BYTEORDER		= 0x00,
	PC_BYTEODER			= 0x01
};

//error code define
#define	SERCMD_SUCCESS				0x00			//命令接收正确
#define	SERCMD_NO_DATA				0xF0			//暂时为未收到数据
#define	SERCMD_NOT_LASTPACK			0xF1			//所收的包不是结束包
#define SERCMD_HEAD_ERR				0x11			//包头数据格式错误
#define SERCMD_HEAD_PARA_ERR		0x12			//包头数据参数错误
#define SERCMD_CRC_ERR				0x1f			//命令CRC错
#define	SERCMD_CMDNO_ERR			0x20			//命令号无法识别
#define SERCMD_PACKLEN_ERR			0x22			//命令长度错误
#define SERCMD_OVERTIME_ERR			0x30			//命令接受超时

#define BUSINESS_COMMON_TYPE		0x10			//总参串口通信类型
#define BUSINESS_XML_TYPE			0x20			//xml数据传输类型
#define BUSINESS_CA_TYPE			0x30			//CA数据通信类型
#define BUSINESS_PRINT_TYPE			0x40			//打印数据传输类型

#define NP_PACKAGE_TYPE				0x01
#define OK_PACKAGE_TYPE				0x02
#define ER_PACKAGE_TYPE				0x03
#define WT_PACKAGE_TYPE				0x04

#define WAIT_TIME					300				//串口等待时间 秒
#define REV_TIME					100		

#define TIME_MS						30			

#endif
