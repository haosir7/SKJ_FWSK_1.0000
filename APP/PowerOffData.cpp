#include "keypad.h"
#include "vfd.h"
#include "PowerOffData.h"
#include "VersionConfig.h"
#include "CaProgressBar.h"
#include "CDB.h"
#include "CGlobalArg.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


struct TYWPwOffData YW_PowerOffData;
struct TYWPwOffData *g_YW_PowerOffData = &YW_PowerOffData;  /**< 掉电保护结构体指针 */


void pw_handler(int x)
{
	bool ret;
	FILE * fp;

	DBG_PRINT(("进入掉电信号处理"));
	if ((fp= fopen(POWOFF_DIR, "w+")) == NULL)
	{
		DBG_PRINT(("can't open file"));
		DBG_PRINT(("写掉电数据错误！！！！！"));
	}
	else
	{
		if(fwrite(g_YW_PowerOffData,sizeof(TYWPwOffData), 1, fp) != 1)
		{
			DBG_PRINT(("File write erro"));
		}
		fclose(fp);
	}

// 	if (GetPowerOffFlag() == 1)
// 	{
// 		CGlobalArg::SysLog(EVENT_POWER_OFF, SUCCESS, "");
// 	}
	
	DBG_PRINT(("存储掉电数据结束"));
	
// 	ret = CDB::GetInstance()->Close();
	CDB *m_db = CDB::GetInstance();
	ret = m_db->Close();
	if ( ret != true)
	{
		DBG_PRINT(("close database false"));
	}
	else
	{
		DBG_PRINT(("SUCCESS in pw_handler"));
	}


	KeyDisable();
	SPI_Release();

	DBG_PRINT(("pw_headler end"));
	exit(0);
}

void PowOff_init()
{
	FILE *fp;
	int flag=0;
	printf("PowOff_init start \n");
	//掉电文件数据读入内存，完成后清除该文件
	if ((fp = fopen(POWOFF_DIR, "r")) != NULL)
	{
		if ( fread(g_YW_PowerOffData, sizeof(TYWPwOffData), 1 , fp) != 1)
		{
// 			CaProgressBar info("");
// 			info.SetText("读掉电数据错误！");
// 			info.ReFresh();
			BAR_DEF();
			BAR_SHOW("读掉电数据错误！");
		}
		fclose(fp);
		flag = 1;
	}
	
	
	POWOFF_DISABLE();

	// 初始化掉电驱动层
	powOff_init_drv();

	//清空文件
	if (flag == 1)
	{
		if (remove(POWOFF_DIR) != 0)
		{
			BAR_DEF();
			BAR_SHOW("清除掉电数据错误！");
		}			
	}
	
	POWOFF_ENABLE();

}

void PowOff_release()
{
	powOff_release_drv();
}

