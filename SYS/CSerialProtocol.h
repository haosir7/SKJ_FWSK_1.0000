/*! \file    CSerialProtocol.h
   \brief    税控款机国标串口通讯协议类定义
   \author   zl
 */

#ifndef __SERIALPORT_COMMUNICATION_H
#define __SERIALPORT_COMMUNICATION_H

#include "SYSModuleHeader.h"
#include "comdatatype.h"
#include "SerialPort.h"

#if (USE_SERIAL_PROTOCOL!=0)


#define SERIAL_MAX_BUFFER	255	
#define CMD_MAX_NO			30
#define SERIAL_OVERTIME_MAX	1000000UL
#define SERIAL_NODATA_MAX	50000UL
#define SERIAL_ERRNUM_MAX	0x40


#define REVPACK_HEAD_LENGTH	5
#define RSPPACK_HEAD_LENGTH	4

enum ByteOrder
{
    WEB_BYTEORDER		= 0x00,
	PC_BYTEODER			= 0x01
};

//error code define
#define	SERCMD_SUCCESS				0x0				//命令接收正确
#define	SERCMD_NO_DATA				0xF0			//暂时为未收到数据
#define	SERCMD_NOT_LASTPACK			0xF1			//所收的包不是结束包
#define SERCMD_CRC_ERR				0x1f			//命令CRC错
#define	SERCMD_CMDNO_ERR			0x20			//命令号无法识别
#define SERCMD_PACKLEN_ERR			0x22			//命令长度错误
#define SERCMD_OVERTIME_ERR			0x30			//命令接受超时


#define FISCAL_CARD_RESET_ERR		0x01   		/* 税控卡上电错误（复位错） 		*/
#define USER_CARD_RESET_ERR			0x02		/* 用户卡上电错误（复位错）			*/
#define MANAGE_CARD_RESET_ERR		0x03		/* 稽查卡上电错误（复位错）			*/
#define USER_CARD_NONE_ERR			0x04		/* 无用户卡							*/
#define FISCAL_CARD_NONE_ERR		0x05		/* 无税控卡							*/
#define MANAGE_CARD_NONE_ERR		0x06		/* 无稽查卡							*/

#define FISCAL_CARD_READ_ERR		0x07		/* 读税控卡错						*/
#define USER_CARD_READ_ERR  		0x08		/* 读用户卡错  						*/
#define MANAGE_CARD_READ_ERR		0x09		/* 读管理卡错						*/
#define FISCAL_CARD_WRITE_ERR		0x0A		/* 写税控卡错						*/
#define USER_CARD_WRITE_ERR  		0x0B		/* 写用户卡错  						*/
#define MANAGE_CARD_WRITE_ERR		0x0C		/* 写稽查卡错						*/
#define FISCAL_CARD_ILLEGAL_ERR		0x0d		/* 非法税控卡						*/	
#define USER_CARD_ILLEGAL_ERR		0x0e		/* 非法用户卡						*/
#define MANAGE_CARD_ILLEGAL_ERR		0x0f		/* 非法稽查卡						*/

#define	SYSTEM_NO_INIT				0x10		/* 没初始化							*/
#define  FISCALCARD_CODE_NOT_MATCH_MACHINE_CODE     0x15    //税控卡的注册号与税控器的注册号不一致
#define  UNVAILD_TAX_ITEM                           0x19    //非法税种税目
#define  DECLARE_DATE_BIGGER_THAN_CURRENT_DATE      0x1A    //申报截至日期大于当前日期
#define  DECLARE_DATE_LITTLE_THAN_LAST_DATE         0x1B    //申报截至日期小于上次申报日期
#define  SYS_INITIALIZED                            0x1E    //税控器已初始化不能再次初始化
#define  TAX_MEM_ERR                                0x25    //税控存储器异常
#define  FISCAL_CARD_INITIALIZED                    0x27    //税控卡已注册

#define	NO_PRINT_TEMPLATE_ERR				0x40		/* 无打印模板 */
#define ERR_CONNECT_PACK					0x41		/* 握手包错误 */


/*!
@struct rev_cmdStruct
@brief 串口命令接收结构体
*/
typedef struct
{
	UINT8 Len;
	UINT8 PackNo;
	UINT8 Para[SERIAL_MAX_BUFFER*2];
}rev_cmdStruct;


/*!
@struct rsp_cmdStruct
@brief 串口命令应答结构体。与rev_cmdStruct相同
@see rev_cmdStruct
*/
typedef rev_cmdStruct rsp_cmdStruct;


/*!
@struct _Ser_ErrMsg
@brief 错误信息结构体
*/
typedef struct _Ser_ErrMsg
{
	INT32 errNo;
	string errMsg;
	
	_Ser_ErrMsg(INT32 No, const char *msg){
		errNo = No;
		errMsg = msg;
	};
}Ser_ErrMsg;


/*!
@class CSerialProtocol
@brief 国标串口通讯协议类
*/
class CSerialProtocol
{														 
public:
	rev_cmdStruct m_cRevCmd;			/**< 接收命令结构	*/
	rev_cmdStruct *m_revCmd;			/**< 接收命令结构指针	*/
	rsp_cmdStruct m_cRspCmd;			/**< 应答命令结构	*/
	rsp_cmdStruct *m_rspCmd;			/**< 应答命令结构指针	*/

private:
	UINT8 m_revBuf[SERIAL_MAX_BUFFER];		/**< 接收缓存	*/
	UINT8 m_rspBuf[SERIAL_MAX_BUFFER];		/**< 应答缓存	*/

	INT16 m_CmdNO;					/**< 接收命令序号	*/
	
	UINT8 m_RegCmdSum;				/**< 当前总共注册的命令数	*/

	typedef void (*ONCMD_FUN)(void *obj, CSerialProtocol *serComm);		/**< 命令处理函数指针，用于回调	*/

	/*!
	@struct _cmdReg[]
	@brief 命令数组，用于存放已注册的命令信息
	*/
	struct _cmdReg{
		void *obj;
		INT16 CmdNO;
		ONCMD_FUN OnCmdFUN;
	}m_cmdReg[CMD_MAX_NO];			

	SerialPort *m_Serial, m_cSerial;	/**< 串口操作句柄	*/

	static Ser_ErrMsg m_errMsg[];		/**< 错误信息	*/

	UINT8 m_fill_count;				/**< 当前已填写的参数字节数	*/
	UINT8 m_get_count;				/**< 当前已取得的参数字节数	*/

public:
	/*!
	@brief 注册一个串口命令	
	@param[in] obj   调用者指针
	@param[in] CmdNO   命令序号
	@param[in] cmdFUN   命令相应函数
	@return  SUCCESS-成功; FAILUR-失败
	*/
	INT8 Add_CMD(void *obj, INT16 CmdNO, ONCMD_FUN cmdFUN);

	/*!
	@brief 等待接收一个数据包
	@return  SERCMD_SUCCESS：接收完成；SERCMD_NOT_LASTPACK：接收完成，但不是最后一包；
			 其他：接收失败
	*/
	INT8 Wait_Cmd_Pack();

	/*!
	@brief 等待接收一个NP包
	@return  SERCMD_SUCCESS：接收完成；
			 其他：接收失败
	*/
	INT8 Wait_NP_Pack();

	/*!
	@brief 应答一个无参数的OK包
	*/
	void Rsp_OK();

	/*!
	@brief 应答一个ERR包
	@param[in] err   错误编号
	*/
	void Rsp_ERR(UINT8 err);

	/*!
	@brief 应答一个WT包
	*/
	void Rsp_WT();

	/*!
	@brief 应答一个NP包
	*/
	void Rsp_NP();

	/*!
	@brief 应答一个带参数的OK包
	@param[in] packNo   包序号
	@param[in] packLen  包长度
	*/
	void Rsp_OK(UINT8 packNo, UINT8 packLen);

	/*!
	@brief 应答一个简化的ER包
	*/
	void Rsp_ER();
	

	/*!
	@brief 向应答包中填入一个单字节参数
	@return  SUCCESS-成功; FAILUR-失败
	*/
	INT8 FillParament(UINT8 byte);

	/*!
	@brief 向应答包中填入一个整形参数
	@param[in] x   整型值
	@param[in] len  长度，单位字节
	@param[in] order   字节序
	@return  SUCCESS-成功; FAILUR-失败
	*/
	INT8 FillParament(INT64 x, UINT8 len, bool order);

	/*!
	@brief 向应答包中填入一段字节流参数
	@param[in] buf   字节流
	@param[in] len  长度，单位字节
	@return  SUCCESS-成功; FAILUR-失败
	*/
	INT8 FillParament(UINT8 *buf, UINT8 len);

	/*!
	@brief 向应答包中填入一段string类型的字节流参数
	@param[in] str   字节流
	@param[in] len  长度，单位字节
	@return  SUCCESS-成功; FAILUR-失败
	*/
	INT8 FillParament(string str, UINT8 len);

	/*!
	@brief 从接收包中获得一个单字节参数
	@return  参数值
	*/
	UINT8 GetParament_BYTE();

	/*!
	@brief 从接收包中获得一个整形参数
	@param[in] len  长度，单位字节
	@param[in] order   字节序
	@return  参数值
	*/
	INT64 GetParament_INT(UINT8 len, bool order);

	/*!
	@brief 从接收包中获得一个BCD型参数
	@param[in] len  长度，单位字节
	@return  参数值
	*/
	INT64 GetParament_BCD(UINT8 len);

	/*!
	@brief 从接收包中获得一段字节流参数
	@param[in] len  指定获取字节流的长度，单位字节
	@param[out] buf   输出字节流
	*/
	void GetParament_BUF(UINT8 len, UINT8 *buf);

	/*!
	@brief 初始化一个串口
	@param[in] portnr  端口号
	@param[in] baud  波特率
	@return  SUCCESS-成功; FAILUR-失败
	*/
	bool InitPort (int portnr, int baud);

	/*!
	@brief 释放当前串口
	@return  SUCCESS-成功; FAILUR-失败
	*/
	bool ReleasePort ();

	/*!
	@brief 将错误号转换为错误描述
	@param[in] errNo  错误号
	@param[out] errStr   错误描述
	*/
	void getErrMsg(string &errStr, UINT8 errNo);

	/*!
	@brief 延时函数（不准确）
	@param[in] Msecs  单位，大约为1毫秒
	*/
	void ser_delay(UINT32 Msecs);

	
	/*!
	@brief 构造函数
	*/
	CSerialProtocol();

	/*!
	@brief 析构函数
	*/
	~CSerialProtocol();

private:
	// function for send & rev
	INT8 Rev_Pack(const char *pack_start);
	void Send_Pack();
	
	// function for serial operate
	INT32 GetReceiveCount();
	UINT8 GetChar();
	void PutChar(UINT8 *byte);

	//common function
	UINT16 Cal_CRC(UINT8 *ptr, UINT8 len);
};


#endif	//USE_SERIAL_PROTOCOL

#endif	//__SERIALPORT_COMMUTION_H


