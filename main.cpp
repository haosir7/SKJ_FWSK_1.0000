#include "IncludeMe.h"
#include "VersionConfig.h"

#include "PrintDriver.h"
#include "Scanner.h"
//#include "usb_keyboard.h"
#include "powoff_protect.h"
#include "vfd.h"
#include "commonFunc.h"
#include "VersionConfig.h"
#include "APIBase.h"

#include "CaProgressBar.h"
#include "CMainFrame.h"
//#include "PowOffRecover.h"
#include "CInvDet.h"
#include "CGlobalArg.h"
#include "YWXMLGY.h"

#include "SysMainMenu.h"
#include "MaintenanceMenu.h"	//xsr
#include "LoginWin.h"
#include "SaleFunc.h"
#include "BusinessBase.h"

#include "netManager.h"

#include <pthread.h>

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#if TYPE_MODE == ZHQ_MODE
#include "APIzhqProc.h"
#endif

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#include<conio.h>
#else
#include "lcd_tiny.h"
#endif

#include "keypad.h"
#include "rtc.h"
#include "beep.h"
#include "USBMount.h"


#include "CDB.h"

#include "gZip.h"


CMainFrame g_mainFrame;

pthread_mutex_t	g_net_mutex;


#ifndef WIN32
static void alarm_handler(int sig)
{

	alarm(1);
}
#endif

int main()
{
	CaProgressBar info("");

#ifdef WIN32 
	LCD_Init();
	LCDClearDisplay();
#else
	Beep_Ini();
	Beep_On();
	LCD_Init();
	info.SetText("款机硬件初始化...");
	info.ReFresh();
	KeyEnable();
	I2C_Init();
    OpenPrinterDEV();
//	PrinterComIni();
//	InitTmplate();
	// PowOff_init();	
	SPI_Init();
#endif

#ifndef WIN32 
	if (signal (SIGALRM, alarm_handler) == SIG_ERR)
	{
		exit (1);
	}
//	alarm (1);
#endif

   int i = 0; //linux平台调试用
   int j = 0; //linux平台调试用
   int key;
   char scanEndSTR[4] = {0x0d, 0x00};

	info.SetText("数据库初始化...");
	info.ReFresh();

	pthread_mutex_init(&g_net_mutex, NULL); 
	
	CDB *m_db = CDB::GetInstance();
	m_db->Open();
	m_db->Init();
	CommonSleep(300);

	g_globalArg = CGlobalArg::GetInstance();
	g_YwXmlArg = CYWXML_GY::GetInstance();

#ifndef WIN32
//	SmartCard_Init();
#endif	

	
	UINT8 index;
	for(index = 0; index<DET_ARRAY_LEN; index++)
	{
		s_DetArray[index] = new CInvDet;
		DBG_ASSERT_EXIT((s_DetArray[index] != NULL), (" s_DetArray[index] == NULL!"));
	}//必须放在掉电恢复之前

	DBG_PRINT(("g_globalArg->InitGlobalArg() begin"));
	g_globalArg->InitGlobalArg();
	DBG_PRINT(("g_globalArg->InitGlobalArg() over"));

	string strErr("");
	g_netManager = CNetManager::GetInstance();
	if (g_netManager->init(strErr) != NET_SUCCESS)
	{ 
		DBG_PRINT(("strErr: %s", strErr.c_str()));
	}

#if TYPE_MODE ==  SKJ_MODE
	CBusinessBase::InitBusiness();
#endif

	CAPIBase::InitAPI();


//	g_globalArg->OnLineTest();  //款机启动时，不开启

	CLoginWin win;			//登录窗口
	win.Create(0,0,SCREEN_W,SCREEN_H);     
	g_mainFrame.CurrWin(&win); 
	win.ReFresh();     
	
	SPI_WRITE_STR("0.00");		//点亮客显

	pthread_t threadWrt;   /**< 接收线程 */


#if (TYPE_MODE == SKJ_MODE)
		DBG_PRINT(("已初始化，启动次线程前"));
		pthread_attr_t threadAttr;
		pthread_attr_init(&threadAttr);

		size_t defaultStackSize=0;
		pthread_attr_getstacksize(&threadAttr,&defaultStackSize);		
		pthread_attr_setstacksize(&threadAttr,1024*1024);		
		pthread_create(&threadWrt, &threadAttr, NetCommunicate, NULL);
		DBG_PRINT(("已初始化，启动次线程后"));
#else
		DBG_PRINT(("已初始化，启动次线程前"));
		pthread_create(&threadWrt, NULL, NetCommunicate, NULL);
		DBG_PRINT(("已初始化，启动次线程后"));
#endif


#if RELEASE_VER
	DBG_PRINT((" *****非税控软件版本：%.4f ********",SOFTWARE_VERSION));
#else
	DBG_PRINT((" *****非税控软件版本：%.6f ********",SOFTWARE_VERSION));
#endif


#ifdef WIN32
	while(true)
	{
		if(_kbhit())
		{
			key =_getche();
   			if (EXIT_APP_KEY == key) 
			{
				break;
   			}
			g_mainFrame.ProcEvent(key,NULL,0);
		}
	}
#else
	
  while(true)
   	{
	  
		//读扫描枪
	  
// 		if(	ScannerStateCheck() )
// 		{
// 				//DBG_PRINT(("ScannerStateCheck!"));
// 			if (ScannerPressed_mode2(scanEndSTR) == SUCCESS)
// 			{
// 				//DBG_PRINT(("ScannerPressed_mode2  SUCCESS!"));
// 			//	DBG_PRINT(("scan:  %s", ScannerCode()));
// 				WB_LCM_BKL_ON();
// 				g_mainFrame.ProcEvent(SCANNER_EVENT_KEY, NULL, 0);
// 			}
// 		}


		//若次线程有错误消息要显示
// 	//	if (1 == g_globalArg->m_ifMsg) 
// 		if ((1 == g_globalArg->m_ifMsg)&&("建立网络连接失败" != g_globalArg->m_strMsg))//过滤下消息内容，by yy 20120823
// 		{
// 			DBG_PRINT(("main.cpp弹出消息框:%s", g_globalArg->m_strMsg.c_str()));
// 			CaMsgBox::ShowMsg(g_globalArg->m_strMsg);
// 			g_globalArg->m_strMsg = "";
// 			g_globalArg->m_ifMsg = 0;
// 
// 		}

		//读键值
		if(	key = ReadKeypad())
   		{

#if (RELEASE_VER == 0)
			if (EXIT_APP_KEY == key) 
			{
				break;
   			}
#endif			
			WB_LCM_BKL_ON();
			g_mainFrame.ProcEvent(key,NULL,0);
   		}

		CommonSleep(50);
   	}
#endif
   
#ifndef WIN32 
	PowOff_release();
	KeyDisable();
	SPI_Release();
#endif

	for(index = 0; index<DET_ARRAY_LEN; index++)
	{
		if (NULL != s_DetArray[index]) 
		{
			delete s_DetArray[index];
		}		
	}

	pthread_exit(NULL);	
	return 1;

}

