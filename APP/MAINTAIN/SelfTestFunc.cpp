/****************************************************************************
文件名           ：SelfTestFunc.cpp
功能             ：awe4000r销售程序中机器自检的底层函数。
起始日期         ：2008-3-19 
作者             ：Yu Yan     
****************************************************************************/

#include "SelfTestFunc.h"
#include "SerialPort.h"
#include "keypad.h"
#include "beep.h"
#include "USBMount.h"
#include "arithmetic.h"
#include"commonFunc.h"
#ifndef WIN32
#include "lcd_tiny.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#define USB_DIR "/mnt"
#define USB_DIR2 "/mnt1"

#if (POS_TYPE == POS_APE5020R)
#define UART_NO		UART3
#define UART_DIR	UART3_DEV_NAME
#else
#define UART_NO		UART2
#define UART_DIR	UART2_DEV_NAME
#endif

UINT8 g_buf[400];

//CPU自检
UINT8 CPUTest(void)
{
	UINT8 i;
	UINT8 sum = 0;
	DBG_PRINT(("CPUTest(void)"));
	
	for (i=0; i<255; i++)
		sum += 1;
		
	DBG_PRINT(("sum is %d", sum));
	if (sum == 255)
	{
		return SUCCESS;
	}
	
	return FAILURE;
}

//Flash
UINT8 FlashTest(void)
{
	return SUCCESS;
}

//RAM
UINT8 RAMTest(void)
{
	return SUCCESS;
}

//Beep
UINT8 BeepTest(void)
{
	Beep_On();
	return SUCCESS;
}

//moneyBox
UINT8 MoneyBoxTest(void)
{
	MoneyBox_Open();
	return SUCCESS;
}

//COM0自检
UINT8 COM0Test(void)
{
	UINT8 res;
	UINT8 chPut, chGet, *pGet;
	SerialPort *pSerial, serialport;
	DBG_PRINT(("COM0Test(void)"));

//	pSerial = new SerialPort;
	pSerial = &serialport;
	SerialPortContruct(pSerial);

	res = pSerial->InitPort(pSerial, "/dev/uart0", 0, 9600);
	if (res == FAILURE) 
	{
//		delete pSerial;
		pSerial = NULL;
		return FAILURE;
	}
	
	chPut = 'a';
	res = SUCCESS;
	pSerial->PutChar(pSerial, &chPut);
#ifndef	WIN32
//	usleep(100000);
	CommonSleep(100);
#endif
	
	INT32 nCount = pSerial->GetReceiveCount(pSerial);
	if(nCount!=0)
	{
		chGet = pSerial->GetChar(pSerial);
		//res = pSerial->ClosePort(pSerial);
		DBG_PRINT(("COM0Test: GetChar !"));	
		if (chPut!=chGet)
		{
			res = FAILURE;
			DBG_PRINT(("COM0Test: GetChar Failure!"));
		}
	}
	else
	{
		res = FAILURE;
		DBG_PRINT(("COM0Test: GetChar Failure!"));
	}
	
pSerial->ClosePort(pSerial);
	DBG_PRINT(("COM0Test(void)"));	
//	delete pSerial;
	pSerial = NULL;
	return res;
	
}

//COM2自检
UINT8 COM2Test(void)
{
	UINT8 res;
	UINT8 chPut, chGet;
	SerialPort *pSerial, serialPort;
	DBG_PRINT(("COM2Test(void)"));

//	pSerial = new SerialPort;
	pSerial = &serialPort;
	SerialPortContruct(pSerial);

	res = pSerial->InitPort(pSerial, UART_DIR, UART_NO, 9600);
	if (res == FAILURE) 
	{
//		delete pSerial;
		pSerial = NULL;
		DBG_PRINT(("COM2Test: InitPort Failure!"));
		return FAILURE;
	}
	DBG_PRINT(("COM2Test: InitPort Success!"));
	
	chPut = 'a';
	res = SUCCESS;
	pSerial->PutChar(pSerial, &chPut);
#ifndef	WIN32
//	usleep(100000);
	CommonSleep(100);
#endif

	INT32 nCount = pSerial->GetReceiveCount(pSerial);
	if(nCount!=0)
	{
		chGet = pSerial->GetChar(pSerial);
		DBG_PRINT(("COM2Test: GetChar !"));	
		if (chPut!=chGet)
		{
			res = FAILURE;
			DBG_PRINT(("COM2Test: GetChar Failure!"));
		}
	}
	else
	{
		res = FAILURE;
		DBG_PRINT(("COM2Test: GetChar Failure!"));
	}
	
		
	pSerial->ClosePort(pSerial);
//	delete pSerial;
	pSerial = NULL;
	return res;	
}

//LCD


UINT8 LCDTest(void)
{
	DBG_PRINT(("LCDClearDisplay()"));	
	INT32 i = 0;	
	UINT8 yPos = 0;
	string strInfo;

	memset(g_buf,0,400);
	LCDClearDisplay();
	
#ifndef WIN32
//	usleep(600000);
	CommonSleep(600);
#endif

	strInfo = "北京市海淀区航天信息股份有限公司";
	for (i=0; i<5; i++)
	{
		str2image((UINT8*)(strInfo.c_str()), 32, g_buf);
		LCDPutImage(0, yPos, g_buf[0], 12, (INT8*)(g_buf+2)); 
		yPos +=13; 
	}
	LCDRedraw();
	DBG_PRINT(("LCDRedraw()"));
	
#ifndef WIN32
//	usleep(600000);
	CommonSleep(600);
#endif

	yPos = 0;
	strInfo = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEF";
	for (i=0; i<5; i++)
	{
		str2image((UINT8*)(strInfo.c_str()), 32, g_buf);
		LCDPutImage(0, yPos, g_buf[0], 12, (INT8*)(g_buf+2)); 
		yPos +=13; 
	}
	LCDRedraw();
	DBG_PRINT(("LCDRedraw()"));

#ifndef WIN32
//	usleep(600000);
	CommonSleep(600);
#endif

	yPos = 0;
	strInfo = "01234567890123456789012345678901";
	for (i=0; i<5; i++)
	{
		str2image((UINT8*)(strInfo.c_str()), 32, g_buf);
		LCDPutImage(0, yPos, g_buf[0], 12, (INT8*)(g_buf+2)); 
		yPos +=13; 
	}
	LCDRedraw();
	DBG_PRINT(("LCDRedraw()"));

#ifndef WIN32
//	usleep(600000);
	CommonSleep(600);
#endif
	
	LCDClearDisplay();
	return SUCCESS;
}


//keyboard
UINT8 KeyboardTest(void)
{

	INT32 key;
	INT8 value[256];

	memset((void*)value, 0, sizeof(value));	
	memset((void*)g_buf, 0, sizeof(g_buf));	
	LCDClearDisplay();//清屏
	
	sprintf(value, "请按任意键：");
	str2image((UINT8*)value, 16, g_buf);
	LCDPutImage(4, 0, g_buf[0], 12, (INT8*)(g_buf+2)); 

	memset((void*)g_buf, 0, sizeof(g_buf));	
	sprintf(value, "ESC键返回");
	str2image((UINT8*)value, 10, g_buf);
// 	LCDPutImage(120, 52, g_buf[0], 12, (INT8*)(g_buf+2)); 
	LCDPutImage(68, 52, g_buf[0], 12, (INT8*)(g_buf+2)); 
	LCDRedraw();
	
	memset((void*)g_buf, 0, sizeof(g_buf));	
	while(true)
	{
		if (key = ReadKeypad())  //获取键盘值
		{
			WB_LCM_BKL_ON();
			DBG_PRINT(("key is 0x%x", key));
			
			if (RETURN_MAIN_MENU == key) 
			{
				return SUCCESS;
   			}
			//显示按键值
			memset((void*)value, 0, sizeof(value));	
			memset((void*)g_buf, 0, sizeof(g_buf));	
			sprintf(value, "%03d", key);
			str2image((UINT8*)value, strlen(value), g_buf);
// 			LCDPutImage(84, 26, g_buf[0], 12, (INT8*)(g_buf+2)); 
			LCDPutImage(54, 26, g_buf[0], 12, (INT8*)(g_buf+2)); 
			LCDRedraw();
		}
	}
	return SUCCESS;		
}

UINT8 USBTest(void)
{
	UINT8 ret = SUCCESS;
	string strCmd = "";
	FILE * fp;
	
	DBG_PRINT(("USBTest(void)"));
	
	//挂U盘分区
	//	strCmd = "mount -t vfat /dev/sdb1 ";
	//	strCmd.append(USB_DIR);
	//	system(strCmd.c_str());

#if (POS_TYPE == POS_APE5020R)
	ret = UsbDiskMount2(NULL);
	if (SUCCESS != ret)
	{
		DBG_PRINT(("挂载U盘1和2失败 ret = %u", ret));
		//		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount2();
		return FAILURE;
	}
#else
	ret = UsbDiskMount(NULL);
	if (SUCCESS != ret)
	{
		DBG_PRINT(("挂载U盘失败 ret = %u", ret));
//		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return FAILURE;
	}
#endif	
	strCmd = USB_DIR;
	strCmd.append("/yu");
	if ((fp = fopen(strCmd.c_str(), "w")) == NULL)
	{
		ret = FAILURE;
	}
	fclose(fp);

#if (POS_TYPE == POS_APE5020R)
	
	strCmd = USB_DIR2;
	strCmd.append("/yan");
	if ((fp = fopen(strCmd.c_str(), "w")) == NULL)
	{
		ret = FAILURE;
	}
	fclose(fp);
      UsbDiskUnMount2();
#else
    UsbDiskUnMount();
#endif	
	//卸U盘分区
	//	system("umount -t vfat /dev/sdb1");
	
	return ret;	
}
//UINT8 ICCardTest(void)
//{
//	DBG_PRINT(("ICCardTest(void)"));
//
// }


