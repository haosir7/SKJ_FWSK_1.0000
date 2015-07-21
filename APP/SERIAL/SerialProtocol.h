#ifndef __SERIALPORT_PROTOCOL_H
#define __SERIALPORT_PROTOCOL_H

#include <string>
#include <string.h>
#include <stdio.h>
#include "SerialConfig.h"
#include "arithmetic.h"
#include "SerialPort.h"

using namespace std;

class SerialProtocol
{	
private:
	//INT32 m_devfd;
	UINT32 m_fill_count;
	UINT32 m_get_count;
	UINT8 packageNo;
	UINT8 headLen;
	UINT8 packageLen;

	UINT8 m_sendBuf[SERIAL_BUFFER_MAX_LEN];
	UINT8 m_revBuf[SERIAL_BUFFER_MAX_LEN];

	//为单例模式设计
	SerialProtocol();  
	SerialProtocol(const SerialProtocol &);  
	SerialProtocol& operator = (const SerialProtocol &); 
	~SerialProtocol();

	SerialPort serial, *pSerial;

	void resetSendStruct();
	void resetRspStruct();

	// function for send & rev
	UINT8 Rev_Pack(string &strErr);
	UINT8 Send_Pack();
	UINT8 Send_Pack(UINT16 len);
//	UINT8 Wait_NP_Pack();
//	UINT8 Wait_OK_Pack();
	UINT8 Wait_Next_Cmd_Pack(string &strErr);

	//common function
	UINT16 Cal_CRC(UINT8 *ptr, UINT16 len);

protected:
	UINT8  businessData[BUSINESS_DATA_BUF_MAX_LEN];
	UINT8  businessType;
	bool   newCmd;

public:
	send_cmdStruct *m_sendCmd, m_cSendCmd;
	rsp_cmdStruct *m_rspCmd, m_cRspCmd;

	//为单例模式设计
	static SerialProtocol* getInstance(){
		static SerialProtocol instance;   //局部静态变量  
		return &instance; 
	}

	// function for serial init
	bool InitPort(INT8 *devpath, int baud);
	bool ReleasePort ();

	//inline INT32 getSerialfd() {return m_devfd;};
	UINT8 sendData(UINT8 type, UINT8 cmd, string &strErr);
	UINT8 revData(string &strErr);
	void resetAll();
	void clearSerial();

	void Rsp_OK();
// 	void Rsp_ERR(UINT8 err);
// 	void Rsp_ERR(string errstr);
	void Rsp_WT();
	void Rsp_NP(UINT8 packageNo);

	//function for fill rsp buff
	INT8 FillParament(UINT8 byte);
	INT8 FillParament(INT64 x, UINT8 len, bool order);
	INT8 FillParament(UINT8 *buf, UINT8 len);
	INT8 FillParament(string str, UINT32 maxlen);

	void ser_delay(UINT32 Msecs);
};

#endif




