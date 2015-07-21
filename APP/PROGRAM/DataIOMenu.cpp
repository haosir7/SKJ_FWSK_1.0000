/*! \file    DataIOMenu.cpp
   \brief    awe4000r数据导入导出菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-8-13
 */


#include "DataIOMenu.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CGlobalArg.h"
#include "DataIOFunc.h"
#include "USBMount.h"
#include "SysArgMac.h"
#include "CSysArg.h"
#include "SysArgMenu.h"

CDataIOMenu::CDataIOMenu():CMultiBtnCommonWin(6,2)
{
}

int CDataIOMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,DATA_IO_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.模板导入");
	SetTitle(2,"B.模板导出");
	SetTitle(3,"C.商品导入");
	SetTitle(4,"D.商品导出");
	SetTitle(5,"E.客户导入");
	SetTitle(6,"F.客户导出");
	
	return 1;
}

void CDataIOMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 j=0, res;
	char tmp[64];
	string file_S = PRINT_TEMPLATE_FILE_NAME;
	string file_D = "/mnt/mb";
	string cmd = "";
	
	CaProgressBar proBar("");
	proBar.SetText("模板导入中..."); 
	proBar.ReFresh();
	
	//挂载U盘
	res = UsbDiskMount(NULL);
	if (SUCCESS != res)
	{
		DBG_PRINT(("挂载U盘失败 res = %u", res));
		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return;
	}
	
	//依次读取模板文件，并拷贝至UPAN
	system("/rw");
	for (j=0; j<PRINT_TEMPLATE_NUMBER; j++)
	{
		file_S = "/mnt/mb";
		file_D = PRINT_TEMPLATE_FILE_NAME;
		
		//读取序号为j的模板，如果该序号没有对应模板则中止发送
		sprintf(tmp, "%02u.bin", j+1);
		file_S += tmp;
		
		sprintf(tmp, "%02u.txt", j+1);
		file_D += tmp;
		
		cmd = "cp " + file_S + " " + file_D;
		DBG_PRINT(("cmd: %s", cmd.c_str()));
		
		if (system(cmd.c_str()) != 0)
			break;
	}	
	
	UsbDiskUnMount();
	sprintf(tmp, "共导入%u个模板", j);
	CaMsgBox::ShowMsg(tmp);
	CGlobalArg::SysLog(EVENT_UPDATE_TEMPLATE, SUCCESS, tmp);
	// 增加判断条件，如果导入模板为0，不在设置1号模板为当前模板
	if(0 != j)
	{
	//设置1号模板为当前模板
	INT32 errorcode;
	string sqlstr;
	CSysArg sysArg;
	
	g_globalArg->m_curTemplateNo = 1;
	
	//修改系统参数区
	sysArg.m_vInt = g_globalArg->m_curTemplateNo;
	sprintf(tmp, "update SYSARG set V_INT = %u where SA_ID = %d", 
		sysArg.m_vInt, SYS_CUR_TEMPLATE_NO );
	DBG_PRINT(("tmp = %s", tmp));
	sqlstr=tmp;
	sysArg.SetSQL(sqlstr);
	errorcode = sysArg.ExecuteSQL();
	if (errorcode != SQLITE_OK)
	{
		return;
	}
	g_globalArg->InitSysArg();
	
	CSysArgMenu *pWin;
	pWin = (CSysArgMenu *)(((CMainFrame *)m_pFrame)->IsWinExist(SYSTEM_ARG_MENU));
	if (pWin != NULL)
	{
		((CSysArgMenu *)(m_pFrame->GetWin(SYSTEM_ARG_MENU)))->ReFresh();
	}
	
	//读当前模板至内存
	g_globalArg->ReadPrintTemplate(g_globalArg->m_templateBuffer, g_globalArg->m_curTemplateNo, PRINT_TEMPLATE_BUF_LEN);
	
	sprintf(tmp, "设置模板%u为当前模板", g_globalArg->m_curTemplateNo);
	CaMsgBox::ShowMsg(tmp);

	CaMsgBox::ShowMsg("请重启款机,使新模板生效!");
	while (1)
	{
		;
	}
	}
	
}

void CDataIOMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	UINT8 j=0, res;
	char tmp[64];
	string file_S = PRINT_TEMPLATE_FILE_NAME;
	string file_D = "/mnt/mb";
	string cmd = "";
	
	CaProgressBar proBar("");
	proBar.SetText("模板导出中..."); 
	proBar.ReFresh();
	
	//挂载U盘
	res = UsbDiskMount(NULL);
	if (SUCCESS != res)
	{
		DBG_PRINT(("挂载U盘失败 res = %u", res));
		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return;
	}
	
	//依次读取模板文件，并拷贝至UPAN 
	for (j=0; j<PRINT_TEMPLATE_NUMBER; j++)
	{
		file_S = PRINT_TEMPLATE_FILE_NAME;
		file_D = "/mnt/mb";
		
		//读取序号为j的模板，如果该序号没有对应模板则中止发送
		sprintf(tmp, "%02u.txt", j+1);
		file_S += tmp;
		//	DBG_PRINT(("Template file: %s", file_S.c_str()));
		
		sprintf(tmp, "%02u.bin", j+1);
		file_D += tmp;
		//	DBG_PRINT(("USB file: %s", file_S.c_str()));
		
		cmd = "cp " + file_S + " " + file_D;
		DBG_PRINT(("cmd: %s", cmd.c_str()));
		
		if (system(cmd.c_str()) != 0)
			break;
	}	
	
	UsbDiskUnMount();
	sprintf(tmp, "共导出%u个模板", j);
	CaMsgBox::ShowMsg(tmp);
}

void CDataIOMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	CaMsgBox::ShowMsg("建议先备份导入的\n商品信息以免丢失");
	UINT8 ret = DeptImportProc();
}
void CDataIOMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	DeptExportProc();
}

void CDataIOMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	CaMsgBox::ShowMsg("建议先备份导入的\n客户信息以免丢失");
	UINT8 ret = ClientImportProc();
}
void CDataIOMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClientExportProc();
}


int CDataIOMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	switch(iEvent) 
	{
	case RETURN_MAIN_MENU:
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);
		return SUCCESS;
		break;	
		
	default: break;
	}

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}




